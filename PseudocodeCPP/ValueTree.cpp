#include "stdafx.h"
#include "ValueTree.h"


template<class T>
ValueTree<T>::ValueTree() {
	_Value = nullptr;
	for (int i = 0; i < 16; i++) {
		ChildNodes[i] = nullptr;
	}
}

template<class T>
ValueTree<T>::~ValueTree() {
	Clear();
}

template<class T>
ValueTree<T>::ValueTree(const ValueTree & other) {
	if (other._Value) {
		_Value = new T(*other._Value);
	} else {
		_Value = nullptr;
	}
	for (int i = 0; i < 16; i++) {
		if (other.ChildNodes[i]) {
			ChildNodes[i] = new ValueTree(other.ChildNodes[i]);
		} else {
			ChildNodes[i] = nullptr;
		}
	}
}

template<class T>
ValueTree<T>::ValueTree(ValueTree && other) {
	_Value = nullptr;
	for (int i = 0; i < 16; i++) {
		ChildNodes[i] = nullptr;
	}
	swap(other);
}

template<class T>
void ValueTree<T>::swap(ValueTree & r) {
	std::swap(_Value, other._Value);
	std::swap(ChildNodes, other.ChildNodes);
}

template<class T>
ValueTree<T>& ValueTree<T>::operator=(ValueTree other) {
	swap(other);
	return *this;
}

template<class T>
ValueTree<T>& ValueTree<T>::operator=(ValueTree && other) {
	swap(other);
	return *this;
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

template<class T>
void ValueTree<T>::Clear() {
	if (_Value) delete _Value;
	_Value = nullptr;
	for (int i = 0; i < 16; i++) {
		if (ChildNodes[i]) delete ChildNodes[i];
		ChildNodes[i] = nullptr;
	}
}

template<class T>
bool ValueTree<T>::HasValue() const {
	return (bool)_Value;
}

template<class T>
const T & ValueTree<T>::Value() const {
	return *_Value;
}

template<class T>
void ValueTree<T>::SetValue(const T & v) {
	if (_Value) {
		*_Value = v;
	} else {
		_Value = new T(v);
	}
}

template<class T>
bool ValueTree<T>::RemoveValue() {
	if (_Value) delete _Value;
	_Value = nullptr;
}
