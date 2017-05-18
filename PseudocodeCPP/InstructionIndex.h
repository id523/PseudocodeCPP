#pragma once

#include <utility>

#include "GarbageCollector.h"
#include "HeapObject.h"
#include "Byte.h"

class InstructionIndex {
private:
	GarbageCollector* GC;
	HeapObject* FunctionRef;
public:
	
	size_t Offset;
	InstructionIndex();
	InstructionIndex(GarbageCollector* gc);
	InstructionIndex(GarbageCollector* gc, HeapObject* funcref, size_t offset);
	InstructionIndex(const InstructionIndex& other);
	InstructionIndex(InstructionIndex&& other);
	void swap(InstructionIndex& r);
	inline friend void swap(InstructionIndex& a, InstructionIndex& b) {
		a.swap(b);
	}
	InstructionIndex& operator=(InstructionIndex other);
	InstructionIndex& operator=(InstructionIndex&& other);
	GarbageCollector* GetGarbageCollector() const;
	void SetGCAndNull(GarbageCollector* gc);
	void Jump(HeapObject* funcref, size_t offset);
	InstructionIndex& operator=(HeapObject* funcref);
	InstructionIndex& operator=(size_t offset);
	InstructionIndex& operator++();
	InstructionIndex& operator++(int);
	InstructionIndex& operator+=(size_t offset);
	byte& operator*();
	void SuspendGC();
	void ResumeGC();
	~InstructionIndex();
};

