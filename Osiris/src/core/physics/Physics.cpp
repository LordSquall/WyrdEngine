/* local includes */
#include "osrpch.h"
#include "core/Log.h"
#include "Physics.h"
#include "core/scene/Scene.h"

/* external includes */
#include <MathHelper.h>

namespace Osiris
{
	Physics::Physics() : _IsRunning(false)
	{
	}
	
	Physics::~Physics()
	{

	}

	void Physics::Start(std::shared_ptr<Scene> scene)
	{
		_IsRunning = true;

		OSR_CORE_TRACE("Physics::Start(std::shared_ptr<Scene> scene) called!");
	}

	void Physics::Update(Timestep ts)
	{
		if (_IsRunning == true)
		{
			OSR_CORE_TRACE("Physics::Update(Timestep ts) called!");
		}
	}

	void Physics::Reset()
	{
		OSR_CORE_TRACE("Physics::Reset() called!");
	}

	void Physics::Stop()
	{
		OSR_CORE_TRACE("Physics::Stop() called!");

		_IsRunning = false;
	}
}
