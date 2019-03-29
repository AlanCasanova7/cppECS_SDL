#include "Box2D/Box2D.h"
#include <string>
#include <iostream>

#include "World.h"

#include "Renderer.h"

struct NameComponent : Component<NameComponent>
{
	std::string name;
};
struct BoxComponent : Component<BoxComponent>
{
	SDL_Rect rect;
	void inline Init(unsigned int width, unsigned int height, unsigned int x, unsigned int y)
	{
		rect.w = width;
		rect.h = height;
		rect.x = x;
		rect.y = y;
	}
};

void PrintSystem(const unsigned int entity, ECS::World& world)
{
	auto& nameComponent = world.GetComponent<NameComponent>(entity);
	std::cout << nameComponent.name.c_str() << "\n";
}

void DrawBoxSystem(const unsigned int entity, ECS::World& world)
{
	auto& boxComponent = world.GetComponent<BoxComponent>(entity);
	if (world.AttachedRenderer != nullptr)
	{
		world.AttachedRenderer->DrawRect(255, 0, 0, 255, &boxComponent.rect);
	}
}

int main(int argc, const char* argv)
{
	ECS::World world;
	auto player = world.CreateEntity();
	auto enemy = world.CreateEntity();
	auto& p_name = world.AddComponent<NameComponent>(player);
	p_name.name = std::string("Paolo"); 
	auto& p_box = world.AddComponent<BoxComponent>(player);
	p_box.Init(200, 50, 400, 300);
	auto& e_name = world.AddComponent<NameComponent>(enemy);
	e_name.name = std::string("Juan");
	world.ForEach<NameComponent>(PrintSystem);

	Renderer renderer("gino", 800, 600);

	world.AttachRenderer(&renderer);
	bool run = true;
	while (run)
	{
		renderer.Clear();

		world.ForEach<BoxComponent>(DrawBoxSystem);

		renderer.Present();
	}

	return 0;
}

//#define W_WIDTH 800
//#define W_HEIGHT 600
//#define ORTHOSIZE 10
//
//#define MAX_X (W_WIDTH / W_HEIGHT) * ORTHOSIZE
//#define MAX_Y ORTHOSIZE 
//
//int main(int argc, char** argv)
//{
//	SDL_Init(SDL_INIT_VIDEO);
//
//	SDL_Window* window = SDL_CreateWindow("Physics Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, 0);
//	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, 0);
//
//	b2Vec2 gravity(0, -10);
//	b2World world(gravity);
//
//	b2BodyDef playerBodyDef;
//	playerBodyDef.type = b2_dynamicBody;
//	playerBodyDef.position.Set(0, 0);
//
//	b2PolygonShape boxCollision;
//	boxCollision.SetAsBox(5, 5);
//
//	b2FixtureDef fixtureDef;
//	fixtureDef.shape = &boxCollision;
//	fixtureDef.density = 1.0f;
//
//	b2Body* body = world.CreateBody(&playerBodyDef);
//
//	body->CreateFixture(&fixtureDef);
//
//	SDL_Rect rect1;
//	rect1.x = 0;
//	rect1.y = 0;
//	rect1.h = 10;
//	rect1.w = 10;
//
//	b2BodyDef colliderBodyDef;
//	colliderBodyDef.type = b2_staticBody;
//	colliderBodyDef.position.Set(0, (- W_HEIGHT / 2));
//
//	b2PolygonShape colliderBoxCollision;
//	colliderBoxCollision.SetAsBox(10, 10);
//
//	b2FixtureDef colliderFixtureDef;
//	colliderFixtureDef.shape = &colliderBoxCollision;
//	colliderFixtureDef.density = 1.0f;
//
//	b2Body* colliderBody = world.CreateBody(&colliderBodyDef);
//
//	colliderBody->CreateFixture(&colliderFixtureDef);
//
//	SDL_Rect colliderRect;
//	colliderRect.x = 0;
//	colliderRect.y = W_HEIGHT / 2;
//	colliderRect.h = 20;
//	colliderRect.w = 20;
//
//	SDL_Event sdl_event;
//	for (;;)
//	{
//
//		while (SDL_PollEvent(&sdl_event))
//		{
//			world.Step(1.0 / 60.0, 8, 3);
//			
//			if (sdl_event.type == SDL_QUIT)
//			{
//				return -1;
//			}
//
//			rect1.x = body->GetPosition().x;
//			rect1.y = (body->GetPosition().y - (body->GetPosition().y * 2) + 5);
//
//			printf("rect1_x = %d, rect1_y = %d\tbody_x = %f, body_y = %f\n", rect1.x, rect1.y, body->GetPosition().x, body->GetPosition().y);
//			printf("rect1_x = %d, rect1_y = %d\tbody_x = %f, body_y = %f\n", colliderRect.x, colliderRect.y, colliderBody->GetPosition().x, colliderBody->GetPosition().y);
//		}
//
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//		SDL_RenderClear(renderer);
//
//		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//		SDL_RenderDrawRect(renderer, &rect1);
//
//		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
//		SDL_RenderDrawRect(renderer, &colliderRect);
//
//		SDL_RenderPresent(renderer);
//	}
//
//	SDL_Quit();
//
//	return 0;
//}