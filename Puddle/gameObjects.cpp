#include "gameObjects.h"
#include "poolBall.h"

void GameObject::setVelocity(Vec2 velocity) {
	_velocity = velocity;
}

void GameObject::setPosition(SDL_Point position) {
	_position = position;
}

GameObjectManager::GameObjectManager() {
	_gameObjects.clear();
}

extern EntityGenerator* entGen;

bool GameObjectManager::generateGameObjects(ResourceManager* resManager, PuddleRenderer* renderer) {
	bool success = true;

	Resource circleText("blackOutlineCircle.png");
	ResHandleShrdPtr circleHandle = resManager->getHandle(&circleText);
	Texture* ballTexture = new Texture(renderer->getRenderer(), circleHandle.get()->buffer(), circleHandle.get()->size());
	if (circleHandle.get()) {
		Entity poolBallOne = entGen->generatateNewEntity();
		SDL_Point point = { 50, 50 };
		PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent({ 50,50 }, 1.0f);
		GameObject* oneBall = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, ballTexture));
		addGameObject(oneBall);
	}
	else
		success = false;

	return success;
}

void GameObjectManager::addGameObject(GameObject* obj) {
	_gameObjects.push_back(obj);
}

std::vector<GameObject*>* GameObjectManager::getGameObjects() {
	return &_gameObjects;
}

void GameObjectManager::update() {
	for (auto object : _gameObjects) {
		object->update();
	}
}