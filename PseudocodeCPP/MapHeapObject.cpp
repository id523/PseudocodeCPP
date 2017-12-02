#include "stdafx.h"
#include "MapHeapObject.h"


MapHeapObject::MapHeapObject() {
}


MapHeapObject::~MapHeapObject() {
}

void MapHeapObject::GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const {
	for (const auto& kvp : Members) {
		if (kvp.second.GetType() == ObjType_HeapObj) {
			objqueue.push_back(kvp.second);
		}
	}
}

PrimitiveObject MapHeapObject::GetMember(const std::string& key) const {
	try {
		return Members.at(key);
	} catch (const std::out_of_range&) {
		return PrimitiveObject();
	}
}

void MapHeapObject::SetMember(const std::string& key, const PrimitiveObject & value) {
	Members[key] = value;
}

void MapHeapObject::SetMember(const std::string& key, PrimitiveObject && value) {
	Members[key] = std::move(value);
}

void MapHeapObject::DeleteMember(const std::string& key) {
	Members.erase(key);
}