#include "context.h"

#include <unordered_set>


BEGIN_NAMESPACE(WndDesign)

struct WndObjectPrivateAccess : public WndObject {
	ref_ptr<WndObjectPrivateAccess> Parent() { return static_cast<ref_ptr<WndObjectPrivateAccess>>(parent); }
};

BEGIN_NAMESPACE(Anonymous)

std::unordered_set<ref_ptr<WndObject>> provider_set;

END_NAMESPACE(Anonymous)


void RegisterContextProvider(WndObject& provider) {
	provider_set.emplace(&provider);
}

void UnregisterContextProvider(WndObject& provider) {
	provider_set.erase(&provider);
}

ref_ptr<WndObject> GetNextContextProvider(WndObject& user) {
	auto parent = static_cast<ref_ptr<WndObjectPrivateAccess>>(&user)->Parent();
	while (parent && !provider_set.contains(parent)) {
		parent = parent->Parent();
	}
	return parent;
}


END_NAMESPACE(WndDesign)