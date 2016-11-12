#pragma once
#include "Interface.h"

class AttributeUsage
	: public IAttribute
{
public:
	enum AttributeTarget : uint8_t
	{
		Class = 0x01,
		Constructor = 0x02,
		Field = 0x04,
		Method = 0x08,
		Parameter = 0x10,
		ReturnValue = 0x20,
		All = 0xFF,
	};

	constexpr explicit AttributeUsage(uint8_t target) noexcept
		: m_Target(target)
	{
	}

	uint8_t GetTarget() const noexcept
	{
		return m_Target;
	}

private:
	uint8_t m_Target;
};
