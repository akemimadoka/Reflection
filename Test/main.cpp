#include <iostream>
#include <typeindex>
#include <Reflection.h>

DECLARE_REFLECTABLE_CLASS(Foo)
{
public:
	GENERATE_METADATA(Foo);

	DECLARE_CONSTRUCTOR(Foo, , int);
	DECLARE_DEFAULT_COPYCONSTRUCTOR(Foo);
	DECLARE_DEFAULT_MOVECONSTRUCTOR(Foo);
	DECLARE_CONST_MEMBER_METHOD(Foo, GetTest, 0, int);
	DECLARE_CONST_MEMBER_METHOD(Foo, GetTest, 1, int, int);

private:
	DECLARE_MEMBER_FIELD(Foo, int, m_Test);
};

GENERATE_METADATA_DEFINITION(Foo);

Foo::Foo(int value)
	: m_Test{ value }
{
}

DEFINE_CONSTRUCTOR(Foo, , int)(int value)
{
	return make_ref<Foo>(value);
}

DEFINE_DEFAULT_COPYCONSTRUCTOR(Foo)
DEFINE_DEFAULT_MOVECONSTRUCTOR(Foo)

DEFINE_CONST_MEMBER_METHOD(Foo, GetTest, 0, int)() const
{
	return m_Test;
}

DEFINE_CONST_MEMBER_METHOD(Foo, GetTest, 1, int, int)(int arg) const
{
	return m_Test + arg;
}

DEFINE_MEMBER_FIELD(Foo, int, m_Test);

int main()
{
	try
	{
		auto type = Reflection::GetInstance().GetType(_T("Foo"));
		std::wcout << type->GetName() << std::endl;
		auto pFoo = type->Construct({ 1 });
		std::wcout << type->InvokeMember(pFoo, _T("GetTest"), {})->Unbox<int>() << std::endl << type->InvokeMember(pFoo, _T("GetTest"), {1})->Unbox<int>() << std::endl;
		std::vector<nTString> members{};
		type->EnumMember([&members](ncTStr name, bool isMethod, natRefPointer<IType> objectType)
		{
			members.emplace_back(name);
			return false;
		});
		for (auto&& item : members)
		{
			std::wcout << item << std::endl;
		}
		std::wcout << type->ReadMemberField(pFoo, _T("m_Test"))->Unbox<int>() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
}
