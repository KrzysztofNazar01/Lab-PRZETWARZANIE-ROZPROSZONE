/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// protect against 'multiple definition' error when including more than once
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H



#include <string>
#include <list>
#include <iostream>
#include <experimental/filesystem>
#include <codecvt>
#include <locale>


using namespace std;
namespace fs = std::experimental::filesystem;

//deklaracje
struct Finfo;
struct Folder;

struct Finfo
{
public:
    bool is_file;
    string name;


    Folder* parent;
};

struct Folder : public Finfo
{
    list<Finfo*> children;
};

Folder* GetFiles(string path);
void WriteFiles(Folder* folder);



#endif
