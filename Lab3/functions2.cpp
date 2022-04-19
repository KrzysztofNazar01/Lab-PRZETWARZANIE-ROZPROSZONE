
#include "functions2.h"


void* GetFiles(void* paraminfo)
{
	ResultStruct* parameters=(ResultStruct*) paraminfo;
	bool isDirinfo = fs::is_directory(parameters->filepath);
	
	Finfo* file = new Finfo(isDirinfo,pthread_self(), parameters->filepath,parameters->filepath, parameters->parent);
	
	cout<<" [ "<< pthread_self()<<  " ] ";
	
	cout << file->name << endl;
	
	if (file->isFolder) 
	{
		for (auto& entry : fs::directory_iterator(parameters->filepath))
		{
			std::string str = entry.path().string();
			char* currpath = new char[str.length() + 1];
			strcpy(currpath, str.c_str());

		   	ResultStruct* temp = new ResultStruct(currpath, file);
		   	
			if(fs::is_directory(currpath))
			{
				pthread_t tid;
				pthread_create(&tid,NULL,GetFiles,temp);
				pthread_join(tid, NULL);
			}
			
			file->children.push_back(temp->result);
		}
	}
	parameters->result=file;
	return NULL;
}

void WriteFiles(Finfo* dir)
{	
	cout << dir->name << endl;

	for (auto file : dir->children)
	{
		Finfo* temp = file;
		while (temp->parent != NULL)
		{
			cout << "\t";
			temp = temp->parent;
		}
		if (file->isFolder)
			WriteFiles(file);
		else
			cout << file->name << endl;

	}
}


