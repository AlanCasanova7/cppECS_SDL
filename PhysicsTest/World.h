#pragma once

#include <bitset>
#include <functional>
#include <queue>

#include "Component.h"

class Renderer;

namespace ECS
{
#define MAX_COMPONENTS 32

	struct CompomentsBitmask
	{
		std::bitset<MAX_COMPONENTS> BitMask;
	};

	class World
	{
	public:
		static unsigned int ComponentTypes;
		static unsigned int EntityCount;

		unsigned int CreateEntity();
		void DestroyEntity(const unsigned int toDelete);

		template<typename T>
		T& AddComponent(const unsigned int entity)
		{
			std::vector<T>& componentsList = T::GetComponentList();

			if (entity >= componentsList.size())
			{
				componentsList.resize(entity + 1);
			}

			Entities[entity].BitMask.set(T::GetTypeId(), true);

			return componentsList[entity] = {};
		}
		template<typename T>
		T& GetComponent(const unsigned int entity)
		{
			std::vector<T>& vecToReturn = T::GetComponentList();
			return vecToReturn[entity];
		}

		template<typename... Components>									// TEMPLATE WHERE MORE THAN ONE "COMPONENTES
																			// CAN BE PASSED SO THAT IT WILL CREATE A
																			// DIFFERENT VERSION OF THIS FOREACH FOR EVERY
																			// SET OF COMPONENTS.
		void ForEach(std::function<void(const unsigned int InEntity, World& world)> Callback)
		{
			std::bitset<MAX_COMPONENTS> Mask = 0;							// INITIALIZE A MASK TO ALL ZEROS.
			[&Mask](...) {}(Mask.set(Components::GetTypeId(), true)...);	// LAMBDA! KEEPS A REFERENCE TO MASK, THEN, 
																			// SETS TO 1 THE VALUE OF THE MASK FOR EVERY
																			// TYPE OF COMPONENTS THAT WE PASSED
			
			for (unsigned int i = 0; i < EntityCount; i++)					// CYCLES ALL MY ENTITIES BITMASKS, UNTIL
			{																// I FIND THE ENTITY WITH THAT SET OF 
				if ((Entities[i].BitMask & Mask) == Mask)					// COMPONENTS, THEN I CALL MY SYSTEM (CALLBACK)
				{															// ON THAT ENTITY.
					const unsigned int FoundEntity = i;
					Callback(FoundEntity, *this);
				}
			}
		}

		void AttachRenderer(Renderer* renderer);
		Renderer* AttachedRenderer;
	private:
		std::vector<CompomentsBitmask> Entities;
		std::deque<unsigned int> EntitiesToOverwrite;
	};
}