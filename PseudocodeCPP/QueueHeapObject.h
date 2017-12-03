#pragma once
#include "MapHeapObject.h"
#include <deque>

class QueueHeapObject :
	public MapHeapObject {
private:
	std::deque<PrimitiveObject> queue;
public:
	QueueHeapObject();
	virtual ~QueueHeapObject();
	virtual void GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const;
	virtual void CustomCommand(size_t cmd, VirtualMachine& vm);
};

