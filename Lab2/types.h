
// protect against 'multiple definition' error when including more than once
#ifndef _TYPES_H_
#define _TYPES_H_


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


#endif
