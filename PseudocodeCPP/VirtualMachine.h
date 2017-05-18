#pragma once
#include <stack>
#include <memory>

#include "GarbageCollector.h"
#include "PrimitiveObject.h"
#include "InstructionIndex.h"
class VirtualMachine {
private:
	std::unique_ptr<GarbageCollector> GC;
	std::stack<PrimitiveObject> MainStack;
	std::stack<InstructionIndex> CallStack;
	InstructionIndex CurrentPosition;
	HeapObject* GlobalObject;
public:
	VirtualMachine();
	~VirtualMachine();
	// TODO:
	// Ability to interpret machine code
};

