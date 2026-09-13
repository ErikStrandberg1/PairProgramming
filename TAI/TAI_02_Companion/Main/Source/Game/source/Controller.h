#pragma once

struct UpdateContext;
struct AIEvent;
namespace AI
{
	class PollingStation;

	enum class eControllerType
	{
		eDummy,
		ePlayer,
		eEvent,
		ePolling
	};

	class Controller
	{
	public:
		virtual Tga::Vector2f Update(const UpdateContext& updateContext, const Tga::Vector2f& aPosition) = 0;

		virtual void OnEvent( [[maybe_unused]]const AIEvent& aEvent){}

	protected:
		Tga::Vector2f myTargetPosition;
	};

	class ControllerFactory
	{
	public:
		ControllerFactory();
		void Init(PollingStation* aPollingStation);

		Controller* CreateController(const eControllerType aControllerType);

	private:
		PollingStation* myPollingStation = nullptr;
	};
}