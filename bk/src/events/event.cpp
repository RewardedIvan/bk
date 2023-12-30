#include "pch/pch.h"

#include "event.h"
#include "utils/log.h"

namespace bk::events {
	void EventManager::subscribe(EventID event, subscriber sb) {
		getOrCreate(event)->push_back(sb);
	}

	void EventManager::broadcast(EventID event, void* data) {
		for (subscriber sub : *getOrCreate(event))
			sub(data);
	}

	std::vector<subscriber>* EventManager::getOrCreate(EventID event) {
		if (!subs.contains(event))
			subs.insert_or_assign(event, new std::vector<subscriber>());
		return subs.at(event);
	}
}
