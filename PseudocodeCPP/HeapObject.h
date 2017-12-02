#pragma once
#include "Byte.h"
#include "PrimitiveObject.h"
#include "VirtualMachine.forward.h"
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
	virtual PrimitiveObject GetMember(const std::string& key) const;
	virtual void SetMember(const std::string& key, const PrimitiveObject& value);
	virtual void SetMember(const std::string& key, PrimitiveObject&& value);
	virtual void DeleteMember(const std::string& key);
	virtual void CustomCommand(size_t cmd, VirtualMachine& vm);
};
