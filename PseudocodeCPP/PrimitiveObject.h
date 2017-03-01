#pragma once

#include <cstdint>
#include <string>

#include "PcObjectType.h"
#include "HeapObject.h"

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

	std::string StrValue;
	bool OnStack;
	void DestroyValue();

public:
	PrimitiveObject();
	~PrimitiveObject();
	PrimitiveType GetType() const;
	bool IsNull() const;
	PrimitiveType GetTypeValue() const;
	bool GetBoolValue() const;
	int64_t GetIntValue() const;
	double GetRealValue() const;
	std::string GetStringValue() const;
	HeapObject* GetHeapObjectValue() const;
	void SetNull();
	void SetTypeValue(PrimitiveType t);
	void SetBoolValue(bool b);
	void SetIntValue(int64_t i);
	void SetRealValue(double d);
	void SetStringValue(const std::string& s);
	void SetStringValue(const std::string&& s);
	void SetHeapObjectValue(HeapObject* r);
};

