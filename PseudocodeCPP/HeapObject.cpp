#include "stdafx.h"
#include "HeapObject.h"
#include "InstructionType.h"


HeapObject::HeapObject()
{
}


HeapObject::~HeapObject()
{
}

void HeapObject::GetReferencedObjects(std::vector<HeapObject*>& objqueue) const
{
}

void HeapObject::ClearCode()
{
	Code.clear();
}

byte HeapObject::GetCodeAt(size_t pos)
{
	if (pos > Code.size()) return InstructionType::Ret;
}
