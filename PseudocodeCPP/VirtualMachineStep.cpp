#include "stdafx.h"
#include "VirtualMachine.h"
#include "InstructionType.h"
#include "ObjOperations.h"
#include "RuntimeError.h"
#include <iostream>
#include <sstream>
#include <string>

namespace VMOperations {
	typedef void(*UnaryOperation)(PrimitiveObject& x);
	typedef void(*BinaryOperation)(PrimitiveObject& x, const PrimitiveObject& y);
	size_t GetFramePointer(VirtualMachine& m) {
		size_t FramePointer = 0;
		if (!m.FrameStack.empty()) {
			FramePointer = m.FrameStack.top();
		}
		return FramePointer;
	}
	void EnsureFrame(VirtualMachine& m, size_t framelength, size_t& FramePointer) {
		FramePointer = GetFramePointer(m);
		if (m.MainStack.size() < FramePointer + framelength) 
			throw RuntimeError("Too few items in current stack frame. This is most likely caused by a bug in the compiler");
	}
	void EnsureFrame(VirtualMachine& m, size_t framelength) {
		size_t FramePointer;
		return EnsureFrame(m, framelength, FramePointer);
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
		EnsureFrame(m, 1);
		HeapObject* funcObj = m.MainStack.back();
		if (!funcObj) throw RuntimeError("Unable to call null as a function");
		if (!funcObj->IsCode) throw RuntimeError("Unable to run code from a non-executable object");
		m.MainStack.pop_back();
		m.CallStack.push(m.IP + 1);
		m.IP.Jump(funcObj);
	}
	void TailCall(VirtualMachine& m) {
		EnsureFrame(m, 1);
		HeapObject* funcObj = m.MainStack.back();
		if (!funcObj) throw RuntimeError("Unable to call null as a function");
		if (!funcObj->IsCode) throw RuntimeError("Unable to run code from a non-executable object");
		m.MainStack.pop_back();
		m.IP.Jump(funcObj);
	}
	void Jump(VirtualMachine& m) {
		size_t pos = m.IP.ReadPosition();
		m.IP.Jump(pos);
	}
	void TrueJump(VirtualMachine& m) {
		EnsureFrame(m, 1);
		size_t pos = m.IP.ReadPosition();
		bool cond = m.MainStack.back();
		m.MainStack.pop_back();
		if (cond) m.IP.Jump(pos);
	}
	void FalseJump(VirtualMachine& m) {
		EnsureFrame(m, 1);
		size_t pos = m.IP.ReadPosition();
		bool cond = m.MainStack.back();
		m.MainStack.pop_back();
		if (!cond) m.IP.Jump(pos);
	}
	void MathOp(VirtualMachine& m, UnaryOperation op) {
		EnsureFrame(m, 1);
		op(m.MainStack.back());
	}
	void MathOp(VirtualMachine& m, BinaryOperation op) {
		EnsureFrame(m, 2);
		PrimitiveObject tmp = m.MainStack.back();
		m.MainStack.pop_back();
		try {
			op(m.MainStack.back(), tmp);
		} catch (RuntimeError err) {
			m.MainStack.push_back(std::move(tmp));
			throw err;
		}
	}
	void Null(VirtualMachine& m) {
		m.MainStack.emplace_back(m.GetGC(), true);
	}
	void Bool(VirtualMachine& m, bool v) {
		m.MainStack.emplace_back(v, m.GetGC(), true);
	}
	void IntLiteral(VirtualMachine& m) {
		m.MainStack.emplace_back(m.IP.ReadInteger(), m.GetGC(), true);
	}
	void RealLiteral(VirtualMachine& m) {
		m.MainStack.emplace_back(m.IP.ReadDouble(), m.GetGC(), true);
	}
	void TypeLiteral(VirtualMachine& m) {
		byte b = m.IP.ReadByte();
		if (b < ObjType_Max) {
			m.MainStack.emplace_back((PrimitiveType)b, m.GetGC(), true);
		} else {
			throw RuntimeError("Invalid type literal. This is most likely caused by a bug in the compiler");
		}
	}
	void CreateObject(VirtualMachine& m) {
		m.MainStack.emplace_back(m.GetGC(), true);
		m.MainStack.back() = new HeapObject();
		m.AllocationCounter++;
	}
	void ShallowCopy(VirtualMachine& m) {
		EnsureFrame(m, 1);
		if (m.MainStack.back().GetType() == ObjType_HeapObj) {
			HeapObject* copy = new HeapObject(*(HeapObject*)m.MainStack.back());
			m.MainStack.back() = copy;
			m.AllocationCounter++;
		}
	}
	void GetGlobalObject(VirtualMachine& m) {
		m.MainStack.emplace_back(m.GetGlobalObject(), m.GetGC(), true);
	}
	void GetFunctionObject(VirtualMachine& m) {
		m.MainStack.emplace_back(const_cast<HeapObject*>(m.IP.GetFunctionRef()), m.GetGC(), true);
	}
	void GetMember(VirtualMachine& m) {
		EnsureFrame(m, 1);
		size_t length = m.IP.ReadByte();
		std::string memberName;
		for (size_t i = 0; i < length; i++) {
			memberName.push_back(m.IP.ReadByte());
		}
		ObjOperations::GetMember(m.MainStack.back(), memberName);
	}
	void SetMember(VirtualMachine& m) {
		EnsureFrame(m, 2);
		size_t length = m.IP.ReadByte();
		std::string memberName;
		for (size_t i = 0; i < length; i++) {
			memberName.push_back(m.IP.ReadByte());
		}
		PrimitiveObject tmp = m.MainStack.back();
		m.MainStack.pop_back();
		ObjOperations::SetMember(m.MainStack.back(), memberName, tmp);
		m.MainStack.pop_back();
	}
	void GetMemberDynamic(VirtualMachine& m) {
		EnsureFrame(m, 2);
		size_t length = m.IP.ReadByte();
		std::ostringstream memberName;
		for (size_t i = 0; i < length; i++) {
			memberName.put(m.IP.ReadByte());
		}
		memberName << (int64_t)m.MainStack.back();
		m.MainStack.pop_back();
		std::string memberNameStr = memberName.str();
		ObjOperations::GetMember(m.MainStack.back(), memberNameStr);
	}
	void SetMemberDynamic(VirtualMachine& m) {
		EnsureFrame(m, 3);
		size_t length = m.IP.ReadByte();
		std::ostringstream memberName;
		for (size_t i = 0; i < length; i++) {
			memberName.put(m.IP.ReadByte());
		}
		PrimitiveObject tmp = m.MainStack.back();
		m.MainStack.pop_back();
		memberName << (int64_t)m.MainStack.back();
		m.MainStack.pop_back();
		std::string memberNameStr = memberName.str();
		ObjOperations::SetMember(m.MainStack.back(), memberNameStr, tmp);
		m.MainStack.pop_back();
	}
	void PushFrame(VirtualMachine& m) {
		size_t FrameSize = m.IP.ReadByte();
		EnsureFrame(m, FrameSize);
		m.FrameStack.push(m.MainStack.size() - FrameSize);
	}
	void PopFramePointer(VirtualMachine& m) {
		if (!m.FrameStack.empty()) {
			m.FrameStack.pop();
		}
	}
	void PopFrame(VirtualMachine& m) {
		size_t FrameSize = m.IP.ReadByte();
		size_t FramePointer;
		EnsureFrame(m, FrameSize, FramePointer);
		PopFramePointer(m);
		size_t FrameEnd = FramePointer + FrameSize;
		while (m.MainStack.size() > FrameEnd) {
			m.MainStack.pop_back();
		}
	}
	void Pick(VirtualMachine& m) {
		size_t IndexInFrame = m.IP.ReadByte();
		size_t FramePointer;
		EnsureFrame(m, IndexInFrame + 1, FramePointer);
		size_t ItemIndex = FramePointer + IndexInFrame;
		m.MainStack.push_back(m.MainStack.at(ItemIndex));
	}
	void Bury(VirtualMachine& m) {
		size_t IndexInFrame = m.IP.ReadByte();
		size_t FramePointer;
		EnsureFrame(m, IndexInFrame + 2, FramePointer);
		size_t ItemIndex = FramePointer + IndexInFrame;
		m.MainStack.at(ItemIndex) = m.MainStack.back();
		m.MainStack.pop_back();
	}
	void PopDiscard(VirtualMachine& m) {
		EnsureFrame(m, 1);
		m.MainStack.pop_back();
	}
	HeapObject* PopHeapObj(VirtualMachine& m) {
		EnsureFrame(m, 1);
		HeapObject* obj = m.MainStack.back();
		if (!obj) throw RuntimeError("Unable to modify a null object");
		m.MainStack.pop_back();
		return obj;
	}
	void ClearCode(VirtualMachine& m) {
		HeapObject* obj = PopHeapObj(m);
		obj->ClearCode();
	}
	void ClearText(VirtualMachine& m) {
		HeapObject* obj = PopHeapObj(m);
		obj->ClearText();
	}
	void AppendCode(VirtualMachine& m) {
		size_t length = m.IP.ReadPosition();
		HeapObject* obj = PopHeapObj(m);
		if (!obj->IsCode) throw RuntimeError("Unable to append code to a non-executable object");
		for (size_t i = 0; i < length; i++) {
			obj->Append(m.IP.ReadByte());
		}
	}
	void AppendCodeLiteral(VirtualMachine& m) {
		EnsureFrame(m, 2);
		PrimitiveObject value = m.MainStack.back();
		m.MainStack.pop_back();
		HeapObject* obj = PopHeapObj(m);
		if (!obj->IsCode) throw RuntimeError("Unable to append code to a non-executable object");
		switch (value.GetType()) {
		case ObjType_Null:
			obj->Append(InstructionType::Null);
			break;
		case ObjType_Type:
			obj->Append(InstructionType::TypeLiteral);
			obj->Append((PrimitiveType)value);
			break;
		case ObjType_Bool:
			obj->Append((bool)value ? InstructionType::BoolTrue : InstructionType::BoolFalse);
			break;
		case ObjType_Int:
			obj->Append(InstructionType::IntLiteral);
			{
				int64_t intval = value;
				for (int i = 56; i >= 0; i -= 8) {
					obj->Append((intval >> i) & 0xFF);
				}
			}
			break;
		case ObjType_Real:
			obj->Append(InstructionType::RealLiteral);
			{
				double realval = value;
				// Reinterpret bits of double as an int
				uint64_t intbits = *reinterpret_cast<uint64_t*>(&realval);
				for (int i = 56; i >= 0; i -= 8) {
					obj->Append((intbits >> i) & 0xFF);
				}
			}
			break;
		case ObjType_HeapObj:
			throw RuntimeError("Unable to transfer a HeapObject literal");
			break;
		}
	}
	void AppendText(VirtualMachine& m) {
		size_t length = m.IP.ReadPosition();
		HeapObject* obj = PopHeapObj(m);
		if (obj->IsCode) throw RuntimeError("Unable to append text to an executable object");
		for (size_t i = 0; i < length; i++) {
			obj->Append(m.IP.ReadByte());
		}
	}
	void AppendFormat(VirtualMachine& m) {
		EnsureFrame(m, 2);
		PrimitiveObject value = m.MainStack.back();
		m.MainStack.pop_back();
		HeapObject* obj = PopHeapObj(m);
		if (obj->IsCode) throw RuntimeError("Unable to append text to an executable object");
		PrimitiveType valtype = value.GetType();
		std::string toAppend;
		if (valtype == ObjType_HeapObj) {
			HeapObject* valObj = (HeapObject*)value;
			if (valObj->IsCode) throw RuntimeError("Unable to read text from an executable object");
			for (byte b : valObj->Code) {
				toAppend.push_back(b);
			}
		} else {
			std::ostringstream formatter;
			switch (valtype) {
				case ObjType_Null: formatter << "Null"; break;
				case ObjType_Type: formatter << TypeToString((PrimitiveType)value); break;
				case ObjType_Bool: formatter << ((bool)value) ? "True" : "False"; break;
				case ObjType_Int: formatter << (int64_t)value; break;
				case ObjType_Real: formatter << (double)value; break;
			}
			toAppend = formatter.str();
		}
		for (char c : toAppend) {
			obj->Append(c);
		}
	}
	void PrintText(VirtualMachine& m) {
		m.MainStack.pop_back();
		HeapObject* obj = PopHeapObj(m);
		if (obj->IsCode) throw RuntimeError("Unable to append text to an executable object");
		std::string str;
		for (byte b : obj->Code) {
			str.push_back(b);
		}
		std::cout << str;
	}
	void DebugLine(VirtualMachine& m) {
		size_t lineNumber = m.IP.ReadPosition();
		m.IP.LineNumber = lineNumber;
	}
}

void VirtualMachine::Step() {
	GarbageCollector* gc = _GC.get();
	if (Completed) return;
	if (gc) gc->Suspend();
	InstructionType cmd = (InstructionType)IP.ReadByte();
	const char* InstrName = cmd < InstructionCount ? InstructionTypeTexts[cmd] : "<unknown instruction>";
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

		case InstructionType::Null: VMOperations::Null(*this); break;
		case InstructionType::BoolFalse: VMOperations::Bool(*this, false); break;
		case InstructionType::BoolTrue: VMOperations::Bool(*this, true); break;
		case InstructionType::IntLiteral: VMOperations::IntLiteral(*this); break;
		case InstructionType::RealLiteral: VMOperations::RealLiteral(*this); break;
		case InstructionType::TypeLiteral: VMOperations::TypeLiteral(*this); break;

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
		case InstructionType::ShallowCopy: VMOperations::ShallowCopy(*this); break;
		case InstructionType::GetGlobalObject: VMOperations::GetGlobalObject(*this); break;
		case InstructionType::GetFunctionObject: VMOperations::GetFunctionObject(*this); break;
		case InstructionType::GetMember: VMOperations::GetMember(*this); break;
		case InstructionType::SetMember: VMOperations::SetMember(*this); break;
		case InstructionType::GetMemberDynamic: VMOperations::GetMemberDynamic(*this); break;
		case InstructionType::SetMemberDynamic: VMOperations::SetMemberDynamic(*this); break;

		case InstructionType::PushFrame: VMOperations::PushFrame(*this); break;
		case InstructionType::PopFrame: VMOperations::PopFrame(*this); break;
		case InstructionType::Pick: VMOperations::Pick(*this); break;
		case InstructionType::Bury: VMOperations::Bury(*this); break;
		case InstructionType::PopDiscard: VMOperations::PopDiscard(*this); break;

		case InstructionType::ClearCode: VMOperations::ClearCode(*this); break;
		case InstructionType::ClearText: VMOperations::ClearText(*this); break;
		case InstructionType::AppendCode: VMOperations::AppendCode(*this); break;
		case InstructionType::AppendCodeLiteral: VMOperations::AppendCodeLiteral(*this); break;
		case InstructionType::AppendText: VMOperations::AppendText(*this); break;
		case InstructionType::AppendFormat: VMOperations::AppendFormat(*this); break;
		case InstructionType::PrintText: VMOperations::PrintText(*this); break;
		case InstructionType::DebugLine: VMOperations::DebugLine(*this); break;

		case InstructionType::PerformGC: this->CollectGarbage();
			// TODO: More opcodes
		default:
			throw RuntimeError("This opcode has not been implemented.");
		}
		if (AllocationCounter >= AllocationCounterMax) {
			CollectGarbage();
		}
	} catch (const RuntimeError& err) {
		std::ostringstream composedError;
		composedError << "Line " << IP.LineNumber << ", " << InstrName << ": " << err.what();
		std::string composedErrorStr = composedError.str();
		throw RuntimeError(composedErrorStr.data(), composedErrorStr.length());
	}
	if (gc) gc->Resume();
}