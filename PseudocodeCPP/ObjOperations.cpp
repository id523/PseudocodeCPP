#include "stdafx.h"
#include "ObjOperations.h"
#include "HeapObject.h"
#include "PrimitiveObject.h"
#include "RuntimeError.h"
#include <cmath>

void ObjOperations::BoolNot(PrimitiveObject & x) {
	x = !x;
}

void ObjOperations::IntNot(PrimitiveObject & x) {
	x = ~(int64_t)x;
}

void ObjOperations::BoolAnd(PrimitiveObject & x, const PrimitiveObject & y) {
	x = x && y;
}

void ObjOperations::IntAnd(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (int64_t)x & (int64_t)y;
}

void ObjOperations::BoolOr(PrimitiveObject & x, const PrimitiveObject & y) {
	x = x || y;
}

void ObjOperations::IntOr(PrimitiveObject & x, const PrimitiveObject & y) {
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

void ObjOperations::StrictEqual(PrimitiveObject & x, const PrimitiveObject & y) {
	x = StrictEqual_Do(x, y);
}

void ObjOperations::StrictNeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = !StrictEqual_Do(x, y);
}

#define COMPARE(x, y) (x > y ? 1 : (x < y ? -1 : 0))

int NumCompare(const PrimitiveObject& x, const PrimitiveObject & y) {
	if (x.GetType() == ObjType_Int && y.GetType() == ObjType_Int) {
		int64_t xi = x, yi = y;
		return COMPARE(xi, yi);
	} else {
		double xf = x, yf = y;
		return COMPARE(xf, yf);
	}
}

void ObjOperations::NumEqual(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) == 0;
}

void ObjOperations::NumNeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) != 0;
}

void ObjOperations::NumLt(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) < 0;
}

void ObjOperations::NumGt(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) > 0;
}

void ObjOperations::NumLeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) <= 0;
}

void ObjOperations::NumGeq(PrimitiveObject & x, const PrimitiveObject & y) {
	x = NumCompare(x, y) >= 0;
}

void ObjOperations::Add(PrimitiveObject & x, const PrimitiveObject & y) {
	if (x.GetType() == ObjType_Real || y.GetType() == ObjType_Real) {
		x = (double)x + (double)y;
	} else {
		x = (int64_t)x + (int64_t)y;
	}
}

void ObjOperations::Sub(PrimitiveObject & x, const PrimitiveObject & y) {
	if (x.GetType() == ObjType_Real || y.GetType() == ObjType_Real) {
		x = (double)x - (double)y;
	} else {
		x = (int64_t)x - (int64_t)y;
	}
}

void ObjOperations::Mul(PrimitiveObject & x, const PrimitiveObject & y) {
	if (x.GetType() == ObjType_Real || y.GetType() == ObjType_Real) {
		x = (double)x * (double)y;
	} else {
		x = (int64_t)x * (int64_t)y;
	}
}

void ObjOperations::Neg(PrimitiveObject & x) {
	if (x.GetType() == ObjType_Real) {
		x = -(double)x;
	} else {
		x = -(int64_t)x;
	}
}

void ObjOperations::IntDiv(PrimitiveObject & x, const PrimitiveObject & y) {
	int64_t denominator = y;
	if (denominator == 0) throw RuntimeError("Illegal division by zero.");
	x = (int64_t)x / denominator;
}

void ObjOperations::IntMod(PrimitiveObject & x, const PrimitiveObject & y) {
	int64_t denominator = y;
	// lim(y->0, x%y) = 0
	if (denominator == 0) x = 0i64;
	else x = (int64_t)x % denominator;
}

void ObjOperations::RealDiv(PrimitiveObject & x, const PrimitiveObject & y) {
	x = (double)x / (double)y;
}

void ObjOperations::RealMod(PrimitiveObject & x, const PrimitiveObject & y) {
	double denominator = y;
	// lim(y->0, x%y) = 0
	if (denominator == 0.0) x = 0.0;
	else x = fmod(x, denominator);
}

void ObjOperations::ToInt(PrimitiveObject & x) {
	x = (int64_t)x;
}

void ObjOperations::ToReal(PrimitiveObject & x) {
	x = (double)x;
}

void ObjOperations::GetMember(PrimitiveObject & x, const std::string & memberName) {
	HeapObject* obj = x;
	if (!obj) throw RuntimeError("Cannot access members of a null value");
	x = obj->GetMember(memberName);
	x.SetOnStack(true);
}

void ObjOperations::SetMember(const PrimitiveObject & x, const std::string & memberName, const PrimitiveObject & v) {
	HeapObject* obj = x;
	if (!obj) throw RuntimeError("Cannot access members of a null value");
	PrimitiveObject inserted = v;
	inserted.SetOnStack(false);
	if (inserted.IsNull()) {
		obj->DeleteMember(memberName);
	} else {
		obj->SetMember(memberName, std::move(inserted));
	}
}