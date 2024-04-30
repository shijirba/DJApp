/*
  ==============================================================================

    Track.h
    Created: 8 Sep 2021 2:03:29pm
    Author:  SHIJIRBA

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Track
{
public:
    Track(juce::File _file);
    juce::File file;
    juce::URL URL;
    juce::String title;
    juce::String length;
    bool operator==(const juce::String& other) const;
};