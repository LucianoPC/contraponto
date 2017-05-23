#include <iostream>
#include <string>
#include "MuMaterial.h"

using namespace std;

int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " [score_file_path] [output_path]" << endl;
        return -1;
    }

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
