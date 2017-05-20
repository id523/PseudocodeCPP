#pragma once
#include "GarbageCollector.h"
#include "HeapObject.forward.h"
#include "PrimitiveType.h"
#include <cstdint>

class PrimitiveObject {
private:
	PrimitiveType Type;
	union DataUnion {
		PrimitiveType TypeValue;
		bool BoolValue;
		int64_t IntValue;
		double RealValue;
		HeapObject* RefValue;
	} Data;
	GarbageCollector* GC;
	bool OnStack;
	void DestroyValue();
public:
	PrimitiveObject();
	PrimitiveObject(GarbageCollector* gc, bool onstack = false);
	PrimitiveObject(const PrimitiveObject& other);
	PrimitiveObject(PrimitiveObject&& other);
	PrimitiveObject(const PrimitiveObject& other, bool weakRef);
	PrimitiveObject(PrimitiveType typeValue, GarbageCollector* gc = nullptr, bool onstack = false);
	PrimitiveObject(bool boolValue, GarbageCollector* gc = nullptr, bool onstack = false);
	PrimitiveObject(int64_t intValue, GarbageCollector* gc = nullptr, bool onstack = false);
	PrimitiveObject(double realValue, GarbageCollector* gc = nullptr, bool onstack = false);
	PrimitiveObject(HeapObject* refValue, GarbageCollector* gc = nullptr, bool onstack = false);
	void swap(PrimitiveObject& r);
	inline friend void swap(PrimitiveObject& a, PrimitiveObject& b) {
		a.swap(b);
	}
	PrimitiveObject& operator=(const PrimitiveObject& other);
	PrimitiveObject& operator=(PrimitiveObject&& other);
	~PrimitiveObject();
	PrimitiveType GetType() const;
	bool IsNull() const;
	operator PrimitiveType() const;
	operator bool() const;
	operator int64_t() const;
	operator double() const;
	operator HeapObject*() const;
	GarbageCollector* GetGarbageCollector() const;
	PrimitiveObject& operator=(std::nullptr_t);
	PrimitiveObject& operator=(PrimitiveType t);
	PrimitiveObject& operator=(bool b);
	PrimitiveObject& operator=(int64_t i);
	PrimitiveObject& operator=(double d);
	PrimitiveObject& operator=(HeapObject* r);
	void SetGCAndNull(GarbageCollector* gc);
	void SetOnStack(bool v);
	void SuspendGC();
	void ResumeGC();
};
