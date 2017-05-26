#include <algorithm>
#include "counter_point.hpp"


CounterPoint::CounterPoint()
{
    this->scale_pitch = 60;
}

void
CounterPoint::SetMaterial(MuMaterial material)
{
    this->material = material;
    SetScalePitch(material.GetNote(0).Pitch());

    long number_of_notes = material.NumberOfNotes();

    for (long index = 0; index < number_of_notes; index++)
    {
        vector<int> harmonic_pitchs = GetHarmonicRange(index);
        harmonic_pitchs = FixPitchsToScale(harmonic_pitchs);

        cout << "pitch: " << material.GetNote(index).Pitch() << "  harmonic_pitchs: ";
        for (int j = 0; j < harmonic_pitchs.size(); j++)
        {
            cout << harmonic_pitchs[j] << " ";
        }
        cout << endl;

    }
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
