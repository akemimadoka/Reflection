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

	explicit NonMemberField(FieldType field)
		: m_Field{ field }
	{
	}

	natRefPointer<IType> GetType() override
	{
		return m_Field->GetType();
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

	explicit MemberField(FieldType field)
		: m_Field{ field }
	{
	}

	natRefPointer<IType> GetType() override
	{
		return Reflection::GetInstance().GetType<boxed_type_t<T>>();
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
	FieldType m_Field;
};
