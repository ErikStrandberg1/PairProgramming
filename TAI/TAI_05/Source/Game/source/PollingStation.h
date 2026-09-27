#pragma once

#include <span>

#include "Actor.h"
#include <vector>

namespace AI
{
	class PollingStation
	{
	public:
		static PollingStation& GetInstance();

		void Init(std::vector<Actor*> aActors, Actor* aPlayer);
		Tga::Vector2f GetPlayerPosition() const;
		Tga::Vector2f GetGuardPosition() const;
		Tga::Vector2f GetBanditPosition() const;

		std::span<Actor*> GetActors() { return myActors; }

		~PollingStation() = default;

	private:
		PollingStation() = default;

		PollingStation(const PollingStation&) = delete;
		PollingStation& operator=(const PollingStation&) = delete;

		Actor* myPlayer;
		Actor* myGuard;
		Actor* myBandit;

		std::vector<Actor*> myActors;
	};
}
