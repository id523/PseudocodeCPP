#pragma once
#include <vector>
#include "Byte.h"
#include "PrimitiveObject.h"
#include "ValueTree.h"
class HeapObject {
private:
	std::vector<byte> Code;
	ValueTree<PrimitiveObject> Members;
public:
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
