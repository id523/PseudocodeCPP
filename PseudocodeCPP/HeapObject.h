#pragma once
#include <vector>
#include "Byte.h"
class HeapObject
{
private:
	std::vector<byte> Code;
public:
	HeapObject();
	~HeapObject();
	void GetReferencedObjects(std::vector<HeapObject*>& objqueue) const;
	template<class ByteIterator> void AppendCode(ByteIterator begin, ByteIterator end);
	void ClearCode();
	byte GetCodeAt(size_t pos);
};

template<class ByteIterator>
inline void HeapObject::AppendCode(ByteIterator begin, ByteIterator end)
{
	while (begin != end) {
		Code.push_back(*begin);
		begin++;
	}
}
