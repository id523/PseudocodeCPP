#pragma once
#include <vector>
#include "Byte.h"
#include "ValueTree.h"
#include "PrimitiveObject.h"
class HeapObject {
private:
	std::vector<byte> Code;
	ValueTree<PrimitiveObject> Members;
	GarbageCollector* GC;
public:
	HeapObject(GarbageCollector* gc);
	// TODO: Implement copying and moving
	HeapObject(const HeapObject& other);
	HeapObject(HeapObject&& other);
	void swap(HeapObject& r);
	HeapObject& operator=(HeapObject other);
	HeapObject& operator=(HeapObject&& other);
	~HeapObject();
	void GetReferencedObjects(std::vector<HeapObject*>& objqueue) const;
	template<class ByteIterator> void AppendCode(ByteIterator begin, ByteIterator end);
	void ClearCode();
	byte GetCodeAt(size_t pos) const;
	const PrimitiveObject& GetMember(std::string key) const;
	void SetMember(std::string key, const PrimitiveObject& value);
	bool RemoveMember(std::string key);
	void RemoveMembers();
};

template<class ByteIterator>
inline void HeapObject::AppendCode(ByteIterator begin, ByteIterator end) {
	while (begin != end) {
		Code.push_back(*begin);
		begin++;
	}
}
