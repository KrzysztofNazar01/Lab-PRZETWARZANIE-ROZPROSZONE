#include "functions.h"

string mainPath = "C:\\Users\\krzyc\\source\\repos\\LABPR_v_next\\LABPR_v_next";
string pipeName = "\\\\.\\pipe\\pipepipepipe";

void passStringToNamedPipe(string name, string content)
{
    wstring temp = wstring(pipeName.begin(), pipeName.end());
    const wchar_t* nameW = temp.c_str();
    int maxNumberOfPipes = 30;
    HANDLE pipeHandle = CreateNamedPipe(nameW,PIPE_ACCESS_DUPLEX,PIPE_TYPE_BYTE,maxNumberOfPipes,0,0,0,NULL);
    if (pipeHandle == NULL)
    {
        cout << "Failed to create outbound pipe instance due to NULL" << GetLastError() << endl;
        system("pause");
        return;
    }
    if (pipeHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Failed to create outbound pipe instance due to INVALID_HANDLE_VALUE" << GetLastError() << endl;
        system("pause");
        return;
    }

    // This call blocks until a client process connects to the pipe
    BOOL resultConnectNamedPipe = ConnectNamedPipe(pipeHandle, NULL);

    /*

    unsigned long ulBytesWritten = 0;
    bool bWriteOk = WriteFile( hPipe,
                               ( LPCVOID )&message, 
                               sizeof( Message ),
                               &ulBytesWritten,
                               NULL );
   */

    // This call blocks until a client process reads all the data
    temp = wstring(content.begin(), content.end());
    const wchar_t* data = temp.c_str();
    DWORD numBytesWritten = 0;
    BOOL resultWriteFile = WriteFile(pipeHandle,data,wcslen(data) * sizeof(wchar_t),&numBytesWritten,NULL);
    if (!resultWriteFile || !resultConnectNamedPipe) cout << "Failed to send data." << GetLastError() << endl;
    CloseHandle(pipeHandle);
}

string getStringFromNamedPipe(string name)
{
    wstring tempString = wstring(pipeName.begin(), pipeName.end());
    const wchar_t* nameW = tempString.c_str();
    HANDLE pipeHandle;
    do{
        pipeHandle = CreateFile(L"\\\\.\\pipe\\pipepipepipe",GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    } while (pipeHandle == INVALID_HANDLE_VALUE);
    Sleep(1);//Process writing has to first listen
    int N = 1000;
    wchar_t *textBuffer = new wchar_t[N + 1];
    DWORD numBytesRead = 0;
    BOOL resultReadFile = ReadFile(pipeHandle,textBuffer,N * sizeof(wchar_t),&numBytesRead,NULL);
    if (resultReadFile) {
        textBuffer[numBytesRead / sizeof(wchar_t)] = '\0';
    }
    else {
        cout << "Failed to read data from the pipe." << GetLastError() << endl;
        return "ERROR";
    }

    /*
        if( bConnected ){
        while( true ){ 
        unsigned long ulBytesRead = 0;
        // read client requests from the pipe. 
        bool bReadOk = ReadFile( hPipe,
                    &message,
                    sizeof( message ),
                    &ulBytesRead,
                    NULL );
    */

    CloseHandle(pipeHandle);
    wstring ws(textBuffer);
    string resultString(ws.begin(), ws.end());

    delete[] textBuffer;
    return resultString;
}


void createNewProcess(string folderName) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    string mainPathToExe = "C:\\Users\\krzyc\\source\\repos\\LABPR_v_next\\Debug\\LABPR_v_next.exe";
    string fullCommand = mainPathToExe + ' ' + folderName;
    TCHAR commandTChar[200];
    _tcscpy_s(commandTChar, CA2T(fullCommand.c_str()));

    // Start the child process. 
    if (!CreateProcess(NULL,commandTChar,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

Folder* GetFiles(string path)
{
    cout << "Current directory: " << path << endl;

    Folder* folder = new Folder();

    string passWithPipe = "";
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

            cout << endl  << temp->name << endl;

            //send message to mainwindow
            if (path == mainPath)
                cout << getStringFromNamedPipe(path);
            
            passWithPipe += temp->name + '\n';


            /*  
            
            
            case WM_COPYDATA:
            *
            *     LPARAM lParam;
    COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
    if (pcds->dwData == 1)
    {
        LPCTSTR lpszString = (LPCTSTR)(pcds->lpData);
        // do something with lpszString...
        cout << "received: " <<  lpszString << endl;
    }
                LPARAM lParam;
                pcData = (PCOPYDATASTRUCT)lParam;
                HWND dhwnd = FindMyTopMostWindow();
                switch (pcData->dwData)
                {
                case iMessage:
                    MessageBox(dhwnd, (LPCWSTR)(LPSTR)((DATA*)(pcData->lpData))->strOrgName, (LPCWSTR)"Organisation Name", MB_OK);
                    MessageBox(dhwnd, (LPCWSTR)(LPSTR)((DATA*)(pcData->lpData))->strLocation, (LPCWSTR)"Location", MB_OK);
                }



            */
        }
    }


    fs::directory_entry* main = new fs::directory_entry(fs::path(path));
    folder->is_file = false;
    folder->name = main->path().string();
    folder->parent = NULL;

    //receive message

    passStringToNamedPipe(path, passWithPipe);

    /*
    LPARAM lParam;
     MessageBox(dhwnd, (LPCWSTR)(LPSTR)((DATA*)(pcData->lpData))->strOrgName, (LPCWSTR)"Organisation Name", MB_OK);
                    MessageBox(dhwnd, (LPCWSTR)(LPSTR)((DATA*)(pcData->lpData))->strLocation, (LPCWSTR)"Location", MB_OK);
    COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
    if (pcds->dwData == 1)
    {
        LPCTSTR lpszString = (LPCTSTR)(pcds->lpData);
        // do something with lpszString...
        cout << "received: " <<  lpszString << endl;
    }


    */

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



