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

	void RegisterNonMemberField(ncTStr name, natRefPointer<IField> field) override
	{
		m_NonMemberFieldMap.emplace(name, field);
	}

	void RegisterMemberField(ncTStr name, natRefPointer<IMemberField> field) override
	{
		m_MemberFieldMap.emplace(name, field);
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

	bool IsNonMemberFieldPointer(ncTStr name) override
	{
		auto iter = m_NonMemberFieldMap.find(name);
		if (iter == m_NonMemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		return iter->second->IsPointer();
	}

	bool IsMemberFieldPointer(ncTStr name) override
	{
		auto iter = m_MemberFieldMap.find(name);
		if (iter == m_MemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		return iter->second->IsPointer();
	}

	natRefPointer<Object> ReadNonMemberField(ncTStr name) override
	{
		auto iter = m_NonMemberFieldMap.find(name);
		if (iter == m_NonMemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		return iter->second->Read();
	}

	natRefPointer<Object> ReadMemberField(natRefPointer<Object> object, ncTStr name) override
	{
		auto iter = m_MemberFieldMap.find(name);
		if (iter == m_MemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		return iter->second->ReadFrom(object);
	}

	void WriteNonMemberField(ncTStr name, natRefPointer<Object> value) override
	{
		auto iter = m_NonMemberFieldMap.find(name);
		if (iter == m_NonMemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		iter->second->Write(value);
	}

	void WriteMemberField(natRefPointer<Object> object, ncTStr name, natRefPointer<Object> value) override
	{
		auto iter = m_MemberFieldMap.find(name);
		if (iter == m_MemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		iter->second->WriteFrom(object, value);
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
		for (auto&& item : m_NonMemberFieldMap)
		{
			if (enumFunc(item.first.c_str(), false, item.second->GetType()))
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
		for (auto&& item : m_MemberFieldMap)
		{
			if (enumFunc(item.first.c_str(), false, item.second->GetType()))
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
	/*bool IsConvertable(natRefPointer<IType> other) const noexcept override
	{
	}*/

	natRefPointer<Object> ConvertTo(natRefPointer<Object> object, natRefPointer<IType> toType) const override
	{
		if (object->GetType().Get() != static_cast<const IType*>(this))
		{
			nat_Throw(ReflectionException, _T("object is not a type of {0}."), GetName());
		}

		return toType->Construct(ArgumentPack{ object });
	}

private:
	bool CompatWithNonMember(ncTStr name, ArgumentPack const& args) const noexcept
	{
		auto range = m_NonMemberMethodMap.equal_range(name);
		if (range.first == range.second)
		{
			return false;
		}

		for (auto&& item : make_range(range.first, range.second))
		{
			if (item.second->CompatWith(args))
			{
				return true;
			}
		}

		return false;
	}

	bool CompatWithMember(ncTStr name, ArgumentPack const& args) const noexcept
	{
		auto range = m_MemberMethodMap.equal_range(name);
		if (range.first == range.second)
		{
			return false;
		}

		for (auto&& item : make_range(range.first, range.second))
		{
			if (item.second->CompatWith(args))
			{
				return true;
			}
		}

		return false;
	}

	std::unordered_multimap<nTString, natRefPointer<IMethod>> m_NonMemberMethodMap;
	std::unordered_multimap<nTString, natRefPointer<IMemberMethod>> m_MemberMethodMap;
	std::unordered_map<nTString, natRefPointer<IField>> m_NonMemberFieldMap;
	std::unordered_map<nTString, natRefPointer<IMemberField>> m_MemberFieldMap;
};
