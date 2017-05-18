#pragma once
#include <stack>
#include <memory>

#include "GarbageCollector.h"
class VirtualMachine {
private:
	std::unique_ptr<GarbageCollector> GC;
public:
	VirtualMachine();
	~VirtualMachine();
	// TODO:
	// Call stack (stack of structs containing object-references and integer offsets).
	// Data stack (stack of PrimitiveObjects)
	// Ability to interpret machine code
};

