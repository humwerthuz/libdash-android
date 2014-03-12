#include "DASHInterface.h"
#include "AndroidLog.h"
#include <iostream>

using namespace libdash::framework::adaptation;
using namespace libdash::framework::mpd;
using namespace libdash::framework::buffer;
using namespace sampleplayer;
using namespace sampleplayer::renderer;
using namespace sampleplayer::managers;
using namespace dash::mpd;
using namespace std;


DASHInterface::DASHInterface(){
	InitializeCriticalSection(&this->monitorMutex);
	this->videoElement = new DroidGLRenderer();
	this->audioElement = new DroidAudioRenderer();
	this->multimediaManager = new MultimediaManager(this->videoElement, this->audioElement);

    #warning "FIXED VALUE AT MultimediaManager::SetFrameRate"
	this->multimediaManager->SetFrameRate(29);
	this->multimediaManager->AttachManagerObserver(this);
}

bool DASHInterface::DownloadMPD(const std::string &url){
	if(!this->multimediaManager->Init(url)){
		return false;
	}

	// this->SetSettings(0, 0, 0, 0, 0);
	return true;
}

void DASHInterface::InitDroidGLRenderer(int w, int h){
	this->videoElement->Init(w, h);
}

void DASHInterface::issueFrameDraw(){
	this->videoElement->update();
}

void DASHInterface::resizeViewPort(int w, int h){
	this->videoElement->resizeViewPort(w, h);
}

void DASHInterface::Start(){  
    this->OnSettingsChanged(0, 0, 0, 0, 0);
	this->multimediaManager->Start();
}

void DASHInterface::Stop(){
	this->multimediaManager->Stop();
}

void DASHInterface::SetSettings (int period, int videoAdaptationSet, int videoRepresentation, int audioAdaptationSet, int audioRepresentation){
    this->currentSettings.period                = period;
    this->currentSettings.videoAdaptationSet    = videoAdaptationSet;
    this->currentSettings.videoRepresentation   = videoRepresentation;
    this->currentSettings.audioAdaptationSet    = audioAdaptationSet;
    this->currentSettings.audioRepresentation   = audioRepresentation;
}

void DASHInterface::OnSettingsChanged(int period, int videoAdaptationSet, int videoRepresentation, int audioAdaptationSet, int audioRepresentation){
    if(this->multimediaManager->GetMPD() == NULL)
        return;

    if (!this->SettingsChanged(period, videoAdaptationSet, videoRepresentation, audioAdaptationSet, audioRepresentation))
        return;

    IPeriod                         *currentPeriod      = this->multimediaManager->GetMPD()->GetPeriods().at(period);
    std::vector<IAdaptationSet *>   videoAdaptationSets = AdaptationSetHelper::GetVideoAdaptationSets(currentPeriod);
    std::vector<IAdaptationSet *>   audioAdaptationSets = AdaptationSetHelper::GetAudioAdaptationSets(currentPeriod);


    if (videoAdaptationSet >= 0 && videoRepresentation >= 0 && videoAdaptationSets.size() > 0){
        this->multimediaManager->SetVideoQuality(currentPeriod,
                                                 videoAdaptationSets.at(videoAdaptationSet),
                                                 videoAdaptationSets.at(videoAdaptationSet)->GetRepresentation().at(videoRepresentation));
    } else {
        this->multimediaManager->SetVideoQuality(currentPeriod, NULL, NULL);
    }

    if (audioAdaptationSet >= 0 && audioRepresentation >= 0 && audioAdaptationSets.size() > 0){
        this->multimediaManager->SetAudioQuality(currentPeriod,
                                                 audioAdaptationSets.at(audioAdaptationSet),
                                                 audioAdaptationSets.at(audioAdaptationSet)->GetRepresentation().at(audioRepresentation));
    } else {
        this->multimediaManager->SetAudioQuality(currentPeriod, NULL, NULL);
    }
}

bool DASHInterface::SettingsChanged(int period, int videoAdaptationSet, int videoRepresentation, int audioAdaptationSet, int audioRepresentation){
    EnterCriticalSection(&this->monitorMutex);

    bool settingsChanged = false;

    if (this->currentSettings.videoRepresentation != videoRepresentation ||
        this->currentSettings.audioRepresentation != audioRepresentation ||
        this->currentSettings.videoAdaptationSet != videoAdaptationSet ||
        this->currentSettings.audioAdaptationSet != audioAdaptationSet ||
        this->currentSettings.period != period)
        settingsChanged = true;

    if (settingsChanged)
        this->SetSettings(period, videoAdaptationSet, videoRepresentation, audioAdaptationSet, audioRepresentation);

    LeaveCriticalSection(&this->monitorMutex);

    return settingsChanged;
}

void DASHInterface::OnVideoBufferStateChanged          (uint32_t fillstateInPercent){
	return;
}

void DASHInterface::OnVideoSegmentBufferStateChanged   (uint32_t fillstateInPercent){
	return;
}

void DASHInterface::OnAudioBufferStateChanged          (uint32_t fillstateInPercent){
	return;
}

void DASHInterface::OnAudioSegmentBufferStateChanged   (uint32_t fillstateInPercent){
	return;
}

DASHInterface::~DASHInterface() {
    this->multimediaManager->Stop();
    this->audioElement->StopPlayback();
    // this->audioElement = NULL;

    delete(this->multimediaManager);
    delete(this->audioElement);

    DeleteCriticalSection(&this->monitorMutex);
}

/// Implement all other methods to interact with java code (hence JNI)