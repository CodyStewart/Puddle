#include <stdio.h>

#include "Views.h"
#include "poolBall.h"

/* DebugView is used to check physics. It has:
* Balls representing pool balls (or any balls)
* Walls representing the walls of a pool table
* Colliders for the balls and walls
*/ 

void loadStartViewResources(std::vector<Resource>& resources) {
	Resource circleText("blackOutlineCircle.png");
	resources.push_back(circleText);
}

void loadDebugViewResources(std::vector<Resource>& resources) {

}

bool loadView(std::vector<Resource>& resources, View* view, ResourceManager* resManager, PuddleRenderer* renderer) {
	bool success = false;

	// get the view resources
	for (auto res : resources) {
		ResHandleShrdPtr handle = resManager->getHandle(&res);
		if (handle.get()) {
			Texture* ballTexture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
			SDL_Point point = { 50, 50 };
			PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent({ 50,50 }, 1.0f);
			GameObject* oneBall = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, ballTexture));
			view->addGameObject(oneBall);
			return true;
		}
		else
			return false;
	}
	// load the view game objects


	return success;
}

View::View(ViewState viewState) {

}

View::View(ViewState viewState, ResourceManager* resManager, PuddleRenderer* renderer) {
	std::vector<Resource> resources;
	switch (viewState) {
	case startView:
		// get the startView resources
		loadStartViewResources(resources);
		// load the resources and instantiate the game objects
		loadView(resources, this, resManager, renderer);
		break;
	case debugView:
		loadDebugViewResources(resources);
		loadView(resources, this, resManager, renderer);
		break;
	default:
		printf("In loadView: viewState is not valid!\n");
		break;
	}
}

View::~View() {
	// free the memory for each gameObject
	for (auto object : _gameObjects) {
		if (object)
			delete object;
	}
}

void View::addGameObject(GameObject* obj) {
	_gameObjects.push_back(obj);
}

void View::update() {
	for (auto object : _gameObjects) {
		object->update();
	}
}