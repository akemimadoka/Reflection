#pragma once
#include <natRefObj.h>
#include <natException.h>
#include <unordered_map>
#include <typeindex>
#include "Method.h"
#include "Field.h"
#include "Type.h"

using namespace NatsuLib;

#define GENERATE_METADATA(classname) typedef classname Self_t_;\
static Reflection::ReflectionClassRegister<Self_t_> _s_RefectionHelper_##classname;\
static ncTStr GetName() noexcept\
{\
	return _T(#classname);\
}\
natRefPointer<IType> GetType() const noexcept override\
{\
	return typeof(Self_t_);\
}

#define GENERATE_METADATA_DEFINITION(classname) Reflection::ReflectionClassRegister<classname> classname::_s_RefectionHelper_##classname

#define DECLARE_REFLECTABLE_CLASS(classname) class classname : virtual public Object
#define DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASS(classname, baseclass) class classname : public std::enable_if_t<std::is_base_of<Object, baseclass>::value, baseclass>

#define DECLARE_NONMEMBER_METHOD(classname, methodname, id, returntype, ...) static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##id##_;\
static returntype methodname(__VA_ARGS__) 

#define DEFINE_NONMEMBER_METHOD(classname, methodname, id, returntype, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##id##_{ _T(#methodname), static_cast<returntype(*)(__VA_ARGS__)>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_CONSTRUCTOR(classname, specifiers, id, ...) static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_##id##_;\
static natRefPointer<Object> Constructor(detail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
specifiers classname(__VA_ARGS__)

#define DEFINE_CONSTRUCTOR(classname, specifiers, id, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_##id##_{ _T("Constructor"), static_cast<natRefPointer<Object>(*)(detail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(detail_::forward_call_t, std::tuple<__VA_ARGS__>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>::type{});\
}\
classname::classname

#define DECLARE_DEFAULT_COPYCONSTRUCTOR(classname) DECLARE_CONSTRUCTOR(classname, , CopyConstructor, classname const&) = default
#define DEFINE_DEFAULT_COPYCONSTRUCTOR(classname) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_CopyConstructor##_{ _T("Constructor"), static_cast<natRefPointer<Object>(*)(detail_::forward_call_t, std::tuple<classname const&>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(detail_::forward_call_t, std::tuple<classname const&>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>::type{});\
}

#define DECLARE_DEFAULT_MOVECONSTRUCTOR(classname) DECLARE_CONSTRUCTOR(classname, , MoveConstructor, classname &&) = default
#define DEFINE_DEFAULT_MOVECONSTRUCTOR(classname) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_MoveConstructor##_{ _T("Constructor"), static_cast<natRefPointer<Object>(*)(detail_::forward_call_t, std::tuple<classname &&>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(detail_::forward_call_t, std::tuple<classname &&>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>::type{});\
}

#define DECLARE_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname(__VA_ARGS__)

#define DEFINE_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberMethod_##methodname##_##id##_{ _T(#methodname), static_cast<returntype(classname::*)(__VA_ARGS__)>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_CONST_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Const_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname(__VA_ARGS__) const

#define DEFINE_CONST_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Const_MemberMethod_##methodname##_##id##_{ _T(#methodname), static_cast<returntype(classname::*)(__VA_ARGS__) const>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_VIRTUAL_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(detail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>)\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
virtual specifiers returntype methodname(__VA_ARGS__)

#define DEFINE_VIRTUAL_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_MemberMethod_##methodname##_##id##_{ _T(#methodname), static_cast<returntype(classname::*)(detail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(detail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple)\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>::type{});\
}\
specifiers returntype classname::methodname

#define DECLARE_VIRTUAL_CONST_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_Const_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(detail_::forward_call_t, std::tuple<__VA_ARGS__>&&) const;\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>) const\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
virtual specifiers returntype methodname(__VA_ARGS__) const

#define DEFINE_VIRTUAL_CONST_MEMBER_METHOD(classname, specifiers, methodname, id, returntype, ...) static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_Const_MemberMethod_##methodname##_##id##_{ _T(#methodname), static_cast<returntype(classname::*)(detail_::forward_call_t, std::tuple<__VA_ARGS__>&&) const>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(detail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple) const\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>::type{});\
}\
specifiers returntype classname::methodname

#define DECLARE_CONVERSION_OPERATOR(classname, targettype) DECLARE_MEMBER_METHOD(classname, , ConvertTo##targettype, , natRefPointer<Object>);\
operator targettype()

#define DEFINE_CONVERSION_OPERATOR(classname, targettype) DEFINE_MEMBER_METHOD(classname, , ConvertTo##targettype, , natRefPointer<Object>)(){ return this->operator targettype(); }\
classname::operator targettype()

#define DECLARE_CONST_CONVERSION_OPERATOR(classname, targettype) DECLARE_CONST_MEMBER_METHOD(classname, , ConvertTo##targettype, , natRefPointer<Object>);\
operator targettype() const

#define DEFINE_CONST_CONVERSION_OPERATOR(classname, targettype) DEFINE_CONST_MEMBER_METHOD(classname, , ConvertTo##targettype, , natRefPointer<Object>)() const { return this->operator targettype(); }\
classname::operator targettype() const

#define DECLARE_NONMEMBER_FIELD(classname, fieldtype, fieldname) static Reflection::ReflectionNonMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_NonMemberField_##fieldtype##_##fieldname##_;\
static fieldtype fieldname

#define DEFINE_NONMEMBER_FIELD(classname, fieldtype, fieldname) Reflection::ReflectionNonMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberField_##fieldtype##_##fieldname##_{ _T(#fieldname), &classname::fieldname };\
fieldtype classname::fieldname

#define DECLARE_NONMEMBER_POINTER_FIELD(classname, pointertotype, fieldname) static Reflection::ReflectionNonMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_NonMemberPointerField_##pointertotype##_##fieldname##_;\
static natRefPointer<pointertotype> fieldname

#define DEFINE_NONMEMBER_POINTER_FIELD(classname, pointertotype, fieldname) Reflection::ReflectionNonMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberPointerField_##pointertotype##_##fieldname##_{ _T(#fieldname), &classname::fieldname };\
natRefPointer<pointertotype> classname::fieldname

#define DECLARE_MEMBER_FIELD(classname, fieldtype, fieldname) static Reflection::ReflectionMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_MemberField_##fieldtype##_##fieldname##_;\
fieldtype fieldname

#define DEFINE_MEMBER_FIELD(classname, fieldtype, fieldname) Reflection::ReflectionMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberField_##fieldtype##_##fieldname##_{ _T(#fieldname), &classname::fieldname }

#define DECLARE_MEMBER_POINTER_FIELD(classname, pointertotype, fieldname) static Reflection::ReflectionMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_MemberPointerField_##pointertotype##_##fieldname##_;\
natRefPointer<pointertotype> fieldname

#define DEFINE_MEMBER_POINTER_FIELD(classname, pointertotype, fieldname) Reflection::ReflectionMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberPointerField_##pointertotype##_##fieldname##_{ _T(#fieldname), &classname::fieldname }

#define typeof(type) Reflection::GetInstance().GetType<type>()
#define typeofexp(expression) Reflection::GetInstance().GetType<decltype(expression)>()
#define typeofname(name) Reflection::GetInstance().GetType(name)

class Reflection
{
public:
	template <typename T>
	struct ReflectionClassRegister
	{
		ReflectionClassRegister()
		{
			GetInstance().RegisterType<T>();
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

	template <typename T>
	struct ReflectionNonMemberFieldRegister
	{
		template <typename U>
		ReflectionNonMemberFieldRegister(ncTStr name, U field)
		{
			GetInstance().RegisterNonMemberField<T>(name, field);
		}
	};

	template <typename T>
	struct ReflectionMemberFieldRegister
	{
		template <typename U>
		ReflectionMemberFieldRegister(ncTStr name, U field)
		{
			GetInstance().RegisterMemberField<T>(name, field);
		}
	};

	static Reflection& GetInstance();

	template <typename Class>
	void RegisterType()
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

	template <typename Class, typename Field>
	void RegisterNonMemberField(ncTStr name, Field field)
	{
		GetType<Class>()->RegisterNonMemberField(name, make_ref<NonMemberField<Field>>(field));
	}

	template <typename Class, typename Field>
	void RegisterMemberField(ncTStr name, Field field)
	{
		GetType<Class>()->RegisterMemberField(name, make_ref<MemberField<Field>>(field));
	}

	template <typename Class>
	natRefPointer<IType> GetType();

	natRefPointer<IType> GetType(ncTStr typeName);

private:
	Reflection();
	~Reflection();

	std::unordered_map<std::type_index, natRefPointer<IType>> m_TypeTable;
};

#include "Object.h"

template <typename T>
class BoxedObject final
	: public Object
{
public:
	friend struct Object;

	typedef BoxedObject Self_t_;
	static ncTStr GetName() noexcept;
	natRefPointer<IType> GetType() const noexcept override
	{
		return Reflection::GetInstance().GetType<Self_t_>();
	}

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

	nTString ToString() const noexcept override
	{
		return _toString(this);
	}

	std::type_index GetUnboxedType() override
	{
		return typeid(T);
	}

private:
	static nTString _toString(const BoxedObject* pThis) noexcept;

	T m_Obj;
};

template <>
class BoxedObject<void> final
	: public Object
{
public:
	typedef BoxedObject<void> Self_t_;
	static ncTStr GetName() noexcept;
	natRefPointer<IType> GetType() const noexcept override
	{
		return Reflection::GetInstance().GetType<Self_t_>();
	}

	nTString ToString() const noexcept override
	{
		return _T("void");
	}

	std::type_index GetUnboxedType() override
	{
		return typeid(void);
	}
};

bool operator==(natRefPointer<Object> const& ptr, nullptr_t);
bool operator==(nullptr_t, natRefPointer<Object> const& ptr);

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
		return static_cast<BoxedObject<T>*>(this)->m_Obj;
	}

	nat_Throw(ReflectionException, _T("Type wrong."));
}

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) typedef BoxedObject<type> alias

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

namespace detail_
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
	: ::detail_::boxed_type_impl<T, std::disjunction<std::is_integral<T>, std::is_floating_point<T>, std::is_void<T>>::value>
{
};

template <typename T>
struct boxed_type<natRefPointer<T>>
	: boxed_type<T>
{
};

template <typename T>
using boxed_type_t = typename boxed_type<T>::type;

template <typename Class>
natRefPointer<IType> Reflection::GetType()
{
	auto iter = m_TypeTable.find(typeid(boxed_type_t<Class>));
	if (iter != m_TypeTable.end())
	{
		return iter->second;
	}

	nat_Throw(ReflectionException, _T("Type not found."));
}
