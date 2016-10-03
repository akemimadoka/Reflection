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
static const char* GetName()\
{\
	return #classname;\
}\
natRefPointer<IType> GetType() override\
{\
	return Reflection::GetInstance().GetType<_Self_t>();\
}

#define STR(x) #x
#define CONSTRUCTOR_NAME CreateInstance
#define CONSTRUCTOR_NAME_STR STR(CreateInstance)

#define GENERATE_METADATA_DEFINITION(classname) Reflection::ReflectionRegister<classname> classname::_s_RefectionHelper_##classname

#define DECLARE_REFLECTABLE_CLASS(classname) class classname : virtual public Object

#define DECLARE_NONMEMBER_METHOD(classname, methodname, returntype, ...) static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_;\
static returntype methodname(__VA_ARGS__) 

#define DEFINE_NONMEMBER_METHOD(classname, methodname, returntype, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_{ #methodname, &methodname };\
returntype classname::methodname

#define DECLARE_CONSTRUCTOR(classname, ...) DECLARE_NONMEMBER_METHOD(classname, CreateInstance, natRefPointer<classname>, __VA_ARGS__);\
classname(__VA_ARGS__)

#define DEFINE_CONSTRUCTOR(classname, ...) DEFINE_NONMEMBER_METHOD(classname, CreateInstance, natRefPointer<classname>, __VA_ARGS__)

#define DECLARE_MEMBER_METHOD(classname, methodname, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Method_##methodname##_;\
returntype methodname(__VA_ARGS__)

#define DEFINE_MEMBER_METHOD(classname, methodname, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Method_##methodname##_{ #methodname, &methodname };\
returntype classname::methodname

#define DECLARE_CONST_MEMBER_METHOD(classname, methodname, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Const_Method_##methodname##_;\
returntype methodname(__VA_ARGS__) const

#define DEFINE_CONST_MEMBER_METHOD(classname, methodname, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Const_Method_##methodname##_{ #methodname, &methodname };\
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
		ReflectionNonMemberMethodRegister(const char* name, Func method)
		{
			GetInstance().RegisterNonMemberMethod<T>(name, method);
		}
	};

	template <typename T>
	struct ReflectionMemberMethodRegister
	{
		template <typename Func>
		ReflectionMemberMethodRegister(const char* name, Func method)
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
	void RegisterNonMemberMethod(const char* name, Func method)
	{
		GetType<Class>()->RegisterNonMemberMethod(name, make_ref<NonMemberMethod<Func>>(method));
	}

	template <typename Class, typename Func>
	void RegisterMemberMethod(const char* name, Func method)
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

		throw std::runtime_error("Type not found.");
	}

	natRefPointer<IType> GetType(const char* typeName);

private:
	Reflection();
	~Reflection();

	std::unordered_map<std::type_index, natRefPointer<IType>> m_TypeTable;
};

#define INITIALIZEBOXEDOBJECT(type, alias) typedef BoxedObject<type> alias;\
Reflection::ReflectionRegister<BoxedObject<type>> BoxedObject<type>::_s_RefectionHelper_BoxedObject

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
	static const char* GetName()
	{
		return "BoxedObject<void>";
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
	if (typeindex == typeid(T))
	{
		auto pRet = natUtil::Expect<T*>::Get(this);
		return *(pRet ? pRet : throw std::runtime_error("Type wrong."));
	}
	if (typeindex == typeid(BoxedObject<T>))
	{
		return *static_cast<T*>(_getUnsafePtr());
	}

	throw std::runtime_error("Type wrong.");
}

template <typename T>
std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, natRefPointer<Object>> Object::Box(T obj)
{
	return make_ref<BoxedObject<T>>(obj);
}
