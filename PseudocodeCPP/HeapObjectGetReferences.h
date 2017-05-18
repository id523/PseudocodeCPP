#pragma once
#include "HeapObject.forward.h"
#include <vector>
void GetReferencedObjects(const HeapObject& obj, std::vector<const HeapObject*>& objqueue);
void DeleteObject(const HeapObject* obj);