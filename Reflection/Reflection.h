#pragma once
#include <natRefObj.h>
#include <natException.h>
#include <unordered_map>
#include <typeindex>
#include "Method.h"
#include "Field.h"
#include "Type.h"

using namespace NatsuLib;

#define GENERATE_METADATA_IMPL(classname) private: typedef classname Self_t_;\
public: static ncTStr GetName() noexcept\
{\
	return _T(#classname);\
}\
natRefPointer<IType> GetType() const noexcept override\
{\
	return typeof(Self_t_);\
}

#define GENERATE_METADATA(classname) GENERATE_METADATA_IMPL(classname)\
static Reflection::ReflectionBaseClassesRegister<Self_t_, Object> _s_ReflectionHelper_##classname;

#define GENERATE_METADATA_WITH_BASE_CLASSES(classname, ...) GENERATE_METADATA_IMPL(classname)\
static Reflection::ReflectionBaseClassesRegister<Self_t_, __VA_ARGS__> _s_ReflectionHelper_##classname

#define GENERATE_METADATA_DEFINITION(classname) Reflection::ReflectionBaseClassesRegister<classname, Object> classname::_s_ReflectionHelper_##classname{}
#define GENERATE_METADATA_DEFINITION_WITH_BASE_CLASSES(classname, ...) Reflection::ReflectionBaseClassesRegister<classname, __VA_ARGS__> classname::_s_ReflectionHelper_##classname{}

#define DECLARE_REFLECTABLE_CLASS(classname) class classname : virtual public Object
#define DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASS(classname, baseclass) class classname : public std::enable_if_t<std::is_base_of<Object, baseclass>::value, baseclass>
#define DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASSES(classname, ...) struct classname : __VA_ARGS__

#define DECLARE_NONMEMBER_METHOD(accessspecifier, classname, methodname, id, returntype, ...) private: static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##id##_;\
accessspecifier: static returntype methodname(__VA_ARGS__) 

#define DEFINE_NONMEMBER_METHOD(accessspecifier, classname, methodname, id, returntype, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#methodname), static_cast<returntype(*)(__VA_ARGS__)>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_CONSTRUCTOR(accessspecifier, classname, specifiers, id, ...) private: static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_##id##_;\
static natRefPointer<Object> Constructor(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
accessspecifier: specifiers classname(__VA_ARGS__)

#define DEFINE_CONSTRUCTOR(accessspecifier, classname, specifiers, id, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T("Constructor"), static_cast<natRefPointer<Object>(*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>::type{});\
}\
classname::classname

#define DECLARE_DEFAULT_COPYCONSTRUCTOR(accessspecifier, classname) DECLARE_CONSTRUCTOR(accessspecifier, classname, , CopyConstructor, classname const&) = default
#define DEFINE_DEFAULT_COPYCONSTRUCTOR(accessspecifier, classname) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_CopyConstructor##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T("Constructor"), static_cast<natRefPointer<Object>(*)(rdetail_::forward_call_t, std::tuple<classname const&>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(rdetail_::forward_call_t, std::tuple<classname const&>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>::type{});\
}

#define DECLARE_DEFAULT_MOVECONSTRUCTOR(accessspecifier, classname) DECLARE_CONSTRUCTOR(accessspecifier, classname, , MoveConstructor, classname &&) = default
#define DEFINE_DEFAULT_MOVECONSTRUCTOR(accessspecifier, classname) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_MoveConstructor##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T("Constructor"), static_cast<natRefPointer<Object>(*)(rdetail_::forward_call_t, std::tuple<classname &&>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(rdetail_::forward_call_t, std::tuple<classname &&>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>::type{});\
}

#define DECLARE_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_MemberMethod_##methodname##_##id##_;\
accessspecifier: specifiers returntype methodname(__VA_ARGS__)

#define DEFINE_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#methodname), static_cast<returntype(classname::*)(__VA_ARGS__)>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Const_MemberMethod_##methodname##_##id##_;\
accessspecifier: specifiers returntype methodname(__VA_ARGS__) const

#define DEFINE_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Const_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#methodname), static_cast<returntype(classname::*)(__VA_ARGS__) const>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_VIRTUAL_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>)\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
accessspecifier: virtual specifiers returntype methodname(__VA_ARGS__)

#define DEFINE_VIRTUAL_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Virtual_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#methodname), static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple)\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>::type{});\
}\
specifiers returntype classname::methodname

#define DECLARE_PURE_VIRTUAL_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_PureVirtual_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>)\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
accessspecifier: virtual specifiers returntype methodname(__VA_ARGS__) = 0

#define DEFINE_PURE_VIRTUAL_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_PureVirtual_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#methodname), static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple)\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>::type{});\
}

#define DECLARE_VIRTUAL_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_Const_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&) const;\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>) const\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
accessspecifier: virtual specifiers returntype methodname(__VA_ARGS__) const

#define DEFINE_VIRTUAL_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Virtual_Const_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#methodname), static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&) const>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple) const\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>::type{});\
}\
specifiers returntype classname::methodname

#define DECLARE_PURE_VIRTUAL_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_PureVirtual_Const_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>)\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
accessspecifier: virtual specifiers returntype methodname(__VA_ARGS__) const = 0

#define DEFINE_PURE_VIRTUAL_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_PureVirtual_Const_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#methodname), static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple)\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>::type{});\
}

#define DECLARE_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DECLARE_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>);\
accessspecifier: operator targettype()

#define DEFINE_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DEFINE_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>)(){ return this->operator targettype(); }\
classname::operator targettype()

#define DECLARE_CONST_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DECLARE_CONST_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>);\
accessspecifier: operator targettype() const

#define DEFINE_CONST_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DEFINE_CONST_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>)() const { return this->operator targettype(); }\
classname::operator targettype() const

#define DECLARE_NONMEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) private: static Reflection::ReflectionNonMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_NonMemberField_##fieldtype##_##fieldname##_;\
accessspecifier: static fieldtype fieldname

#define DEFINE_NONMEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) Reflection::ReflectionNonMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberField_##fieldtype##_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#fieldname), &classname::fieldname };\
fieldtype classname::fieldname

#define DECLARE_NONMEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) private: static Reflection::ReflectionNonMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_NonMemberPointerField_##pointertotype##_##fieldname##_;\
accessspecifier: static natRefPointer<pointertotype> fieldname

#define DEFINE_NONMEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) Reflection::ReflectionNonMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberPointerField_##pointertotype##_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#fieldname), &classname::fieldname };\
natRefPointer<pointertotype> classname::fieldname

#define DECLARE_MEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) private: static Reflection::ReflectionMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_MemberField_##fieldtype##_##fieldname##_;\
accessspecifier: fieldtype fieldname

#define DEFINE_MEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) Reflection::ReflectionMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberField_##fieldtype##_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#fieldname), &classname::fieldname }

#define DECLARE_MEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) private: static Reflection::ReflectionMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_MemberPointerField_##pointertotype##_##fieldname##_;\
accessspecifier: natRefPointer<pointertotype> fieldname

#define DEFINE_MEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) Reflection::ReflectionMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberPointerField_##pointertotype##_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, _T(#fieldname), &classname::fieldname }

#define typeof(type) Reflection::GetInstance().GetType<type>()
#define typeofexp(expression) Reflection::GetInstance().GetType<decltype(expression)>()
#define typeofname(name) Reflection::GetInstance().GetType(name)

class Reflection
{
	template <typename... T>
	struct ExplicitRegisterClass;

	template <typename T, typename... Rest>
	struct ExplicitRegisterClass<T, Rest...>
	{
		static void Execute()
		{
			GetInstance().RegisterType<T>();
			ExplicitRegisterClass<Rest...>::Execute();
		}
	};

	template <>
	struct ExplicitRegisterClass<>
	{
		static void Execute()
		{
		}
	};

public:
	template <typename T>
	struct ReflectionClassRegister
	{
		ReflectionClassRegister()
		{
			GetInstance().RegisterType<T>();
		}
	};

	template <typename T, typename... Base>
	struct ReflectionBaseClassesRegister
	{
		ReflectionBaseClassesRegister()
		{
			ExplicitRegisterClass<T, Base...>::Execute();
			GetInstance().RegisterBaseClasses<T, Base...>();
		}
	};

	template <typename T>
	struct ReflectionNonMemberMethodRegister
	{
		template <typename Func>
		ReflectionNonMemberMethodRegister(AccessSpecifier accessSpecifier, ncTStr name, Func method)
		{
			GetInstance().RegisterNonMemberMethod<T>(accessSpecifier, name, method);
		}
	};

	template <typename T>
	struct ReflectionMemberMethodRegister
	{
		template <typename Func>
		ReflectionMemberMethodRegister(AccessSpecifier accessSpecifier, ncTStr name, Func method)
		{
			GetInstance().RegisterMemberMethod<T>(accessSpecifier, name, method);
		}
	};

	template <typename T>
	struct ReflectionNonMemberFieldRegister
	{
		template <typename U>
		ReflectionNonMemberFieldRegister(AccessSpecifier accessSpecifier, ncTStr name, U field)
		{
			GetInstance().RegisterNonMemberField<T>(accessSpecifier, name, field);
		}
	};

	template <typename T>
	struct ReflectionMemberFieldRegister
	{
		template <typename U>
		ReflectionMemberFieldRegister(AccessSpecifier accessSpecifier, ncTStr name, U field)
		{
			GetInstance().RegisterMemberField<T>(accessSpecifier, name, field);
		}
	};

	static Reflection& GetInstance();

	template <typename Class>
	void RegisterType()
	{
		if (m_TypeTable.find(typeid(Class)) == m_TypeTable.end())
		{
			m_TypeTable.emplace(typeid(Class), make_ref<Type<Class>>());
		}
	}

	template <typename Class, typename... Base>
	void RegisterBaseClasses()
	{
		GetType<Class>()->RegisterBaseClasses({ GetType<Base>()... });
	}

	template <typename Class, typename Func>
	void RegisterNonMemberMethod(AccessSpecifier accessSpecifier, ncTStr name, Func method)
	{
		GetType<Class>()->RegisterNonMemberMethod(name, make_ref<NonMemberMethod<Func>>(accessSpecifier, method));
	}

	template <typename Class, typename Func>
	void RegisterMemberMethod(AccessSpecifier accessSpecifier, ncTStr name, Func method)
	{
		GetType<Class>()->RegisterMemberMethod(name, make_ref<MemberMethod<Func>>(accessSpecifier, method));
	}

	template <typename Class, typename Field>
	void RegisterNonMemberField(AccessSpecifier accessSpecifier, ncTStr name, Field field)
	{
		GetType<Class>()->RegisterNonMemberField(name, make_ref<NonMemberField<Field>>(accessSpecifier, field));
	}

	template <typename Class, typename Field>
	void RegisterMemberField(AccessSpecifier accessSpecifier, ncTStr name, Field field)
	{
		GetType<Class>()->RegisterMemberField(name, make_ref<MemberField<Field>>(accessSpecifier, field));
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

template <typename T>
std::enable_if_t<std::is_base_of<Object, T>::value, bool> operator==(natRefPointer<T> const& ptr, nullptr_t)
{
	return ptr->GetType()->GetTypeIndex() == typeid(BoxedObject<void>) || ptr.Get() == nullptr;
}

template <typename T>
std::enable_if_t<std::is_base_of<Object, T>::value, bool> operator==(nullptr_t, natRefPointer<T> const& ptr)
{
	return ptr == nullptr;
}

template <typename T>
void Type<T>::RegisterBaseClasses(std::initializer_list<natRefPointer<IType>> baseClasses)
{
	m_BaseClasses.assign(baseClasses);
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

namespace rdetail_
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
	: ::rdetail_::boxed_type_impl<T, std::disjunction<std::is_integral<T>, std::is_floating_point<T>, std::is_void<T>>::value>
{
};

template <typename T>
struct boxed_type<natRefPointer<T>>
	: boxed_type<T>
{
};

template <typename T>
using boxed_type_t = typename boxed_type<T>::type;

template <typename T>
T& Object::Unbox()
{
	auto type = GetType();
	auto typeindex = type->GetTypeIndex();
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
	// 开销巨大所以留在最后判断
	auto Ttype = typeof(boxed_type_t<T>);
	if (type->IsExtendFrom(Ttype) || Ttype->IsExtendFrom(type))
	{
		auto pRet = natUtil::Expect<T*>::Get(this);
		if (pRet)
		{
			return *pRet;
		}
	}

	nat_Throw(ReflectionException, _T("Type wrong."));
}

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

template <typename Class, typename T>
natRefPointer<IType> MemberField<T(Class::*)>::GetType()
{
	return Reflection::GetInstance().GetType<boxed_type_t<T>>();
}
