#pragma once
#include "Interface.h"
#include "Object.h"
#include <deque>

class ArgumentPack
{
public:
	template <typename... Args>
	ArgumentPack(Args&&... args)
		: m_Args{ Object::Box(std::forward<Args>(args))... }
	{
	}
	~ArgumentPack();

	natRefPointer<Object> Extract();
	natRefPointer<Object> Get(size_t n) const;
	natRefPointer<IType> GetType(size_t n) const;
	size_t Size() const;

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
		return InvokeWithArgPackHelper(method, pack, typename std::make_index_sequence<sizeof...(Args)>::type{});
	}

	static natRefPointer<Object> Invoke(MethodType method, ArgumentPack const& pack)
	{
		return Object::Box(InvokeWithArgPack(method, pack));
	}

	static bool CompatWith(ArgumentPack const& pack)
	{
		return CompatWithImpl(pack, typename std::make_index_sequence<sizeof...(Args)>::type{});
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(method, pack.Get(i)->Unbox<Args>()...);
	}

	template <size_t... i>
	static bool CompatWithImpl(ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return pack.Size() == sizeof...(i) && std::make_tuple((pack.GetType(i)->GetTypeIndex() == typeid(boxed_type_t<Args>))...) == std::make_tuple(std::bool_constant<i >= 0>::value...);
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

	static bool CompatWith(ArgumentPack const& pack)
	{
		return CompatWithImpl(pack, typename std::make_index_sequence<sizeof...(Args)>::type{});
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(method, pack.Get(i)->Unbox<Args>()...);
	}

	template <size_t... i>
	static bool CompatWithImpl(ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return pack.Size() == sizeof...(i) && std::make_tuple((pack.GetType(i)->GetTypeIndex() == typeid(boxed_type_t<Args>))...) == std::make_tuple(std::bool_constant<i >= 0>::value...);
	}
};

template <typename Ret, typename Class, typename... Args>
struct MethodHelper<Ret(Class::*)(Args...)>
{
	typedef Ret(Class::*MethodType)(Args...);

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

	static bool CompatWith(natRefPointer<Object> object, ArgumentPack const& pack)
	{
		return object->GetType()->GetTypeIndex() == typeid(Class) && CompatWithImpl(pack, typename std::make_index_sequence<sizeof...(Args)>::type{});
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}

	template <size_t... i>
	static bool CompatWithImpl(ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return pack.Size() == sizeof...(i) && std::make_tuple((pack.GetType(i)->GetTypeIndex() == typeid(boxed_type_t<Args>))...) == std::make_tuple(std::bool_constant<i >= 0>::value...);
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

	static bool CompatWith(natRefPointer<Object> object, ArgumentPack const& pack)
	{
		return object->GetType()->GetTypeIndex() == typeid(Class) && CompatWithImpl(pack, typename std::make_index_sequence<sizeof...(Args)>::type{});
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}

	template <size_t... i>
	static bool CompatWithImpl(ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return pack.Size() == sizeof...(i) && std::make_tuple((pack.GetType(i)->GetTypeIndex() == typeid(boxed_type_t<Args>))...) == std::make_tuple(std::bool_constant<i >= 0>::value...);
	}
};

template <typename Ret, typename Class, typename... Args>
struct MethodHelper<Ret(Class::*)(Args...) const>
{
	typedef Ret(Class::*MethodType)(Args...) const;

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

	static bool CompatWith(natRefPointer<Object> object, ArgumentPack const& pack)
	{
		return object->GetType()->GetTypeIndex() == typeid(Class) && CompatWithImpl(pack, typename std::make_index_sequence<sizeof...(Args)>::type{});
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(const Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}

	template <size_t... i>
	static bool CompatWithImpl(ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return pack.Size() == sizeof...(i) && std::make_tuple((pack.GetType(i)->GetTypeIndex() == typeid(boxed_type_t<Args>))...) == std::make_tuple(std::bool_constant<i >= 0>::value...);
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

	static bool CompatWith(natRefPointer<Object> object, ArgumentPack const& pack)
	{
		return object->GetType()->GetTypeIndex() == typeid(Class) && CompatWithImpl(pack, typename std::make_index_sequence<sizeof...(Args)>::type{});
	}

private:
	template <size_t... i>
	static decltype(auto) InvokeWithArgPackHelper(const Class* object, MethodType method, ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return InvokeWithArgs(object, method, pack.Get(i)->Unbox<Args>()...);
	}

	template <size_t... i>
	static bool CompatWithImpl(ArgumentPack const& pack, std::index_sequence<i...>)
	{
		return pack.Size() == sizeof...(i) && std::make_tuple((pack.GetType(i)->GetTypeIndex() == typeid(boxed_type_t<Args>))...) == std::make_tuple(std::bool_constant<i >= 0>::value...);
	}
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

	bool CompatWith(ArgumentPack const& pack) const noexcept override
	{
		return MethodHelper<MethodType>::CompatWith(pack);
	}

	natRefPointer<IType> GetReturnType() const noexcept override
	{
		return Reflection::GetInstance().GetType<Ret>();
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

	bool CompatWith(natRefPointer<Object> object, ArgumentPack const& pack) const noexcept override
	{
		return MethodHelper<MethodType>::CompatWith(object, pack);
	}

	natRefPointer<IType> GetReturnType() const noexcept override
	{
		return Reflection::GetInstance().GetType<Ret>();
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

	bool CompatWith(natRefPointer<Object> object, ArgumentPack const& pack) const noexcept override
	{
		return MethodHelper<MethodType>::CompatWith(object, pack);
	}

	natRefPointer<IType> GetReturnType() const noexcept override
	{
		return Reflection::GetInstance().GetType<Ret>();
	}

	MethodType Get() const noexcept
	{
		return m_Func;
	}

private:
	MethodType m_Func;
};
