#pragma once
#include "Byte.h"

enum InstructionType : byte {
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
	Null, // Pushes a null value to the stack.
	BoolFalse, BoolTrue, // Pushes a literal boolean constant.
	IntLiteral, // Skips the next eight bytes of program, interpreting them as the representation of a twos-complement signed integer. This integer is pushed to the stack.
	RealLiteral, // Skips the next eight bytes of program, interpreting them as the representation of a double-precision floating-point number. This number is pushed to the stack.
	TypeLiteral, // Skips the next byte of program, interpreting it as a member of the PrimitiveType enum, which is pushed to the stack.
	// ARITHMETIC
	Add, Sub, Mul, Neg, // Performs an arithmetic operation. The result is an integer if and only if both operands are integers.
	IntDiv, // Rounds both arguments towards zero and calculates their quotient, rounded towards zero.
	IntMod, // Rounds both arguments towards zero and calculates the remainder: IntMod(A, B) = A - B * IntDiv(A, B)
	RealDiv, // Converts both arguments to floating-point and calculates their quotient.
	RealMod, // http://www.cplusplus.com/reference/cmath/fmod/
	ToInt, // Rounds a real number towards zero.
	ToReal, // Converts an integer to a floating-point number.
	// OBJECTS
	GetType, // Pops a value and pushes its datatype.
	CreateObject, // Creates and pushes a pointer to a new empty object.
	ShallowCopy, // Pops a pointer to an object, makes a shallow copy of it, and pushes the pointer to the copy.
	GetGlobalObject, // Gets a pointer to the global object.
	GetFunctionObject, // Gets a pointer to the object containing the current function.
	GetMember, // Gets a member of an object. * The next byte is a member-name length and the next [length] bytes are the member name. These are all skipped.
	SetMember, // Sets a member of an object. * The next byte is a member-name length and the next [length] bytes are the member name. These are all skipped.
	GetMemberDynamic, // Gets a member of an object. ** The next byte is a member-name length and the next [length] bytes are a prefix for the member name.
	SetMemberDynamic, // Sets a member of an object. ** The next byte is a member-name length and the next [length] bytes are a prefix for the member name.
	// STACK MANIPULATION
	PushFrame, // Reads a byte N from the program, starts a new stack frame, and moves the top N items of the stack into it.
	PopFrame, // Reads a byte N from the program, pops items until there are N items remaining in the stack frame, and moves those items into the stack frame below.
	Pick, // Reads a byte N from the program and pushes the item that is N places away from the bottom of the stack frame to the top of the stack.
	Bury, // Reads a byte N from the program and replaces the item that is N places away from the bottom of the stack frame with the item at the top of the stack.
	PopDiscard, // Pops an item off the stack and discards it.
	// CODE/TEXT
	ClearCode, // Pops an object pointer and removes the binary data contained in that object, marking it as executable.
	ClearText, // Pops an object pointer and removes the binary data contained in that object, marking it as non-executable.
	AppendCode, // Skips the next four bytes of program, interpreting them as a length, pops an object pointer off the stack, and appends the next [length] bytes to the object as code.
	AppendCodeLiteral, // Pops an object pointer and a non-pointer value, and appends code to the object that pushes that value to the stack.
	AppendText, // Skips the next four bytes of program, interpreting them as a length, pops an object pointer off the stack, and appends the next [length] bytes to the object as text.
	AppendFormat, // Pops an object pointer and a value off the stack, and appends the string representation of that value to the text of the object.
	PrintText, // Pops an object pointer and prints the text of the object to standard output.
	// DEBUG
	DebugLine, // Skips the next four bytes of program, interpreting them as a line number. If an error occurs, it will display that line number.
	// GC
	PerformGC,
    // Count
	InstructionCount
};

/*
	*  The GetMember and SetMember instructions expect the values on the stack to be in the order:
	   [Object] [Value if applicable]
	   They look up the member of the [Object] with the member name specified in the instruction.

	** The GetMemberDynamic and SetMemberDynamic instructions expect the values on the stack to be in the order:
	   [Object] [Index] [Value if applicable]
	   They get the string representation of the [Index], which must be an integer,
	   and append it after the member-name prefix specified in the instruction.
	   Then, the member of [Object] with the resulting name is looked up.
*/

const char* const InstructionTypeTexts[InstructionCount] {
	"Ret", "Call", "TailCall", "Jump", "TrueJump", "FalseJump",
	"BoolNot", "IntNot", "BoolAnd", "IntAnd", "BoolOr", "IntOr",
	"StrictEqual", "StrictNeq", "NumEqual", "NumNeq",
	"NumLt", "NumGt", "NumLeq", "NumGeq",
	"Null", "BoolFalse", "BoolTrue", 
	"IntLiteral", "RealLiteral", "TypeLiteral", 
	"Add", "Sub", "Mul", "Neg", 
	"IntDiv", "IntMod", "RealDiv", "RealMod", 
	"ToInt", "ToReal", 
	"GetType", 
	"CreateObject", "ShallowCopy", "GetGlobalObject", "GetFunctionObject", 
	"GetMember", "SetMember", "GetMemberDynamic", "SetMemberDynamic", 
	"PushFrame", "PopFrame", "Pick", "Bury", "PopDiscard",
	"ClearCode", "ClearText", "AppendCode", "AppendCodeLiteral", "AppendText", "AppendFormat", "PrintText",
	"DebugLine", 
	"PerformGC",
};