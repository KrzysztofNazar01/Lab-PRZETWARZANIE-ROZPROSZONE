#pragma once

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H

#define INIT_PORT 8080
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVER_ARG_COUNT 2
#include <string>
#include <list>
#include <iostream>
#include <filesystem>
#include <codecvt>
#include <ctype.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <atlstr.h>
#include <winuser.h>
#include <stdlib.h>
#include <sstream>

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

void handleClientSocketError(const SOCKET& mainSocket, bool& retflag);
void connectClientToServer(int portID, const SOCKET& mainSocket, bool& retflag);
void sendStringFromClientToServer(std::string& content, const SOCKET& mainSocket, bool& retflag);
void getClient(int portID, string content);
void receiveMsgFromClient(const SOCKET& mainSocket, char  recvbuf[4096]);
void setServerService(sockaddr_in& service, int port);
void connectClientToServer(SOCKET& acceptSocket, const SOCKET& mainSocket);
void handleServerInitError(int result);
void handleServerInvalidSocketError(const SOCKET& mainSocket, bool& retFlag);
void handleServerSocketError(const SOCKET& mainSocket, sockaddr_in& service, bool& retFlag);
string getServer(int portID);
void printFolder(std::string& path, std::string& stringToPass);
void createNewProcess(int port, std::string& dir, std::string& stringToPass, bool& retflag);
void GetFiles(string path, int port);
void handleNewFoldersInDirectory(std::string& path, int port, std::string& stringToPass, bool& retflag);
void WriteFiles(Folder* folder);
#endif
