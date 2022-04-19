#include "functions.h"

void createNewProcess(string folderName) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    string mainPath = "C:\\Users\\krzyc\\source\\repos\\LABPR_v_next\\Debug\\LABPR_v_next.exe";
    string fullCommand = mainPath + ' ' + folderName;
    TCHAR commandTChar[200];
    _tcscpy_s(commandTChar, CA2T(fullCommand.c_str()));

    // Start the child process. 
    if (!CreateProcess(NULL,   // No module name (use command line)
        commandTChar,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    cout << "PROCESS ID: " << GetCurrentProcessId() << endl;

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

Folder* GetFiles(string path)
{
    cout << "Current directory: " << path << endl;

    Folder* folder = new Folder();
    for (auto& entry : fs::directory_iterator(path))
    {
        string dir = entry.path().string();
        //folder
        if (!fs::is_regular_file(entry))
        {
            fs::path p(dir);
            createNewProcess(dir);
        }
        //plik
        else
        {
            Finfo* temp = new Finfo();
            temp->is_file = true;

            temp->name = entry.path().string();
            temp->parent = folder;

            folder->children.push_back(temp);

            cout << temp->name << endl;
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



