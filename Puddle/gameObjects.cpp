#include "gameObjects.h"

void GameObject::render(SDL_Renderer* renderer) {

}

void GameObject::move(SDL_Point point) {

}

PoolBall::PoolBall() {
	_ball = Entity();
	_ballTexture = nullptr;
	//_pos = { 0,0 };
	_shape = Circle();
	_collision = CollisionCircle();
}

PoolBall::PoolBall(Entity ball, Texture* ballTexture) : _ball(ball) {
	_ballTexture = ballTexture;
	//_pos = { 0, 0 };
	_shape = Circle();
	_collision = CollisionCircle();
}

PoolBall::PoolBall(Entity ball, Texture* ballTexture, SDL_Point pos, float radius) {
	_ball = ball;
	_ballTexture = ballTexture;
	//_pos = pos;
	_shape = Circle(pos, radius);
	_collision = CollisionCircle(pos, radius);
}

PoolBall::~PoolBall() {
	if (_ballTexture)
		delete _ballTexture;
}

void PoolBall::move(SDL_Point point) {
	_shape._point = point;
	_collision._circle._point = point;
}

void PoolBall::render(SDL_Renderer* renderer) {
	//_ballTexture->render(renderer, this->_shape._point.x, this->_shape._point.y);
	int size = UNIT_SIZE;
	_ballTexture->renderScaled(renderer, this->_shape._point.x, this->_shape._point.y, size, size);
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
		PoolBall* oneBall = new PoolBall(poolBallOne, ballTexture, point, 1.0f);
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