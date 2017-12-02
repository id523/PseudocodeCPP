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

void HeapObject::GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const {
}

PrimitiveObject HeapObject::GetMember(std::string key) const {
	return PrimitiveObject(nullptr, nullptr, false);
}

void HeapObject::SetMember(std::string key, const PrimitiveObject & value) {
}

void HeapObject::SetMember(std::string key, PrimitiveObject && value) {
}

void HeapObject::DeleteMember(std::string key) {
}

void GetReferencedObjects(const HeapObject& obj, std::vector<const HeapObject*>& objqueue) {
	obj.GetReferencedObjects(objqueue);
}

void DeleteObject(const HeapObject* obj) {
	delete obj;
}