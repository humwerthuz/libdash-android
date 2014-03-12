/*
 * DroidGLRenderer.cpp
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "DroidGLRenderer.h"

using namespace sampleplayer::renderer;

/**************** UTILS ******************/
static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", op, error);
    }
}

static GLuint s_disable_caps[] = {
    GL_FOG,
    GL_LIGHTING,
    GL_CULL_FACE,
    GL_ALPHA_TEST,
    GL_BLEND,
    GL_COLOR_LOGIC_OP,
    GL_DITHER,
    GL_STENCIL_TEST,
    GL_DEPTH_TEST,
    GL_COLOR_MATERIAL,
    0
};

int find_pow_2(int num)
{
    int res = 1;
    while (res < num)
        res *= 2;
    return res;
}

/**********************************/

DroidGLRenderer::DroidGLRenderer(){
	InitializeCriticalSection (&this->monitorMutex);
    InitializeCriticalSection (&this->monitorVsync);
    
	this->iHaveSomethingToDraw = false;
    this->texture = -1;
    this->textureBuffer = NULL;
    this->fh = 0;
    this->fw = 0;
}

DroidGLRenderer::~DroidGLRenderer() {
    DeleteCriticalSection (&this->monitorMutex);
}

void DroidGLRenderer::Init(int width, int height) {
    this->width = width;
    this->height = height;

    EnterCriticalSection(&this->monitorMutex);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.f, 1.f, 0.f, 0.f);

    glClearDepthf(1.0f);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glDisable(GL_CULL_FACE | GL_NORMALIZE | GL_LIGHTING | GL_BLEND | GL_FOG | GL_COLOR_MATERIAL | GL_TEXTURE_2D);
    LeaveCriticalSection(&this->monitorMutex);
}


void DroidGLRenderer::SetImage(RawFrame  *image) {
    EnterCriticalSection(&this->monitorMutex);
    this->updateTextureBuffer(image);
    this->iHaveSomethingToDraw = true;
    LeaveCriticalSection(&this->monitorMutex);
}

void DroidGLRenderer::resizeViewPort(int w, int h){
	LOGD(("Resizing viewport to " + patch::to_string(w) + "x" + patch::to_string(h)).c_str());
	EnterCriticalSection(&this->monitorMutex);

    this->width = w;
    this->height = h;

    this->tex_width = find_pow_2(this->width);
    this->tex_height = find_pow_2(this->height);

    this->resizeWindow();
    this->createTexture();

	LeaveCriticalSection(&this->monitorMutex);
}

void DroidGLRenderer::update() {
    EnterCriticalSection(&this->monitorMutex);

    this->drawGLScene();
    this->iHaveSomethingToDraw = false;

    LeaveCriticalSection(&this->monitorMutex);
}

void DroidGLRenderer::resizeWindow(){
    glViewport(0, 0, this->width, this->height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrthox(0, this->width, 0, this->height, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*** DO NOT CALL DIRECTLY, THIS METHOD IS NOT SYNCHRONIZED  ***/
void DroidGLRenderer::drawGLScene(){
    if(this->textureBuffer == NULL){
        return;
    }

    GLfloat vertices[4][3];
    GLfloat texcoord[4][2];

    float rgba[4];

    rgba[0] = rgba[1] = rgba[2] = 0.f;
    rgba[0] = 1.f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, rgba);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, rgba);
    glDisable(GL_CULL_FACE | GL_NORMALIZE | GL_LIGHTING | GL_BLEND | GL_FOG | GL_COLOR_MATERIAL | GL_TEXTURE_2D);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /***
        Load frame in current texture. Use subImage to only update the 
        portion needed for the frame
    ***/

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->fw, this->fh,
                    GL_RGBA, GL_UNSIGNED_BYTE, this->textureBuffer);


    /***
        This is where the magic happens, this crops the current texture to the
        specified points. Must implement aspect ratio and sub texture location
        in order to render smaller videos with correct AR, and also for them 
        to be centered on screen
    ***/

    int cropRect[4] = { 0, this->fh, this->fw, -this->fh};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, cropRect);

    glDrawTexsOES(0, 0, 0, this->width, this->height);

    glDisable(GL_TEXTURE_2D);

    glFinish();
}

/*** DO NOT CALL DIRECTLY, THIS METHOD IS NOT SYNCHRONIZED  ***/
void DroidGLRenderer::createTexture(){
    if(this->texture >= 0){
        this->releaseTexture();
    }

    glGenTextures(1, &(this->texture));

    // this->textureBuffer = (GLubyte *) malloc(3 * this->tex_width * this->tex_height);
    // memset(this->textureBuffer, 100, 3 * this->tex_height * this->tex_width);

    glBindTexture(GL_TEXTURE_2D, this->texture);

    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
    glTexEnvx(GL_TEXTURE_ENV,       GL_TEXTURE_ENV_MODE,    GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,      GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,      GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tex_width, this->tex_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);

    LOGD(("Created new texture, size: " + patch::to_string(this->tex_width)
        + "x" + patch::to_string(this->tex_height)).c_str());
}

/*** DO NOT CALL DIRECTLY, THIS METHOD IS NOT SYNCHRONIZED  ***/
void DroidGLRenderer::releaseTexture(){
    if(this->texture >= 0){
        glDeleteTextures(1, &(this->texture));
        this->texture = -1;
    }
    if(this->textureBuffer != NULL){
        delete this->textureBuffer;
        this->textureBuffer = NULL;
    }
}

/*** DO NOT CALL DIRECTLY, THIS METHOD IS NOT SYNCHRONIZED  ***/
void DroidGLRenderer::updateTextureBuffer(RawFrame * frame){
    // if(this->textureBuffer != NULL){
    //     for(int i=0; i < (frame->getHeight()); i++){
    //         int offset = (this->tex_width*i + (this->tex_width - frame->getWidth()));
    //         for(int j=0; j < (frame->getWidth() * 3); j++, offset++){
    //             this->textureBuffer[offset] = (frame->getData())[i*j];
    //         } 
    //     }
    // }

    if(this->textureBuffer != NULL){
        delete this->textureBuffer;
    }

    // TODO: GUESS INTERNAL PIXEL FORMAT BASED ON frame->getPixelSize() 3 -> RGB, 4 -> RGBA

    int size = frame->getPixelSize() * frame->getWidth() * frame->getHeight();
    this->textureBuffer = (GLubyte *) malloc(size);

    memcpy(this->textureBuffer, frame->getData(), size);

    this->fw = frame->getWidth();
    this->fh = frame->getHeight();
}