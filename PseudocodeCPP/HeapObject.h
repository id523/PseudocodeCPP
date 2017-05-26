#pragma once
#include "Byte.h"
#include "PrimitiveObject.h"
#include <map>
#include <string>
#include <vector>
class HeapObject {
private:
	
public:
	std::vector<byte> Code;
	bool IsCode;
	std::map<std::string, PrimitiveObject> Members;
	HeapObject();
	~HeapObject();
	void Append(byte b);
	void ClearCode();
	void ClearText();
	byte GetCodeAt(size_t pos) const;

};
