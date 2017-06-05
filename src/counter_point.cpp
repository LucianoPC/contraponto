#include <algorithm>
#include <map>
#include "MuUtil.h"
#include "counter_point.hpp"
#include "voice.hpp"


CounterPoint::CounterPoint(MuMaterial material)
{
    SetMaterial(material);
    InitializeVoiceManager();
}

MuMaterial
CounterPoint::GenerateCounterPointMaterial()
{
    SetVoices(this->material.GetNote(0).Pitch());
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
    map< int, map<int, bool> > used_pitchs;

    for (long index = 0; index < this->material.NumberOfNotes(); index++)
    {
        int last_pitch = index > 0 ? counter_point_pitchs[index - 1] : 0;

        vector<int> possible_pitchs = GetPossiblePitchs(last_pitch, index);

        FixPitchsToVoice(possible_pitchs);
        RemoveUsedPitchs(possible_pitchs, used_pitchs[index]);

        bool has_possible_pitchs = possible_pitchs.size() > 0;

        if(has_possible_pitchs)
        {
            int pitch_index = Between(0, possible_pitchs.size() - 1);
            int pitch = possible_pitchs[pitch_index];

            used_pitchs[index][pitch] = true;

            counter_point_pitchs.push_back(pitch);
        }
        else
        {
            used_pitchs[index] = map<int, bool>();
            if(index == 0)
            {
                cout << "Dont have solution" << endl;
                break;
            }
            else
            {
                index -= 2;
                counter_point_pitchs.pop_back();
            }
        }
    }

    PrintVector(counter_point_pitchs, "counter_point_pitchs");

    return counter_point_pitchs;
}

vector<int>
CounterPoint::GetPossiblePitchs (int last_pitch, int harmonic_pitchs_index)
{
    vector<int> possible_pitchs;

    int index = harmonic_pitchs_index;
    vector<int> harmonic_pitchs = this->harmonic_pitchs_list[index];

    bool is_first_note = harmonic_pitchs_index == 0;
    bool is_last_note = index == this->material.NumberOfNotes() - 1;

    if(is_first_note || is_last_note)
    {
        possible_pitchs = harmonic_pitchs;
    }
    else
    {
        vector<int> melodic_pitchs = GetMelodicRange(last_pitch);

        for (int j = 0; j < melodic_pitchs.size(); j++)
        {
            bool found_pitch = false;
            int melodic_pitch = melodic_pitchs[j];
            for(int a = 0; a < harmonic_pitchs.size() && !found_pitch; a++)
            {
                int harmonic_pitch = harmonic_pitchs[a];
                bool same_note = (melodic_pitch % 12) == (harmonic_pitch % 12);
                bool is_lower = melodic_pitch <= harmonic_pitch;

                found_pitch = same_note && is_lower;
            }

            if(found_pitch)
            {
                possible_pitchs.push_back(melodic_pitch);
            }
        }

        // cout << endl << "last_pitch: " << last_pitch << " (" << last_pitch % 12 << ")" << endl;
        PrintVector(melodic_pitchs, "melodic_pitchs");
        PrintVector(harmonic_pitchs, "harmonic_pitchs_list");
        PrintVector(possible_pitchs, "possible_pitchs");
    }

    return possible_pitchs;
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
        for (int j = 0; j < harmonic_pitchs_list[i].size(); j++)
        {
            cout << this->harmonic_pitchs_list[i][j] % 12 << " ";
        }
        cout << "]" << endl;
    }
}

void
CounterPoint::UpdateHarmonicPitchs ()
{
    this->harmonic_pitchs_list.clear();
    for (long index = 0; index < material.NumberOfNotes(); index++)
    {
        vector<int> harmonic_pitch_vector = GetHarmonicRange(index);
        this->harmonic_pitchs_list.push_back(harmonic_pitch_vector);
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
CounterPoint::FixPitchsToVoice (vector<int>& pitchs)
{
    Voice voice = this->counter_point_voice;
    vector<int> old_pitchs = pitchs;
    pitchs = vector<int>();

    for (unsigned int index = 0; index < old_pitchs.size(); index++)
    {
        int pitch = old_pitchs[index];
        bool pitch_is_on_voice = voice.IsOnVoice(pitch);

        if(pitch_is_on_voice)
        {
            pitchs.push_back(pitch);
        }
    }
}

void
CounterPoint::RemoveUsedPitchs (vector<int>& pitchs,
                                map<int, bool> used_pitchs)
{

    vector<int> old_pitchs = pitchs;
    pitchs = vector<int>();

    for (unsigned int index = 0; index < old_pitchs.size(); index++)
    {
        int pitch = old_pitchs[index];
        bool is_unused_pitch = !used_pitchs[pitch];

        if(is_unused_pitch)
        {
            pitchs.push_back(pitch);
        }
    }
}

void
CounterPoint::InitializeVoiceManager ()
{
    this->voice_manager = Voice::Manager();
    this->voice_manager.AddVoice("Soprano", 60, 79);
    this->voice_manager.AddVoice("Contralto", 55, 72);
    this->voice_manager.AddVoice("Tenor", 47, 67);
    this->voice_manager.AddVoice("Baixo", 40, 60);
}

void
CounterPoint::SetVoices (int pitch)
{
    this->cantus_firmus_voice = this->voice_manager.GetVoice(pitch);

    Voice low_voice = this->cantus_firmus_voice;
    this->counter_point_voice = this->voice_manager.GetLowVoice(low_voice);
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
