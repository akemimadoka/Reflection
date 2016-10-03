#include "Method.h"

ArgumentPack::~ArgumentPack()
{
}

natRefPointer<Object> ArgumentPack::Extract()
{
	auto ret = std::move(m_Args.front());
	m_Args.pop_front();
	return ret;
}

natRefPointer<Object> ArgumentPack::Get(size_t n) const
{
	return m_Args.at(n);
}

natRefPointer<IType> ArgumentPack::GetType(size_t n) const
{
	return m_Args.at(n)->GetType();
}

size_t ArgumentPack::Size() const
{
	return m_Args.size();
}
