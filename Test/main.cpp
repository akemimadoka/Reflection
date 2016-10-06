#include <iostream>
#include <typeindex>
#include <Reflection.h>

DECLARE_REFLECTABLE_CLASS(Foo)
{
public:
	GENERATE_METADATA(Foo)

	DECLARE_CONSTRUCTOR(Foo, explicit, , int);
	DECLARE_DEFAULT_COPYCONSTRUCTOR(Foo);
	DECLARE_DEFAULT_MOVECONSTRUCTOR(Foo);
	DECLARE_CONST_MEMBER_METHOD(Foo, , GetTest, 0, int);
	DECLARE_CONST_MEMBER_METHOD(Foo, , GetTest, 1, int, int);
	DECLARE_VIRTUAL_MEMBER_METHOD(Foo, , Test, , int);

private:
	DECLARE_MEMBER_FIELD(Foo, int, m_Test);
};

GENERATE_METADATA_DEFINITION(Foo);

DEFINE_CONSTRUCTOR(Foo, , , int)(int value)
	: m_Test{ value }
{
}

DEFINE_DEFAULT_COPYCONSTRUCTOR(Foo)
DEFINE_DEFAULT_MOVECONSTRUCTOR(Foo)

DEFINE_CONST_MEMBER_METHOD(Foo, , GetTest, 0, int)() const
{
	return m_Test;
}

DEFINE_CONST_MEMBER_METHOD(Foo, , GetTest, 1, int, int)(int arg) const
{
	return m_Test + arg;
}

DEFINE_VIRTUAL_MEMBER_METHOD(Foo, , Test, , int)()
{
	return ++m_Test;
}

DEFINE_MEMBER_FIELD(Foo, int, m_Test);

DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASS(Bar, Foo)
{
public:
	GENERATE_METADATA(Bar)

	DECLARE_CONSTRUCTOR(Bar, explicit, , int);
	DECLARE_VIRTUAL_MEMBER_METHOD(Bar, , Test, , int);
};

GENERATE_METADATA_DEFINITION(Bar);

DEFINE_CONSTRUCTOR(Bar, explicit, , int)(int arg)
	: Foo(arg)
{
}

DEFINE_VIRTUAL_MEMBER_METHOD(Bar, , Test, , int)()
{
	return Foo::Test() + 1;
}

int main()
{
	try
	{
		auto type = typeofname(_T("Foo"));
		auto pFoo = type->Construct({ 1 });
		std::wcout << pFoo->ToString() << std::endl << type->InvokeMember(pFoo, _T("GetTest"), {})->ToString() << std::endl << type->InvokeMember(pFoo, _T("GetTest"), { 1 })->ToString() << std::endl;
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
		std::wcout << type->InvokeMember(pFoo, _T("Test"), {})->ToString() << std::endl;
		std::wcout << type->ReadMemberField(pFoo, _T("m_Test"))->ToString() << std::endl;

		auto type2 = typeofname(_T("Bar"));
		auto pBar = type2->Construct({ 1 });
		std::wcout << type2->InvokeMember(pBar, _T("Test"), {})->ToString() << std::endl;
	}
	catch (ReflectionException& e)
	{
		std::wcout << e.GetDesc() << std::endl;
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
}
