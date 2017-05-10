#include "stdafx.h"
#include "HeapObject.h"
#include "InstructionType.h"


HeapObject::HeapObject() {
}


HeapObject::~HeapObject() {
}

void HeapObject::ClearCode() {
	Code.clear();
}

byte HeapObject::GetCodeAt(size_t pos) {
	if (pos > Code.size()) return InstructionType::Ret;
	return Code[pos];
}

void GetReferencedObjects(const HeapObject& obj, std::vector<HeapObject*>& objqueue) {

}

void DeleteObject(HeapObject* obj) {
	delete obj;
}