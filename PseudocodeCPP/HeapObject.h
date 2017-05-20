#pragma once
#include "Byte.h"
#include "PrimitiveObject.h"
#include <map>
#include <string>
#include <vector>
class HeapObject {
private:
	std::vector<byte> Code;
public:
	std::map<std::string, PrimitiveObject> Members;
	HeapObject();
	~HeapObject();
	template<class ByteIterator> void AppendCode(ByteIterator begin, ByteIterator end);
	void ClearCode();
	byte GetCodeAt(size_t pos) const;

};

template<class ByteIterator>
inline void HeapObject::AppendCode(ByteIterator begin, ByteIterator end) {
	while (begin != end) {
		Code.push_back(*begin);
		begin++;
	}
}
