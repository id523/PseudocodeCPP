#pragma once
#include "HeapObject.h"
class ArrayHeapObject :
	public HeapObject {
private:
	PrimitiveObject* arr;
	size_t length;
public:
	ArrayHeapObject(size_t len);
	virtual ~ArrayHeapObject();
	virtual void GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const;
	virtual PrimitiveObject GetMember(const std::string& key) const;
	virtual void SetMember(const std::string& key, const PrimitiveObject& value);
	virtual void SetMember(const std::string& key, PrimitiveObject&& value);
	virtual void DeleteMember(const std::string& key);
};

bool ArrayParseIndexFromKey(const std::string& key, size_t& index, size_t length);