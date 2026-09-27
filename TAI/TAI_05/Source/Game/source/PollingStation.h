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

		void Init(std::vector<Actor*> aActors, Actor* aPlayer, std::vector<Tga::Vector2f> aHidingSpots);
		Tga::Vector2f GetPlayerPosition() const;
		Tga::Vector2f GetGuardPosition() const;
		Tga::Vector2f GetBanditPosition() const;

		std::span<Actor*> GetActors() { return myActors; }
		std::vector<Tga::Vector2f> GetHidingSpots() const;

		void SetBanditEscaped() { myBanditEscaped = true; }
		bool HasBanditEscaped() const { return myBanditEscaped; }
		void AddBanditCapture() { ++myBanditCaptures; }
		int GetBanditCaptures() const { return myBanditCaptures; }

		~PollingStation() = default;

	private:
		PollingStation() = default;

		PollingStation(const PollingStation&) = delete;
		PollingStation& operator=(const PollingStation&) = delete;

		Actor* myPlayer;
		Actor* myGuard;
		Actor* myBandit;

		std::vector<Actor*> myActors;
		std::vector<Tga::Vector2f> myHidingSpots;

		bool myBanditEscaped = false;
		int myBanditCaptures = 0;
	};
}
