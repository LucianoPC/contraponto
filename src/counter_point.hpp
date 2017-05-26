#ifndef _COUNTER_POINT_H_
#define _COUNTER_POINT_H_

#include "vector"
#include "MuMaterial.h"

class CounterPoint
{
public:

    CounterPoint();

    void SetMaterial(MuMaterial material);

private:

    MuMaterial material;

    vector<int> GetHarmonicRange (int note_pitch);
};

#endif
