#ifndef DASHPLAYER_H_
#define DASHPLAYER_H_

#include <iostream>
#include <sstream>
#include "libdash.h"
#include "../Managers/IMultimediaManagerObserver.h"
#include "../Renderer/DroidGLRenderer.h"
#include "../Renderer/DroidAudioRenderer.h"
#include "../Managers/MultimediaManager.h"
#include "../libdashframework/Adaptation/AlwaysLowestLogic.h"
#include "../libdashframework/Adaptation/ManualAdaptation.h"
#include "../libdashframework/Buffer/IBufferObserver.h"
#include "../libdashframework/MPD/AdaptationSetHelper.h"

struct settings_t
{
    int period;
    int videoAdaptationSet;
    int audioAdaptationSet;
    int videoRepresentation;
    int audioRepresentation;
};


class DASHInterface : sampleplayer::managers::IMultimediaManagerObserver{
        private:
            dash::mpd::IMPD                             *mpd;
            sampleplayer::renderer::DroidGLRenderer        *videoElement;
            sampleplayer::renderer::DroidAudioRenderer     *audioElement;
            sampleplayer::managers::MultimediaManager   *multimediaManager;
            settings_t                                  currentSettings;
            CRITICAL_SECTION                            monitorMutex;

        public:
        	DASHInterface();
        	~DASHInterface();
        	bool DownloadMPD(const std::string &url);
        	void Start();
        	void Stop();

            void InitDroidGLRenderer(int, int);
            void resizeViewPort(int, int);
            void issueFrameDraw();

            void OnSettingsChanged(int period, int videoAdaptationSet, int videoRepresentation, int audioAdaptationSet, int audioRepresentation);
            bool SettingsChanged (int period, int videoAdaptationSet, int videoRepresentation, int audioAdaptationSet, int audioRepresentation);
            void SetSettings     (int period, int videoAdaptationSet, int videoRepresentation, int audioAdaptationSet, int audioRepresentation);
    
            void OnVideoBufferStateChanged          (uint32_t fillstateInPercent);
            void OnVideoSegmentBufferStateChanged   (uint32_t fillstateInPercent);
            void OnAudioBufferStateChanged          (uint32_t fillstateInPercent);
            void OnAudioSegmentBufferStateChanged   (uint32_t fillstateInPercent);
};

#endif