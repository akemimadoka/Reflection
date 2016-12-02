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

	AttributeUsage() noexcept
		: AttributeUsage(All)
	{
	}

	explicit AttributeUsage(uint8_t target) noexcept
		: m_Target(target)
	{
	}

	static nStrView GetName() noexcept
	{
		return "AttributeUsage"_nv;
	}

	natRefPointer<IType> GetType() const noexcept override;

	uint8_t GetTarget() const noexcept
	{
		return m_Target;
	}

private:
	uint8_t m_Target;
};
