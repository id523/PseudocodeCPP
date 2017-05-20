#include "stdafx.h"
#include "InstructionIndex.h"


InstructionIndex::InstructionIndex() : 
	GC(nullptr), FunctionRef(nullptr), Offset(0) {}

InstructionIndex::InstructionIndex(GarbageCollector * gc) : 
	GC(gc), FunctionRef(nullptr), Offset(0) {}

InstructionIndex::InstructionIndex(GarbageCollector * gc, const HeapObject* funcref, size_t offset) {
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

InstructionIndex & InstructionIndex::operator=(const InstructionIndex& other) {
	InstructionIndex othercopy(other);
	swap(othercopy);
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

void InstructionIndex::Jump(const HeapObject * funcref, size_t offset) {
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

InstructionIndex & InstructionIndex::operator=(const HeapObject * funcref) {
	Jump(funcref, 0);
	return *this;
}

InstructionIndex & InstructionIndex::operator=(size_t offset) {
	Offset = offset;
	return *this;
}

InstructionIndex & InstructionIndex::operator++() {
	++Offset;
	return *this;
}

InstructionIndex & InstructionIndex::operator++(int) {
	InstructionIndex copy = *this;
	++*this;
	return copy;
}

InstructionIndex & InstructionIndex::operator+=(size_t offset) {
	Offset += offset;
	return *this;
}

byte InstructionIndex::operator*() {
	if (!FunctionRef) return 0;
	return FunctionRef->GetCodeAt(Offset);
}

byte InstructionIndex::ReadByte() {
	if (!FunctionRef) return 0;
	return FunctionRef->GetCodeAt(Offset++);
}

size_t InstructionIndex::ReadOffset() {
	if (!FunctionRef) return 0;
	size_t result = FunctionRef->GetCodeAt(Offset);
	result <<= 8; result |= FunctionRef->GetCodeAt(Offset + 1);
	result <<= 8; result |= FunctionRef->GetCodeAt(Offset + 2);
	result <<= 8; result |= FunctionRef->GetCodeAt(Offset + 3);
	Offset += 4;
	return result;
}

uint64_t InstructionIndex::ReadUnsignedInteger() {
	if (!FunctionRef) return 0;
	uint64_t result = FunctionRef->GetCodeAt(Offset);
	for (int i = 1; i < 8; i++) {
		result <<= 8;
		result |= FunctionRef->GetCodeAt(Offset + i);
	}
	Offset += 8;
	return result;
}

int64_t InstructionIndex::ReadInteger() {
	return (int64_t)ReadUnsignedInteger();
}

double InstructionIndex::ReadDouble() {
	uint64_t intbits = ReadUnsignedInteger();
	// Reinterpret the bit-pattern of the integer as if it was a double
	return *(double*)&intbits;
}

std::string InstructionIndex::ReadString() {
	if (!FunctionRef) return std::string();
	size_t length = FunctionRef->GetCodeAt(Offset++);
	std::string result;
	result.reserve(length);
	for (size_t i = 0; i < length; i++) {
		result.push_back(FunctionRef->GetCodeAt(Offset++));
	}
	return result;
}

const HeapObject * InstructionIndex::GetFunctionRef() {
	return FunctionRef;
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
