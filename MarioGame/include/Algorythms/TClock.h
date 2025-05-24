#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <functional>
#include <unordered_map>

struct TClockEvent
{
	float start_time;
	float current_time;
	float target_time;
	bool is_playing;
	std::function<void()> func;

	TClockEvent() = default;
	TClockEvent(float start_time, float target_time, const std::function<void()>& func) : start_time(start_time), current_time(0.f), target_time(target_time),
		func(func), is_playing(true) {}
};

 class TClock
{
private:
	std::unordered_map<std::string, TClockEvent> clocks;
	float target_time;
	float current_time;
public:
	TClock();
	virtual ~TClock();

	void addClock(float target_time, const std::function<void()>& func, const std::string& name);
	TClockEvent* getClock(const std::string& name);

	void clear();

	void update(float deltaTime);
};

