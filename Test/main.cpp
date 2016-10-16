#include <iostream>
#include <chrono>
#include <Reflection.h>

DECLARE_REFLECTABLE_CLASS(Foo)
{
	GENERATE_METADATA(Foo)

	DECLARE_CONSTRUCTOR(public, Foo, explicit, , int);
	DECLARE_DEFAULT_COPYCONSTRUCTOR(public, Foo);
	DECLARE_DEFAULT_MOVECONSTRUCTOR(public, Foo);
	DECLARE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 0, int);
	DECLARE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 1, int, int);
	DECLARE_VIRTUAL_MEMBER_METHOD(public, Foo, , Test, , int);

	DECLARE_MEMBER_FIELD(private, Foo, int, m_Test);
};

GENERATE_METADATA_DEFINITION(Foo);

DEFINE_CONSTRUCTOR(public, Foo, , , int)(int value)
	: m_Test{ value }
{
}

DEFINE_DEFAULT_COPYCONSTRUCTOR(public, Foo)
DEFINE_DEFAULT_MOVECONSTRUCTOR(public, Foo)

DEFINE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 0, int)() const
{
	return m_Test;
}

DEFINE_CONST_MEMBER_METHOD(public, Foo, , GetTest, 1, int, int)(int arg) const
{
	return m_Test + arg;
}

DEFINE_VIRTUAL_MEMBER_METHOD(public, Foo, , Test, , int)()
{
	return ++m_Test;
}

DEFINE_MEMBER_FIELD(private, Foo, int, m_Test);

DECLARE_REFLECTABLE_CLASS(Bar), public Foo
{
	GENERATE_METADATA_WITH_BASE_CLASSES(Bar, Foo);

	DECLARE_CONSTRUCTOR(public, Bar, explicit, , int);
	DECLARE_VIRTUAL_MEMBER_METHOD(public, Bar, , Test, , int);
};

GENERATE_METADATA_DEFINITION_WITH_BASE_CLASSES(Bar, Foo);

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
		std::wcout << typeof(int)->GetName() << std::endl;

		auto type = typeofname(_T("Foo"));
		auto pFoo = type->Construct({ 1 });
		std::wcout << pFoo->ToString() << std::endl << type->InvokeMember(pFoo, _T("GetTest"), {})->ToString() << std::endl << type->InvokeMember(pFoo, _T("GetTest"), { 1 })->ToString() << std::endl;
		std::vector<std::pair<bool, nTString>> members{};
		type->EnumMember(true, [&members](ncTStr name, bool isMethod, natRefPointer<IType> objectType)
		{
			members.emplace_back(isMethod, name);
			return false;
		});
		for (auto&& item : members)
		{
			std::wcout << (item.first ? _T("Method : ") : _T("Field : ")) << item.second << std::endl;
		}
		std::wcout << type->InvokeMember(pFoo, _T("Test"), {})->ToString() << std::endl;
		std::wcout << type->ReadMemberField(pFoo, _T("m_Test"))->ToString() << std::endl;

		decltype(auto) UnboxedFoo = pFoo->Unbox<Foo>();
		std::wcout << UnboxedFoo.GetTest() << std::endl;

		auto type2 = typeofname(_T("Bar"));
		auto pBar = type2->Construct({ 1 });
		std::wcout << type2->InvokeMember(pBar, _T("Test"), {})->ToString() << std::endl << std::endl;

		// Benchmark
		/*auto Test = type2->GetMemberMethod(_T("Test"), {});
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
		std::wcerr << natUtil::FormatString(_T("Exception caught from {0}, file \"{1}\" line {2},\nDescription: {3}"), e.GetSource(), e.GetFile(), e.GetLine(), e.GetDesc()) << std::endl;
#ifdef EnableExceptionStackTrace
		std::wcerr << _T("Call stack:") << std::endl;
		for (size_t i = 0; i < e.GetStackWalker().GetFrameCount(); ++i)
		{
			auto&& symbol = e.GetStackWalker().GetSymbol(i);
			std::wcerr << natUtil::FormatString(_T("{3}: (0x%p) {4} at address 0x%p (file {5}:{6} at address 0x%p)"), symbol.OriginalAddress, reinterpret_cast<const void*>(symbol.SymbolAddress), reinterpret_cast<const void*>(symbol.SourceFileAddress), i, symbol.SymbolName, symbol.SourceFileName, symbol.SourceFileLine) << std::endl;
		}
#endif
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	system("pause");
}
