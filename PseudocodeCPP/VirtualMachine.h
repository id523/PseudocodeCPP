#pragma once
#include "GarbageCollector.h"
#include "InstructionIndex.h"
#include "PrimitiveObject.h"
#include <memory>
#include <stack>

class VirtualMachine {
private:
	std::unique_ptr<GarbageCollector> _GC;
	HeapObject* _GlobalObject;
	void Step();
public:
	bool Completed;
	InstructionIndex IP;
	std::stack<PrimitiveObject> MainStack;
	std::stack<InstructionIndex> CallStack;
	VirtualMachine();
	~VirtualMachine();
	GarbageCollector* GetGC();
	HeapObject* GetGlobalObject();
	void Reset();
	void Run();
};

