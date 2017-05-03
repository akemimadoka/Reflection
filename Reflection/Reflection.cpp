#include "Reflection.h"

#ifdef _WIN32
	#ifdef _UNICODE
		#define toTString std::to_wstring
		typedef WideStringView TStrView;
	#else
		#define toTString std::to_string
		typedef AnsiStringView TStrView;
	#endif
#else
	#define toTString std::to_string
	typedef U8StringView TStrView;
#endif

Reflection& Reflection::GetInstance()
{
	static Reflection s_Instance;
	return s_Instance;
}

natRefPointer<IType> Reflection::GetType(nStrView typeName)
{
	for (auto&& item : m_TypeTable)
	{
		if (item.second->GetName() == typeName)
		{
			return item.second;
		}
	}

	nat_Throw(ReflectionException, "Type not found."_nv);
}

Linq<const natRefPointer<IType>> Reflection::GetTypes() const
{
	return from(m_TypeTable).select([](auto&& pair) -> natRefPointer<IType> const& { return pair.second; });
}

natRefPointer<IType> IAttribute::GetType() const noexcept
{
	return typeof(IAttribute);
}

natRefPointer<IType> AttributeUsage::GetType() const noexcept
{
	return typeof(AttributeUsage);
}

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) template class BoxedObject<type>

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
INITIALIZEBOXEDOBJECT(nString, RefString);
INITIALIZEBOXEDOBJECT(void, Void);

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) template <> Reflection::ReflectionNonMemberMethodRegister<INITIALIZEBOXEDOBJECTFOR> INITIALIZEBOXEDOBJECTFOR::s_BoxedObject_Constructor_##type##_ = { AccessSpecifier::AccessSpecifier_public, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(type)>(&INITIALIZEBOXEDOBJECTFOR::Constructor) }

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

Reflection::ReflectionNonMemberMethodRegister<RefString> RefString::s_BoxedObject_Constructor_nString_ = { AccessSpecifier::AccessSpecifier_public, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(nString)>(&RefString::Constructor) };

template <>
nString Bool::_toString(const Bool* pThis) noexcept
{
	return pThis->m_Obj ? "true"_nv : "false"_nv;
}

template <>
nString Char::_toString(const Char* pThis) noexcept
{
	return AnsiStringView{ pThis->m_Obj };
}

template <>
nString WChar::_toString(const WChar* pThis) noexcept
{
	return WideStringView{ pThis->m_Obj };
}

template <>
nString SByte::_toString(const SByte* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString Byte::_toString(const Byte* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString Short::_toString(const Short* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString UShort::_toString(const UShort* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString Integer::_toString(const Integer* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString UInteger::_toString(const UInteger* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString Long::_toString(const Long* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString ULong::_toString(const ULong* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString Float::_toString(const Float* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

template <>
nString Double::_toString(const Double* pThis) noexcept
{
	const auto tmpStr{ toTString(pThis->m_Obj) };
	return TStrView{ tmpStr.c_str(), tmpStr.size() };
}

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) RegisterType<alias>()

Reflection::Reflection()
{
	RegisterType<Object>();
	RegisterType<IAttribute>();
	RegisterType<AttributeUsage>()->UncheckedRegisterAttributes({ AttributeUsage{ AttributeTarget::Class } });
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
	INITIALIZEBOXEDOBJECT(nString, RefString);
	INITIALIZEBOXEDOBJECT(void, Void);
}

Reflection::~Reflection()
{
}

Object::~Object()
{
}

natRefPointer<IType> Object::GetType() const noexcept
{
	return typeof(Object);
}

nString Object::ToString() const noexcept
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
