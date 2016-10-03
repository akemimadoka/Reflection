#pragma once
#include <typeindex>
#include <natRefObj.h>
#include <natDelegate.h>

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

////////////////////////////////////////////////////////////////////////////////
/// @brief	类型接口
////////////////////////////////////////////////////////////////////////////////
struct IType : Interface
{
	virtual void RegisterNonMemberMethod(const char* name, natRefPointer<IMethod> method) = 0;
	virtual void RegisterMemberMethod(const char* name, natRefPointer<IMemberMethod> method) = 0;

	virtual const char* GetName() const noexcept = 0;
	virtual natRefPointer<Object> Construct(ArgumentPack const& args) = 0;
	virtual natRefPointer<Object> InvokeNonMember(const char* name, ArgumentPack const& args) = 0;
	virtual natRefPointer<Object> InvokeMember(natRefPointer<Object> object, const char* name, ArgumentPack const& args) = 0;

	/// @brief	枚举该类型下的非成员，当枚举函数返回true时立即结束枚举
	/// @param	enumFunc	枚举函数，接受参数的含义为 非成员名，是否为方法，非方法的类型（若是方法则为nullptr），返回bool值
	/// @return	是否因枚举函数返回true而结束枚举
	virtual bool EnumNonMember(Delegate<bool(const char*, bool, natRefPointer<IType>)> enumFunc) const = 0;

	/// @brief	枚举该类型下的成员，当枚举函数返回true时立即结束枚举
	/// @param	enumFunc	枚举函数，接受参数的含义为 成员名，是否为方法，非方法的类型（若是方法则为nullptr），返回bool值
	/// @return	是否因枚举函数返回true而结束枚举
	virtual bool EnumMember(Delegate<bool(const char*, bool, natRefPointer<IType>)> enumFunc) const = 0;

	virtual std::type_index GetTypeIndex() const noexcept = 0;
	virtual bool Equal(const IType* other) const noexcept = 0;

	virtual bool IsConvertable(natRefPointer<IType> other) const noexcept = 0;
	virtual natRefPointer<Object> ConvertTo(natRefPointer<Object> object, NatsuLib::natRefPointer<IType> toType) const noexcept = 0;
};
