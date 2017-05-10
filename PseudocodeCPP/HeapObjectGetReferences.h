#pragma once
#include "HeapObject.forward.h"
#include <vector>
void GetReferencedObjects(const HeapObject& obj, std::vector<HeapObject*>& objqueue);
void DeleteObject(HeapObject* obj);