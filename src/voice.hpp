#ifndef _VOICE_H_
#define _VOICE_H_

#include <string>
#include <vector>

using namespace std;

class Voice
{
public:
    class Manager;

    Voice (string name, int min_pitch, int max_pitch);

    bool IsOnVoice (int pitch);

    string name;
    int min_pitch;
    int max_pitch;
};

class Voice::Manager
{
public:

    void AddVoice (string name, int min_pitch, int max_pitch);

    Voice GetVoice (int pitch);

    Voice GetLowVoice (Voice base_voice);

private:

    vector<Voice> voices;
};

#endif
