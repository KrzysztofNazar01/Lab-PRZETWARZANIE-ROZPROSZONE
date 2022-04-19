#include "functions.h"

/* Source for server-client code:
* https://cpp0x.pl/kursy/Kurs-WinAPI-C++/Zaawansowane/Winsock/371
* 
*/

void handleClientSocketError(const SOCKET& mainSocket, bool& retflag)
{
    retflag = true;
    if (mainSocket == INVALID_SOCKET)
    {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        WSACleanup();
        return;
    }
    retflag = false;
}

void connectClientToServer(int port, const SOCKET& mainSocket, bool& retflag)
{
    retflag = true;
    sockaddr_in service;
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(port);

    if (connect(mainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        printf("Failed to connect.\n");
        WSACleanup();
        return;
    }
    retflag = false;
}

void sendStringFromClientToServer(std::string& content, const SOCKET& mainSocket, bool& retflag)
{
    retflag = true;
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;
    const char* sendbuf = content.c_str();
    const int recvbufSize = 64;
    char recvbuf[recvbufSize] = "";

    bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);
    printf("Bytes sent: %ld\n", bytesSent);

    while (bytesRecv == SOCKET_ERROR)
    {
        bytesRecv = recv(mainSocket, recvbuf, recvbufSize, 0);

        if (bytesRecv == 0 || bytesRecv == WSAECONNRESET)
        {
            printf("Connection closed.\n");
            break;
        }

        if (bytesRecv < 0)
            return;
    }
    retflag = false;
}

void getClient(int port, string content)
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != NO_ERROR)
        printf("Initialization error.\n");

    SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bool retflag;
    handleClientSocketError(mainSocket, retflag);
    if (retflag) return;

    connectClientToServer(port, mainSocket, retflag);
    if (retflag) return;

    cout << "Connected to the server" << endl;

    sendStringFromClientToServer(content, mainSocket, retflag);
    if (retflag) return;

    closesocket(mainSocket);
    WSACleanup();
}


void receiveMsgFromClient(const SOCKET& mainSocket, char  recvbuf[8000])
{
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;
    const int sendbufize = 64;
    char sendbuf[sendbufize] = "Server hello world";

    bytesRecv = recv(mainSocket, recvbuf, 8000, 0);
    printf("Received text: %s\n", recvbuf);

    bytesSent = send(mainSocket, sendbuf, strlen(sendbuf), 0);

    closesocket(mainSocket);
    WSACleanup();
}

void setServerService(sockaddr_in& service, int port)
{
    memset(&service, 0, sizeof(service));
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(port); 
}

void connectClientToServer(SOCKET& acceptSocket, const SOCKET& mainSocket)
{
    printf("Waiting for a client to connect...\n");

    while (acceptSocket == SOCKET_ERROR)
        acceptSocket = accept(mainSocket, NULL, NULL);
}

void handleServerInitError(int result)
{
    if (result != NO_ERROR)
        printf("Initialization error.\n");
}

void handleServerInvalidSocketError(const SOCKET& mainSocket, bool& retFlag)
{
    if (mainSocket == INVALID_SOCKET)
    {
        printf("Error creating socket: %ld\n", WSAGetLastError());
        WSACleanup();
        retFlag = true;
    }
}


void handleServerSocketError(const SOCKET& mainSocket, sockaddr_in& service, bool& retFlag)
{

    if (bind(mainSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
    {
        printf("bind() failed.\n");
        closesocket(mainSocket);
        retFlag = true;
    }
}

string getServer(int port)
{
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    handleServerInitError(result);

    SOCKET mainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bool retFlag = false;
    handleServerInvalidSocketError(mainSocket, retFlag);
    if (retFlag) return "Server Socket Invalid Socket Error";

    sockaddr_in service;
    setServerService(service, port);
    handleServerSocketError(mainSocket, service, retFlag);
    if (retFlag) return "Server Socket Error";


    if (listen(mainSocket, 1) == SOCKET_ERROR)
        printf("Error listening on socket.\n");

    SOCKET acceptSocket = SOCKET_ERROR;
    connectClientToServer(acceptSocket, mainSocket);

    printf("Client connected.\n");
    mainSocket = acceptSocket;

    char recvbuf[8000] = "";
    receiveMsgFromClient(mainSocket, recvbuf);

    return recvbuf;
}

void printFolder(std::string& path, std::string& stringToPass)
{
    for (auto& entry : fs::directory_iterator(path))
    {
        string dir = "\t" + entry.path().string() + "\n";
        cout << dir;
        stringToPass += dir;
    }
}

void createNewProcess(int port, std::string& dir, std::string& stringToPass, bool& retflag)
{
    retflag = true;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
   
    int newPort = port + 1;

    string mainPath = "C:\\Users\\krzyc\\source\\repos\\LABPR_v_next\\Debug\\LABPR_v_next.exe ";
    string stringPort = " " + to_string(newPort);
    string command = mainPath + dir + stringPort;

    TCHAR commandTChar[300];
    _tcscpy_s(commandTChar, CA2T(command.c_str()));

    if (!CreateProcess(NULL,commandTChar,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si,&pi))
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }
    stringToPass += getServer(newPort);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    retflag = false;
}
void handleNewFoldersInDirectory(std::string& path, int port, std::string& stringToPass, bool& retflag)
{
    retflag = true;
    for (auto& entry : fs::directory_iterator(path))
    {
        string dir = entry.path().string();

        if (entry.is_directory())
        {
            fs::path p(dir);

            bool retflag;
            createNewProcess(port, dir, stringToPass, retflag);
            if (retflag) return;
        }
    }
    retflag = false;
}
void GetFiles(string path, int port)
{
    cout << "Current Directory: " << path << endl;
    string stringToPass = "\n" + path + "\n";
    printFolder(path, stringToPass);

    bool retflag;
    handleNewFoldersInDirectory(path, port, stringToPass, retflag);
    if (retflag) return;

    if (port == INIT_PORT)
        cout << "Ending print:" << endl << stringToPass << endl;
    else
        getClient(port, stringToPass);
}



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



