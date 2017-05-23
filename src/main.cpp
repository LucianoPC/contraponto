#include <iostream>
#include <cstdlib>
#include <string>
#include "MuMaterial.h"

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

    material.SetDefaultFunctionTables();
    material.Score(output_path + "score");
    material.Orchestra(output_path + "orchestra");

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
