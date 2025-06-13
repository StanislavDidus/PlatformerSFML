#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

class EventBus
{
public:
	//Singleton
	static EventBus& Get() {
		static EventBus instance;
		return instance;
	}

	void AddListener(const std::string& name, std::shared_ptr<std::function<void()>> func)
	{
		events[name].push_back(func);
	}

	void RemoveListener(const std::string& name, std::shared_ptr < std::function<void()>> func)
	{
		auto v = events[name];
		v.erase(std::remove_if(v.begin(), v.end(), [&](std::shared_ptr<std::function<void()>> f) {if (f == func) return f; }));
	}

	void Push(const std::string& name)
	{
		for (auto& f : events[name])
		{
			queue.push_back(f);
		}
	}

	void process()
	{
		for (auto& f : queue)
		{
			(*f)();
		}
		queue.clear();
	}
private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<std::function<void()>>>> events;
	std::vector< std::shared_ptr<std::function<void()>>> queue;

	EventBus() {}
};