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

	void RegisterBaseClasses(std::initializer_list<natRefPointer<IType>> baseClasses) override;

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

	bool IsBoxed() const noexcept override;

	natRefPointer<Object> Construct(ArgumentPack const& args) override
	{
		return InvokeNonMember(CONSTRUCTOR_NAME_STR, args);
	}

	size_t GetBaseClassesCount() const noexcept override
	{
		return m_BaseClasses.size();
	}

	natRefPointer<IType> GetBaseClass(size_t n) const noexcept override
	{
		return m_BaseClasses.at(n);
	}

	Linq<const natRefPointer<IType>> GetBaseClasses() const noexcept override
	{
		return from(m_BaseClasses);
	}

	natRefPointer<Object> InvokeNonMember(ncTStr name, ArgumentPack const& args) override
	{
		auto range = m_NonMemberMethodMap.equal_range(name);
		if (range.first == range.second)
		{
			for (auto&& item : m_BaseClasses)
			{
				try
				{
					return item->InvokeNonMember(name, args);
				}
				catch (...)
				{
					// if all, rethrow below
				}
			}
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
			for (auto&& item : m_BaseClasses)
			{
				try
				{
					return item->InvokeMember(object, name, args);
				}
				catch (...)
				{
					// if all, rethrow below
				}
			}
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

	natRefPointer<IMethod> GetNonMemberMethod(ncTStr name, std::initializer_list<natRefPointer<IType>> const& argTypes) override
	{
		auto range = m_NonMemberMethodMap.equal_range(name);
		if (range.first == range.second)
		{
			for (auto&& item : m_BaseClasses)
			{
				try
				{
					return item->GetNonMemberMethod(name, argTypes);
				}
				catch (...)
				{
					// if all, rethrow below
				}
			}
			nat_Throw(ReflectionException, _T("No such nonmember method named {0}."), name);
		}

		for (auto&& item : make_range(range.first, range.second))
		{
			if (item.second->GetArgumentCount() == argTypes.size())
			{
				for (size_t i = 0; i < argTypes.size(); ++i)
				{
					if (!item.second->GetArgumentType(i)->Equal(std::next(begin(argTypes), i)->Get()))
					{
						goto NotMatch;
					}
				}
				return item.second;
			}
		NotMatch:
			continue;
		}

		nat_Throw(ReflectionException, _T("None of overloaded nonmember method named {0} can be invoked with given args."), name);
	}

	natRefPointer<IMemberMethod> GetMemberMethod(ncTStr name, std::initializer_list<natRefPointer<IType>> const& argTypes) override
	{
		auto range = m_MemberMethodMap.equal_range(name);
		if (range.first == range.second)
		{
			for (auto&& item : m_BaseClasses)
			{
				try
				{
					return item->GetMemberMethod(name, argTypes);
				}
				catch (...)
				{
					// if all, rethrow below
				}
			}
			nat_Throw(ReflectionException, _T("No such member method named {0}."), name);
		}

		for (auto&& item : make_range(range.first, range.second))
		{
			if (item.second->GetArgumentCount() == argTypes.size())
			{
				for (size_t i = 0; i < argTypes.size(); ++i)
				{
					if (!item.second->GetArgumentType(i)->Equal(std::next(begin(argTypes), i)->Get()))
					{
						goto NotMatch;
					}
				}
				return item.second;
			}
		NotMatch:
			continue;
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
	
	natRefPointer<IField> GetNonMemberField(ncTStr name) override
	{
		auto iter = m_NonMemberFieldMap.find(name);
		if (iter == m_NonMemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		return iter->second;
	}

	natRefPointer<IMemberField> GetMemberField(ncTStr name) override
	{
		auto iter = m_MemberFieldMap.find(name);
		if (iter == m_MemberFieldMap.end())
		{
			nat_Throw(ReflectionException, _T("No such member field named {0}."), name);
		}

		return iter->second;
	}

	bool EnumNonMember(bool recurse, Delegate<bool(ncTStr, bool, natRefPointer<IType>)> enumFunc) const override
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
		if (recurse)
		{
			for (auto&& item : m_BaseClasses)
			{
				item->EnumNonMember(true, enumFunc);
			}
		}

		return false;
	}

	Linq<const std::pair<const nTString, natRefPointer<IMethod>>> GetNonMemberMethods() const noexcept override
	{
		Linq<const std::pair<const nTString, natRefPointer<IMethod>>> ret = from(m_NonMemberMethodMap);
		for (auto&& item : m_BaseClasses)
		{
			ret = ret.concat(item->GetNonMemberMethods());
		}
		return ret;
	}

	Linq<const std::pair<const nTString, natRefPointer<IField>>> GetNonMemberFields() const noexcept override
	{
		Linq<const std::pair<const nTString, natRefPointer<IField>>> ret = from(m_NonMemberFieldMap);
		for (auto&& item : m_BaseClasses)
		{
			ret = ret.concat(item->GetNonMemberFields());
		}
		return ret;
	}

	bool EnumMember(bool recurse, Delegate<bool(ncTStr, bool, natRefPointer<IType>)> enumFunc) const override
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
		if (recurse)
		{
			for (auto&& item : m_BaseClasses)
			{
				item->EnumMember(true, enumFunc);
			}
		}

		return false;
	}

	Linq<const std::pair<const nTString, natRefPointer<IMemberMethod>>> GetMemberMethods() const noexcept override
	{
		Linq<const std::pair<const nTString, natRefPointer<IMemberMethod>>> ret = from(m_MemberMethodMap);
		for (auto&& item : m_BaseClasses)
		{
			ret = ret.concat(item->GetMemberMethods());
		}
		return ret;
	}

	Linq<const std::pair<const nTString, natRefPointer<IMemberField>>> GetMemberFields() const noexcept override
	{
		Linq<const std::pair<const nTString, natRefPointer<IMemberField>>> ret = from(m_MemberFieldMap);
		for (auto&& item : m_BaseClasses)
		{
			ret = ret.concat(item->GetMemberFields());
		}
		return ret;
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

	bool IsExtendFrom(natRefPointer<IType> type) const override
	{
		if (!type)
		{
			return false;
		}

		for (auto&& item : m_BaseClasses)
		{
			if (item->Equal(type) || item->IsExtendFrom(type))
			{
				return true;
			}
		}

		return false;
	}

private:
	std::vector<natRefPointer<IType>> m_BaseClasses;
	std::unordered_multimap<nTString, natRefPointer<IMethod>> m_NonMemberMethodMap;
	std::unordered_multimap<nTString, natRefPointer<IMemberMethod>> m_MemberMethodMap;
	std::unordered_map<nTString, natRefPointer<IField>> m_NonMemberFieldMap;
	std::unordered_map<nTString, natRefPointer<IMemberField>> m_MemberFieldMap;
};
