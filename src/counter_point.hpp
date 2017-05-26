#ifndef _COUNTER_POINT_H_
#define _COUNTER_POINT_H_

#include "vector"
#include "MuMaterial.h"

class CounterPoint
{
public:

    CounterPoint();

    void SetMaterial(MuMaterial material);
    void SetScalePitch(int scale_pitch);

    void PrintHarmonicPitchs();

private:

    int scale_pitch;

    vector<int> scale_pitchs;

    vector< vector<int> > harmonic_pitchs;

    MuMaterial material;

    vector<int> GetHarmonicRange (int note_pitch);
    vector<int> FixPitchsToScale (vector<int> pitchs);

    void PrintVector (vector<int> v, string message);
};

#endif
