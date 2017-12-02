#include "stdafx.h"
#include "VirtualMachine.h"
#include "MapHeapObject.h"

VirtualMachine::VirtualMachine() : _GC(new GarbageCollector()), Completed(false) {
	_GlobalObject = new MapHeapObject();
	_GC->IncrementRefCount(_GlobalObject, true);
	IP = InstructionIndex(_GC.get(), _GlobalObject, 0);
	AllocationCounter = 0;
	AllocationCounterMax = 32768;
}

GarbageCollector* VirtualMachine::GetGC() {
	return _GC.get();
}

HeapObject* VirtualMachine::GetGlobalObject() {
	return _GlobalObject;
}

void VirtualMachine::Reset() {
	IP.Jump(_GlobalObject);
	_GC->Suspend();
	while (!MainStack.empty()) MainStack.pop_back();
	while (!CallStack.empty()) CallStack.pop();
	_GC->Clear();
	_GC->Resume();
	AllocationCounter = 0;
	Completed = false;
}

void VirtualMachine::Run() {
	while (!Completed) Step();
}

void VirtualMachine::CollectGarbage() {
	_GC->SlowCollect();
	AllocationCounter = 0;
}

VirtualMachine::~VirtualMachine() {

}
