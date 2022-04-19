#include "functions.h"
#include "LABPR_v_next.h"


using namespace std;

void getPortFromArg(int argc, int& currPort, char* argv[])
{
    stringstream ss;

    if (argc == SERVER_ARG_COUNT)
        currPort = INIT_PORT;
    else {
        ss << (string)argv[2];
        ss >> currPort;
    }  
}

int main(int argc, char* argv[])
{
    string mainPath = argv[1];

    int currPort;

    getPortFromArg(argc, currPort, argv);

    GetFiles(mainPath, currPort);

    cout << "DIR: " << mainPath << endl << endl;

    system("PAUSE");
    return 0;
}