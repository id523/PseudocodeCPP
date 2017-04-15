#include "stdafx.h"
#include "ValueTree.h"

template<class T>
ValueTree<T>::ValueTree() {
}

template<class T>
ValueTree<T>::~ValueTree() {
	for (int i = 0; i < 16; i++) {
		if (ChildNodes[i]) delete ChildNodes[i];
	}
}

template<class T>
ValueTree<T>* ValueTree<T>::Subtree(byte key, bool add) {
	int high = (key >> 4) & 0x0F;
	int low = key & 0x0F;
	ValueTree<T>* highNode = ChildNodes[high];
	if (!highNode) {
		if (add) {
			highNode = new ValueTree<T>();
			ChildNodes[high] = highNode;
		} else {
			return nullptr;
		}
	}
	ValueTree<T>* lowNode = highNode->ChildNodes[low];
	if (add && !lowNode) {
		lowNode = new ValueTree<T>();
		highNode->ChildNodes[low] = lowNode;
	}
	return lowNode;
}

template<class T>
ValueTree<T>* ValueTree<T>::Subtree(byte key) const {
	return Subtree(key, false);
}

template<class T>
bool ValueTree<T>::DeleteSubtree(byte key) {
	int high = (key >> 4) & 0x0F;
	int low = key & 0x0F;
	ValueTree<T>* highNode = ChildNodes[high];
	if (!highNode) {
		return false;
	}
	ValueTree<T>*& lowNode = highNode->ChildNodes[low];
	if (lowNode) {
		delete lowNode;
		lowNode = nullptr;
	} else {
		return false;
	}
}
