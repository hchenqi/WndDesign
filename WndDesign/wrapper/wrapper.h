#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd, template<class Wnd> class Wrapper, template<class Wnd> class... Wrappers>
class Decorate : public Decorate<Wrapper<Wnd>, Wrappers...> {
public:
	using Base = Decorate;
	using Decorate<Wrapper<Wnd>, Wrappers...>::Decorate;
};

template<class Wnd, template<class Wnd> class Wrapper>
class Decorate<Wnd, Wrapper> : public Wrapper<Wnd> {
public:
	using Base = Decorate;
	using Wrapper<Wnd>::Wrapper;
};


END_NAMESPACE(WndDesign)