#pragma once

#include <span>

#include "Actor.h"
#include <vector>

namespace AI
{
	constexpr float HIDE_RADIUS = 0.05f;

	class PollingStation
	{
	public:
		static PollingStation& GetInstance();

		void Init(std::vector<Actor*> aActors, Actor* aPlayer, std::vector<Tga::Vector2f> aHidingSpots,
		          std::vector<Tga::Vector2f> aWaterSpots);
		Tga::Vector2f GetPlayerPosition() const;
		Tga::Vector2f GetGuardPosition() const;
		Tga::Vector2f GetBanditPosition() const;
		Tga::Vector2f GetDogPosition() const;

		std::span<Actor*> GetActors() { return myActors; }
		std::vector<Tga::Vector2f> GetHidingSpots() const;
		std::vector<Tga::Vector2f> GetWaterSpots() const;
		bool IsBanditHiding(Tga::Vector2f* outHidingSpot = nullptr) const;

		void SetPreys(std::vector<Actor*> aPreys);
		void SetPredators(std::vector<Actor*> aPredators);
		const std::vector<Actor*>& GetPreys() const { return myPreys; }
		const std::vector<Actor*>& GetPredators() const { return myPredators; }
		int GetAlivePredatorCount() const;
		Actor* GetClosestPrey(const Tga::Vector2f& aPosition, float aRange) const;
		Actor* GetClosestPredator(const Tga::Vector2f& aPosition, float aRange) const;

		~PollingStation() = default;

	private:
		PollingStation() = default;

		PollingStation(const PollingStation&) = delete;
		PollingStation& operator=(const PollingStation&) = delete;

		Actor* GetClosestActor(const std::vector<Actor*>& aActors, const Tga::Vector2f& aPosition,
		                       float aRange) const;

		std::vector<Actor*> myPreys;
		std::vector<Actor*> myPredators;

		Actor* myPlayer;
		Actor* myGuard;
		Actor* myBandit;
		Actor* myDog;

		std::vector<Actor*> myActors;
		std::vector<Tga::Vector2f> myHidingSpots;
		std::vector<Tga::Vector2f> myWaterSpots;
	};
}
