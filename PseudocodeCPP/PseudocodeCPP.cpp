// PseudocodeCPP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "VirtualMachine.h"
#include "RuntimeError.h"
#include <iostream>
#include <fstream>

#define BUFSIZE 1024
using namespace std;

int main(size_t argc, char** argv) {
	VirtualMachine vm;
	HeapObject* globalObj = vm.GetGlobalObject();
	vector<byte>& code = globalObj->Code;
	std::string codefileName;
	if (argc > 1) {
		codefileName = argv[1];
	} else {
		cout << "Enter code file name: ";
		getline(cin, codefileName);
	}
	ifstream codefile(codefileName, ios::in | ios::binary);
	char buf[BUFSIZE];
	size_t readChars = BUFSIZE;
	while (codefile.read(buf, BUFSIZE)) {
		for (size_t i = 0; i < readChars; i++) {
			code.push_back(buf[i]);
		}
	}
	readChars = codefile.gcount();
	for (size_t i = 0; i < readChars; i++) {
		code.push_back(buf[i]);
	}
	try {
		vm.Run();
		cout << "Program Complete" << endl;
	} catch (const RuntimeError& err) {
		cout << "Error: " << err.what() << endl;
	}
	getchar();
	return 0;
}

