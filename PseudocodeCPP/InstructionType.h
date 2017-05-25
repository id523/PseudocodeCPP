#pragma once
#include "Byte.h"

enum class InstructionType : byte {
	// All sequences of bytes representing numbers are big-endian.
	// FLOW CONTROL
	Ret, // If the call stack is empty, exits the program. Otherwise, it pops an address from the call stack and jumps to it.
	Call, // Pops (a pointer to) a function from the main stack, pushes the address of the instruction after the Call to the call stack, and jumps to the function pointer specified. 
	TailCall, // Pops (a pointer to) a function from the main stack, and jumps to the function pointer specified, without changing the call stack. This is equivalent to "Call" followed by "Ret", but with less memory usage.
	Jump, // Skips the next four bytes of program, interpreting them as an offset into the current function. This offset is jumped to.
	TrueJump, // A Boolean value is popped from the stack, and the operand is "Jump"ed to if that Boolean was true.
	FalseJump, // A Boolean value is popped from the stack, and the operand is "Jump"ed to if that Boolean was false.
	// BOOLEAN OPS
	BoolNot, IntNot, // Performs a logical/bitwise NOT operation on the top of the stack.
	BoolAnd, IntAnd, // Performs a logical/bitwise AND operation on the top two items of the stack.
	BoolOr, IntOr, // Performs a logical/bitwise OR operation on the top two items of the stack.
	// COMPARISONS
	StrictEqual, StrictNeq, // Checks for (in)equality between two values. For two values to be equal, they must also be equal in type.
	NumEqual, NumNeq, // Checks for (in)equality between the numeric values of two numbers. Both operands must be either Null, Int or Real. Null is considered equal to zero.
	NumLt, NumGt, NumLeq, NumGeq, // Compares two numbers by numeric value. Both operands must be either Null, Int or Real. Null is considered equal to zero.
	// LITERALS
	BoolFalse, BoolTrue, // Pushes a literal boolean constant.
	IntLiteral, // Skips the next eight bytes of program, interpreting them as the representation of a twos-complement signed integer. This integer is pushed to the stack.
	RealLiteral, // Skips the next eight bytes of program, interpreting them as the representation of a double-precision floating-point number. This number is pushed to the stack.
	// ARITHMETIC
	Add, Sub, Mul, Neg, // Performs an arithmetic operation. The result is an integer if and only if both operands are integers.
	IntDiv, // Rounds both arguments towards zero and calculates their quotient, rounded towards zero.
	IntMod, // Rounds both arguments towards zero and calculates the remainder: IntMod(A, B) = A - B * IntDiv(A, B)
	RealDiv, // Converts both arguments to floating-point and calculates their quotient.
	RealMod, // http://www.cplusplus.com/reference/cmath/fmod/
	ToInt, // Rounds a real number towards zero.
	ToReal, // Converts an integer to a floating-point number.
	// OBJECTS
	CreateObject, // Creates and pushes a pointer to a new empty object.
	ShallowCopy, // Pops a pointer to an object, makes a shallow copy of it, and pushes the pointer to the copy.
	GetGlobalObject, // Gets a pointer to the global object.
	GetFunctionObject, // Gets a pointer to the object containing the current function.
	GetMember, // Gets a member of an object. The next byte is a member-name length and the next [length] bytes are the member name. These are all skipped.
	SetMember, // Sets a member of an object. The next byte is a member-name length and the next [length] bytes are the member name. These are all skipped.
	PopDiscard, // Pops an item off the stack and discards it.
	// CODE
	ClearCode, // Pops an object pointer and removes the code contained in that object.
	BeginAppendCode, // Skips all instructions up to the matching EndAppendCode, appending them to the code of the object pointed to by the top of the stack, without removing the pointer.
	EndAppendCode, // Stops skipping instructions and pops the object pointer left at the top of the stack.
	// DEBUG
	DebugLine, // Skips the next four bytes of program, interpreting them as a line number. If an error occurs, it will display that line number.
};