#pragma once

namespace bk::events {
	typedef uint64_t EventID;
	typedef std::function<void(void*)> subscriber;

	struct EventManager {
		static void subscribe(EventID event, subscriber sub);
		static void broadcast(EventID event, void* data);
	private:
		static std::vector<subscriber>* getOrCreate(EventID event);
		inline static auto subs = std::map<EventID, std::vector<subscriber>*>();
	};
}
