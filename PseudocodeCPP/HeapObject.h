#pragma once
#include <vector>
#include <map>
#include <string>
#include "Byte.h"
#include "PrimitiveObject.h"
class HeapObject {
private:
	std::vector<byte> Code;
public:
	std::map<std::string, PrimitiveObject> Members;
	HeapObject();
	~HeapObject();
	template<class ByteIterator> void AppendCode(ByteIterator begin, ByteIterator end);
	void ClearCode();
	byte GetCodeAt(size_t pos);

};

template<class ByteIterator>
inline void HeapObject::AppendCode(ByteIterator begin, ByteIterator end) {
	while (begin != end) {
		Code.push_back(*begin);
		begin++;
	}
}
