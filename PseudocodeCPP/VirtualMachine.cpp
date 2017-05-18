#include "stdafx.h"
#include "VirtualMachine.h"


VirtualMachine::VirtualMachine() : GC(new GarbageCollector()) {
	GlobalObject = new HeapObject();
	GC->IncrementRefCount(GlobalObject, true);
	CurrentPosition = InstructionIndex(GC.get(), GlobalObject, 0);
}


VirtualMachine::~VirtualMachine() {

}
