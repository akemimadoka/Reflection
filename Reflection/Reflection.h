#pragma once
#include <natRefObj.h>
#include <natException.h>
#include <unordered_map>
#include <typeindex>
#include "Method.h"
#include "Type.h"

using namespace NatsuLib;

#undef RegisterClass

#define GENERATE_METADATA(classname) typedef classname _Self_t;\
static Reflection::ReflectionRegister<_Self_t> _s_RefectionHelper_##classname;\
static ncTStr GetName()\
{\
	return _T(#classname);\
}\
natRefPointer<IType> GetType() override\
{\
	return Reflection::GetInstance().GetType<_Self_t>();\
}

#define GENERATE_METADATA_DEFINITION(classname) Reflection::ReflectionRegister<classname> classname::_s_RefectionHelper_##classname

#define DECLARE_REFLECTABLE_CLASS(classname) class classname : virtual public Object

#define DECLARE_NONMEMBER_METHOD(classname, methodname, id, returntype, ...) static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##returntype##_##id##_;\
static returntype methodname(__VA_ARGS__) 

#define DEFINE_NONMEMBER_METHOD(classname, methodname, id, returntype, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##returntype##_##id##_{ _T(#methodname), static_cast<returntype(*)(__VA_ARGS__)>(&methodname) };\
returntype classname::methodname

#define DECLARE_CONSTRUCTOR(classname, id, ...) static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##id##_;\
static natRefPointer<Object> Constructor(__VA_ARGS__);\
classname(__VA_ARGS__)

#define DEFINE_CONSTRUCTOR(classname, id, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##id##_{ _T("Constructor"), static_cast<natRefPointer<Object>(*)(__VA_ARGS__)>(&Constructor) };\
natRefPointer<Object> classname::Constructor

#define DECLARE_DEFAULT_COPYCONSTRUCTOR(classname) DECLARE_CONSTRUCTOR(classname, CopyConstructor, classname const&) = default
#define DEFINE_DEFAULT_COPYCONSTRUCTOR(classname) DEFINE_CONSTRUCTOR(classname, CopyConstructor, classname const&)(classname const& other) { return make_ref<classname>(other); }

#define DECLARE_DEFAULT_MOVECONSTRUCTOR(classname) DECLARE_CONSTRUCTOR(classname, MoveConstructor, classname &&) = default
#define DEFINE_DEFAULT_MOVECONSTRUCTOR(classname) DEFINE_CONSTRUCTOR(classname, MoveConstructor, classname &&)(classname && other) { return make_ref<classname>(std::move(other)); }

#define DECLARE_MEMBER_METHOD(classname, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Method_##methodname##_##returntype##_##id##_;\
returntype methodname(__VA_ARGS__)

#define DEFINE_MEMBER_METHOD(classname, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Method_##methodname##_##returntype##_##id##_{ _T(#methodname), static_cast<returntype(classname::*)(__VA_ARGS__)>(&methodname) };\
returntype classname::methodname

#define DECLARE_CONST_MEMBER_METHOD(classname, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Const_Method_##methodname##_##returntype##_##id##_;\
returntype methodname(__VA_ARGS__) const

#define DEFINE_CONST_MEMBER_METHOD(classname, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Const_Method_##methodname##_##returntype##_##id##_{ _T(#methodname), static_cast<returntype(classname::*)(__VA_ARGS__) const>(&methodname) };\
returntype classname::methodname

#define typeof(expression) Reflection::GetInstance().GetType<decltype(expression)>()

class Reflection
{
public:
	template <typename T>
	struct ReflectionRegister
	{
		ReflectionRegister()
		{
			GetInstance().RegisterClass<T>();
		}
	};

	template <typename T>
	struct ReflectionNonMemberMethodRegister
	{
		template <typename Func>
		ReflectionNonMemberMethodRegister(ncTStr name, Func method)
		{
			GetInstance().RegisterNonMemberMethod<T>(name, method);
		}
	};

	template <typename T>
	struct ReflectionMemberMethodRegister
	{
		template <typename Func>
		ReflectionMemberMethodRegister(ncTStr name, Func method)
		{
			GetInstance().RegisterMemberMethod<T>(name, method);
		}
	};

	static Reflection& GetInstance()
	{
		static Reflection s_Instance;
		return s_Instance;
	}

	template <typename Class>
	void RegisterClass()
	{
		m_TypeTable[typeid(Class)] = make_ref<Type<Class>>();
	}

	template <typename Class, typename Func>
	void RegisterNonMemberMethod(ncTStr name, Func method)
	{
		GetType<Class>()->RegisterNonMemberMethod(name, make_ref<NonMemberMethod<Func>>(method));
	}

	template <typename Class, typename Func>
	void RegisterMemberMethod(ncTStr name, Func method)
	{
		GetType<Class>()->RegisterMemberMethod(name, make_ref<MemberMethod<Func>>(method));
	}

	template <typename Class>
	natRefPointer<IType> GetType()
	{
		auto iter = m_TypeTable.find(typeid(Class));
		if (iter != m_TypeTable.end())
		{
			return iter->second;
		}

		nat_Throw(ReflectionException, _T("Type not found."));
	}

	natRefPointer<IType> GetType(ncTStr typeName);

private:
	Reflection();
	~Reflection();

	std::unordered_map<std::type_index, natRefPointer<IType>> m_TypeTable;
};

#include "Object.h"

template <typename T>
class BoxedObject
	: public Object
{
public:
	GENERATE_METADATA(BoxedObject)

	BoxedObject()
		: m_Obj{}
	{
	}
	BoxedObject(T const& obj)
		: m_Obj(obj)
	{
	}
	BoxedObject(T&& obj)
		: m_Obj(std::move(obj))
	{
	}

	operator T() const noexcept
	{
		return m_Obj;
	}

	operator T&() noexcept
	{
		return m_Obj;
	}

	operator T const&() const noexcept
	{
		return m_Obj;
	}

	operator T && () noexcept
	{
		return m_Obj;
	}

private:
	void* _getUnsafePtr() override
	{
		return &m_Obj;
	}

	T m_Obj;
};

template <>
class BoxedObject<void>
	: public Object
{
public:
	typedef BoxedObject<void> _Self_t;
	static Reflection::ReflectionRegister<_Self_t> _s_RefectionHelper_BoxedObject;
	static ncTStr GetName()
	{
		return _T("BoxedObject<void>");
	}
	natRefPointer<IType> GetType() override
	{
		return Reflection::GetInstance().GetType<_Self_t>();
	}

private:
	void* _getUnsafePtr() override
	{
		return nullptr;
	}
};

bool operator==(natRefPointer<Object> const& ptr, nullptr_t);

template <typename T>
T& Object::Unbox()
{
	auto typeindex = GetType()->GetTypeIndex();
	if (typeindex == typeid(BoxedObject<void>))
	{
		nat_Throw(ReflectionException, _T("Cannot unbox a void object."));
	}
	if (typeindex == typeid(T))
	{
		auto pRet = natUtil::Expect<T*>::Get(this);
		if (pRet)
		{
			return *pRet;
		}

		nat_Throw(ReflectionException, _T("Type wrong."));
	}
	if (typeindex == typeid(BoxedObject<T>))
	{
		return *static_cast<T*>(_getUnsafePtr());
	}

	nat_Throw(ReflectionException, _T("Type wrong."));
}

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) typedef BoxedObject<type> alias;

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

template <typename T>
std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, natRefPointer<Object>> Object::Box(T obj)
{
	return make_ref<BoxedObject<T>>(obj);
}

namespace _detail
{
	template <typename T, bool test>
	struct boxed_type_impl
	{
		typedef T type;
	};

	template <typename T>
	struct boxed_type_impl<T, true>
	{
		typedef BoxedObject<T> type;
	};
}

template <typename T>
struct boxed_type
	: _detail::boxed_type_impl<T, std::is_integral<T>::value || std::is_floating_point<T>::value>
{
};

template <typename T>
using boxed_type_t = typename boxed_type<T>::type;
