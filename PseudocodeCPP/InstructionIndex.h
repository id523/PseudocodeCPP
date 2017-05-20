#pragma once
#include "Byte.h"
#include "GarbageCollector.h"
#include "HeapObject.h"
#include <cstdint>
#include <string>
#include <utility>

// This will create a division-by-zero if double is not eight bytes in size
enum CheckDoublesRightSize {
	DoublesRightSize = sizeof(double) == 8 ? 1 : 1 / 0
};

class InstructionIndex {
private:
	GarbageCollector* GC;
	const HeapObject* FunctionRef;
public:
	size_t Offset;
	InstructionIndex();
	InstructionIndex(GarbageCollector* gc);
	InstructionIndex(GarbageCollector* gc, const HeapObject* funcref, size_t offset);
	InstructionIndex(const InstructionIndex& other);
	InstructionIndex(InstructionIndex&& other);
	void swap(InstructionIndex& r);
	inline friend void swap(InstructionIndex& a, InstructionIndex& b) {
		a.swap(b);
	}
	InstructionIndex& operator=(const InstructionIndex& other);
	InstructionIndex& operator=(InstructionIndex&& other);
	GarbageCollector* GetGarbageCollector() const;
	void SetGCAndNull(GarbageCollector* gc);
	void Jump(const HeapObject* funcref, size_t offset);
	InstructionIndex& operator=(const HeapObject* funcref);
	InstructionIndex& operator=(size_t offset);
	InstructionIndex& operator++();
	InstructionIndex operator++(int);
	InstructionIndex& operator+=(size_t offset);
	byte operator*();
	byte ReadByte();
	size_t ReadOffset();
	uint64_t ReadUnsignedInteger();
	int64_t ReadInteger();
	double ReadDouble();
	std::string ReadString();
	const HeapObject* GetFunctionRef();
	void SuspendGC();
	void ResumeGC();
	~InstructionIndex();
};

