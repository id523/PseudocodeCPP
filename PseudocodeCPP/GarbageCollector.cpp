#include "stdafx.h"
#include "GarbageCollector.h"

#include "RuntimeError.h"
#include <unordered_set>

GarbageCollector::GarbageCollector() {
	randstate = 4283798574385720ui64;
}

GarbageCollector::~GarbageCollector() {
	Clear();
}

void GarbageCollector::IncrementRefCount(const HeapObject * ref, bool stack) {
	if (!ref) return; // Do nothing if ref is null
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

size_t GarbageCollector::RandNext(size_t max) {
	randstate *= 6364136223846793005ui64;
	randstate += 1442695040888963407ui64;
	return (size_t)((randstate >> 16) % max);
}

void GarbageCollector::DecrementRefCount(const HeapObject * ref, bool stack) {
	if (!ref) return; // Do nothing if ref is null
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
	objects.push_back(ref);
	FastCollect(false);
}

void GarbageCollector::Suspend() {
	suspense++;
}

void GarbageCollector::Resume() {
	if (suspense > 0) suspense--;
	FastCollect(false);
}

void GarbageCollector::FastCollect(bool force) {
	if (force || suspense == 0) {
		// Suspend to prevent recursion
		suspense++;
		// While there are objects to decrement the reference count of,
		while (!objects.empty()) {
			// Pick an object and decrement its reference count
			const HeapObject* ref = Pick(objects);
			// If there is an entry in the reference-count map, decrement it
			if (totalrefcount.count(ref) > 0) {
				totalrefcount[ref]--;
				// If the resulting refcount is zero, remove the object
				if (totalrefcount[ref] <= 0) {
					totalrefcount.erase(ref);
					DeleteObject(ref);
				}
			}
			// Otherwise error
			else throw RuntimeError("Memory error: Unable to decrease reference count of unknown object.");
		}
		suspense--;
	}
}

void GarbageCollector::SlowCollect() {
	FastCollect(true);
	size_t FreeCount = 0;
	std::unordered_set<const HeapObject*> whiteSet; // Objects which have not been referenced yet
	std::vector<const HeapObject*> grayList; // Objects to check for referencing
	std::vector<const HeapObject*> referenceList; // Temporarily stores the references each object has
	// Get set of all known objects into whiteSet
	size_t ObjCount = 0;
	for (const auto& kvp : totalrefcount) {
		whiteSet.insert(kvp.first);
		ObjCount++;
	}
	
	// Move objects to the gray list if they are directly accessible from the stack
	size_t StackCount = 0;
	for (const auto& kvp : stackrefcount) {
		if (whiteSet.erase(kvp.first)) {
			grayList.push_back(kvp.first);
			StackCount++;
		}
	}
	// Remove objects from the white set if they are accessible from the gray list
	while (!grayList.empty()) {
		// Take an object out of the gray list
		const HeapObject* grayObj = Pick(grayList);
		// Get objects that are referenced by it
		referenceList.clear();
		GetReferencedObjects(*grayObj, referenceList);
		for (const HeapObject* referencedObj : referenceList) {
			// If the object is in the white set, move it to the gray list
			if (whiteSet.erase(referencedObj)) {
				grayList.push_back(referencedObj);
			}
		}
	}
	// All objects that should be deleted are in whiteObj, so disable recursive deallocation.
	suspense++;
	// Free all objects in the white set
	for (const HeapObject* whiteObj : whiteSet) {
		// When whiteObj's deleter is called,
		// its members are added to the 'objects' collection for deallocation.
		DeleteObject(whiteObj);
		FreeCount++;
	}
	// Everything added to 'objects' has already been deleted during the above cleanup, so just delete the reference-count info.
	while (!objects.empty()) {
		// Pick an object and decrement its reference count
		const HeapObject* ref = Pick(objects);
		// If there is an entry in the reference-count map, decrement it
		if (totalrefcount.count(ref) > 0) {
			totalrefcount[ref]--;
			// If the resulting refcount is zero, remove the entry from the refcount table
			if (totalrefcount[ref] <= 0) {
				totalrefcount.erase(ref);
			}
		}
		// Otherwise error
		else throw RuntimeError("Memory error: Unable to decrease reference count of unknown object.");
	}
	suspense--;
	printf("%d objects\n", ObjCount);
	printf("%d objects on stack\n", StackCount);
	printf("%d objects freed\n", FreeCount);
}

void GarbageCollector::Clear() {
	for (auto& item : totalrefcount) {
		DeleteObject(item.first);
	}
	totalrefcount.clear();
	stackrefcount.clear();
	objects.clear();
}
