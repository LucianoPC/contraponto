#ifndef _COUNTER_POINT_H_
#define _COUNTER_POINT_H_

#include "vector"
#include "MuMaterial.h"
#include "voice.hpp"

class CounterPoint
{
public:
    CounterPoint (MuMaterial material);

    MuMaterial GenerateCounterPointMaterial ();

    void SetMaterial (MuMaterial material);
    void SetScalePitch (int scale_pitch);

    void PrintHarmonicPitchs ();

    Voice cantus_firmus_voice;
    Voice counter_point_voice;
private:

    int scale_pitch;

    vector<int> scale_pitchs;

    vector< vector<int> > harmonic_pitchs_list;

    MuMaterial material;

    Voice::Manager voice_manager;

    void UpdateHarmonicPitchs ();

    void InitializeVoiceManager ();
    void SetVoices (int pitch);

    vector<int> GetHarmonicRange (int note_index);
    vector<int> GetMelodicRange (int note_pitch);

    vector<int> FixPitchsToScale (vector<int> pitchs);

    vector<int> GenerateCounterPointPitchs();

    vector<int> GetPossiblePitchs (int last_pitch,
                                   int harmonic_pitchs_index);

    void PrintVector (vector<int> v, string message);
};

#endif
