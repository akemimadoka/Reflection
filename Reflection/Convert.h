#pragma once
#include "Object.h"

struct Convert
{
	// TODO: ʹ��IConvertibleʵ��
	static natRefPointer<Object> ConvertTo(natRefPointer<Object> obj, natRefPointer<IType> toType);

	template <typename T>
	static natRefPointer<Object> ConvertTo(natRefPointer<Object> obj);
};
