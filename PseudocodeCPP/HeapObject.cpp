#include "stdafx.h"
#include "HeapObject.h"
#include "InstructionType.h"


HeapObject::HeapObject(GarbageCollector * gc) {
	GC = gc;
}

HeapObject::~HeapObject() {
}

void HeapObject::GetReferencedObjects(std::vector<HeapObject*>& objqueue) const {
}

void HeapObject::ClearCode() {
	Code.clear();
}

byte HeapObject::GetCodeAt(size_t pos) const {
	if (pos > Code.size()) return InstructionType::Ret;
	return Code[pos];
}

const PrimitiveObject & HeapObject::GetMember(std::string key) const {
	ValueTree<PrimitiveObject>* subtree = Members.Subtree(key);
	if (subtree) return subtree->Value();
	else return PrimitiveObject(GC);
}

void HeapObject::SetMember(std::string key, const PrimitiveObject & value) {
	ValueTree<PrimitiveObject>* subtree = Members.Subtree(key, true);
	subtree->SetValue(value);
}

bool HeapObject::RemoveMember(std::string key) {
	ValueTree<PrimitiveObject>* subtree = Members.Subtree(key);
	if (subtree) {
		return subtree->DestroyValue();
	} else return false;
}

void HeapObject::RemoveMembers() {
	Members.Clear();
}
