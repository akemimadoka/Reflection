#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <functional>
#include <deque>
#include <natRefObj.h>
#include <type_traits>

using namespace NatsuLib;

#undef RegisterClass

#define GENERATE_METADATA(classname) typedef classname _Self_t;\
static Reflection::ReflectionRegister<classname> _s_RefectionHelper_##classname;\
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

#define INITIALIZEBOXEDOBJECT(type, alias) typedef BoxedObject<type> alias;\
Reflection::ReflectionRegister<BoxedObject<type>> BoxedObject<type>::_s_RefectionHelper_BoxedObject

#define typeof(expression) Reflection::GetInstance().GetType<decltype(expression)>()

struct Interface
	: natRefObj
{
};

struct Object;
class ArgumentPack;

struct IMethod;
struct IMemberMethod;

struct IType : Interface
{
	virtual void RegisterNonMemberMethod(const char* name, natRefPointer<IMethod> method) = 0;
	virtual void RegisterMemberMethod(const char* name, natRefPointer<IMemberMethod> method) = 0;

	virtual const char* GetName() const noexcept = 0;
	virtual natRefPointer<Object> Construct(ArgumentPack const& args) = 0;
	virtual natRefPointer<Object> InvokeNonMember(const char* name, ArgumentPack const& args) = 0;
	virtual natRefPointer<Object> InvokeMember(natRefPointer<Object> object, const char* name, ArgumentPack const& args) = 0;
	virtual std::type_index GetTypeIndex() const noexcept = 0;
	virtual bool Equal(const IType* other) const noexcept = 0;
};

template <typename T>
class BoxedObject;

struct Object
	: natRefObjImpl<Interface>
{
	virtual natRefPointer<IType> GetType() = 0;

	template <typename T>
	T& Unbox()
	{
		auto typeindex = GetType()->GetTypeIndex();
		if (typeindex != typeid(T) && typeindex != typeid(BoxedObject<T>))
		{
			throw std::runtime_error("Type wrong.");
		}

		return *static_cast<T*>(_getUnsafePtr());
	}

	template <typename T>
	static std::enable_if_t<std::is_base_of<Object, T>::value, natRefPointer<Object>> Box(T const& obj)
	{
		return make_ref<T>(obj);
	}

	template <typename T>
	static std::enable_if_t<std::is_base_of<Object, T>::value, natRefPointer<Object>> Box(T& obj)
	{
		return{ &obj };
	}

	template <typename T>
	static std::enable_if_t<std::is_base_of<Object, T>::value, natRefPointer<Object>> Box(T&& obj)
	{
		return make_ref<T>(std::move(obj));
	}

	template <typename T>
	static natRefPointer<Object> Box(natRefPointer<T> const& ptr)
	{
		return ptr;
	}

	template <typename T>
	static std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, natRefPointer<Object>> Box(T obj);

	static natRefPointer<Object> Box();

protected:
	virtual void* _getUnsafePtr()
	{
		return this;
	}
};

class ArgumentPack
{
public:
	template <typename... Args>
	ArgumentPack(Args&&... args)
		: m_Args{ Object::Box(std::forward<Args>(args))... }
	{
	}
	~ArgumentPack()
	{
	}

	natRefPointer<Object> Extract()
	{
		auto ret = std::move(m_Args.front());
		m_Args.pop_front();
		return ret;
	}

	natRefPointer<Object> Get(size_t n) const
	{
		return m_Args.at(n);
	}

	size_t Size() const
	{
		return m_Args.size();
	}

private:
	std::deque<natRefPointer<Object>> m_Args;
};

template <typename Func>
struct MethodHelper;

template <typename Ret, typename... Args>
struct MethodHelper<Ret(*)(Args...)>
{
	typedef Ret(*MethodType)(Args...);

	static decltype(auto) InvokeWithArgs(MethodType method, Args const&... args)
	{
		return method(args...);
	}

	static decltype(auto) InvokeWithArgPack(MethodType method, ArgumentPack const& pack)
	{
		typedef typename std::make_index_sequence<sizeof...(Args)>::type indexseq;
		return InvokeWithArgPackHelper(method, pack, indexseq{});
	}

	static natRefPointer<Object> Invoke(MethodType method, ArgumentPack const& pack)
	{
		return Object::Box(InvokeWithArgPack(method, pack));
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(method, pack.Get(i)->Unbox<Args>()...);
	}
};

template <typename... Args>
struct MethodHelper<void(*)(Args...)>
{
	typedef void(*MethodType)(Args...);

	static decltype(auto) InvokeWithArgs(MethodType method, Args const&... args)
	{
		return method(args...);
	}

	static decltype(auto) InvokeWithArgPack(MethodType method, ArgumentPack const& pack)
	{
		typedef typename std::make_index_sequence<sizeof...(Args)>::type indexseq;
		return InvokeWithArgPackHelper(method, pack, indexseq{});
	}

	static natRefPointer<Object> Invoke(MethodType method, ArgumentPack const& pack)
	{
		InvokeWithArgPack(method, pack);
		return Object::Box();
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(method, pack.Get(i)->Unbox<Args>()...);
	}
};

template <typename Ret, typename Class, typename... Args>
struct MethodHelper<Ret (Class::*)(Args...)>
{
	typedef Ret (Class::*MethodType)(Args...);

	static decltype(auto) InvokeWithArgs(Class* object, MethodType method, Args const&... args)
	{
		return (object->*method)(args...);
	}

	static decltype(auto) InvokeWithArgPack(Class* object, MethodType method, ArgumentPack const& pack)
	{
		typedef typename std::make_index_sequence<sizeof...(Args)>::type indexseq;
		return InvokeWithArgPackHelper(object, method, pack, indexseq{});
	}

	static natRefPointer<Object> Invoke(natRefPointer<Object> object, MethodType method, ArgumentPack const& pack)
	{
		return Object::Box(InvokeWithArgPack(&object->Unbox<Class>(), method, pack));
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}
};

template <typename Class, typename... Args>
struct MethodHelper<void (Class::*)(Args...)>
{
	typedef void (Class::*MethodType)(Args...);

	static decltype(auto) InvokeWithArgs(Class* object, MethodType method, Args const&... args)
	{
		return (object->*method)(args...);
	}

	static decltype(auto) InvokeWithArgPack(Class* object, MethodType method, ArgumentPack const& pack)
	{
		typedef typename std::make_index_sequence<sizeof...(Args)>::type indexseq;
		return InvokeWithArgPackHelper(object, method, pack, indexseq{});
	}

	static natRefPointer<Object> Invoke(natRefPointer<Object> object, MethodType method, ArgumentPack const& pack)
	{
		InvokeWithArgPack(&object->Unbox<Class>(), method, pack);
		return Object::Box();
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}
};

template <typename Ret, typename Class, typename... Args>
struct MethodHelper<Ret (Class::*)(Args...) const>
{
	typedef Ret (Class::*MethodType)(Args...) const;

	static decltype(auto) InvokeWithArgs(const Class* object, MethodType method, Args const&... args)
	{
		return (object->*method)(args...);
	}

	static decltype(auto) InvokeWithArgPack(const Class* object, MethodType method, ArgumentPack const& pack)
	{
		typedef typename std::make_index_sequence<sizeof...(Args)>::type indexseq;
		return InvokeWithArgPackHelper(object, method, pack, indexseq{});
	}

	static natRefPointer<Object> Invoke(natRefPointer<Object> object, MethodType method, ArgumentPack const& pack)
	{
		return Object::Box(InvokeWithArgPack(&object->Unbox<Class>(), method, pack));
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(const Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}
};

template <typename Class, typename... Args>
struct MethodHelper<void (Class::*)(Args...) const>
{
	typedef void (Class::*MethodType)(Args...) const;

	static decltype(auto) InvokeWithArgs(const Class* object, MethodType method, Args const&... args)
	{
		return (object->*method)(args...);
	}

	static decltype(auto) InvokeWithArgPack(const Class* object, MethodType method, ArgumentPack const& pack)
	{
		typedef typename std::make_index_sequence<sizeof...(Args)>::type indexseq;
		return InvokeWithArgPackHelper(object, method, pack, indexseq{});
	}

	static natRefPointer<Object> Invoke(natRefPointer<Object> object, MethodType method, ArgumentPack const& pack)
	{
		InvokeWithArgPack(&object->Unbox<Class>(), method, pack);
		return Object::Box();
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(const Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}
};

struct IMethod
	: Interface
{
	virtual bool Match(ArgumentPack const& pack) = 0;
	virtual natRefPointer<Object> Invoke(ArgumentPack const& pack) = 0;
};

struct IMemberMethod
	: Interface
{
	virtual bool Match(natRefPointer<Object> object, ArgumentPack const& pack) = 0;
	virtual natRefPointer<Object> Invoke(natRefPointer<Object> object, ArgumentPack const& pack) = 0;
};

template <typename Func>
class NonMemberMethod;

template <typename Ret, typename... Args>
class NonMemberMethod<Ret(*)(Args...)>
	: public natRefObjImpl<IMethod>
{
public:
	typedef Ret(*MethodType)(Args...);

	explicit NonMemberMethod(MethodType method)
		: m_Func(method)
	{
	}

	bool Match(ArgumentPack const& pack) override
	{
		return true;
	}

	natRefPointer<Object> Invoke(ArgumentPack const& pack) override
	{
		return MethodHelper<MethodType>::Invoke(m_Func, pack);
	}

	MethodType Get() const noexcept
	{
		return m_Func;
	}

private:
	MethodType m_Func;
};

template <typename Func>
class MemberMethod;

template <typename Ret, typename Class, typename... Args>
class MemberMethod<Ret(Class::*)(Args...)>
	: public natRefObjImpl<IMemberMethod>
{
public:
	typedef	Ret(Class::*MethodType)(Args...);

	explicit MemberMethod(MethodType method)
		: m_Func(method)
	{
	}

	bool Match(natRefPointer<Object> object, ArgumentPack const& pack) override
	{
		return true;
	}

	natRefPointer<Object> Invoke(natRefPointer<Object> object, ArgumentPack const& pack) override
	{
		return MethodHelper<MethodType>::Invoke(object, m_Func, pack);
	}

	MethodType Get() const noexcept
	{
		return m_Func;
	}

private:
	MethodType m_Func;
};

template <typename Ret, typename Class, typename... Args>
class MemberMethod<Ret(Class::*)(Args...) const>
	: public natRefObjImpl<IMemberMethod>
{
public:
	typedef	Ret(Class::*MethodType)(Args...) const;

	explicit MemberMethod(MethodType method)
		: m_Func(method)
	{
	}

	bool Match(natRefPointer<Object> object, ArgumentPack const& pack) override
	{
		return true;
	}

	natRefPointer<Object> Invoke(natRefPointer<Object> object, ArgumentPack const& pack) override
	{
		return MethodHelper<MethodType>::Invoke(object, m_Func, pack);
	}

	MethodType Get() const noexcept
	{
		return m_Func;
	}

private:
	MethodType m_Func;
};

template <typename T>
class Type
	: public natRefObjImpl<IType>
{
public:
	typedef T type;

	void RegisterNonMemberMethod(const char* name, natRefPointer<IMethod> method) override
	{
		m_NonMemberMethodMap[name] = method;
	}

	void RegisterMemberMethod(const char* name, natRefPointer<IMemberMethod> method) override
	{
		m_MemberMethodMap[name] = method;
	}

	const char* GetName() const noexcept override
	{
		return T::GetName();
	}

	natRefPointer<Object> Construct(ArgumentPack const& args) override
	{
		return InvokeNonMember("CreateInstance", args);
	}

	natRefPointer<Object> InvokeNonMember(const char* name, ArgumentPack const& args) override
	{
		auto iter = m_NonMemberMethodMap.find(name);
		if (iter == m_NonMemberMethodMap.end())
		{
			throw std::runtime_error("No such method.");
		}

		return iter->second->Invoke(args);
	}

	natRefPointer<Object> InvokeMember(natRefPointer<Object> object, const char* name, ArgumentPack const& args) override
	{
		auto iter = m_MemberMethodMap.find(name);
		if (iter == m_MemberMethodMap.end())
		{
			throw std::runtime_error("No such method.");
		}

		return iter->second->Invoke(object, args);
	}

	std::type_index GetTypeIndex() const noexcept override
	{
		return typeid(T);
	}

	bool Equal(const IType* other) const noexcept override
	{
		return GetTypeIndex() == other->GetTypeIndex();
	}

private:
	std::unordered_map<std::string, natRefPointer<IMethod>> m_NonMemberMethodMap;
	std::unordered_map<std::string, natRefPointer<IMemberMethod>> m_MemberMethodMap;
};

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

	natRefPointer<IType> GetType(const char* typeName)
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

private:
	Reflection()
	{
	}

	~Reflection()
	{
	}

	std::unordered_map<std::type_index, natRefPointer<IType>> m_TypeTable;
};

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

	operator T&&() noexcept
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

template <typename T>
Reflection::ReflectionRegister<BoxedObject<T>> BoxedObject<T>::_s_RefectionHelper_BoxedObject;

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

Reflection::ReflectionRegister<BoxedObject<void>> BoxedObject<void>::_s_RefectionHelper_BoxedObject;

typedef BoxedObject<void> Void;

bool operator==(natRefPointer<Object> const& ptr, nullptr_t)
{
	return ptr->GetType()->GetTypeIndex() == typeid(BoxedObject<void>) ? true : ptr.Get() == nullptr;
}

template <typename T>
std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, natRefPointer<Object>> Object::Box(T obj)
{
	return make_ref<BoxedObject<T>>(obj);
}

natRefPointer<Object> Object::Box()
{
	return make_ref<BoxedObject<void>>();
}

DECLARE_REFLECTABLE_CLASS(Foo)
{
public:
	GENERATE_METADATA(Foo);

	DECLARE_CONSTRUCTOR(Foo, int);
	DECLARE_CONST_MEMBER_METHOD(Foo, GetTest, int, int);

private:
	int m_Test;
};

GENERATE_METADATA_DEFINITION(Foo);

Foo::Foo(int value)
	: m_Test{ value }
{
}

DEFINE_CONSTRUCTOR(Foo, int)(int value)
{
	return make_ref<Foo>(value);
}

DEFINE_CONST_MEMBER_METHOD(Foo, GetTest, int, int)(int arg) const
{
	return m_Test + arg;
}

int main()
{
	try
	{
		auto type = Reflection::GetInstance().GetType("Foo");
		std::cout << type->GetName() << std::endl;
		auto pFoo = type->Construct({ 1 });
		std::cout << type->InvokeMember(pFoo, "GetTest", { 1 })->Unbox<int>() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
}
