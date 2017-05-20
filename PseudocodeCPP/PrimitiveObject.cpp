#include "stdafx.h"
#include "PrimitiveObject.h"

#include "RuntimeError.h"
#include <sstream>
#include <string>

RuntimeError TypeConvertError(PrimitiveType from, PrimitiveType to) {
	std::ostringstream formatter;
	formatter << "Conversion error: Unable to implicitly convert ";
	formatter << TypeToString(from);
	formatter << " to ";
	formatter << TypeToString(to);
	std::string str = formatter.str();
	return RuntimeError(str.c_str());
}

void PrimitiveObject::DestroyValue() {
	PrimitiveType oldType = Type;
	Type = ObjType_Null;
	if (GC && oldType == ObjType_HeapObj) {
		GC->DecrementRefCount(Data.RefValue, OnStack);
	}
}

PrimitiveObject::PrimitiveObject() {
	Type = ObjType_Null;
	GC = nullptr;
	OnStack = false;
}

PrimitiveObject::PrimitiveObject(GarbageCollector * gc) {
	Type = ObjType_Null;
	GC = gc;
	OnStack = false;
}

PrimitiveObject::PrimitiveObject(const PrimitiveObject & other) {
	Type = other.Type;
	GC = other.GC;
	OnStack = other.OnStack;
	if (GC && other.Type == ObjType_HeapObj) {
		GC->IncrementRefCount(other.Data.RefValue, OnStack);
	}
	Data = other.Data;
}

PrimitiveObject::PrimitiveObject(PrimitiveObject && other) {
	Type = other.Type;
	GC = other.GC;
	OnStack = other.OnStack;
	Data = other.Data;
	other.Type = ObjType_Null;
}

PrimitiveObject::PrimitiveObject(const PrimitiveObject & other, bool weakRef) {
	Type = other.Type;
	GC = weakRef ? nullptr : other.GC;
	OnStack = other.OnStack;
	if (GC && other.Type == ObjType_HeapObj) {
		GC->IncrementRefCount(other.Data.RefValue, OnStack);
	}
	Data = other.Data;
}

PrimitiveObject::PrimitiveObject(PrimitiveType typeValue, GarbageCollector * gc) {
}

PrimitiveObject::PrimitiveObject(bool boolValue, GarbageCollector * gc) {
}

PrimitiveObject::PrimitiveObject(int64_t intValue, GarbageCollector * gc) {
}

PrimitiveObject::PrimitiveObject(double realValue, GarbageCollector * gc) {
}

PrimitiveObject::PrimitiveObject(HeapObject * refValue, GarbageCollector * gc) {
}

void PrimitiveObject::swap(PrimitiveObject & r) {
	using std::swap;
	swap(Type, r.Type);
	swap(GC, r.GC);
	swap(OnStack, r.OnStack);
	swap(Data, r.Data);
}

PrimitiveObject & PrimitiveObject::operator=(const PrimitiveObject& other) {
	PrimitiveObject othercopy(other);
	this->swap(othercopy);
	return *this;
}

PrimitiveObject & PrimitiveObject::operator=(PrimitiveObject && other) {
	DestroyValue();
	Type = other.Type;
	GC = other.GC;
	OnStack = other.OnStack;
	Data = other.Data;
	other.Type = ObjType_Null;
	return *this;
}

PrimitiveObject::~PrimitiveObject() {
	DestroyValue();
}

PrimitiveType PrimitiveObject::GetType() const {
	return Type;
}

bool PrimitiveObject::IsNull() const {
	return Type == ObjType_Null;
}

PrimitiveType PrimitiveObject::GetTypeValue() const {
	if (Type == ObjType_Type) return Data.TypeValue;
	else throw TypeConvertError(Type, ObjType_Type);
}

bool PrimitiveObject::GetBoolValue() const {
	if (Type == ObjType_Bool) return Data.BoolValue;
	else throw TypeConvertError(Type, ObjType_Bool);
}

int64_t PrimitiveObject::GetIntValue() const {
	switch (Type) {
	case ObjType_Int: return Data.IntValue;
	case ObjType_Type: return (int64_t)Data.TypeValue;
	default: throw TypeConvertError(Type, ObjType_Int);
	}
}

double PrimitiveObject::GetRealValue() const {
	switch (Type) {
	case ObjType_Int: return (double)Data.IntValue;
	case ObjType_Type: return (double)Data.TypeValue;
	case ObjType_Real: return Data.RealValue;
	default: throw TypeConvertError(Type, ObjType_Real);
	}
}

HeapObject* PrimitiveObject::GetHeapObjectValue() const {
	if (Type == ObjType_HeapObj) return Data.RefValue;
	else throw TypeConvertError(Type, ObjType_HeapObj);
}

GarbageCollector* PrimitiveObject::GetGarbageCollector() const {
	return GC;
}

void PrimitiveObject::SetNull() {
	DestroyValue();
}

void PrimitiveObject::SetTypeValue(PrimitiveType t) {
	DestroyValue();
	Type = ObjType_Type;
	Data.TypeValue = t;
}

void PrimitiveObject::SetBoolValue(bool b) {
	DestroyValue();
	Type = ObjType_Bool;
	Data.BoolValue = b;
}

void PrimitiveObject::SetIntValue(int64_t i) {
	DestroyValue();
	Type = ObjType_Int;
	Data.IntValue = i;
}

void PrimitiveObject::SetRealValue(double d) {
	DestroyValue();
	Type = ObjType_Real;
	Data.RealValue = d;
}

void PrimitiveObject::SetHeapObjectValue(HeapObject* r) {
	if (GC) GC->Suspend();
	DestroyValue();
	if (GC) {
		GC->IncrementRefCount(r, OnStack);
		GC->Resume();
	}
	Type = ObjType_HeapObj;
	Data.RefValue = r;
}

void PrimitiveObject::SetGCAndNull(GarbageCollector * gc) {
	PrimitiveObject r(gc);
	r.SetOnStack(OnStack);
	this->swap(r);
}

void PrimitiveObject::SetOnStack(bool v) {
	if (Type == ObjType_HeapObj) {
		if (v != OnStack) {
			if (GC) {
				GC->IncrementRefCount(Data.RefValue, v);
				GC->DecrementRefCount(Data.RefValue, OnStack);
			}
		}
	}
	OnStack = v;
}

void PrimitiveObject::SuspendGC() {
	if (GC) GC->Suspend();
}

void PrimitiveObject::ResumeGC() {
	if (GC) GC->Resume();
}
