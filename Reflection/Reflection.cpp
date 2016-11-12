#include "Reflection.h"

#ifdef _UNICODE
#define toTString std::to_wstring
#else
#define toTString std::to_string
#endif

Reflection& Reflection::GetInstance()
{
	static Reflection s_Instance;
	return s_Instance;
}

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

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) ncTStr BoxedObject<type>::GetName() noexcept\
{\
	/*return _T("BoxedObject<"## #type ##">");*/\
	return _T(#alias);\
}

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) Reflection::ReflectionNonMemberMethodRegister<INITIALIZEBOXEDOBJECTFOR> INITIALIZEBOXEDOBJECTFOR::s_BoxedObject_Constructor_##type##_{ AccessSpecifier::AccessSpecifier_public, _T("Constructor"), static_cast<natRefPointer<Object>(*)(type)>(&INITIALIZEBOXEDOBJECTFOR::Constructor) };

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Bool

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Char

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR WChar

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR SByte

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Byte

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Short

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR UShort

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Integer

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR UInteger

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Long

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR ULong

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Float

INITIALIZEBOXEDOBJECT(bool, Bool);
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

#undef INITIALIZEBOXEDOBJECTFOR
#define INITIALIZEBOXEDOBJECTFOR Double

INITIALIZEBOXEDOBJECT(bool, Bool);
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

nTString Bool::_toString(const Bool* pThis) noexcept
{
	return pThis->m_Obj ? _T("true") : _T("false");
}

nTString Char::_toString(const Char* pThis) noexcept
{
#ifdef _UNICODE
	return natUtil::C2Wstr(std::string{ pThis->m_Obj });
#else
	return { pThis->m_Obj };
#endif
}

nTString WChar::_toString(const WChar* pThis) noexcept
{
#ifdef _UNICODE
	return { pThis->m_Obj };
#else
	return natUtil::W2Cstr(std::wstring{ pThis->m_Obj });
#endif	
}

nTString SByte::_toString(const SByte* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString Byte::_toString(const Byte* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString Short::_toString(const Short* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString UShort::_toString(const UShort* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString Integer::_toString(const Integer* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString UInteger::_toString(const UInteger* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString Long::_toString(const Long* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString ULong::_toString(const ULong* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString Float::_toString(const Float* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

nTString Double::_toString(const Double* pThis) noexcept
{
	return toTString(pThis->m_Obj);
}

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) RegisterType<alias>()

Reflection::Reflection()
{
	RegisterType<Object>();
	INITIALIZEBOXEDOBJECT(bool, Bool);
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
}

Reflection::~Reflection()
{
}

Object::~Object()
{
}

ncTStr Object::GetName() noexcept
{
	return _T("Object");
}

natRefPointer<IType> Object::GetType() const noexcept
{
	return typeof(Object);
}

nTString Object::ToString() const noexcept
{
	return GetType()->GetName();
}

std::type_index Object::GetUnboxedType()
{
	return GetType()->GetTypeIndex();
}

natRefPointer<Object> Object::Box()
{
	return make_ref<BoxedObject<void>>();
}
