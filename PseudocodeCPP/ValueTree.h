#pragma once
#include "Byte.h"

template<class T>
class ValueTree {
private:
	ValueTree* ChildNodes[16];
	T* _Value;
public:
	ValueTree();
	~ValueTree();
	ValueTree(const ValueTree& other);
	ValueTree(ValueTree&& other);
	void swap(ValueTree& r);
	ValueTree& operator=(ValueTree other);
	ValueTree& operator=(ValueTree&& other);
	ValueTree* Subtree(byte key, bool add);
	ValueTree* Subtree(byte key) const;
	bool DeleteSubtree(byte key);
	void Clear();
	bool HasValue() const;
	const T& Value() const;
	void SetValue(const T& Value);
	bool RemoveValue();
};
