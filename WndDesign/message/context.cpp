#include "context.h"

#include <unordered_set>


BEGIN_NAMESPACE(WndDesign)

struct WndObjectPrivateAccess : public WndObject {
	ref_ptr<WndObjectPrivateAccess> Parent() { return static_cast<ref_ptr<WndObjectPrivateAccess>>(parent); }
};

BEGIN_NAMESPACE(Anonymous)

std::unordered_set<ref_ptr<WndObject>> provider_set;

END_NAMESPACE(Anonymous)


ContextProvider::ContextProvider(WndObject& provider) : provider(provider) {
	if (provider_set.contains(&provider)) {
		throw std::invalid_argument("ContextProvider already registered");
	}
	provider_set.emplace(&provider);
}

ContextProvider::~ContextProvider() {
	provider_set.erase(&provider);
}

ref_ptr<WndObject> Context::GetNextProvider(WndObject& user) {
	auto parent = static_cast<ref_ptr<WndObjectPrivateAccess>>(&user)->Parent();
	while (parent && !provider_set.contains(parent)) {
		parent = parent->Parent();
	}
	return parent;
}


END_NAMESPACE(WndDesign)