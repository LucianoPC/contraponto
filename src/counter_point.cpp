#include <algorithm>
#include "counter_point.hpp"


CounterPoint::CounterPoint(MuMaterial material)
{
    SetMaterial(material);
}

void
CounterPoint::SetMaterial(MuMaterial material)
{
    this->material = material;
    SetScalePitch(material.GetNote(0).Pitch());
    UpdateHarmonicPitchs();
}

void
CounterPoint::SetScalePitch(int scale_pitch)
{
    scale_pitch = scale_pitch % 12;
    this->scale_pitch = scale_pitch;
    this->scale_pitchs = { 0 + scale_pitch, 2 + scale_pitch,
                           4 + scale_pitch, 5 + scale_pitch,
                           7 + scale_pitch, 9 + scale_pitch,
                           11 + scale_pitch };
}

void
CounterPoint::PrintHarmonicPitchs()
{
    for (long i = 0; i < this->material.NumberOfNotes(); i++)
    {
        short pitch = this->material.GetNote(i).Pitch();

        cout << "pitch: " << pitch << " [ ";
        for (int j = 0; j < harmonic_pitchs[i].size(); j++)
        {
            cout << this->harmonic_pitchs[i][j] << " ";
        }
        cout << "]" << endl;
    }
}

void
CounterPoint::UpdateHarmonicPitchs ()
{
    this->harmonic_pitchs.clear();
    for (long index = 0; index < material.NumberOfNotes(); index++)
    {
        vector<int> harmonic_pitch_vector = GetHarmonicRange(index);
        harmonic_pitch_vector = FixPitchsToScale(harmonic_pitch_vector);

        this->harmonic_pitchs.push_back(harmonic_pitch_vector);
    }
}

vector<int>
CounterPoint::GetHarmonicRange (int note_index)
{
    int note_pitch = this->material.GetNote(note_index).Pitch();

    int unison = note_pitch;
    int third_minor = note_pitch - 3;
    int third_major = note_pitch - 4;
    int fifith_perfect = note_pitch - 7;
    int sixth_minor = note_pitch - 8;
    int sixth_major = note_pitch - 9;

    bool is_first_note = note_index == 0;
    bool is_last_note = note_index == this->material.NumberOfNotes() - 1;

    vector<int> harmonic_range;
    if(is_first_note) {
        harmonic_range = { unison, third_minor, third_major, fifith_perfect,
                           sixth_minor, sixth_major };
    } else if(is_last_note) {
        harmonic_range = { this->material.GetNote(0).Pitch() };
    } else {
        harmonic_range = { third_minor, third_major, fifith_perfect,
                           sixth_minor, sixth_major };
    }

    return harmonic_range;
}

vector<int>
CounterPoint::FixPitchsToScale (vector<int> pitchs)
{
    vector<int> pitchs_on_scale;

    for (unsigned int index = 0; index < pitchs.size(); index++)
    {
        int pitch = pitchs[index] % 12;
        bool found_pitch = find(this->scale_pitchs.begin(),
                                this->scale_pitchs.end(),
                                pitch) != this->scale_pitchs.end();

        if(found_pitch)
        {
            pitchs_on_scale.push_back(pitchs[index]);
        }
    }

    return pitchs_on_scale;
}

void
CounterPoint::PrintVector (vector<int> v, string message)
{
    cout << message << ": ";
    for (unsigned int index = 0; index < v.size(); index++)
    {
        cout << v[index] << " ";
    }
    cout << endl;
}
