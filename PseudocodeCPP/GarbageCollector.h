#pragma once

#include <unordered_map>

#include "HeapObject.h"

class GarbageCollector {
private:
	std::unordered_map<HeapObject*, size_t> stackrefcount;
	std::unordered_map<HeapObject*, size_t> totalrefcount;
public:
	GarbageCollector();
	~GarbageCollector();
	void IncrementRefCount(HeapObject* ref, bool stack);
	void DecrementRefCount(HeapObject* ref, bool stack);
	void SlowCollect();
};