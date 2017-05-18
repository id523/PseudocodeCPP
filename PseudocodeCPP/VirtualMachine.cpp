#include "stdafx.h"
#include "VirtualMachine.h"


VirtualMachine::VirtualMachine() : GC(new GarbageCollector()) {
}


VirtualMachine::~VirtualMachine() {
}
