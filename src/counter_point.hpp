#ifndef _COUNTER_POINT_H_
#define _COUNTER_POINT_H_

#include "vector"
#include "MuMaterial.h"

class CounterPoint
{
public:

    CounterPoint (MuMaterial material);

    MuMaterial GenerateCounterPointMaterial ();

    void SetMaterial (MuMaterial material);
    void SetScalePitch (int scale_pitch);

    void PrintHarmonicPitchs ();

private:

    int scale_pitch;

    vector<int> scale_pitchs;

    vector< vector<int> > harmonic_pitchs_list;

    MuMaterial material;

    void UpdateHarmonicPitchs ();

    vector<int> GetHarmonicRange (int note_index);
    vector<int> GetMelodicRange (int note_pitch);

    vector<int> FixPitchsToScale (vector<int> pitchs);

    vector<int> GenerateCounterPointPitchs();

    vector<int> GetPossiblePitchs (int last_pitch,
                                   int harmonic_pitchs_index);

    void PrintVector (vector<int> v, string message);
};

#endif
