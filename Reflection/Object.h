#pragma once
#include <natRefObj.h>

using namespace NatsuLib;

template <typename Class, typename... Args, size_t... i>
natRefPointer<Object> CommonConstructor(std::tuple<Args...>&& args, std::index_sequence<i...>)
{
	return make_ref<Class>(std::forward<Args>(std::get<i>(args))...);
}

struct Object
	: natRefObjImpl<Interface>
{
	virtual ~Object();

	static ncTStr GetName() noexcept;

	virtual natRefPointer<IType> GetType() const noexcept;
	virtual nTString ToString() const noexcept;
	virtual std::type_index GetUnboxedType();

	template <typename T>
	T& Unbox();

	template <typename T>
	static std::enable_if_t<std::is_base_of<Object, T>::value, natRefPointer<Object>> Box(T&& obj)
	{
		return make_ref<T>(std::forward<T>(obj));
	}

	template <typename T>
	static natRefPointer<Object> Box(natRefPointer<T> const& ptr)
	{
		return ptr;
	}

	template <typename T>
	static std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, natRefPointer<Object>> Box(T obj);

	static natRefPointer<Object> Box();
};

#include "Reflection.h"
