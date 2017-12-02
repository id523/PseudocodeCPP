#include "stdafx.h"
#include "ArrayHeapObject.h"
#include "RuntimeError.h"
#include <sstream>

ArrayHeapObject::ArrayHeapObject(size_t len) {
	arr = new PrimitiveObject[len];
	length = len;
}

ArrayHeapObject::~ArrayHeapObject() {
	delete[] arr;
}

void ArrayHeapObject::GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const {
	for (int i = 0; i < length; i++) {
		const PrimitiveObject& item = arr[i];
		if (item.GetType() == ObjType_HeapObj) {
			objqueue.push_back(item);
		}
	}
}

PrimitiveObject ArrayHeapObject::GetMember(const std::string& key) const {
	size_t index;
	if (key == "length") {
		return (int64_t)length;
	} else if (ArrayParseIndexFromKey(key, index, length)) {
		return arr[index];
	}
	return PrimitiveObject();
}

void ArrayHeapObject::SetMember(const std::string& key, const PrimitiveObject & value) {
	size_t index;
	if (key == "length") {
		throw RuntimeError("Arrays cannot be resized after they are created.");
	} else if (ArrayParseIndexFromKey(key, index, length)) {
		arr[index] = value;
	}
}

void ArrayHeapObject::SetMember(const std::string& key, PrimitiveObject && value) {
	size_t index;
	if (key == "length") {
		throw RuntimeError("Arrays cannot be resized after they are created.");
	} else if (ArrayParseIndexFromKey(key, index, length)) {
		arr[index] = std::move(value);
	}
}

void ArrayHeapObject::DeleteMember(const std::string& key) {
	size_t index;
	if (key == "length") {
		throw RuntimeError("Arrays cannot be resized after they are created.");
	} else if (ArrayParseIndexFromKey(key, index, length)) {
		arr[index] = nullptr;
	}
}

bool ArrayParseIndexFromKey(const std::string & key, size_t& index, size_t length) {
	std::istringstream reader = std::istringstream(key, std::ios_base::in);
	char firstChars[2];
	reader.read(firstChars, 2);
	if (reader && firstChars[0] == '_' && firstChars[1] == '_') {
		size_t idx;
		reader >> idx;
		if (reader) {
			if (idx < length) {
				index = idx;
				return true;
			} else {
				throw RuntimeError("Unable to access items beyond the end of the array.");
			}
		}
	}
	return false;
}
