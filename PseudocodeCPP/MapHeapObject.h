#pragma once
#include "HeapObject.h"
#include <map>
class MapHeapObject :
	public HeapObject {
private:
	std::map<std::string, PrimitiveObject> Members;
public:
	MapHeapObject();
	virtual ~MapHeapObject();
	virtual void GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const;
	virtual PrimitiveObject GetMember(const std::string& key) const;
	virtual void SetMember(const std::string& key, const PrimitiveObject& value);
	virtual void SetMember(const std::string& key, PrimitiveObject&& value);
	virtual void DeleteMember(const std::string& key);
};

