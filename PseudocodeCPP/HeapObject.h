#pragma once
#include <vector>
class HeapObject
{
public:
	HeapObject();
	virtual ~HeapObject();
	virtual void GetReferencedObjects(std::vector<HeapObject*>& objqueue) const;
};

