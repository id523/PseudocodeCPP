#include "stdafx.h"

#include "GarbageCollector.h"
#include "RuntimeError.h"



GarbageCollector::GarbageCollector()
{
}

GarbageCollector::~GarbageCollector()
{
}

void GarbageCollector::IncrementRefCount(HeapObject * ref, bool stack)
{
	if (stack) {
		// If there is an entry in the reference-count map, increment it
		if (stackrefcount.count(ref) > 0) stackrefcount[ref]++;
		// Otherwise create an entry with the value 1
		else stackrefcount[ref] = 1;
	}
	
	// If there is an entry in the reference-count map, increment it
	if (totalrefcount.count(ref) > 0) totalrefcount[ref]++;
	// Otherwise create an entry with the value 1
	else totalrefcount[ref] = 1;
}

bool GarbageCollector::DecrementWithoutCollect(HeapObject * ref)
{
	// If there is an entry in the reference-count map, decrement it
	if (totalrefcount.count(ref) > 0) {
		totalrefcount[ref]--;
		// If the resulting refcount is zero, remove that key from the map
		if (totalrefcount[ref] <= 0) {
			totalrefcount.erase(ref);
			// Notify the caller that the reference count has dropped to zero
			return true;
		} else {
			// Notify the caller that the reference count has not dropped to zero
			return false;
		}
	}
	// Otherwise error
	else throw RuntimeError("Memory error: Unable to decrease reference count of unknown object.");
}

size_t GarbageCollector::RandNext(size_t max)
{
	randstate *= 6364136223846793005ui64;
	randstate += 1442695040888963407ui64;
	return (size_t)((randstate >> 16) % max);
}

void GarbageCollector::DecrementRefCount(HeapObject * ref, bool stack)
{
	if (stack) {
		// If there is an entry in the reference-count map, decrement it
		if (stackrefcount.count(ref) > 0) {
			stackrefcount[ref]--;
			// If the resulting refcount is zero, remove that key from the map
			if (stackrefcount[ref] <= 0)
				stackrefcount.erase(ref);
		}
		// Otherwise error
		else throw RuntimeError("Memory error: Unable to decrease reference count of unknown object.");
	}

	bool doDeallocate = DecrementWithoutCollect(ref);
	if (doDeallocate) {
		std::vector<HeapObject*> objects;
		objects.push_back(ref);
		while (!objects.empty()) {
			HeapObject* obj = Pick(objects);

		}
	}
}

void GarbageCollector::SlowCollect()
{
}
