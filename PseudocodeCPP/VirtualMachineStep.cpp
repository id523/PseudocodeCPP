#include "stdafx.h"
#include "VirtualMachine.h"
#include "InstructionType.h"
#include "ObjOperations.h"
#include "RuntimeError.h"

namespace VMOperations {
	typedef void(*UnaryOperation)(PrimitiveObject& x);
	typedef void(*BinaryOperation)(PrimitiveObject& x, const PrimitiveObject& y);
	void EnsureStack(VirtualMachine& m, size_t items) {
		if (m.MainStack.size() < items) 
			throw RuntimeError("Too few items on stack. This means that there is a bug in the compiler");
	}
	void Ret(VirtualMachine& m) {
		if (m.CallStack.empty()) {
			m.Completed = true;
		} else {
			m.IP = std::move(m.CallStack.top());
			m.CallStack.pop();
		}
	}
	void NormalCall(VirtualMachine& m) {
		EnsureStack(m, 1);
		HeapObject* funcObj = m.MainStack.top();
		m.MainStack.pop();
		if (!funcObj) throw RuntimeError("Unable to call null as a function");
		m.CallStack.push(m.IP + 1);
		m.IP.Jump(funcObj);
	}
	void TailCall(VirtualMachine& m) {
		EnsureStack(m, 1);
		HeapObject* funcObj = m.MainStack.top();
		m.MainStack.pop();
		if (!funcObj) throw RuntimeError("Unable to call null as a function");
		m.IP.Jump(funcObj);
	}
	void Jump(VirtualMachine& m) {
		size_t pos = m.IP.ReadPosition();
		m.IP.Jump(pos);
	}
	void TrueJump(VirtualMachine& m) {
		EnsureStack(m, 1);
		size_t pos = m.IP.ReadPosition();
		bool cond = m.MainStack.top();
		m.MainStack.pop();
		if (cond) m.IP.Jump(pos);
	}
	void FalseJump(VirtualMachine& m) {
		EnsureStack(m, 1);
		size_t pos = m.IP.ReadPosition();
		bool cond = m.MainStack.top();
		m.MainStack.pop();
		if (!cond) m.IP.Jump(pos);
	}
	void MathOp(VirtualMachine& m, UnaryOperation op) {
		EnsureStack(m, 1);
		op(m.MainStack.top());
	}
	void MathOp(VirtualMachine& m, BinaryOperation op) {
		EnsureStack(m, 2);
		PrimitiveObject tmp = m.MainStack.top();
		m.MainStack.pop();
		op(m.MainStack.top(), tmp);
	}
	void Bool(VirtualMachine& m, bool v) {
		m.MainStack.push(v);
	}
	void IntLiteral(VirtualMachine& m) {
		m.MainStack.push(m.IP.ReadInteger());
	}
	void RealLiteral(VirtualMachine& m) {
		m.MainStack.push(m.IP.ReadDouble());
	}
	void CreateObject(VirtualMachine& m) {
		PrimitiveObject newObj(m.GetGC(), true);
		newObj = new HeapObject();
		m.MainStack.push(std::move(newObj));
	}
}

void VirtualMachine::Step() {
	if (Completed) return;
	InstructionType cmd = (InstructionType)IP.ReadByte();
	try {
		switch (cmd) {
		case InstructionType::Ret: VMOperations::Ret(*this); break;
		case InstructionType::Call: VMOperations::NormalCall(*this); break;
		case InstructionType::TailCall: VMOperations::TailCall(*this); break;
		case InstructionType::Jump: VMOperations::Jump(*this); break;
		case InstructionType::TrueJump: VMOperations::TrueJump(*this); break;
		case InstructionType::FalseJump: VMOperations::FalseJump(*this); break;
		case InstructionType::BoolNot: VMOperations::MathOp(*this, ObjOperations::BoolNot); break;
		case InstructionType::IntNot: VMOperations::MathOp(*this, ObjOperations::IntNot); break;
		case InstructionType::BoolAnd: VMOperations::MathOp(*this, ObjOperations::BoolAnd); break;
		case InstructionType::IntAnd: VMOperations::MathOp(*this, ObjOperations::IntAnd); break;
		case InstructionType::BoolOr: VMOperations::MathOp(*this, ObjOperations::BoolOr); break;
		case InstructionType::IntOr: VMOperations::MathOp(*this, ObjOperations::IntOr); break;
		case InstructionType::StrictEqual: VMOperations::MathOp(*this, ObjOperations::StrictEqual); break;
		case InstructionType::StrictNeq: VMOperations::MathOp(*this, ObjOperations::StrictNeq); break;
		case InstructionType::NumEqual: VMOperations::MathOp(*this, ObjOperations::NumEqual); break;
		case InstructionType::NumNeq: VMOperations::MathOp(*this, ObjOperations::NumNeq); break;
		case InstructionType::NumLt: VMOperations::MathOp(*this, ObjOperations::NumLt); break;
		case InstructionType::NumGt: VMOperations::MathOp(*this, ObjOperations::NumGt); break;
		case InstructionType::NumLeq: VMOperations::MathOp(*this, ObjOperations::NumLeq); break;
		case InstructionType::NumGeq: VMOperations::MathOp(*this, ObjOperations::NumGeq); break;
		case InstructionType::BoolFalse: VMOperations::Bool(*this, false); break;
		case InstructionType::BoolTrue: VMOperations::Bool(*this, true); break;
		case InstructionType::IntLiteral: VMOperations::IntLiteral(*this); break;
		case InstructionType::RealLiteral: VMOperations::RealLiteral(*this); break;
		case InstructionType::Add: VMOperations::MathOp(*this, ObjOperations::Add); break;
		case InstructionType::Sub: VMOperations::MathOp(*this, ObjOperations::Sub); break;
		case InstructionType::Mul: VMOperations::MathOp(*this, ObjOperations::Mul); break;
		case InstructionType::Neg: VMOperations::MathOp(*this, ObjOperations::Neg); break;
		case InstructionType::IntDiv: VMOperations::MathOp(*this, ObjOperations::IntDiv); break;
		case InstructionType::IntMod: VMOperations::MathOp(*this, ObjOperations::IntMod); break;
		case InstructionType::RealDiv: VMOperations::MathOp(*this, ObjOperations::RealDiv); break;
		case InstructionType::RealMod: VMOperations::MathOp(*this, ObjOperations::RealMod); break;
		case InstructionType::ToInt: VMOperations::MathOp(*this, ObjOperations::ToInt); break;
		case InstructionType::ToReal: VMOperations::MathOp(*this, ObjOperations::ToReal); break;
		case InstructionType::CreateObject: VMOperations::CreateObject(*this); break;
			// TODO
		}
	} catch (RuntimeError err) {
		Completed = true;
		// TODO
	}
}