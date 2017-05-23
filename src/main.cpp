#include "MuMaterial.h"

using namespace std;

int main()
{
    MuInit();

    MuMaterial material;

    material.LoadScore("cantus_score");

    material.SetDefaultFunctionTables();
    material.Score("./score");
    material.Orchestra("./orchestra");

    return 0;
}
