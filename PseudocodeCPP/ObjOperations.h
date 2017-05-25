#pragma once
#include "PrimitiveObject.forward.h"
#include <string>

namespace ObjOperations {
	void BoolNot(PrimitiveObject& x);
	void IntNot(PrimitiveObject& x);
	void BoolAnd(PrimitiveObject& x, const PrimitiveObject& y);
	void IntAnd(PrimitiveObject& x, const PrimitiveObject& y);
	void BoolOr(PrimitiveObject& x, const PrimitiveObject& y);
	void IntOr(PrimitiveObject& x, const PrimitiveObject& y);
	void StrictEqual(PrimitiveObject& x, const PrimitiveObject& y);
	void StrictNeq(PrimitiveObject& x, const PrimitiveObject& y);
	void NumEqual(PrimitiveObject& x, const PrimitiveObject& y);
	void NumNeq(PrimitiveObject& x, const PrimitiveObject& y);
	void NumLt(PrimitiveObject& x, const PrimitiveObject& y);
	void NumGt(PrimitiveObject& x, const PrimitiveObject& y);
	void NumLeq(PrimitiveObject& x, const PrimitiveObject& y);
	void NumGeq(PrimitiveObject& x, const PrimitiveObject& y);
	void Add(PrimitiveObject& x, const PrimitiveObject& y);
	void Sub(PrimitiveObject& x, const PrimitiveObject& y);
	void Mul(PrimitiveObject& x, const PrimitiveObject& y);
	void Neg(PrimitiveObject& x);
	void IntDiv(PrimitiveObject& x, const PrimitiveObject& y);
	void IntMod(PrimitiveObject& x, const PrimitiveObject& y);
	void RealDiv(PrimitiveObject& x, const PrimitiveObject& y);
	void RealMod(PrimitiveObject& x, const PrimitiveObject& y);
	void ToInt(PrimitiveObject& x);
	void ToReal(PrimitiveObject& x);
	void GetMember(PrimitiveObject& x, const std::string& memberName);
	void SetMember(PrimitiveObject& x, const std::string& memberName, const PrimitiveObject& v);
}