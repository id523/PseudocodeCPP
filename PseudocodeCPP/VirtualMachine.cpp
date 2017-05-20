#include "stdafx.h"
#include "VirtualMachine.h"

VirtualMachine::VirtualMachine() : _GC(new GarbageCollector()), Completed(false) {
	_GlobalObject = new HeapObject();
	_GC->IncrementRefCount(_GlobalObject, true);
	IP = InstructionIndex(_GC.get(), _GlobalObject, 0);
}

GarbageCollector* VirtualMachine::GC() {
	return _GC.get();
}

HeapObject* VirtualMachine::GlobalObject() {
	return _GlobalObject;
}

void VirtualMachine::Reset() {
	IP = _GlobalObject;
}

void VirtualMachine::Step() {

}

void VirtualMachine::Run() {
	while (!Completed) Step();
}

VirtualMachine::~VirtualMachine() {

}
