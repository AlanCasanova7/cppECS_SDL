#pragma once

#include <vector>

template<typename T>
struct Component
{
	static unsigned int GetTypeId()
	{
		static unsigned int TypeId = ECS::World::ComponentTypes++;
		return TypeId;
	}

	static std::vector<T>& GetComponentList()
	{
		static std::vector<T> ComponentList;
		return ComponentList;
	}

protected:
	Component() = default;
};