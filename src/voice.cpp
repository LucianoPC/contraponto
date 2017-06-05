#include "voice.hpp"

#include <iostream>
using namespace std;

Voice::Voice () : Voice("NONE", 0, 0)
{

}

Voice::Voice (string name, int min_pitch, int max_pitch)
{
    this->name = name;
    this->min_pitch = min_pitch;
    this->max_pitch = max_pitch;
}

bool
Voice::IsOnVoice (int pitch)
{
    return pitch >= this->min_pitch && pitch <= this->max_pitch;
}

void
Voice::Manager::AddVoice (string name, int min_pitch, int max_pitch)
{
    Voice voice(name, min_pitch, max_pitch);

    this->voices.push_back(voice);
}

Voice
Voice::Manager::GetVoice (int pitch)
{
    vector<Voice> possible_voices;

    for (unsigned int index = 0; index < this->voices.size(); index++)
    {
        Voice voice = this->voices[index];

        if (voice.IsOnVoice(pitch)) {
            possible_voices.push_back(voice);
        }
    }

    if (possible_voices.size() == 0) {
        return this->voices[0];
    }

    srand(static_cast<unsigned int>(time(0)));

    int random_index = rand() % possible_voices.size();

    // cout << endl << "voices: " << possible_voices.size() << " random: " << random_index << " voice: " << possible_voices[random_index].name << endl << endl;

    return possible_voices[random_index];
}

Voice
Voice::Manager::GetLowVoice (Voice base_voice)
{
    int low_voice_index = 0;

    for (unsigned int index = 1; index < this->voices.size(); index++)
    {
        Voice voice = this->voices[index];
        int voice_distance = base_voice.max_pitch - voice.max_pitch;

        Voice low_voice = this->voices[low_voice_index];
        int low_voice_distance = base_voice.max_pitch - low_voice.max_pitch;

        bool voice_has_min_distance = voice_distance < low_voice_distance;
        bool voice_is_lower = voice_distance > 0;
        bool low_voice_invalid = low_voice_distance <= 0;
        bool is_new_lower_voice = low_voice_invalid ||
                                  (voice_has_min_distance && voice_is_lower);

        // cout << endl;
        // cout << "voice[" << index << "]: " << this->voices[index].name << endl;
        // cout << "voice_distance: " << voice_distance << endl;
        // cout << "voice_has_min_distance: " << voice_has_min_distance << endl;
        // cout << "voice_is_lower: " << voice_is_lower << endl;
        // cout << "low_voice_invalid: " << low_voice_invalid << endl;
        // cout << "is_new_lower_voice: " << is_new_lower_voice << endl;

        if (is_new_lower_voice)
        {
            low_voice_index = index;
        }
    }

    return this->voices[low_voice_index];
}
