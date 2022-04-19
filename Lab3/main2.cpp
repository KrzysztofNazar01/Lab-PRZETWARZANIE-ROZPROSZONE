#include "functions2.h"

int main() {
	ResultStruct* temp=new ResultStruct("./");
	GetFiles(temp);
	WriteFiles(temp->result);
}

