#pragma once
#include <natRefObj.h>

using namespace NatsuLib;

struct Object
	: natRefObjImpl<Interface>
{
	virtual natRefPointer<IType> GetType() const noexcept = 0;
	virtual nTString ToString() const noexcept
	{
		return GetType()->GetName();
	}

	virtual std::type_index GetUnboxedType()
	{
		return GetType()->GetTypeIndex();
	}

	template <typename T>
	T& Unbox();

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

#include "Reflection.h"
