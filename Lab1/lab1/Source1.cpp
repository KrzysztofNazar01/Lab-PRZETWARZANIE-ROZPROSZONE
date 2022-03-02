#define _crt_secure_no_warnings
#include <string>
#include <list>
#include <iostream>
#include <windows.h>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

#define FolderType 0
#define FileType 1

class Node {
	public:
		string name;
		string date;
		int type;
		int size;
		Node* children;
};

class Folder : public Node {
	public:
		list<Node*> children;
		string path = "./";
};

void getFiles(Folder* Fol, string Dir);
void handleDir(Node*& node, const std::filesystem::directory_entry& entry);
void writeFiles(Folder* Fol, int iter);
void writeSingleFile(int iter, Node* curr);

int main() {
	string root = "./";
	Folder* F = new Folder;
	getFiles(F, root);
	writeFiles(F, 0);
}

void getFiles(Folder* n, string path) {
	Node* node = new Node;
	Node* folder = new Folder;
	for (const auto& entry : fs::directory_iterator(path)) {
		Node* node = new Node;
		node->type = FileType; //type set to file
		if (entry.is_directory())
			handleDir(node, entry);
		n->children.push_back(node); //save children
		node->name = string(entry.path().u8string()); //save the name with character type char8_t
		node->size = entry.file_size(); //save size
	}
}

void handleDir(Node*& node, const std::filesystem::directory_entry& entry){
	Node* file = new Node;
	node = new Folder;
	node->type = FolderType;
	getFiles((Folder*)node, string(entry.path().u8string()));//recursively get files from the directory
}

void writeFiles(Folder* Fol, int iter) {
	cout << Fol->name << "\n"; //Folder name and next line
	for (Node* curr : Fol->children) //Write all the files
		writeSingleFile(iter, curr);
}

void writeSingleFile(int iter, Node* curr){
	for (int i = 0; i < iter; i++)
		cout << "\t"; //tabs for better visual effect
	if (curr->type == FileType) cout << curr->name << endl;
	else writeFiles((Folder*)curr, iter + 1);//recursively write files from the folder
}
