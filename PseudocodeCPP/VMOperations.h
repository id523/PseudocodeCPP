#pragma once
#include "VirtualMachine.h"
namespace VMOperations {
	typedef void(*UnaryOperation)(PrimitiveObject& x);
	typedef void(*BinaryOperation)(PrimitiveObject& x, const PrimitiveObject& y);
	size_t GetFramePointer(VirtualMachine& m);
	void EnsureFrame(VirtualMachine& m, size_t framelength, size_t& FramePointer);
	void EnsureFrame(VirtualMachine& m, size_t framelength);
	void Ret(VirtualMachine& m);
	void NormalCall(VirtualMachine& m);
	void TailCall(VirtualMachine& m);
	void Jump(VirtualMachine& m);
	void TrueJump(VirtualMachine& m);
	void FalseJump(VirtualMachine& m);
	void MathOp(VirtualMachine& m, UnaryOperation op);
	void MathOp(VirtualMachine& m, BinaryOperation op);
	void Null(VirtualMachine& m);
	void Bool(VirtualMachine& m, bool v);
	void IntLiteral(VirtualMachine& m);
	void RealLiteral(VirtualMachine& m);
	void TypeLiteral(VirtualMachine& m);
	void CreateObject(VirtualMachine& m);
	void ShallowCopy(VirtualMachine& m);
	void GetGlobalObject(VirtualMachine& m);
	void GetFunctionObject(VirtualMachine& m);
	void GetMember(VirtualMachine& m);
	void SetMember(VirtualMachine& m);
	void GetMemberDynamic(VirtualMachine& m);
	void SetMemberDynamic(VirtualMachine& m);
	void PushFrame(VirtualMachine& m);
	void PopFramePointer(VirtualMachine& m);
	void PopFrame(VirtualMachine& m);
	void Pick(VirtualMachine& m);
	void Bury(VirtualMachine& m);
	void PopDiscard(VirtualMachine& m);
	HeapObject* PopHeapObj(VirtualMachine& m);
	void ClearCode(VirtualMachine& m);
	void ClearText(VirtualMachine& m);
	void AppendCode(VirtualMachine& m);
	void AppendCodeLiteral(VirtualMachine& m);
	void AppendText(VirtualMachine& m);
	void AppendFormat(VirtualMachine& m);
	void PrintText(VirtualMachine& m);
	void DebugLine(VirtualMachine& m);
	void ThrowError(VirtualMachine& m);
}