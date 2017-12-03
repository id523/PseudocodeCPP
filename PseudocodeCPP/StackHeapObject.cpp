#include "stdafx.h"
#include "StackHeapObject.h"

#include "VMOperations.h"
#include "VirtualMachine.h"
#include "RuntimeError.h"

StackHeapObject::StackHeapObject() {
}

StackHeapObject::~StackHeapObject() {
}

void StackHeapObject::GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const {
	for (const PrimitiveObject& item : stack) {
		if (item.GetType() == ObjType_HeapObj) {
			objqueue.push_back(item);
		}
	}
}

void StackHeapObject::CustomCommand(size_t cmd, VirtualMachine & vm) {
	// 0 = count
	// 1 = push
	// 2 = pop
	switch (cmd) {
	case 0: // count
		vm.MainStack.emplace_back((int64_t)stack.size(), vm.GetGC(), true);
		break;
	case 1: // push
		VMOperations::EnsureFrame(vm, 1);
		stack.push_back(vm.MainStack.back());
		vm.MainStack.pop_back();
		stack.back().SetOnStack(false);
		break;
	case 2: // pop
		if (stack.empty()) {
			vm.MainStack.emplace_back(vm.GetGC(), true);
		} else {
			vm.MainStack.push_back(stack.back());
			vm.MainStack.back().SetOnStack(true);
			stack.pop_back();
		}
		break;
	default:
		throw RuntimeError("Invalid custom command for this object. This is normally caused by a compiler error.");
		break;
	}
}
