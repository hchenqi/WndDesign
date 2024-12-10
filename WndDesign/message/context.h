#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


void RegisterContextProvider(WndObject& provider);
void UnregisterContextProvider(WndObject& provider);
ref_ptr<WndObject> GetNextContextProvider(WndObject& user);


template<class Wnd> requires std::is_base_of_v<WndObject, Wnd>
class ContextProvider {
private:
	Wnd& provider;
public:
	ContextProvider(Wnd& provider) : provider(provider) { RegisterContextProvider(provider); }
	~ContextProvider() { UnregisterContextProvider(provider); }
};


template<class Wnd> requires std::is_base_of_v<WndObject, Wnd>
class Context {
private:
	WndObject& user;
	ref_ptr<Wnd> provider;
public:
	Context(WndObject& user) : user(user), provider(nullptr) {}
public:
	Wnd& Get() {
		if (!provider) {
			ref_ptr<WndObject> next = &user;
			do {
				next = GetNextContextProvider(*next);
				if (next == nullptr) {
					throw std::invalid_argument("context provider not found");
				}
				provider = dynamic_cast<ref_ptr<Wnd>>(next);
			} while (!provider);
		}
		return *provider;
	}
};


END_NAMESPACE(WndDesign)