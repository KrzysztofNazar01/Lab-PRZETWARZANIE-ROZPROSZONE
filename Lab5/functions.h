#pragma once

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H



#include <string>
#include <list>
#include <iostream>
#include <filesystem>
#include <codecvt>
#include <locale>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <atlstr.h>
#include <stdio.h>
#include <WinUser.h>

using namespace std;
namespace fs = std::filesystem;
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
void createNewProcess(string folderName);

#endif
