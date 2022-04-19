/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// main app


#include "functions.h"
#include <stdio.h>


int main(int argc, char* argv[]) {

    std::string path = "./";

    Folder* folder;

    folder = GetFiles(path);

    WriteFiles(folder);

    return 0;
}
