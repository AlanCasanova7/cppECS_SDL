#include "Box2D/Box2D.h"
#include <string>
#include <iostream>

#include "World.h"

#include "Renderer.h"

static bool run;

struct NameComponent : public Component<NameComponent>
{
	std::string name;
};
struct TransformComponent : public Component<TransformComponent>
{
	float x, y;
	void Init(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};
struct DirectionComponent : public Component<DirectionComponent>
{
	float dirX, dirY;
};
struct BoxComponent : public Component<BoxComponent>
{
	SDL_Rect rect;
	void Init(unsigned int width, unsigned int height, unsigned int x, unsigned int y)
	{
		rect.w = width;
		rect.h = height;
		rect.x = x;
		rect.y = y;
	}
};
struct InputComponent : public Component<InputComponent>
{
	bool keys[322];
};

class Player
{
public:
	Player(ECS::World& world, float posX = 0, float posY = 0, float size = 50) : ownerWorld(world)
	{
		EntityId = world.CreateEntity();
		auto& transformComponent = world.AddComponent<TransformComponent>(EntityId);
		transformComponent.Init(posX, posY);
		auto& boxComponent = world.AddComponent<BoxComponent>(EntityId);
		boxComponent.Init(size, size, transformComponent.x, transformComponent.y);
		auto& inputComponent = world.AddComponent<InputComponent>(EntityId);
	}
	unsigned int EntityId;
	~Player()
	{
		ownerWorld.DestroyEntity(EntityId);
	}
	Player(const Player&) = delete;
private:
	ECS::World& ownerWorld;
};

void PrintSystem(const unsigned int entity, ECS::World& world)
{
	auto& nameComponent = world.GetComponent<NameComponent>(entity);
	std::cout << nameComponent.name.c_str() << "\n";
}
void DrawBoxSystem(const unsigned int entity, ECS::World& world)
{
	auto& transformComponent = world.GetComponent<TransformComponent>(entity);
	auto& boxComponent = world.GetComponent<BoxComponent>(entity);

	boxComponent.rect.x = (int)transformComponent.x;
	boxComponent.rect.y = (int)transformComponent.y;

	if (world.AttachedRenderer != nullptr)
	{
		world.AttachedRenderer->DrawRect(255, 0, 0, 255, &boxComponent.rect);
	}
}
void UpdateInputSystem(const unsigned int entity, ECS::World& world)
{
	auto& inputComponent = world.GetComponent<InputComponent>(entity);
	SDL_Event _event;
	while (SDL_PollEvent(&_event))
	{
		switch (_event.type)
		{
		case SDL_QUIT:
			run = false;
			break;
		case SDL_KEYDOWN:
			if (_event.key.keysym.sym >= 322) { break; }
			inputComponent.keys[_event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			if (_event.key.keysym.sym >= 322) { break; }
			inputComponent.keys[_event.key.keysym.sym] = false;
			break;
		default:
			break;
		}
	}
}
void InputBasedMovementSystem(const unsigned int entity, ECS::World& world)
{
	auto& transformComponent = world.GetComponent<TransformComponent>(entity);
	auto& inputComponent = world.GetComponent<InputComponent>(entity);

	if (inputComponent.keys[SDLK_d])
	{
		transformComponent.x += 0.1f;
	}
	if (inputComponent.keys[SDLK_a])
	{
		transformComponent.x -= 0.1f;
	}
	if (inputComponent.keys[SDLK_w])
	{
		transformComponent.y -= 0.1f;
	}
	if (inputComponent.keys[SDLK_s])
	{
		transformComponent.y += 0.1f;
	}
}
void DirectionBasedMovementSystem(const unsigned int entity, ECS::World& world)
{
	auto& transformComponent = world.GetComponent<TransformComponent>(entity);
	auto& directionComponent = world.GetComponent<DirectionComponent>(entity);

	transformComponent.x += directionComponent.dirX;
	transformComponent.y += directionComponent.dirY;

	if (transformComponent.x < 200.0f || transformComponent.x > 600.0f)
	{
		directionComponent.dirX -= directionComponent.dirX * 2;
		directionComponent.dirY = ((float)(std::rand() % 10)) / 100.0f;
	}
	if (transformComponent.y < 150.0f || transformComponent.y > 450.0f)
	{
		directionComponent.dirX = ((float)(std::rand() % 10)) / 100.0f;
		directionComponent.dirY -= directionComponent.dirY * 2;
	}
}
//void MoveTransformSystem(const unsigned int entity, ECS::World& world)
//{
//	auto transform = world.GetComponent<TransformComponent>(entity);
//	SDL_Event _event = input.GetEventOfType(SDL_QUIT);
//}

int main(int argc, const char* argv)
{
	ECS::World world;
	//auto player = world.CreateEntity();
	//auto enemy = world.CreateEntity();
	//auto& p_name = world.AddComponent<NameComponent>(player);
	//p_name.name = std::string("Paolo"); 
	//auto& p_box = world.AddComponent<BoxComponent>(player);
	//p_box.Init(200, 50, 400, 300);
	//auto& e_name = world.AddComponent<NameComponent>(enemy);
	//e_name.name = std::string("Juan");
	//world.ForEach<NameComponent>(PrintSystem);
	Player player(world);

	unsigned int ball = world.CreateEntity();
	auto& dir_comp = world.AddComponent<DirectionComponent>(ball);
	dir_comp.dirX = 0.1f;
	dir_comp.dirY = 0.0f;
	auto& tra_comp = world.AddComponent<TransformComponent>(ball);
	tra_comp.x = 400.0f;
	tra_comp.y = 300.0f;
	auto& box_comp = world.AddComponent<BoxComponent>(ball);
	box_comp.Init(50, 50, 0, 0);

	Renderer renderer("Test Window", 800, 600);
	world.AttachRenderer(&renderer);

	run = true;
	while (run)
	{
		renderer.Clear();

		//renderer.GetWindowEvent(input);
		world.ForEach<InputComponent>(UpdateInputSystem);

		world.ForEach<InputComponent, TransformComponent>(InputBasedMovementSystem);

		world.ForEach<DirectionComponent, TransformComponent>(DirectionBasedMovementSystem);

		world.ForEach<TransformComponent, BoxComponent>(DrawBoxSystem);

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