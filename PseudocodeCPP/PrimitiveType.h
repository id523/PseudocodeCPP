#pragma once

#include "Byte.h"

enum PrimitiveType : byte {
	ObjType_Null,
	ObjType_Type,
	ObjType_Bool,
	ObjType_Int,
	ObjType_Real,
	ObjType_HeapObj,
};

inline const char* TypeToString(PrimitiveType t)
{
	switch (t) {
	case ObjType_Null: return "Null";
	case ObjType_Type: return "Type";
	case ObjType_Bool: return "Boolean";
	case ObjType_Int: return "Integer";
	case ObjType_Real: return "Real";
	case ObjType_HeapObj: return "Object";
	default: return "Unknown Type";
	}
}

