#pragma once
#include "Interface.h"
#include "Object.h"

template <typename T>
struct IsRefPointer
	: std::false_type
{
};

template <typename T>
struct IsRefPointer<natRefPointer<T>>
	: std::true_type
{
};

template <typename T>
class NonMemberField;

template <typename T>
class NonMemberField<T*>
	: public natRefObjImpl<IField>
{
public:
	typedef T* FieldType;

	explicit NonMemberField(AccessSpecifier accessSpecifier, FieldType field)
		: m_AccessSpecifier{ accessSpecifier }, m_Field { field }
	{
	}

	natRefPointer<IType> GetType() override
	{
		return m_Field->GetType();
	}

	AccessSpecifier GetAccessSpecifier() const noexcept override
	{
		return m_AccessSpecifier;
	}

	AccessSpecifier SetAccessSpecifier(AccessSpecifier accessSpecifier) noexcept override
	{
		return std::exchange(m_AccessSpecifier, accessSpecifier);
	}

	bool IsPointer() const noexcept override
	{
		return IsRefPointer<T>::value;
	}

	natRefPointer<Object> Read() override
	{
		return Object::Box(*m_Field);
	}

	void Write(natRefPointer<Object> value) override
	{
		*m_Field = value->Unbox<T>();
	}

private:
	AccessSpecifier m_AccessSpecifier;
	FieldType m_Field;
};

template <typename T>
class MemberField;

template <typename Class, typename T>
class MemberField<T(Class::*)>
	: public natRefObjImpl<IMemberField>
{
public:
	typedef T(Class::*FieldType);

	explicit MemberField(AccessSpecifier accessSpecifier, FieldType field)
		: m_AccessSpecifier{ accessSpecifier }, m_Field{ field }
	{
	}

	natRefPointer<IType> GetType() override;

	AccessSpecifier GetAccessSpecifier() const noexcept override
	{
		return m_AccessSpecifier;
	}

	AccessSpecifier SetAccessSpecifier(AccessSpecifier accessSpecifier) noexcept override
	{
		return std::exchange(m_AccessSpecifier, accessSpecifier);
	}

	bool IsPointer() const noexcept override
	{
		return IsRefPointer<T>::value;
	}

	natRefPointer<Object> ReadFrom(natRefPointer<Object> object) override
	{
		return Object::Box(object->Unbox<Class>().*m_Field);
	}

	void WriteFrom(natRefPointer<Object> object, natRefPointer<Object> value) override
	{
		object->Unbox<Class>().*m_Field = value->Unbox<T>();
	}

private:
	AccessSpecifier m_AccessSpecifier;
	FieldType m_Field;
};
