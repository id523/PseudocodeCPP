#pragma once
#include "Byte.h"
#include "PrimitiveObject.h"
#include <map>
#include <string>
#include <vector>
class HeapObject {
protected:
	HeapObject();
public:
	std::vector<byte> Code;
	bool IsCode;
	virtual ~HeapObject();
	void Append(byte b);
	void ClearCode();
	void ClearText();
	byte GetCodeAt(size_t pos) const;
	virtual void GetReferencedObjects(std::vector<const HeapObject*>& objqueue) const;
	virtual PrimitiveObject GetMember(std::string key) const;
	virtual void SetMember(std::string key, const PrimitiveObject& value);
	virtual void SetMember(std::string key, PrimitiveObject&& value);
	virtual void DeleteMember(std::string key);
};
