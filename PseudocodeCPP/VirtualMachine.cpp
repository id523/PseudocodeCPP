#include "stdafx.h"
#include "VirtualMachine.h"

VirtualMachine::VirtualMachine() : _GC(new GarbageCollector()), Completed(false) {
	_GlobalObject = new HeapObject();
	_GC->IncrementRefCount(_GlobalObject, true);
	IP = InstructionIndex(_GC.get(), _GlobalObject, 0);
}

GarbageCollector* VirtualMachine::GetGC() {
	return _GC.get();
}

HeapObject* VirtualMachine::GetGlobalObject() {
	return _GlobalObject;
}

void VirtualMachine::Reset() {
	IP.Jump(_GlobalObject);
	while (!MainStack.empty()) MainStack.pop();
	while (!CallStack.empty()) CallStack.pop();
	_GC->SlowCollect();
}

void VirtualMachine::Run() {
	while (!Completed) Step();
}

VirtualMachine::~VirtualMachine() {

}
