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
	size_t FunctionPos;
	size_t LineNumber;
	InstructionIndex();
	InstructionIndex(GarbageCollector* gc);
	InstructionIndex(GarbageCollector* gc, const HeapObject* funcref, size_t pos);
	InstructionIndex(const InstructionIndex& other);
	InstructionIndex(InstructionIndex&& other);
	~InstructionIndex();
	void swap(InstructionIndex& r);
	inline friend void swap(InstructionIndex& a, InstructionIndex& b) {
		a.swap(b);
	}
	InstructionIndex& operator=(const InstructionIndex& other);
	InstructionIndex& operator=(InstructionIndex&& other);
	GarbageCollector* GetGarbageCollector() const;
	void SetGCAndNull(GarbageCollector* gc);
	void Jump(const HeapObject* funcref, size_t pos);
	void Jump(const HeapObject* funcref);
	void Jump(size_t pos);
	InstructionIndex& operator++();
	InstructionIndex operator++(int);
	InstructionIndex& operator+=(int offset);
	InstructionIndex& operator-=(int offset);
	InstructionIndex operator+(int offset);
	InstructionIndex operator-(int offset);
	byte PeekByte();
	byte ReadByte();
	size_t ReadPosition();
	uint64_t ReadUnsignedInteger();
	int64_t ReadInteger();
	double ReadDouble();
	std::string ReadString();
	const HeapObject* GetFunctionRef();
	void SuspendGC();
	void ResumeGC();
};

