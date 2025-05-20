#include "Algorythms/TClock.h"

TClock::TClock()
{
}

TClock::~TClock()
{
}

void TClock::addClock(float target_time, const std::function<void()>& func, const std::string& name)
{
	clocks[name] = TClockEvent(0.f, target_time, func);
}

TClockEvent* TClock::getClock(const std::string& name)
{
	if (clocks.find(name) != clocks.end())
	{
		return &clocks[name];
	}
	else
		return nullptr;
}

void TClock::update(float deltaTime)
{
	std::vector<std::string> removeList;
	
	for (auto& it : clocks)
	{
		auto& clock = it.second;
	
		clock.current_time += deltaTime;
		clock.is_playing = true;	

		if (clock.start_time + clock.current_time >= clock.target_time)
		{
			clock.func();
			removeList.push_back(it.first);
		}
	}

	for (auto& clock : removeList)
	{
		clocks.erase(clock);
	}
}
