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

byte HeapObject::GetCodeAt(size_t pos) const {
	if (pos > Code.size()) return (byte)InstructionType::Ret;
	return Code[pos];
}

void GetReferencedObjects(const HeapObject& obj, std::vector<const HeapObject*>& objqueue) {
	for (const auto& item : obj.Members) {
		if (item.second.GetType() == ObjType_HeapObj) {
			objqueue.push_back((HeapObject*)item.second);
		}
	}
}

void DeleteObject(const HeapObject* obj) {
	delete obj;
}