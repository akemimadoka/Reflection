#include "Reflection.h"

natRefPointer<IType> Reflection::GetType(const char* typeName)
{
	for (auto&& item : m_TypeTable)
	{
		if (!strcmp(item.second->GetName(), typeName))
		{
			return item.second;
		}
	}

	throw std::runtime_error("Type not found.");
}

Reflection::Reflection()
{
}

Reflection::~Reflection()
{
}

INITIALIZEBOXEDOBJECT(char, Char);
INITIALIZEBOXEDOBJECT(wchar_t, WChar);
INITIALIZEBOXEDOBJECT(int8_t, SByte);
INITIALIZEBOXEDOBJECT(uint8_t, Byte);
INITIALIZEBOXEDOBJECT(int16_t, Short);
INITIALIZEBOXEDOBJECT(uint16_t, UShort);
INITIALIZEBOXEDOBJECT(int32_t, Integer);
INITIALIZEBOXEDOBJECT(uint32_t, UInteger);
INITIALIZEBOXEDOBJECT(int64_t, Long);
INITIALIZEBOXEDOBJECT(uint64_t, ULong);
INITIALIZEBOXEDOBJECT(float, Float);
INITIALIZEBOXEDOBJECT(double, Double);
INITIALIZEBOXEDOBJECT(void, Void);

natRefPointer<Object> Object::Box()
{
	return make_ref<BoxedObject<void>>();
}

bool operator==(natRefPointer<Object> const& ptr, nullptr_t)
{
	return ptr->GetType()->GetTypeIndex() == typeid(BoxedObject<void>) || ptr.Get() == nullptr;
}
