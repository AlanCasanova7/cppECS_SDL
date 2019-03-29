#include "World.h"

unsigned int ECS::World::EntityCount = 0;
unsigned int ECS::World::ComponentTypes = 0;

unsigned int ECS::World::CreateEntity()
{
	unsigned int entityId = -1;
	if (EntitiesToOverwrite.empty())
	{
		entityId = ECS::World::EntityCount++;
		Entities.push_back(ECS::CompomentsBitmask());
	}
	else
	{
		entityId = EntitiesToOverwrite.front();
		EntitiesToOverwrite.pop_front();
	}
	return entityId;
}

void ECS::World::DestroyEntity(const unsigned int toDelete)
{
	if (toDelete >= Entities.size())
	{
		//TRYING TO DELETE AN ENTITY THAT'S NOT INSIDE THE WORLD.
		return;
	}
	else if (std::find(EntitiesToOverwrite.begin(), EntitiesToOverwrite.end(), toDelete) == EntitiesToOverwrite.end())
	{
		//ENTITY HAS ALREADY BEEN MARKED FOR DELETION.
		return;
	}
	else
	{
		EntitiesToOverwrite.push_back(toDelete);
		Entities[toDelete].BitMask.reset();
	}
}

void ECS::World::AttachRenderer(Renderer* renderer)
{
	AttachedRenderer = renderer;
}