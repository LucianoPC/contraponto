#include <algorithm>
#include "MuUtil.h"
#include "counter_point.hpp"


CounterPoint::CounterPoint(MuMaterial material)
{
    SetMaterial(material);
}

MuMaterial
CounterPoint::GenerateCounterPointMaterial()
{
    vector<int> counter_point_pitchs = GenerateCounterPointPitchs();

    MuMaterial counter_point_material;

    long number_of_notes = this->material.NumberOfNotes();
    for (int index = 0; index < number_of_notes; index++)
    {
        MuNote note = this->material.GetNote(index);
        note.SetPitch(counter_point_pitchs[index]);

        counter_point_material += note;
    }

    return counter_point_material;
}

vector<int>
CounterPoint::GenerateCounterPointPitchs()
{
    vector<int> counter_point_pitchs;

    for (long i = 0; i < this->material.NumberOfNotes(); i++)
    {
        bool is_first_note = i == 0;
        bool is_last_note = i == this->material.NumberOfNotes() - 1;

        if(is_first_note)
        {
            int pitch_index = Between(0, this->harmonic_pitchs[i].size() - 1);
            int pitch = this->harmonic_pitchs[i][pitch_index];

            counter_point_pitchs.push_back(pitch);
        }
        else if(is_last_note)
        {
            int pitch = this->harmonic_pitchs[i][0];

            counter_point_pitchs.push_back(pitch);
        }
        else
        {
            int last_pitch = counter_point_pitchs[i - 1];

            vector<int> melodic_pitchs = GetMelodicRange(last_pitch);

            vector<int> possible_pitchs;
            for (int j = 0; j < melodic_pitchs.size(); j++)
            {

                bool found_pitch = false;
                int melodic_pitch = melodic_pitchs[j];
                for(int a = 0; a < this->harmonic_pitchs[i].size() && !found_pitch; a++)
                {
                    int harmonic_pitch = this->harmonic_pitchs[i][a];

                    bool same_note = (melodic_pitch % 12) == (harmonic_pitch % 12);
                    bool is_lower = melodic_pitch <= harmonic_pitch;

                    found_pitch = same_note && is_lower;
                }

                if(found_pitch)
                {
                    possible_pitchs.push_back(melodic_pitch);
                }
            }

            cout << endl << "last_pitch: " << last_pitch << " (" << last_pitch % 12 << ")" << endl;
            PrintVector(melodic_pitchs, "melodic_pitchs");
            PrintVector(this->harmonic_pitchs[i], "harmonic_pitchs");
            PrintVector(possible_pitchs, "possible_pitchs");

            int pitch_index = Between(0, melodic_pitchs.size() - 1);
            int pitch = melodic_pitchs[pitch_index];

            counter_point_pitchs.push_back(pitch);

        }
    }

    PrintVector(counter_point_pitchs, "counter_point_pitchs");

    return counter_point_pitchs;
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
    cout << "Harmonic Pitchs" << endl;
    for (long i = 0; i < this->material.NumberOfNotes(); i++)
    {
        short pitch = this->material.GetNote(i).Pitch();

        cout << "pitch: " << pitch << " [ ";
        for (int j = 0; j < harmonic_pitchs[i].size(); j++)
        {
            cout << this->harmonic_pitchs[i][j] % 12 << " ";
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

    harmonic_range = FixPitchsToScale(harmonic_range);

    return harmonic_range;
}

vector<int>
CounterPoint::GetMelodicRange (int note_pitch)
{
    int second_minor_descending = note_pitch - 1;
    int second_major_descending = note_pitch - 2;
    int third_minor_descending = note_pitch - 3;
    int third_major_descending = note_pitch - 4;
    int fourth_perfect_descending = note_pitch - 5;
    int fifith_perfect_descending = note_pitch - 7;
    int octave_descending = note_pitch - 8;

    int second_minor_ascending = note_pitch + 1;
    int second_major_ascending = note_pitch + 2;
    int third_minor_ascending = note_pitch + 3;
    int third_major_ascending = note_pitch + 4;
    int fourth_perfect_ascending = note_pitch + 5;
    int fifith_perfect_ascending = note_pitch + 7;
    int sixth_minor_ascending = note_pitch + 8;
    int octave_ascending = note_pitch + 8;

    vector<int> melodic_range { octave_descending,
                                fifith_perfect_descending,
                                fourth_perfect_descending,
                                third_major_descending, third_minor_descending,
                                second_major_descending,
                                second_minor_descending,
                                second_minor_ascending, second_major_ascending,
                                third_minor_ascending, third_major_ascending,
                                fourth_perfect_ascending,
                                fifith_perfect_ascending,
                                sixth_minor_ascending,
                                octave_ascending };

    melodic_range = FixPitchsToScale(melodic_range);

    return melodic_range;
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
