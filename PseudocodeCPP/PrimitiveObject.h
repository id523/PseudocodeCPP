#pragma once

#include <cstdint>
#include <string>

#include "PrimitiveType.h"
#include "HeapObject.h"
#include "GarbageCollector.h"

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
	PrimitiveObject(GarbageCollector* gc);
	PrimitiveObject(const PrimitiveObject& other);
	PrimitiveObject(PrimitiveObject&& other);
	void swap(PrimitiveObject& r);
	PrimitiveObject& operator=(PrimitiveObject other);
	PrimitiveObject& operator=(PrimitiveObject&& other);
	~PrimitiveObject();
	PrimitiveType GetType() const;
	bool IsNull() const;
	PrimitiveType GetTypeValue() const;
	bool GetBoolValue() const;
	int64_t GetIntValue() const;
	double GetRealValue() const;
	HeapObject* GetHeapObjectValue() const;
	GarbageCollector* GetGarbageCollector() const;
	void SetNull();
	void SetTypeValue(PrimitiveType t);
	void SetBoolValue(bool b);
	void SetIntValue(int64_t i);
	void SetRealValue(double d);
	void SetHeapObjectValue(HeapObject* r);
	void SetGCAndNull(GarbageCollector* gc);
	void SetOnStack(bool v);
	void SuspendGC();
	void ResumeGC();
};
namespace std {
	template<>
	void swap(PrimitiveObject& lhs, PrimitiveObject& rhs) {
		lhs.swap(rhs);
	}
}


