#pragma once
#include "Byte.h"
template<class T>
class ValueTree {
private:
	ValueTree* ChildNodes[16];
public:
	ValueTree();
	~ValueTree();
	ValueTree* Subtree(byte key, bool add);
	ValueTree* Subtree(byte key) const;
	bool DeleteSubtree(byte key);
	// TODO: Access to optional value of type T
};

