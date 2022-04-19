/* Distributed Programming
   Lab2 - libraries - example

   (C) by Mariusz Matuszek
   Gdańsk University of Technology
*/


// function definition(s)


#include "functions.h"

Folder* GetFiles(string path)
{

    Folder* folder = new Folder();

    for (auto& entry : fs::directory_iterator(path))
    {

        string dir = entry.path().string();

        //folder
        if (!fs::is_regular_file(entry))
        {
            fs::path p(dir);

            Finfo* temp = GetFiles(dir);
            temp->is_file = false;


            temp->name = entry.path().string();
            temp->parent = folder;

            folder->children.push_back(temp);

        }
        //plik
        else
        {
            Finfo* temp = new Finfo();
            temp->is_file = true;

            temp->name = entry.path().string();
            temp->parent = folder;

            folder->children.push_back(temp);
        }

    }

    fs::directory_entry* main = new fs::directory_entry(fs::path(path));


    folder->is_file = false;

    folder->name = main->path().string();
    folder->parent = NULL;


    return folder;
}

//wypisaæ drzewo dla folderu
void WriteFiles(Folder* folder)
{
    cout << folder->name << endl;

    for (auto file : folder->children)
    {

        Finfo* temp = file;
        while (temp->parent != NULL)
        {
            cout << "\t";
            temp = temp->parent;
        }
        //folder
        if (file->is_file == false)
        {

            WriteFiles((Folder*)file);
        }
        //plik
        else
        {
            cout << file->name << endl;
        }

    }
}
