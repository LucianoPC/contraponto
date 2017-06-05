#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "MuMaterial.h"
#include "counter_point.hpp"
#include "voice.hpp"

using namespace std;

void ValidateArguments (int argc, char* argv[]);

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

    material.AddVoices(1);
    material.SetVoice(1, counter_point_material, 0);

    material.SetInstrument(0, 2);
    material.SetInstrument(1, 3);

    material.Show();

    material.SetDefaultFunctionTables();
    material.Score(output_path + "score");
    material.Orchestra(output_path + "orchestra");


    Voice voice = counter_point.cantus_firmus_voice;
    cout << "Cantus Firmus Voice: " << voice.name << endl;

    voice = counter_point.counter_point_voice;
    cout << "Counter Point Voice: " << voice.name << endl;

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
