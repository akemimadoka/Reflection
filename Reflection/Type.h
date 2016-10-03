#pragma once
#include "Interface.h"
#include <unordered_map>
#include <natMisc.h>

#define STR(x) L#x
#define CONSTRUCTOR_NAME Constructor
#define CONSTRUCTOR_NAME_STR STR(Constructor)

template <typename T>
class Type
	: public natRefObjImpl<IType>
{
public:
	typedef T type;

	void RegisterNonMemberMethod(ncTStr name, natRefPointer<IMethod> method) override
	{
		m_NonMemberMethodMap.emplace(name, method);
	}

	void RegisterMemberMethod(ncTStr name, natRefPointer<IMemberMethod> method) override
	{
		m_MemberMethodMap.emplace(name, method);
	}

	ncTStr GetName() const noexcept override
	{
		return T::GetName();
	}

	natRefPointer<Object> Construct(ArgumentPack const& args) override
	{
		return InvokeNonMember(CONSTRUCTOR_NAME_STR, args);
	}

	natRefPointer<Object> InvokeNonMember(ncTStr name, ArgumentPack const& args) override
	{
		auto range = m_NonMemberMethodMap.equal_range(name);
		if (range.first == range.second)
		{
			nat_Throw(ReflectionException, _T("No such nonmember method named {0}."), name);
		}

		for (auto&& item : make_range(range.first, range.second))
		{
			if (item.second->CompatWith(args))
			{
				return item.second->Invoke(args);
			}
		}

		nat_Throw(ReflectionException, _T("None of overloaded nonmember method named {0} can be invoked with given args."), name);
	}

	natRefPointer<Object> InvokeMember(natRefPointer<Object> object, ncTStr name, ArgumentPack const& args) override
	{
		auto range = m_MemberMethodMap.equal_range(name);
		if (range.first == range.second)
		{
			nat_Throw(ReflectionException, _T("No such member method named {0}."), name);
		}

		for (auto&& item : make_range(range.first, range.second))
		{
			if (item.second->CompatWith(object, args))
			{
				return item.second->Invoke(object, args);
			}
		}

		nat_Throw(ReflectionException, _T("None of overloaded member method named {0} can be invoked with given args."), name);
	}

	bool EnumNonMember(Delegate<bool(ncTStr, bool, natRefPointer<IType>)> enumFunc) const override
	{
		for (auto&& item : m_NonMemberMethodMap)
		{
			if (enumFunc(item.first.c_str(), true, {}))
			{
				return true;
			}
		}

		return false;
	}

	bool EnumMember(Delegate<bool(ncTStr, bool, natRefPointer<IType>)> enumFunc) const override
	{
		for (auto&& item : m_MemberMethodMap)
		{
			if (enumFunc(item.first.c_str(), true, {}))
			{
				return true;
			}
		}

		return false;
	}

	std::type_index GetTypeIndex() const noexcept override
	{
		return typeid(T);
	}

	bool Equal(const IType* other) const noexcept override
	{
		return GetTypeIndex() == other->GetTypeIndex();
	}

	// TODO: Implement converting function
	bool IsConvertable(natRefPointer<IType> other) const noexcept override
	{
		return true;
	}

	natRefPointer<Object> ConvertTo(natRefPointer<Object> object, natRefPointer<IType> toType) const override
	{
		if (object->GetType().Get() != static_cast<const IType*>(this))
		{
			nat_Throw(ReflectionException, _T("object is not a type of {0}."), GetName());
		}

		return toType->Construct(ArgumentPack{ object });
	}

private:
	std::unordered_multimap<nTString, natRefPointer<IMethod>> m_NonMemberMethodMap;
	std::unordered_multimap<nTString, natRefPointer<IMemberMethod>> m_MemberMethodMap;
};
