#pragma once

#include <vector>
#include <unordered_map>

#include "HeapObjectGetReferences.h"

class GarbageCollector {
private:
	std::unordered_map<HeapObject*, size_t> stackrefcount; // The number of references to each object which reside on the stack.
	std::unordered_map<HeapObject*, size_t> totalrefcount; // The number of references to each object in both the stack and heap.
	std::vector<HeapObject*> objects; // The objects whose reference counts need to be decremented.
	uint64_t randstate; // The state of an internal pseudorandom number generator.
	bool DecrementWithoutCollect(HeapObject* ref); // Decrements the heap reference count of a variable without collecting it.
	template<class T> T Pick(std::vector<T>& fromvec); // Picks and removes an item from a vector.
	size_t RandNext(size_t max); // Generates a "random" (term used very loosely here) index below specified maximum.
	unsigned int suspense; // If this value is greater than zero, then objects should not be collected as soon as their reference count reaches zero.
public:
	GarbageCollector();
	~GarbageCollector();
	void IncrementRefCount(HeapObject* ref, bool stack);
	void DecrementRefCount(HeapObject* ref, bool stack);
	void Suspend(); // Causes the garbage collector not to collect things when their reference count reaches zero.
	void Resume(); // Undoes the effect of a matching Suspend(). If the garbage collector is completely unsuspended, this calls FastCollect().
	void FastCollect(); // Frees all of the objects with a reference count of zero.
	void SlowCollect(); // Frees all of the objects which are inaccessible.
};

template<class T>
inline T GarbageCollector::Pick(std::vector<T>& fromvec) {
	if (fromvec.empty()) {
		throw RuntimeError("Memory error: Unable to pick an item from an empty container.");
	} else if (fromvec.size() <= 2) {
		T result = *fromvec.begin();
		fromvec.erase(fromvec.begin());
		return result;
	} else {
		size_t loc = RandNext(fromvec.size() - 1);
		const auto last = fromvec.rbegin();
		T result = fromvec[loc];
		fromvec[loc] = *last;
		fromvec.pop_back();
		return result;
	}
}
