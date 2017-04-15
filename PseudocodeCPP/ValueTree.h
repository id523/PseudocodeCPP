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
	ValueTree* Subtree(byte key, bool add);
	ValueTree* Subtree(byte key) const;
	template<class String> ValueTree* Subtree(String key, bool add);
	template<class String> ValueTree* Subtree(String key) const;
	bool DeleteSubtree(byte key);
	bool HasValue() const;
	const T& Value() const;
	void SetValue(const T& v);
	bool DestroyValue();
	void Clear();
	// TODO: Iterator over keys/values
};

template<class T>
template<class String>
inline ValueTree<T>* ValueTree<T>::Subtree(String key, bool add) {
	ValueTree* currSubtree = this;
	for (byte c : key) {
		if (currSubtree) {
			currSubtree = currSubtree->Subtree(c, add);
		} else return nullptr;
	}
	return currSubtree;
}

template<class T>
template<class String>
inline ValueTree<T>* ValueTree<T>::Subtree(String key) const {
	ValueTree* currSubtree = this;
	for (byte c : key) {
		if (currSubtree) {
			currSubtree = currSubtree->Subtree(c);
		} else return nullptr;
	}
	return currSubtree;
}
