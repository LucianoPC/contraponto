#include <iostream>
#include <string>
#include "MuMaterial.h"

using namespace std;

int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " [score_file_path]" << endl;
        return -1;
    }

    MuInit();

    MuMaterial material;

    material.LoadScore(argv[1]);

    material.SetDefaultFunctionTables();
    material.Score("./score");
    material.Orchestra("./orchestra");

    return 0;
}
