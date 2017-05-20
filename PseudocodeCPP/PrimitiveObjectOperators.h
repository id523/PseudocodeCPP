#pragma once
#include "PrimitiveObject.forward.h"
#include <string>

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
void IntAdd(PrimitiveObject& x, const PrimitiveObject& y);
void IntSub(PrimitiveObject& x, const PrimitiveObject& y);
void IntMul(PrimitiveObject& x, const PrimitiveObject& y);
void IntDiv(PrimitiveObject& x, const PrimitiveObject& y);
void IntMod(PrimitiveObject& x, const PrimitiveObject& y);
void IntNeg(PrimitiveObject& x);
void RealAdd(PrimitiveObject& x, const PrimitiveObject& y);
void RealSub(PrimitiveObject& x, const PrimitiveObject& y);
void RealMul(PrimitiveObject& x, const PrimitiveObject& y);
void RealDiv(PrimitiveObject& x, const PrimitiveObject& y);
void RealMod(PrimitiveObject& x, const PrimitiveObject& y);
void RealNeg(PrimitiveObject& x);
void GetMember(PrimitiveObject& x, const std::string& memberName);
void SetMember(PrimitiveObject& x, const PrimitiveObject& v, const std::string& memberName);