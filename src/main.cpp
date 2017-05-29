#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "MuMaterial.h"
#include "counter_point.hpp"

using namespace std;

void ValidateArguments (int argc, char* argv[]);
void PrintVector (vector<int> v, string message);
vector<int> GetHarmonicRange (int note_pitch);
vector<int> FixPitchsToScale (int scale_pitch, vector<int> pitchs);
vector<int> RemoveUnisonFromPitchs(int unison_pitch, vector<int> pitchs);

int main (int argc, char* argv[])
{
    ValidateArguments(argc, argv);

    string output_path = argc > 2 ? argv[2] : "./";
    string score_file_path = argv[1];

    MuInit();
    MuMaterial material;
    material.LoadScore(argv[1]);

    CounterPoint counter_point(material);
    counter_point.SetScalePitch(60);
    counter_point.PrintHarmonicPitchs();


    MuMaterial counter_point_material = counter_point.GenerateCounterPointMaterial();

    // material.Show();
    // counter_point_material.Show();

    material.AddVoices(1);
    material.SetVoice(1, counter_point_material, 0);

    material.SetInstrument(0, 2);
    material.SetInstrument(1, 3);

    material.Show();

    material.SetDefaultFunctionTables();
    material.Score(output_path + "score");
    material.Orchestra(output_path + "orchestra");

    // counter_point_material.SetDefaultFunctionTables();
    // counter_point_material.Score(output_path + "score_cp");
    // counter_point_material.Orchestra(output_path + "orchestra_cp");

    // MuNote note;
    // MuMaterial material;

    // note.SetPitch(60);
    // material += note;

    // note.SetPitch(62);
    // material += note;

    // note.SetPitch(64);
    // material += note;

    // CounterPoint counter_point(material);
    // counter_point.PrintHarmonicPitchs();

    return 0;
}

void ValidateArguments (int argc, char* argv[])
{
    bool has_arguments = argc < 2;
    if (has_arguments)
    {
        cout << "Usage: " << argv[0] << " [score_file_path] [output_path]" << endl;
        exit(-1);
    }
}

void PrintVector (vector<int> v, string message)
{
    cout << message << ": ";
    for (unsigned int index = 0; index < v.size(); index++)
    {
        cout << v[index] << " ";
    }
    cout << endl;
}

vector<int> GetHarmonicRange (int note_pitch)
{
    int unison = note_pitch;
    int third_minor = note_pitch - 3;
    int third_major = note_pitch - 4;
    int fifith_perfect = note_pitch - 7;
    int sixth_minor = note_pitch - 8;
    int sixth_major = note_pitch - 9;

    vector<int> harmonic_range {unison, third_minor, third_major,
                                  fifith_perfect, sixth_minor, sixth_major};

    return harmonic_range;
}

vector<int> FixPitchsToScale (int scale_pitch, vector<int> pitchs)
{
    scale_pitch = scale_pitch % 12;

    vector<int> scale_pitchs = { 0 + scale_pitch, 2 + scale_pitch,
                                 4 + scale_pitch, 5 + scale_pitch,
                                 7 + scale_pitch, 9 + scale_pitch,
                                 11 + scale_pitch };

    vector<int> pitchs_on_scale;

    for (unsigned int index = 0; index < pitchs.size(); index++)
    {
        int pitch = pitchs[index] % 12;
        bool found_pitch = find(scale_pitchs.begin(), scale_pitchs.end(),
                                pitch) != scale_pitchs.end();

        if(found_pitch)
        {
            pitchs_on_scale.push_back(pitchs[index]);
        }
    }

    return pitchs_on_scale;
}

vector<int> RemoveUnisonFromPitchs(int unison_pitch, vector<int> pitchs)
{
    vector<int> pitchs_without_unison = pitchs;

    pitchs_without_unison.erase(remove(pitchs_without_unison.begin(),
                                       pitchs_without_unison.end(),
                                       unison_pitch),
                                pitchs_without_unison.end());

    return pitchs_without_unison;
}
