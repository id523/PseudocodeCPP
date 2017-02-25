#pragma once

#include "Byte.h"

enum InstructionType : byte {
	Jump, Conditional,
	LongJump, LongConditional,
	PushFrame, PopFrame,
	SetBreak, Break,
	SetContinue, Continue,
	SetReturn, Return,
	BoolNot, IntNot,
	BoolAnd, IntAnd,
	BoolOr, IntOr,
	BoolEq, IntEq, RealEq, StringEq,
	BoolNeq, IntNeq, RealNeq, StringNeq,
	IntLt, RealLt, IntGt, RealGt,
	IntLeq, RealLeq, IntGeq, RealGeq,
	BoolFalse, BoolTrue, IntLiteral, RealLiteral,
	StrLiteral, StrLongLiteral,
	StringConcat,
	IntAdd, RealAdd, 
	IntSub, RealSub,
	IntMul, RealMul,
	IntDiv, RealDiv,
	IntMod, RealMod,
	IntNeg, RealNeg,
	BoolToString,
	IntToReal, IntToString,
	RealToInt, RealToString,
	StringToBool, StringToInt, StringToReal,
};