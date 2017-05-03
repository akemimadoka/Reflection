#pragma once
#include "Interface.h"
#include "Object.h"
#include <vector>

#define WITH(...) AttributeSet{ __VA_ARGS__ }

struct IAttribute
	: virtual Object
{
	static nStrView GetName() noexcept
	{
		return "IAttribute"_nv;
	}

	natRefPointer<IType> GetType() const noexcept override;
};

struct AttributeSet
{
	template <typename... Attr>
	AttributeSet(Attr&&... attributes)
		: Attributes{ make_ref<Attr>(std::forward<Attr>(attributes))... }
	{
	}

	std::vector<natRefPointer<IAttribute>> Attributes;
};

enum class AttributeTarget : uint8_t
{
	None = 0x00,

	Class = 0x01,
	Constructor = 0x02,
	Field = 0x04,
	Method = 0x08,
	Parameter = 0x10,
	ReturnValue = 0x20,
	All = 0xFF,
};

MAKE_ENUM_CLASS_BITMASK_TYPE(AttributeTarget);

class AttributeUsage
	: public IAttribute
{
public:
	AttributeUsage() noexcept
		: AttributeUsage(AttributeTarget::All)
	{
	}

	explicit AttributeUsage(AttributeTarget target) noexcept
		: m_Target(target)
	{
	}

	static nStrView GetName() noexcept
	{
		return "AttributeUsage"_nv;
	}

	natRefPointer<IType> GetType() const noexcept override;

	AttributeTarget GetTarget() const noexcept
	{
		return m_Target;
	}

private:
	AttributeTarget m_Target;
};
