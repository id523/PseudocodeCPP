#pragma once
#include "MapHeapObject.h"
#include <stack>

class StackHeapObject :
	public MapHeapObject {
private:
	std::vector<PrimitiveObject> stack;
public:
	StackHeapObject();
	virtual ~StackHeapObject();
	virtual void GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const;
	virtual void CustomCommand(size_t cmd, VirtualMachine& vm);
};

