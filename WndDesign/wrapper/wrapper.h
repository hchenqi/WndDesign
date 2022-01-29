#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd, template<class Wnd> class Wrapper, template<class Wnd> class... Wrappers>
struct _Decorate_Helper {
	using type = typename _Decorate_Helper<Wrapper<Wnd>, Wrappers...>::type;
};

template<class Wnd, template<class Wnd> class Wrapper>
struct _Decorate_Helper<Wnd, Wrapper> {
	using type = Wrapper<Wnd>;
};


template<class Wnd, template<class Wnd> class... Wrappers>
using Decorate = typename _Decorate_Helper<Wnd, Wrappers...>::type;


END_NAMESPACE(WndDesign)