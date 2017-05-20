#include "stdafx.h"
#include "PrimitiveObjectOperators.h"
#include "PrimitiveObject.h"
#include "RuntimeError.h"
#include <cmath>

void BoolNot(PrimitiveObject & x) {
	x = !x;
}

void IntNot(PrimitiveObject & x) {
	x = ~(int64_t)x;
}

void BoolAnd(PrimitiveObject & x, const PrimitiveObject & y) {
	x = x && y;
}

void IntAnd(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (int64_t)x & (int64_t)y;
}

void BoolOr(PrimitiveObject & x, const PrimitiveObject & y) {
	x = x || y;
}

void IntOr(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (int64_t)x | (int64_t)y;
}

bool StrictEqual_Do(const PrimitiveObject& x, const PrimitiveObject & y) {
	bool equal = true;
	if (x.GetType() != y.GetType()) {
		equal = false;
	} else {
		switch (x.GetType()) {
		case ObjType_Type: equal = (PrimitiveType)x == (PrimitiveType)y; break;
		case ObjType_Bool: equal = (bool)x == (bool)y; break;
		case ObjType_Int: equal = (int64_t)x == (int64_t)y; break;
		case ObjType_Real: equal = (double)x == (double)y; break;
		case ObjType_HeapObj: equal = (HeapObject*)x == (HeapObject*)y; break;
		default: equal = true; break;
		}
	}
	return equal;
}

void StrictEqual(PrimitiveObject & x, const PrimitiveObject & y) {
	x = StrictEqual_Do(x, y);
}

void StrictNeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = !StrictEqual_Do(x, y);
}

inline int CompareInt(int64_t x, int64_t y) {
	return x > y ? 1 : (x < y ? -1 : 0);
}

inline int CompareFloat(double x, double y) {
	return x > y ? 1 : (x < y ? -1 : 0);
}

int NumCompare(const PrimitiveObject& x, const PrimitiveObject & y) {
	if (x.GetType() == ObjType_Int && y.GetType() == ObjType_Int) {
		return CompareInt(x, y);
	} else {
		return CompareFloat(x, y);
	}
}

void NumEqual(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) == 0;
}

void NumNeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) != 0;
}

void NumLt(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) < 0;
}

void NumGt(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) > 0;
}

void NumLeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) <= 0;
}

void NumGeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) >= 0;
}

void IntAdd(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (int64_t)x + (int64_t)y;
}

void IntSub(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (int64_t)x - (int64_t)y;
}

void IntMul(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (int64_t)x * (int64_t)y;
}

void IntDiv(PrimitiveObject & x, const PrimitiveObject & y) {
	int64_t denominator = y;
	if (denominator == 0) throw RuntimeError("Illegal division by zero.");
	x = (int64_t)x / denominator;
}

void IntMod(PrimitiveObject & x, const PrimitiveObject & y) {
	int64_t denominator = y;
	// lim(y->0, x%y) = 0
	if (denominator == 0) x = 0i64;
	else x = (int64_t)x % denominator;
}

void IntNeg(PrimitiveObject & x) {
	x = -(int64_t)x;
}

void RealAdd(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (double)x + (double)y;
}

void RealSub(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (double)x - (double)y;
}

void RealMul(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (double)x * (double)y;
}

void RealDiv(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (double)x / (double)y;
}

void RealMod(PrimitiveObject & x, const PrimitiveObject & y) {
	double denominator = y;
	// lim(y->0, x%y) = 0
	if (denominator == 0.0) x = 0.0;
	else x = fmod(x, denominator);
}

void RealNeg(PrimitiveObject & x) {
	x = -(double)x;
}

void GetMember(PrimitiveObject & x, const std::string & memberName) {
}

void SetMember(PrimitiveObject & x, const PrimitiveObject & v, const std::string & memberName) {
}