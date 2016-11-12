#pragma once
#include "Object.h"

struct Convert
{
	// TODO: 使用IConvertible实现
	static natRefPointer<Object> ConvertTo(natRefPointer<Object> obj, natRefPointer<IType> toType);

	template <typename T>
	static natRefPointer<Object> ConvertTo(natRefPointer<Object> obj);
};
