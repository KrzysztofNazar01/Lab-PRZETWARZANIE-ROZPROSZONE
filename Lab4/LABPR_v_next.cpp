#include "functions.h"


using namespace std;

void main(int argc, char* argv[])
{
    string mainPath = argv[1];

    Folder* folder = GetFiles(mainPath);

    system("PAUSE");
}