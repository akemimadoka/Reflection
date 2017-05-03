#pragma once
#include "Interface.h"
#include "Object.h"
#include "Convert.h"

namespace rdetail_
{
	template <typename T>
	struct FieldSetter
	{
		static void Set(T& val, natRefPointer<Object> const& pObj)
		{
			val = Convert::ConvertTo<std::remove_cv_t<std::remove_reference_t<T>>>(pObj)->Unbox<std::remove_cv_t<std::remove_reference_t<T>>>();
		}
	};

	template <typename T>
	struct FieldSetter<natRefPointer<T>>
	{
		static void Set(natRefPointer<T>& val, natRefPointer<Object> const& pObj)
		{
			if (!pObj)
			{
				val = nullptr;
			}
			else
			{
				auto tmp = static_cast<natRefPointer<T>>(pObj);
				if (!tmp)
				{
					tmp = Convert::ConvertTo<std::remove_cv_t<std::remove_reference_t<T>>>(pObj);
					//nat_Throw(ReflectionException, "pObj cannot assign to val."_nv);
				}
				
				val = std::move(tmp);
			}
		}
	};

	template <typename T>
	struct IsRefPointerImpl
		: std::false_type
	{
	};

	template <typename T>
	struct IsRefPointerImpl<natRefPointer<T>>
		: std::true_type
	{
	};
}

template <typename T>
struct IsRefPointer
	: rdetail_::IsRefPointerImpl<std::remove_cv_t<std::remove_reference_t<T>>>
{
};

template <typename T>
class NonMemberField;

template <typename T>
class NonMemberField<T*>
	: public natRefObjImpl<NonMemberField<T*>, IField>
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
		//*m_Field = value->Unbox<T>();
		rdetail_::FieldSetter<std::remove_volatile_t<std::remove_reference_t<T>>>::Set(*m_Field, value);
	}

private:
	AccessSpecifier m_AccessSpecifier;
	FieldType m_Field;
};

template <typename T>
class MemberField;

template <typename Class, typename T>
class MemberField<T(Class::*)>
	: public natRefObjImpl<MemberField<T(Class::*)>, IMemberField>
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
		//object->Unbox<Class>().*m_Field = value->Unbox<T>();
		rdetail_::FieldSetter<std::remove_volatile_t<std::remove_reference_t<T>>>::Set(object->Unbox<Class>().*m_Field, value);
	}

private:
	AccessSpecifier m_AccessSpecifier;
	FieldType m_Field;
};
