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

template<class T>
inline ValueTree<T>::ValueTree() {
	_Value = nullptr;
	for (int i = 0; i < 16; i++) {
		ChildNodes[i] = nullptr;
	}
}

template<class T>
inline ValueTree<T>::~ValueTree() {
	Clear();
}

template<class T>
inline ValueTree<T>::ValueTree(const ValueTree & other) {
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
inline ValueTree<T>::ValueTree(ValueTree && other) {
	_Value = nullptr;
	for (int i = 0; i < 16; i++) {
		ChildNodes[i] = nullptr;
	}
	swap(other);
}

template<class T>
inline void ValueTree<T>::swap(ValueTree & r) {
	std::swap(_Value, other._Value);
	std::swap(ChildNodes, other.ChildNodes);
}

template<class T>
inline ValueTree<T>& ValueTree<T>::operator=(ValueTree other) {
	swap(other);
	return *this;
}

template<class T>
inline ValueTree<T>& ValueTree<T>::operator=(ValueTree && other) {
	swap(other);
	return *this;
}

template<class T>
inline ValueTree<T>* ValueTree<T>::Subtree(byte key, bool add) {
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
inline ValueTree<T>* ValueTree<T>::Subtree(byte key) const {
	return Subtree(key, false);
}

template<class T>
inline bool ValueTree<T>::DeleteSubtree(byte key) {
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
inline void ValueTree<T>::Clear() {
	if (_Value) delete _Value;
	_Value = nullptr;
	for (int i = 0; i < 16; i++) {
		if (ChildNodes[i]) delete ChildNodes[i];
		ChildNodes[i] = nullptr;
	}
}

template<class T>
inline bool ValueTree<T>::HasValue() const {
	return (bool)_Value;
}

template<class T>
inline const T & ValueTree<T>::Value() const {
	return *_Value;
}

template<class T>
inline void ValueTree<T>::SetValue(const T & v) {
	if (_Value) {
		*_Value = v;
	} else {
		_Value = new T(v);
	}
}

template<class T>
inline bool ValueTree<T>::RemoveValue() {
	if (_Value) delete _Value;
	_Value = nullptr;
}
