#pragma once
#include "Object.h"
#include <deque>

class ArgumentPack
{
public:
	template <typename... Args>
	ArgumentPack(Args&&... args)
		: m_Args{ Object::Box(std::forward<Args>(args))... }
	{
	}
	~ArgumentPack();

	natRefPointer<Object> Extract();
	natRefPointer<Object> Get(size_t n) const;
	natRefPointer<IType> GetType(size_t n) const;
	size_t Size() const;

private:
	std::deque<natRefPointer<Object>> m_Args;
};
