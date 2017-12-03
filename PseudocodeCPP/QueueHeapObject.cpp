#include "stdafx.h"
#include "QueueHeapObject.h"

#include "VMOperations.h"
#include "VirtualMachine.h"
#include "RuntimeError.h"

QueueHeapObject::QueueHeapObject() {
}

QueueHeapObject::~QueueHeapObject() {
}

void QueueHeapObject::GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const {
	for (const PrimitiveObject& item : queue) {
		if (item.GetType() == ObjType_HeapObj) {
			objqueue.push_back(item);
		}
	}
}

void QueueHeapObject::CustomCommand(size_t cmd, VirtualMachine & vm) {
	// 0 = count
	// 1 = enqueue
	// 2 = dequeue
	switch (cmd) {
	case 0: // count
		vm.MainStack.emplace_back((int64_t)queue.size(), vm.GetGC(), true);
		break;
	case 1: // enqueue
		VMOperations::EnsureFrame(vm, 1);
		queue.push_back(vm.MainStack.back());
		vm.MainStack.pop_back();
		queue.back().SetOnStack(false);
		break;
	case 2: // dequeue
		if (queue.empty()) {
			vm.MainStack.emplace_back(vm.GetGC(), true);
		} else {
			vm.MainStack.push_back(queue.front());
			vm.MainStack.back().SetOnStack(true);
			queue.pop_front();
		}
		break;
	default:
		throw RuntimeError("Invalid custom command for this object. This is normally caused by a compiler error.");
		break;
	}
}
