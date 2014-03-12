/*
 * AbstractSetHelper.cpp
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "AdaptationSetHelper.h"
#include "../../AndroidLog.h"

using namespace libdash::framework::mpd;
using namespace dash::mpd;

std::vector<IAdaptationSet *>   AdaptationSetHelper::GetAudioAdaptationSets (dash::mpd::IPeriod *period)
{
    LOGD("Searching for audio adaptionsets");
    std::vector<IAdaptationSet *> audioAdaptationSets;
    std::vector<IAdaptationSet *> adaptationSets = period->GetAdaptationSets();

    for (size_t i = 0; i < adaptationSets.size(); i++)
        if (AdaptationSetHelper::IsAudioAdaptationSet(adaptationSets.at(i)))
            audioAdaptationSets.push_back(adaptationSets.at(i));

    LOGD(("Found " + patch::to_string(audioAdaptationSets.size()) + " audio sets").c_str());
    return audioAdaptationSets;
}
std::vector<IAdaptationSet *>   AdaptationSetHelper::GetVideoAdaptationSets (dash::mpd::IPeriod *period)
{
    LOGD("Searching for videoadaptationsets");
    std::vector<IAdaptationSet *> videoAdaptationSets;
    std::vector<IAdaptationSet *> adaptationSets = period->GetAdaptationSets();

    for (size_t i = 0; i < adaptationSets.size(); i++)
        if (AdaptationSetHelper::IsVideoAdaptationSet(adaptationSets.at(i)))
            videoAdaptationSets.push_back(adaptationSets.at(i));

    LOGD(("Found " + patch::to_string(videoAdaptationSets.size()) + " video sets").c_str());
    return videoAdaptationSets;
}
bool                            AdaptationSetHelper::IsAudioAdaptationSet   (dash::mpd::IAdaptationSet *adaptationSet)
{
    return IsContainedInMimeType(adaptationSet, "audio");
}
bool                            AdaptationSetHelper::IsVideoAdaptationSet   (dash::mpd::IAdaptationSet *adaptationSet)
{
    return IsContainedInMimeType(adaptationSet, "video");
}
bool                            AdaptationSetHelper::IsContainedInMimeType  (dash::mpd::IAdaptationSet *adaptationSet, const std::string& value)
{

	std::string * mimeType = adaptationSet->GetMimeType();

	if(mimeType->find(value) != std::string::npos){
		LOGD(("MimeType '" + (*mimeType) + "' found").c_str());
		return true;
	}

	LOGD("AdaptationSet header does not have a MimeType declaration. Searching in representations...");

    for (size_t i = 0; i < adaptationSet->GetRepresentation().size(); i++){

    	mimeType = adaptationSet->GetRepresentation().at(i)->GetMimeType();

        if(mimeType->find(value) != std::string::npos){
        	LOGD(("MimeType '" + (*mimeType) + "' found").c_str());
        	return true;
        }
    }
    return false;
}
