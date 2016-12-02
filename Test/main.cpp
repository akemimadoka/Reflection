#include <iostream>
#include <chrono>
#include <Reflection.h>

/*DECLARE_REFLECTABLE_INTERFACE(ISerializable)
{
	GENERATE_METADATA(ISerializable);

	DECLARE_PURE_VIRTUAL_MEMBER_METHOD(public, ISerializable, , Serialize, , size_t, );
	DECLARE_PURE_VIRTUAL_MEMBER_METHOD(public, ISerializable, , Deserialize, , size_t, );
};

GENERATE_METADATA_DEFINITION(ISerializable);

DEFINE_PURE_VIRTUAL_MEMBER_METHOD(public, ISerializable, , Serialize, , size_t, );
DEFINE_PURE_VIRTUAL_MEMBER_METHOD(public, ISerializable, , Deserialize, , size_t, );*/

DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASSES(TestAttribute, IAttribute)
{
	GENERATE_METADATA_WITH_BASE_CLASSES(TestAttribute, WITH(AttributeUsage(AttributeUsage::Class)), IAttribute);

	DECLARE_CONST_MEMBER_METHOD(public, TestAttribute, , Test, , int);
};

GENERATE_METADATA_DEFINITION_WITH_BASE_CLASSES(TestAttribute, WITH(AttributeUsage(AttributeUsage::Class)), IAttribute);

DEFINE_CONST_MEMBER_METHOD(public, TestAttribute, , Test, , int)() const
{
	return 1;
}

DECLARE_REFLECTABLE_CLASS(Foo)
{
	GENERATE_METADATA(Foo, WITH(TestAttribute()))

	DECLARE_CONSTRUCTOR(public, Foo, explicit, , int);
	DECLARE_DEFAULT_COPYCONSTRUCTOR(public, Foo);
	DECLARE_DEFAULT_MOVECONSTRUCTOR(public, Foo);
	DECLARE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 0, int const&);
	DECLARE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 1, int, int const&);
	DECLARE_VIRTUAL_MEMBER_METHOD(public, Foo, , Test, , int);
	DECLARE_MEMBER_METHOD(public, Foo, , Test1, , void);

	DECLARE_MEMBER_FIELD(private, Foo, int, m_Test);
};

GENERATE_METADATA_DEFINITION(Foo, WITH(TestAttribute()));

DEFINE_CONSTRUCTOR(public, Foo, , , int)(int value)
	: m_Test{ value }
{
}

DEFINE_DEFAULT_COPYCONSTRUCTOR(public, Foo)
DEFINE_DEFAULT_MOVECONSTRUCTOR(public, Foo)

DEFINE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 0, int const&)() const
{
	return m_Test;
}

DEFINE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 1, int, int const&)(int const& arg) const
{
	return m_Test + arg;
}

DEFINE_VIRTUAL_MEMBER_METHOD(public, Foo, , Test, , int)()
{
	return ++m_Test;
}

DEFINE_MEMBER_METHOD(public, Foo, , Test1, , void)()
{
	std::cout << m_Test << std::endl;
}

DEFINE_MEMBER_FIELD(private, Foo, int, m_Test);

DECLARE_REFLECTABLE_CLASS_WITH_BASE_CLASS(Bar, Foo)
{
	GENERATE_METADATA_WITH_BASE_CLASSES(Bar, WITH(), Foo);

	DECLARE_CONSTRUCTOR(public, Bar, explicit, , int);
	DECLARE_VIRTUAL_MEMBER_METHOD(public, Bar, , Test, , int);
};

GENERATE_METADATA_DEFINITION_WITH_BASE_CLASSES(Bar, WITH(), Foo);

DEFINE_CONSTRUCTOR(public, Bar, explicit, , int)(int arg)
	: Foo(arg)
{
}

DEFINE_VIRTUAL_MEMBER_METHOD(public, Bar, , Test, , int)()
{
	return Foo::Test() + 1;
}

int main()
{
	try
	{
		for (auto&& item : Reflection::GetInstance().GetTypes())
		{
			std::wcout << item->GetName() << std::endl;
		}

		std::wcout << typeof(int)->GetName() << std::endl;

		auto type = typeofname("Foo"_nv);
		std::wcout << type->GetAttribute<TestAttribute>()->Test() << std::endl;
		auto pFoo = type->Construct({ 1 });
		std::wcout << pFoo->ToString() << std::endl << type->InvokeMember(pFoo, "GetTest"_nv, {})->ToString() << std::endl << type->InvokeMember(pFoo, "GetTest"_nv, { 1 })->ToString() << std::endl;

		std::wcout << type->InvokeMember(pFoo, "Test"_nv, {})->ToString() << std::endl;
		std::wcout << type->ReadMemberField(pFoo, "m_Test"_nv)->ToString() << std::endl;

		decltype(auto) UnboxedFoo = pFoo->Unbox<Foo>();
		std::wcout << UnboxedFoo.GetTest() << std::endl;

		auto type2 = typeofname("Bar"_nv);
		auto pBar = type2->Construct({ 1 });
		std::wcout << type2->InvokeMember(pBar, "Test"_nv, {})->ToString() << std::endl << std::endl;

		for (auto&& item : type2->GetBaseClasses())
		{
			std::wcout << item->GetName() << std::endl;
		}

		std::wcout << std::endl;

		for (auto&& item : type2->GetMemberMethods()
			.where([](std::pair<const nString, natRefPointer<IMemberMethod>> const& method)
				{
					return method.second->GetAccessSpecifier() == AccessSpecifier::AccessSpecifier_public;
				}))
		{
			std::wcout << item.first << std::endl;
		}

		std::wcout << std::endl;

		std::vector<std::tuple<bool, nString, natRefPointer<IType>>> members{};
		type2->EnumMember(true, [&members](nStrView name, bool isMethod, natRefPointer<IType> objectType)
		{
			members.emplace_back(isMethod, name, objectType);
			return false;
		});
		for (auto&& item : members)
		{
			if (std::get<0>(item))
			{
				std::wcout << "Method : "_nv << std::get<1>(item) << std::endl;
			}
			else
			{
				std::wcout << "Field : "_nv << std::get<1>(item) << " ("_nv << std::get<2>(item)->GetName() << ")"_nv << std::endl;
			}
		}

		std::wcout << std::boolalpha << type2->IsExtendFrom(type) << std::endl << type2->IsBoxed() << std::endl << (*type2->GetMemberFields().where([](auto&& pair) { return pair.first == "m_Test"_nv; }).begin()).second->GetType()->IsBoxed() << std::endl;

		// Benchmark
		/*auto Test = type2->GetMemberMethod("Test"_nv, {});
		ArgumentPack args{};

		auto time = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < 1000000; ++i)
		{
			Test->Invoke(pBar, args);
		}
		std::wcout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time).count() << std::endl;
		auto bar = static_cast<natRefPointer<Bar>>(pBar);
		time = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < 1000000; ++i)
		{
			bar->Test();
		}
		std::wcout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time).count() << std::endl;*/
	}
	catch (ReflectionException& e)
	{
		std::wcerr << natUtil::FormatString("Exception caught from {0}, file \"{1}\" line {2},\nDescription: {3}"_nv, e.GetSource(), e.GetFile(), e.GetLine(), e.GetDesc()) << std::endl;
#ifdef EnableExceptionStackTrace
		std::wcerr << "Call stack:"_nv << std::endl;
		for (size_t i = 0; i < e.GetStackWalker().GetFrameCount(); ++i)
		{
			auto&& symbol = e.GetStackWalker().GetSymbol(i);
			std::wcerr << natUtil::FormatString("{3}: (0x%p) {4} at address 0x%p (file {5}:{6} at address 0x%p)"_nv, symbol.OriginalAddress, reinterpret_cast<const void*>(symbol.SymbolAddress), reinterpret_cast<const void*>(symbol.SourceFileAddress), i, symbol.SymbolName, symbol.SourceFileName, symbol.SourceFileLine) << std::endl;
		}
#endif
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

#ifdef _WIN32
	system("pause");
#else
	std::cin.get();
#endif
}
