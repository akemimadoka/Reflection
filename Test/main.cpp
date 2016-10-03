#include <iostream>
#include <typeindex>
#include <Reflection.h>

DECLARE_REFLECTABLE_CLASS(Foo)
{
public:
	GENERATE_METADATA(Foo);

	DECLARE_CONSTRUCTOR(Foo, int);
	DECLARE_CONST_MEMBER_METHOD(Foo, GetTest, int);
	DECLARE_CONST_MEMBER_METHOD(Foo, GetTestWithArg, int, int);

private:
	int m_Test;
};

GENERATE_METADATA_DEFINITION(Foo);

Foo::Foo(int value)
	: m_Test{ value }
{
}

DEFINE_CONSTRUCTOR(Foo, int)(int value)
{
	return make_ref<Foo>(value);
}

DEFINE_CONST_MEMBER_METHOD(Foo, GetTest, int)() const
{
	return m_Test;
}

DEFINE_CONST_MEMBER_METHOD(Foo, GetTestWithArg, int, int)(int arg) const
{
	return m_Test + arg;
}

int main()
{
	try
	{
		auto type = Reflection::GetInstance().GetType("Foo");
		std::cout << type->GetName() << std::endl;
		auto pFoo = type->Construct({ 1 });
		std::cout << type->InvokeMember(pFoo, "GetTest", {})->Unbox<int>() << std::endl << type->InvokeMember(pFoo, "GetTestWithArg", {1})->Unbox<int>() << std::endl;
		std::vector<std::string> members{};
		type->EnumMember([&members](const char* name, bool isMethod, natRefPointer<IType> objectType)
		{
			members.emplace_back(name);
			return false;
		});
		for (auto&& item : members)
		{
			std::cout << item << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
}
