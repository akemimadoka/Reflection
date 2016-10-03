#include "Reflection.h"

natRefPointer<IType> Reflection::GetType(ncTStr typeName)
{
	for (auto&& item : m_TypeTable)
	{
		if (!_tcscmp(item.second->GetName(), typeName))
		{
			return item.second;
		}
	}

	nat_Throw(ReflectionException, _T("Type not found."));
}

Reflection::Reflection()
{
}

Reflection::~Reflection()
{
}

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) Reflection::ReflectionClassRegister<BoxedObject<type>> BoxedObject<type>::_s_RefectionHelper_BoxedObject

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

bool operator==(nullptr_t, natRefPointer<Object> const& ptr)
{
	return ptr == nullptr;
}
