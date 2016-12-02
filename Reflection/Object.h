#pragma once
#include "Interface.h"

template <typename Class, typename... Args, size_t... i>
natRefPointer<Object> CommonConstructor(std::tuple<Args...>&& args, std::index_sequence<i...>)
{
	return make_ref<Class>(std::forward<Args>(std::get<i>(args))...);
}

struct Object
	: natRefObjImpl<Interface>
{
	virtual ~Object();

	static nStrView GetName() noexcept
	{
		return "Object"_nv;
	}

	virtual natRefPointer<IType> GetType() const noexcept;
	virtual nString ToString() const noexcept;
	virtual std::type_index GetUnboxedType();

	template <typename T>
	T& Unbox();

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

	// ¿ª¶´×¢Òâ
	static natRefPointer<Object> Box(nStrView const& view)
	{
		return Box(nString{ view });
	}

	template <typename T>
	static std::enable_if_t<std::disjunction<std::is_arithmetic<T>, std::is_same<T, nString>>::value, natRefPointer<Object>> Box(T obj);

	static natRefPointer<Object> Box();
};

#include "Reflection.h"
