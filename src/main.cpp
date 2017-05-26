#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "MuMaterial.h"

using namespace std;

void ValidateArguments (int argc, char* argv[]);
vector<short> GetMelodicPitchs (MuNote note);
bool isValidMelodicNote (MuNote note_one, MuNote note_two);

int main (int argc, char* argv[])
{
    // ValidateArguments(argc, argv);

    // string output_path = argc > 2 ? argv[2] : "./";
    // string score_file_path = argv[1];

    // MuInit();
    // MuMaterial material;
    // material.LoadScore(argv[1]);

    // material.SetDefaultFunctionTables();
    // material.Score(output_path + "score");
    // material.Orchestra(output_path + "orchestra");


    MuNote note;
    note.SetPitch(62);
    vector<short> melodic_pitchs = GetMelodicPitchs(note);

    for (unsigned int index = 0; index < melodic_pitchs.size(); index++)
    {
        cout << melodic_pitchs[index] << " ";
    }
    cout << endl;

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

vector<short> GetMelodicPitchs (MuNote note)
{
    short key = note.Pitch() % 12;
    vector<short> melodic_pitchs;

    for (int index = 1; index <= 8; index++)
    {
        MuMaterial material;
        material += note;
        material.DiatonicTranspose(0, MAJOR_MODE, index, ASCENDING);

        if(isValidMelodicNote(note, material.GetFirstNote()))
        {
            melodic_pitchs.push_back(material.GetFirstNote().Pitch());
        }

        material.Clear();
        material += note;
        material.DiatonicTranspose(0, MAJOR_MODE, index, DESCENDING);

        if(isValidMelodicNote(note, material.GetFirstNote()))
        {
            melodic_pitchs.push_back(material.GetFirstNote().Pitch());
        }
    }

    sort(melodic_pitchs.begin(), melodic_pitchs.end());

    return melodic_pitchs;
}

bool isValidMelodicNote (MuNote note_one, MuNote note_two)
{
    short interval = note_one.Pitch() - note_two.Pitch();
    short abs_interval = abs(interval);

    cout << "interval: " << abs_interval << "  pitch: " << note_two.Pitch() << endl;

    bool second_minor = abs_interval == 1;
    bool second_major = abs_interval == 2;
    bool third_minor = abs_interval == 3;
    bool third_major = abs_interval == 4;
    bool fourth_perfect = abs_interval == 5;
    bool fifth_perfect = abs_interval == 7;
    bool sixth_minor = interval == 8;
    bool octave = abs_interval == 12;

    bool isValid = second_minor || second_major;
    isValid |= third_minor || third_major;
    isValid |= fourth_perfect;
    isValid |= fifth_perfect;
    isValid |= sixth_minor;
    isValid |= octave;

    return isValid;
}
