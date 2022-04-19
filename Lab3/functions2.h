
// protect against 'multiple definition' error when including more than once
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H

#include <string>
#include <iostream>
#include <experimental/filesystem>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;
namespace fs = std::experimental::filesystem;

class Finfo
{
public:
	pthread_t pid;
	bool isFolder;
	char* name;
	Finfo* parent;
	std::vector<Finfo*> children;
	char* path;
	
	Finfo(bool dirinfo, pthread_t pidinfo, char* pathinfo, char* path, Finfo* parentinfo){
		isFolder = dirinfo;
		pid = pidinfo;
		path = pathinfo;
		name = extractName(path);
		parent = parentinfo;
	}
private:
	char* extractName(char* path)
	{
		char* newPath = (char*)calloc(strlen(path) + 1, sizeof(char));
		strcpy(newPath, path);
		char* pch = strtok(newPath, "/ ");
		char* path2 = NULL;
		while (pch != NULL)
		{
			path2 = pch; newPath = path2;
			pch = strtok(NULL, " / ");
		}
		return path2;
	}
};

class ResultStruct
{
public:
	Finfo* result;
	char* filepath;
	Finfo* parent;
	
	ResultStruct(char* path, Finfo* parentifo = NULL){
		filepath = path;
		parent = parentifo;
		result = NULL;
	}
};

void* GetFiles(void* paraminfo);
void WriteFiles(Finfo* folder);



#endif
