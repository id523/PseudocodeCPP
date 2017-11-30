#include "stdafx.h"
#include "HeapObject.h"
#include "InstructionType.h"
#include "RuntimeError.h"

HeapObject::HeapObject() : IsCode(true) {
}


HeapObject::~HeapObject() {
}

void HeapObject::Append(byte b) {
	Code.push_back(b);
}

void HeapObject::ClearCode() {
	Code.clear();
	IsCode = true;
}

void HeapObject::ClearText() {
	Code.clear();
	IsCode = false;
}

byte HeapObject::GetCodeAt(size_t pos) const {
	if (!IsCode) throw RuntimeError("Attempted to execute code from a non-executable object");
	if (pos >= Code.size()) return (byte)InstructionType::Ret;
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