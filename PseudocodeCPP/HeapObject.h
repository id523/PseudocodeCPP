#pragma once
#include <vector>
class HeapObject
{
public:
	HeapObject();
	virtual ~HeapObject();
	virtual void GetReferencedObjects(std::vector<HeapObject*>& objqueue) const;
	// TODO: Objects should each contain:
	//   members, accessible by string and by numeric index
	//   a code block (a vector of bytes)
};

