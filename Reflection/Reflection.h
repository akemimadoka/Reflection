#pragma once
#include <natRefObj.h>
#include <natException.h>
#include <natConcepts.h>
#include <natString.h>
#include <unordered_map>
#include <typeindex>

#include "Type.h"
#include "Attribute.h"

using namespace NatsuLib;

#define GENERATE_METADATA_IMPL(classname) private: typedef classname Self_t_;\
public: static nStrView GetName() noexcept\
{\
	return #classname##_nv;\
}\
natRefPointer<IType> GetType() const noexcept override\
{\
	return typeof(Self_t_);\
}

#define GENERATE_METADATA(classname, attributes) GENERATE_METADATA_IMPL(classname)\
private: static Reflection::ReflectionBaseClassesRegister<Self_t_, Object> _s_ReflectionHelper_##classname;

#define GENERATE_METADATA_WITH_BASE_CLASSES(classname, attributes, ...) GENERATE_METADATA_IMPL(classname)\
private: static Reflection::ReflectionBaseClassesRegister<Self_t_, __VA_ARGS__> _s_ReflectionHelper_##classname

#define GENERATE_METADATA_DEFINITION(classname, attributes) Reflection::ReflectionBaseClassesRegister<classname, Object> classname::_s_ReflectionHelper_##classname{ attributes }
#define GENERATE_METADATA_DEFINITION_WITH_BASE_CLASSES(classname, attributes, ...) Reflection::ReflectionBaseClassesRegister<classname, __VA_ARGS__> classname::_s_ReflectionHelper_##classname{ attributes }

#define DECLARE_REFLECTABLE_CLASS(classname) class classname : public Object
#define DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASS(classname, baseclass) class classname : public std::enable_if_t<std::is_base_of<Object, baseclass>::value, baseclass>
#define DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASSES(classname, ...) struct classname : __VA_ARGS__

#define DECLARE_REFLECTABLE_INTERFACE(classname) struct classname : Object
#define DECLARE_REFLECTABLE_INTERFACE_WITH_BASE_CLASS(classname, baseclass) struct classname : std::enable_if_t<std::is_base_of<Object, baseclass>::value, baseclass>
#define DECLARE_REFLECTABLE_INTERFACE_WITH_BASE_CLASSES(classname, ...) struct classname : __VA_ARGS__

#define DECLARE_NONMEMBER_METHOD(accessspecifier, classname, methodname, id, returntype, ...) private: static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##id##_;\
accessspecifier: static returntype methodname(__VA_ARGS__) 

#define DEFINE_NONMEMBER_METHOD(accessspecifier, classname, methodname, id, returntype, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, #methodname##_nv, static_cast<returntype(*)(__VA_ARGS__)>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_CONSTRUCTOR(accessspecifier, classname, specifiers, id, ...) private: static Reflection::ReflectionNonMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_##id##_;\
static natRefPointer<Object> Constructor(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
accessspecifier: specifiers classname(__VA_ARGS__)

#define DEFINE_CONSTRUCTOR(accessspecifier, classname, specifiers, id, ...) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>{});\
}\
classname::classname

#define DECLARE_DEFAULT_COPYCONSTRUCTOR(accessspecifier, classname) DECLARE_CONSTRUCTOR(accessspecifier, classname, , CopyConstructor, classname const&) = default
#define DEFINE_DEFAULT_COPYCONSTRUCTOR(accessspecifier, classname) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_CopyConstructor##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(rdetail_::forward_call_t, std::tuple<classname const&>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(rdetail_::forward_call_t, std::tuple<classname const&>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>{});\
}

#define DECLARE_DEFAULT_MOVECONSTRUCTOR(accessspecifier, classname) DECLARE_CONSTRUCTOR(accessspecifier, classname, , MoveConstructor, classname &&) = default
#define DEFINE_DEFAULT_MOVECONSTRUCTOR(accessspecifier, classname) Reflection::ReflectionNonMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberMethod_##Constructor##_MoveConstructor##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(rdetail_::forward_call_t, std::tuple<classname &&>&&)>(&classname::Constructor) };\
natRefPointer<Object> classname::Constructor(rdetail_::forward_call_t, std::tuple<classname &&>&& args)\
{\
	return ::CommonConstructor<classname>(std::move(args), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(args)>>::value>{});\
}

#define DECLARE_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_MemberMethod_##methodname##_##id##_;\
accessspecifier: specifiers returntype methodname(__VA_ARGS__)

#define DEFINE_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, false, #methodname##_nv, static_cast<returntype(classname::*)(__VA_ARGS__)>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Const_MemberMethod_##methodname##_##id##_;\
accessspecifier: specifiers returntype methodname(__VA_ARGS__) const

#define DEFINE_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Const_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, false, #methodname##_nv, static_cast<returntype(classname::*)(__VA_ARGS__) const>(&classname::methodname) };\
returntype classname::methodname

#define DECLARE_VIRTUAL_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&);\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>)\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
accessspecifier: virtual specifiers returntype methodname(__VA_ARGS__)

#define DEFINE_VIRTUAL_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Virtual_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, true, #methodname##_nv, static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple)\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>{});\
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

#define DEFINE_PURE_VIRTUAL_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_PureVirtual_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, true, #methodname##_nv, static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple)\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>{});\
}

#define DECLARE_VIRTUAL_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) private: static Reflection::ReflectionMemberMethodRegister<classname> _s_ReflectionHelper_##classname##_Virtual_Const_MemberMethod_##methodname##_##id##_;\
specifiers returntype methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&) const;\
template <typename... Args, size_t... i>\
specifiers returntype methodname##_forwarder_impl_(std::tuple<Args...>&& argTuple, std::index_sequence<i...>) const\
{\
	return methodname(std::forward<Args>(std::get<i>(argTuple))...);\
}\
accessspecifier: virtual specifiers returntype methodname(__VA_ARGS__) const

#define DEFINE_VIRTUAL_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_Virtual_Const_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, true, #methodname##_nv, static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&) const>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple) const\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>{});\
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

#define DEFINE_PURE_VIRTUAL_CONST_MEMBER_METHOD(accessspecifier, classname, specifiers, methodname, id, returntype, ...) Reflection::ReflectionMemberMethodRegister<classname> classname::_s_ReflectionHelper_##classname##_PureVirtual_Const_MemberMethod_##methodname##_##id##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, true, #methodname##_nv, static_cast<returntype(classname::*)(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&&)>(&classname::methodname##_forwarder_) };\
specifiers returntype classname::methodname##_forwarder_(rdetail_::forward_call_t, std::tuple<__VA_ARGS__>&& argTuple)\
{\
	return methodname##_forwarder_impl_(std::move(argTuple), typename std::make_index_sequence<std::tuple_size<std::remove_reference_t<decltype(argTuple)>>::value>{});\
}

#define DECLARE_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DECLARE_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>);\
accessspecifier: operator targettype()

#define DEFINE_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DEFINE_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>)(){ return this->operator targettype(); }\
classname::operator targettype()

#define DECLARE_CONST_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DECLARE_CONST_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>);\
accessspecifier: operator targettype() const

#define DEFINE_CONST_CONVERSION_OPERATOR(accessspecifier, classname, targettype) DEFINE_CONST_MEMBER_METHOD(accessspecifier, classname, , ConvertTo##targettype, , natRefPointer<Object>)() const { return this->operator targettype(); }\
classname::operator targettype() const

#define DECLARE_NONMEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) private: static Reflection::ReflectionNonMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_NonMemberField_##fieldname##_;\
accessspecifier: static fieldtype fieldname

#define DEFINE_NONMEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) Reflection::ReflectionNonMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberField_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, #fieldname##_nv, &classname::fieldname };\
fieldtype classname::fieldname

#define DECLARE_NONMEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) private: static Reflection::ReflectionNonMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_NonMemberPointerField_##fieldname##_;\
accessspecifier: static natRefPointer<pointertotype> fieldname

#define DEFINE_NONMEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) Reflection::ReflectionNonMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_NonMemberPointerField_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, #fieldname##_nv, &classname::fieldname };\
natRefPointer<pointertotype> classname::fieldname

#define DECLARE_MEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) private: static Reflection::ReflectionMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_MemberField_##fieldname##_;\
accessspecifier: fieldtype fieldname

#define DEFINE_MEMBER_FIELD(accessspecifier, classname, fieldtype, fieldname) Reflection::ReflectionMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberField_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, #fieldname##_nv, &classname::fieldname }

#define DECLARE_MEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) private: static Reflection::ReflectionMemberFieldRegister<classname> _s_ReflectionHelper_##classname##_MemberPointerField_##fieldname##_;\
accessspecifier: natRefPointer<pointertotype> fieldname

#define DEFINE_MEMBER_POINTER_FIELD(accessspecifier, classname, pointertotype, fieldname) Reflection::ReflectionMemberFieldRegister<classname> classname::_s_ReflectionHelper_##classname##_MemberPointerField_##fieldname##_{ AccessSpecifier::AccessSpecifier_##accessspecifier, #fieldname##_nv, &classname::fieldname }

#define typeof(type) Reflection::GetInstance().GetType<type>()
#define typeofexp(expression) Reflection::GetInstance().GetType<decltype(expression)>()
#define typeofname(name) Reflection::GetInstance().GetType(name)

#define nameof_(name) #name##_nv
#define nameof(name) ((static_cast<void>(name), nameof_(name)))

namespace rdetail_
{
	template <typename... T>
	struct ExplicitRegisterClass;

	template <typename T, typename... Rest>
	struct ExplicitRegisterClass<T, Rest...>
	{
		static void Execute();
	};

	template <>
	struct ExplicitRegisterClass<>
	{
		static void Execute()
		{
		}
	};
}

class Reflection
{
public:
	template <typename T>
	struct ReflectionClassRegister
	{
		explicit ReflectionClassRegister(AttributeSet&& attributes)
		{
			GetInstance().RegisterType<T>()->RegisterAttributes(std::move(attributes));
		}
	};

	template <typename T, typename... Base>
	struct ReflectionBaseClassesRegister
	{
		explicit ReflectionBaseClassesRegister(AttributeSet&& attributes)
		{
			rdetail_::ExplicitRegisterClass<T, Base...>::Execute();
			GetInstance().RegisterAttributes<T>(std::move(attributes));
			GetInstance().RegisterBaseClasses<T, Base...>();
		}
	};

	template <typename T>
	struct ReflectionNonMemberMethodRegister
	{
		template <typename Func>
		ReflectionNonMemberMethodRegister(AccessSpecifier accessSpecifier, nStrView name, Func method)
		{
			GetInstance().RegisterNonMemberMethod<T>(accessSpecifier, name, method);
		}
	};

	template <typename T>
	struct ReflectionMemberMethodRegister
	{
		template <typename Func>
		ReflectionMemberMethodRegister(AccessSpecifier accessSpecifier, bool isVirtual, nStrView name, Func method)
		{
			GetInstance().RegisterMemberMethod<T>(accessSpecifier, isVirtual, name, method);
		}
	};

	template <typename T>
	struct ReflectionNonMemberFieldRegister
	{
		template <typename U>
		ReflectionNonMemberFieldRegister(AccessSpecifier accessSpecifier, nStrView name, U field)
		{
			GetInstance().RegisterNonMemberField<T>(accessSpecifier, name, field);
		}
	};

	template <typename T>
	struct ReflectionMemberFieldRegister
	{
		template <typename U>
		ReflectionMemberFieldRegister(AccessSpecifier accessSpecifier, nStrView name, U field)
		{
			GetInstance().RegisterMemberField<T>(accessSpecifier, name, field);
		}
	};

	static Reflection& GetInstance();

	template <typename Class>
	natRefPointer<Type<Class>> RegisterType()
	{
		auto iter = m_TypeTable.find(typeid(Class));
		if (iter == m_TypeTable.end())
		{
			auto type = make_ref<Type<Class>>();
			m_TypeTable.emplace(typeid(Class), type);
			return type;
		}
		return iter->second;
	}

	template <typename Class>
	void RegisterAttributes(AttributeSet&& attributes)
	{
		GetType<Class>()->RegisterAttributes(std::move(attributes));
	}

	template <typename Class, typename... Base>
	void RegisterBaseClasses()
	{
		GetType<Class>()->RegisterBaseClasses({ GetType<Base>()... });
	}

	template <typename Class, typename Func>
	void RegisterNonMemberMethod(AccessSpecifier accessSpecifier, nStrView name, Func method);

	template <typename Class, typename Func>
	void RegisterMemberMethod(AccessSpecifier accessSpecifier, bool isVirtual, nStrView name, Func method);

	template <typename Class, typename Field>
	void RegisterNonMemberField(AccessSpecifier accessSpecifier, nStrView name, Field field);

	template <typename Class, typename Field>
	void RegisterMemberField(AccessSpecifier accessSpecifier, nStrView name, Field field);

	template <typename Class>
	natRefPointer<IType> GetType();

	natRefPointer<IType> GetType(nStrView typeName);

	Linq<const natRefPointer<IType>> GetTypes() const;

private:
	Reflection();
	~Reflection();

	std::unordered_map<std::type_index, natRefPointer<IType>> m_TypeTable;
};

namespace rdetail_
{
	template <typename T, typename Test = void>
	struct RegisterHelper
	{
		static void Do()
		{
		}
	};

	template <typename T>
	struct RegisterHelper<T, std::void_t<std::enable_if_t<std::is_base_of<Object, T>::value>>>
	{
		static void Do()
		{
			Reflection::GetInstance().RegisterType<T>();
		}
	};
}

template <typename T, typename ... Rest>
void rdetail_::ExplicitRegisterClass<T, Rest...>::Execute()
{
	RegisterHelper<T>::Do();
	ExplicitRegisterClass<Rest...>::Execute();
}

#include "Object.h"

template <typename T, typename Test = void>
struct should_box
	: std::false_type
{
};

template <typename T>
struct should_box<T, std::void_t<std::enable_if_t<!std::is_base_of<Object, T>::value>>>
	: std::true_type
{
};

// 若需要将并非派生于Object的类型作为可反射对象使用（例如函数的参数或返回值等），需要在此对BoxedObject特化此类型
// 注意你仍然需要手动或者利用宏来注册
// BoxedObject必须继承于Object且GetObj方法返回第一个模板参数的类型的左值引用
template <typename T, typename Test = void>
class BoxedObject final
	: public Object
{
public:
	[[noreturn]] T& GetObj()
	{
		nat_Throw(ReflectionException, "Cannot get object."_nv);
	}
};

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) private: static Reflection::ReflectionNonMemberMethodRegister<Self_t_> s_BoxedObject_Constructor_##type##_;\
public: BoxedObject(type value) : m_Obj { static_cast<UnderlyingType>(value) } {}\
static natRefPointer<Object> Constructor(type value) { return make_ref<BoxedObject>(std::move(value)); }

#pragma warning (push)
#pragma warning (disable : 4800)

template <typename T>
class BoxedObject<T, std::void_t<std::enable_if_t<std::disjunction<std::is_arithmetic<T>>::value>>> final
	: public Object
{
public:
	typedef BoxedObject Self_t_;
	typedef T UnderlyingType;
	static nStrView GetName() noexcept;
	natRefPointer<IType> GetType() const noexcept override
	{
		return typeof(Self_t_);
	}

	BoxedObject()
		: m_Obj{}
	{
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

	T& GetObj() noexcept
	{
		return m_Obj;
	}

	nString ToString() const noexcept override
	{
		return _toString(this);
	}

	std::type_index GetUnboxedType() override
	{
		return typeid(T);
	}

private:
	static nString _toString(const BoxedObject* pThis) noexcept;

	T m_Obj;
};

#pragma warning (pop)

template <>
class BoxedObject<nString> final
	: public Object
{
public:
	typedef BoxedObject Self_t_;
	typedef nString UnderlyingType;

	static nStrView GetName() noexcept
	{
		return "RefString"_nv;
	}

	natRefPointer<IType> GetType() const noexcept override
	{
		return typeof(BoxedObject<nString>);
	}

	BoxedObject()
		: m_Obj{}
	{
	}

	INITIALIZEBOXEDOBJECT(nString, RefString);

	operator nString() const noexcept
	{
		return m_Obj;
	}

	operator nString&() noexcept
	{
		return m_Obj;
	}

	operator nString const&() const noexcept
	{
		return m_Obj;
	}

	nString& GetObj() noexcept
	{
		return m_Obj;
	}

	nString ToString() const noexcept override
	{
		return m_Obj;
	}

	std::type_index GetUnboxedType() override
	{
		return typeid(nString);
	}

private:
	nString m_Obj;
};

template <>
class BoxedObject<void> final
	: public Object
{
public:
	typedef BoxedObject<void> Self_t_;
	static nStrView GetName() noexcept;
	natRefPointer<IType> GetType() const noexcept override
	{
		return Reflection::GetInstance().GetType<Self_t_>();
	}

	nString ToString() const noexcept override
	{
		return "void"_nv;
	}

	std::type_index GetUnboxedType() override
	{
		return typeid(void);
	}

	[[noreturn]] void GetObj()
	{
		nat_Throw(ReflectionException, "Cannot get object."_nv);
	}
};

#undef INITIALIZEBOXEDOBJECT
#define INITIALIZEBOXEDOBJECT(type, alias) template <> inline nStrView BoxedObject<type>::GetName() noexcept\
{\
	/*return "BoxedObject<"## #type ##">"_nv;*/\
	return #alias##_nv;\
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

inline nStrView BoxedObject<void>::GetName() noexcept
{
	return "Void"_nv;
}

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
#define INITIALIZEBOXEDOBJECT(type, alias) extern template class BoxedObject<type>

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

template <typename T>
std::enable_if_t<!std::is_base_of<Object, T>::value, natRefPointer<Object>> Object::Box(natRefPointer<T> const& ptr)
{
	return make_ref<BoxedObject<T>>(ptr);
}

template <typename T>
std::enable_if_t<!std::is_base_of<Object, T>::value, natRefPointer<Object>> Object::Box(T obj)
{
	return make_ref<BoxedObject<T>>(std::move(obj));
}

namespace rdetail_
{
	template <typename T, bool test>
	struct boxed_type_impl_
	{
		typedef T type;
	};

	template <typename T>
	struct boxed_type_impl_<T, true>
	{
		typedef BoxedObject<T> type;
	};

	template <typename T>
	struct boxed_type_impl
		: boxed_type_impl_<T, should_box<T>::value>
	{
	};

	template <typename T>
	struct boxed_type_impl<natRefPointer<T>>
		: boxed_type_impl<T>
	{
	};
}

template <typename T>
struct boxed_type
	: rdetail_::boxed_type_impl<std::remove_cv_t<std::remove_reference_t<T>>>
{
};

template <typename T>
using boxed_type_t = typename boxed_type<T>::type;

template <typename T>
struct is_boxed
	: std::false_type
{
};

template <typename T>
struct is_boxed<BoxedObject<T>>
	: std::true_type
{
};

template <typename T>
bool Type<T>::IsBoxed() const noexcept
{
	return is_boxed<T>::value;
}

template <typename T>
T& Object::Unbox()
{
	auto type = GetType();
	auto typeindex = type->GetTypeIndex();
	if (typeindex == typeid(BoxedObject<void>))
	{
		nat_Throw(ReflectionException, "Cannot unbox a void object."_nv);
	}
	if (typeindex == typeid(T))
	{
		auto pRet = natUtil::Expect<T*>::Get(this);
		if (pRet)
		{
			return *pRet;
		}

		nat_Throw(ReflectionException, "Type wrong."_nv);
	}
	if (typeindex == typeid(BoxedObject<T>))
	{
		return static_cast<BoxedObject<T>*>(this)->GetObj();
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

	nat_Throw(ReflectionException, "Type wrong."_nv);
}

template <typename Class>
natRefPointer<IType> Reflection::GetType()
{
	auto iter = m_TypeTable.find(typeid(boxed_type_t<Class>));
	if (iter != m_TypeTable.end())
	{
		return iter->second;
	}

	nat_Throw(ReflectionException, "Type not found."_nv);
}

// 要求这个类型可以被实例化以及移动构造，如果你的类型不符合这个要求，你需要自行特化
#define REGISTER_BOXED_OBJECT(type) template <>\
class BoxedObject<type> final : public Object\
{\
public:\
	static nStrView GetName() noexcept { return #type##_nv; }\
	natRefPointer<IType> GetType() const noexcept override { return typeof(BoxedObject); }\
	std::type_index GetUnboxedType() override { return typeid(type); }\
	BoxedObject(type&& value) : m_Obj(std::move(value)) {}\
	operator type&() { return m_Obj; }\
	type& GetObj() noexcept { return m_Obj; }\
private:\
	static Reflection::ReflectionBaseClassesRegister<BoxedObject, Object> _s_ReflectionHelper_BoxedObject;\
	static Reflection::ReflectionNonMemberMethodRegister<BoxedObject> _s_ReflectionHelper_BoxedObject_Constructor;\
	static natRefPointer<Object> Constructor(type&& value) { return make_ref<BoxedObject<type>>(std::move(value)); }\
	type m_Obj;\
}

#define REGISTER_BOXED_OBJECT_DEF(type) Reflection::ReflectionBaseClassesRegister<BoxedObject<type>, Object> BoxedObject<type>::_s_ReflectionHelper_BoxedObject { WITH() };\
Reflection::ReflectionNonMemberMethodRegister<BoxedObject<type>> BoxedObject<type>::_s_ReflectionHelper_BoxedObject_Constructor { AccessSpecifier::AccessSpecifier_public, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(type&&)>(&BoxedObject<type>::Constructor) }

#define REGISTER_BOXED_REFOBJECT(type) template <>\
class BoxedObject<type> final : public Object\
{\
public:\
	static nStrView GetName() noexcept { return #type##_nv; }\
	natRefPointer<IType> GetType() const noexcept override { return typeof(BoxedObject); }\
	std::type_index GetUnboxedType() override { return typeid(type); }\
	BoxedObject(natRefPointer<type> value) : m_Obj(std::move(value)) {}\
	BoxedObject(BoxedObject const& other) = default;\
	BoxedObject(BoxedObject&& other) noexcept = default;\
	operator type&() { return *m_Obj; }\
	type& GetObj() noexcept { return *m_Obj; }\
private:\
	static Reflection::ReflectionBaseClassesRegister<BoxedObject, Object> _s_ReflectionHelper_BoxedObject;\
	static Reflection::ReflectionNonMemberMethodRegister<BoxedObject> _s_ReflectionHelper_BoxedObject_CopyConstructor;\
	static Reflection::ReflectionNonMemberMethodRegister<BoxedObject> _s_ReflectionHelper_BoxedObject_MoveConstructor;\
	static natRefPointer<Object> Constructor(BoxedObject const& other) { return make_ref<BoxedObject<type>>(other); }\
	static natRefPointer<Object> Constructor(BoxedObject&& other) { return make_ref<BoxedObject<type>>(std::move(other)); }\
	natRefPointer<type> m_Obj;\
}

#define REGISTER_BOXED_REFOBJECT_DEF(type) Reflection::ReflectionBaseClassesRegister<BoxedObject<type>, Object> BoxedObject<type>::_s_ReflectionHelper_BoxedObject { WITH() };\
Reflection::ReflectionNonMemberMethodRegister<BoxedObject<type>> BoxedObject<type>::_s_ReflectionHelper_BoxedObject_CopyConstructor { AccessSpecifier::AccessSpecifier_public, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(BoxedObject const&)>(&BoxedObject<type>::Constructor) };\
Reflection::ReflectionNonMemberMethodRegister<BoxedObject<type>> BoxedObject<type>::_s_ReflectionHelper_BoxedObject_MoveConstructor { AccessSpecifier::AccessSpecifier_public, "Constructor"_nv, static_cast<natRefPointer<Object>(*)(BoxedObject&&)>(&BoxedObject<type>::Constructor) }

#include "Field.h"

template <typename Class, typename T>
natRefPointer<IType> MemberField<T(Class::*)>::GetType()
{
	return Reflection::GetInstance().GetType<boxed_type_t<T>>();
}

template <typename Class, typename Field>
void Reflection::RegisterNonMemberField(AccessSpecifier accessSpecifier, nStrView name, Field field)
{
	GetType<Class>()->RegisterNonMemberField(name, make_ref<NonMemberField<Field>>(accessSpecifier, field));
}

template <typename Class, typename Field>
void Reflection::RegisterMemberField(AccessSpecifier accessSpecifier, nStrView name, Field field)
{
	GetType<Class>()->RegisterMemberField(name, make_ref<MemberField<Field>>(accessSpecifier, field));
}

#include "Method.h"

template <typename Class, typename Func>
void Reflection::RegisterNonMemberMethod(AccessSpecifier accessSpecifier, nStrView name, Func method)
{
	GetType<Class>()->RegisterNonMemberMethod(name, make_ref<NonMemberMethod<Func>>(accessSpecifier, method));
}

template <typename Class, typename Func>
void Reflection::RegisterMemberMethod(AccessSpecifier accessSpecifier, bool isVirtual, nStrView name, Func method)
{
	GetType<Class>()->RegisterMemberMethod(name, make_ref<MemberMethod<Func>>(accessSpecifier, isVirtual, method));
}

#include "Convert.h"
#include "ArgumentPack.h"

inline natRefPointer<Object> Convert::ConvertTo(natRefPointer<Object> obj, natRefPointer<IType> toType)
{
	if (obj->GetType()->Equal(toType.Get()))
	{
		return obj;
	}

	return toType->Construct({ obj });
}

template <typename T>
natRefPointer<Object> Convert::ConvertTo(natRefPointer<Object> obj)
{
	return ConvertTo(obj, typeof(T));
}
