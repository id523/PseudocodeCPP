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
	if (pos > Code.size()) return InstructionType::Ret;
	return Code[pos];
}

void GetReferencedObjects(const HeapObject& obj, std::vector<const HeapObject*>& objqueue) {
	for (const auto& item : obj.Members) {
		if (item.second.GetType() == ObjType_HeapObj) {
			objqueue.push_back(item.second.GetHeapObjectValue());
		}
	}
}

void DeleteObject(const HeapObject* obj) {
	delete obj;
}