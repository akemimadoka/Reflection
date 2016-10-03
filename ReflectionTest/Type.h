#pragma once
#include "Interface.h"
#include <unordered_map>

template <typename T>
class Type
	: public natRefObjImpl<IType>
{
public:
	typedef T type;

	void RegisterNonMemberMethod(const char* name, natRefPointer<IMethod> method) override
	{
		m_NonMemberMethodMap[name] = method;
	}

	void RegisterMemberMethod(const char* name, natRefPointer<IMemberMethod> method) override
	{
		m_MemberMethodMap[name] = method;
	}

	const char* GetName() const noexcept override
	{
		return T::GetName();
	}

	natRefPointer<Object> Construct(ArgumentPack const& args) override
	{
		return InvokeNonMember("CreateInstance", args);
	}

	natRefPointer<Object> InvokeNonMember(const char* name, ArgumentPack const& args) override
	{
		auto iter = m_NonMemberMethodMap.find(name);
		if (iter == m_NonMemberMethodMap.end())
		{
			throw std::runtime_error("No such method.");
		}

		return iter->second->Invoke(args);
	}

	natRefPointer<Object> InvokeMember(natRefPointer<Object> object, const char* name, ArgumentPack const& args) override
	{
		auto iter = m_MemberMethodMap.find(name);
		if (iter == m_MemberMethodMap.end())
		{
			throw std::runtime_error("No such method.");
		}

		return iter->second->Invoke(object, args);
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
		return false;
	}

	natRefPointer<Object> ConvertTo(natRefPointer<Object> object, natRefPointer<IType> toType) const noexcept override
	{
		return object;
	}

private:
	std::unordered_map<std::string, natRefPointer<IMethod>> m_NonMemberMethodMap;
	std::unordered_map<std::string, natRefPointer<IMemberMethod>> m_MemberMethodMap;
};
