/*
  ==============================================================================

    Track.cpp
    Created: 8 Sep 2021 2:03:29pm
    Author:  SHIJIRBA

  ==============================================================================
*/

#include "Track.h"
#include <filesystem>

Track::Track(juce::File _file) : file(_file),
title(_file.getFileNameWithoutExtension()),
URL(juce::URL{ _file })
{
}

bool Track::operator==(const juce::String& other) const
{
    return title == other;
}