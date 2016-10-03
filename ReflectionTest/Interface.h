#pragma once
#include <typeindex>
#include <natRefObj.h>

class ArgumentPack;
using namespace NatsuLib;

struct Interface
	: natRefObj
{
};

struct Object;

struct IMethod
	: Interface
{
	virtual bool Match(ArgumentPack const& pack) = 0;
	virtual natRefPointer<Object> Invoke(ArgumentPack const& pack) = 0;
};

struct IMemberMethod
	: Interface
{
	virtual bool Match(natRefPointer<Object> object, ArgumentPack const& pack) = 0;
	virtual natRefPointer<Object> Invoke(natRefPointer<Object> object, ArgumentPack const& pack) = 0;
};

struct IType : Interface
{
	virtual void RegisterNonMemberMethod(const char* name, natRefPointer<IMethod> method) = 0;
	virtual void RegisterMemberMethod(const char* name, natRefPointer<IMemberMethod> method) = 0;

	virtual const char* GetName() const noexcept = 0;
	virtual natRefPointer<Object> Construct(ArgumentPack const& args) = 0;
	virtual natRefPointer<Object> InvokeNonMember(const char* name, ArgumentPack const& args) = 0;
	virtual natRefPointer<Object> InvokeMember(natRefPointer<Object> object, const char* name, ArgumentPack const& args) = 0;
	virtual std::type_index GetTypeIndex() const noexcept = 0;
	virtual bool Equal(const IType* other) const noexcept = 0;

	virtual bool IsConvertable(natRefPointer<IType> other) const noexcept = 0;
	virtual natRefPointer<Object> ConvertTo(natRefPointer<Object> object, NatsuLib::natRefPointer<IType> toType) const noexcept = 0;
};
