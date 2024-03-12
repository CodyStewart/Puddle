#include <stdio.h>

#include <tinyxml2.h>

#include "Views.h"
#include "poolBall.h"
#include "poolTable.h"

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
	tinyxml2::XMLDocument doc;
	doc.LoadFile("assets/gameViewData.xml");
	//doc.PrintError();
	//printf("%d", doc.ErrorID());
	const tinyxml2::XMLElement* viewElement = doc.FirstChildElement("View")->FirstChildElement("gameObjects")->FirstChildElement("oneBall");
	const tinyxml2::XMLNode* node = viewElement->FirstChild();
	const tinyxml2::XMLElement* element = node->ToElement();
	const tinyxml2::XMLNode* node2 = element->FirstChild();
	const tinyxml2::XMLText* text = node2->ToText();
	const char* tex = text->Value();

	viewElement = viewElement->NextSiblingElement();
	

	Resource circleText("blackOutlineCircle.png");
	Resource RectTexture("blackOutlineRect.png");
	resources.push_back(circleText);
	resources.push_back(RectTexture);
}

bool loadView(std::vector<Resource>& resources, View* view, ResourceManager* resManager, PuddleRenderer* renderer) {
	bool success = false;

	// get the view resources
	/*for (auto res : resources) {
		ResHandleShrdPtr handle = resManager->getHandle(&res);
		if (handle.get()) {
			Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
			SDL_Point point = { 50, 50 };
			PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent({ 50,50 }, 1.0f);
			GameObject* oneBall = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, texture));
			view->addGameObject(oneBall);
			return true;
		}
		else
			return false;
	}*/
	ResHandleShrdPtr handle = resManager->getHandle(&resources.at(0));
	if (handle.get()) {
		Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
		SDL_Point point = { 50, 50 };
		PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent(point, 1.0f);
		GameObject* oneBall = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, texture));
		view->addGameObject(oneBall);
		success = true;
	}
	handle = resManager->getHandle(&resources.at(1));
	if (handle.get()) {
		Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
		SDL_Point point = { 60, 1000 };
		PoolWallPhysicsComponent* physicsComp = new PoolWallPhysicsComponent(point, 1800, 50);
		GameObject* poolWallOne = new GameObject(new PoolWallInputComponent(), physicsComp, new PoolWallGraphicsComponent(renderer, physicsComp, texture));
		view->addGameObject(poolWallOne);
		success = true;
	}
	// load the view game objects

	// sort the game objects by their z-values so that we render objects layered correctly

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