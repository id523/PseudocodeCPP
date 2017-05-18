#include "stdafx.h"
#include "InstructionIndex.h"


InstructionIndex::InstructionIndex() : 
	GC(nullptr), FunctionRef(nullptr), Offset(0) {}

InstructionIndex::InstructionIndex(GarbageCollector * gc) : 
	GC(gc), FunctionRef(nullptr), Offset(0) {}

InstructionIndex::InstructionIndex(GarbageCollector * gc, HeapObject* funcref, size_t offset) {
	GC = gc;
	if (GC) GC->IncrementRefCount(funcref, true);
	FunctionRef = funcref;
	Offset = offset;
}

InstructionIndex::InstructionIndex(const InstructionIndex & other) {
	GC = other.GC;
	if (GC) GC->IncrementRefCount(other.FunctionRef, true);
	FunctionRef = other.FunctionRef;
	Offset = other.Offset;
}

InstructionIndex::InstructionIndex(InstructionIndex && other) :
	GC(nullptr), FunctionRef(nullptr), Offset(0) {
	swap(other);
}

void InstructionIndex::swap(InstructionIndex & r) {
	using std::swap;
	swap(GC, r.GC);
	swap(FunctionRef, r.FunctionRef);
	swap(Offset, r.Offset);
}

InstructionIndex & InstructionIndex::operator=(InstructionIndex other) {
	swap(other);
	return *this;
}

InstructionIndex & InstructionIndex::operator=(InstructionIndex && other) {
	swap(other);
	return *this;
}

GarbageCollector * InstructionIndex::GetGarbageCollector() const {
	return GC;
}

void InstructionIndex::SetGCAndNull(GarbageCollector * gc) {
	if (GC) GC->DecrementRefCount(FunctionRef, true);
	FunctionRef = nullptr;
	Offset = 0;
	GC = gc;
}

void InstructionIndex::Jump(HeapObject * funcref, size_t offset) {
	// FunctionRef != funcref not necessary for correctness but avoids calling expensive GC functions
	bool doGC = GC && FunctionRef != funcref;
	if (doGC) {
		GC->Suspend();
		GC->DecrementRefCount(FunctionRef, true);
		GC->IncrementRefCount(funcref, true);
	}
	FunctionRef = funcref;
	Offset = offset;
	if (doGC) GC->Resume();
}

void InstructionIndex::SuspendGC() {
	if (GC) GC->Suspend();
}

void InstructionIndex::ResumeGC() {
	if (GC) GC->Resume();
}

InstructionIndex::~InstructionIndex() {
	if (GC) GC->DecrementRefCount(FunctionRef, true);
}
