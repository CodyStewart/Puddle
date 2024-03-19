#include <stdio.h>

#include <tinyxml2.h>

#include "Views.h"
#include "poolBall.h"
#include "poolTable.h"


void loadStartView(View* view, ResourceManager* resManager, PuddleRenderer* renderer) {
	//Resource circleText("blackOutlineCircle.png");
	//resources.push_back(circleText);
}

/* DebugView is used to check physics. It has:
*	Balls representing pool balls (or any balls)
*	Walls representing the walls of a pool table
*	Colliders for the balls and walls
*/ 

void loadDebugView(View* view, ResourceManager* resManager, PuddleRenderer* renderer) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile("assets/gameViewData.xml");
	doc.PrintError();
	printf("%d", doc.ErrorID());

	// find the correct element
	const tinyxml2::XMLElement* debugElement = doc.FirstChildElement("DebugView");

	// load the background texture
	const char* bgTextureNode = debugElement->FirstChildElement()->FirstChild()->Value();

	// set the game object element
	const tinyxml2::XMLElement* gameObjectElement = debugElement->FirstChildElement("gameObjects");

	// load the PoolBalls
	const tinyxml2::XMLElement* poolBallElement = gameObjectElement->FirstChildElement("PoolBalls");
	const tinyxml2::XMLNode* poolBall = poolBallElement->FirstChild();
	for (int poolBallCount = 0; poolBallCount < poolBallElement->ChildElementCount(); poolBallCount++) {
		// pull the data out of the xml file
		
		// get the resource file
		const char* resourceText;
		const tinyxml2::XMLNode* attribute = poolBall->FirstChild();
		resourceText = attribute->FirstChild()->Value();
		std::string resourceName(resourceText);
		Resource objectResourceName(resourceName);

		// get the position
		Point position = {};
		attribute = attribute->NextSibling();
		const char* xVal = attribute->FirstChild()->FirstChild()->Value();
		const char* yVal = attribute->FirstChild()->NextSibling()->FirstChild()->Value();
		float xPos = std::stof(xVal);
		float yPos = std::stof(yVal);
		position.x = xPos;
		position.y = yPos;

		// get the radius
		float radius = 0;
		attribute = attribute->NextSibling();
		const char* radiusText = attribute->FirstChild()->Value();
		radius = std::stof(radiusText);

		// instantiate the object using the above data
		ResHandleShrdPtr handle = resManager->getHandle(&objectResourceName);
		if (handle.get()) {
			Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
			PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent(position, radius);
			GameObject* poolBallObject = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, texture));
			poolBallObject->setMovable(true);
			poolBallObject->setShapeType(CIRCLE);
			poolBallObject->setMass(1);
			view->addGameObject(poolBallObject);
		}

		// move on to the next poolBall object
		poolBall = poolBall->NextSibling();
	}

	// load the PoolWalls
	const tinyxml2::XMLElement* poolWallElement = gameObjectElement->FirstChildElement("PoolWalls");
	const tinyxml2::XMLNode* poolWall = poolWallElement->FirstChild();
	for (int poolWallCount = 0; poolWallCount < poolWallElement->ChildElementCount(); poolWallCount++) {
		// get the resource file
		const char* resourceText;
		const tinyxml2::XMLNode* attribute = poolWall->FirstChild();
		resourceText = attribute->FirstChild()->Value();
		std::string resourceName(resourceText);
		Resource objectResourceName(resourceName);

		// get the position
		Point position = {};
		attribute = attribute->NextSibling();
		const char* xValText = attribute->FirstChild()->FirstChild()->Value();
		const char* yValText = attribute->FirstChild()->NextSibling()->FirstChild()->Value();
		float xPos = std::stof(xValText);
		float yPos = std::stof(yValText);
		position.x = xPos;
		position.y = yPos;

		// get the width and height
		int width, height = 0;
		attribute = attribute->NextSibling();
		const char* widthText = attribute->FirstChild()->Value();
		attribute = attribute->NextSibling();
		const char* heightText = attribute->FirstChild()->Value();
		width = std::stoi(widthText);
		height = std::stoi(heightText);

		// get the normal
		Vec2 normal = Vec2();
		attribute = attribute->NextSibling();
		const char* xDirText = attribute->FirstChild()->FirstChild()->Value();
		const char* yDirText = attribute->FirstChild()->NextSibling()->FirstChild()->Value();
		float xDir = std::stof(xDirText);
		float yDir = std::stof(yDirText);
		normal._x = xDir;
		normal._y = yDir;

		ResHandleShrdPtr handle = resManager->getHandle(&objectResourceName);
		if (handle.get()) {
			Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
			PoolWallPhysicsComponent* physicsComp = new PoolWallPhysicsComponent(position, width, height, normal);
			GameObject* poolWallObject = new GameObject(new PoolWallInputComponent(), physicsComp, new PoolWallGraphicsComponent(renderer, physicsComp, texture));
			poolWallObject->setMovable(false);
			poolWallObject->setShapeType(RECT);
			view->addGameObject(poolWallObject);
		}

		poolWall = poolWall->NextSibling();
	}

	//NOTE: we need to sort the game objects by their z-values so that we render objects layered correctly

}

//bool loadView(std::vector<Resource>& resources, View* view, ResourceManager* resManager, PuddleRenderer* renderer) {
//	bool success = false;
//
//	// get the view resources
//	/*for (auto res : resources) {
//		ResHandleShrdPtr handle = resManager->getHandle(&res);
//		if (handle.get()) {
//			Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
//			SDL_Point point = { 50, 50 };
//			PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent({ 50,50 }, 1.0f);
//			GameObject* oneBall = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, texture));
//			view->addGameObject(oneBall);
//			return true;
//		}
//		else
//			return false;
//	}*/
//	ResHandleShrdPtr handle = resManager->getHandle(&resources.at(0));
//	if (handle.get()) {
//		Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
//		SDL_Point point = { 50, 50 };
//		PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent(point, 1.0f);
//		GameObject* oneBall = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, texture));
//		view->addGameObject(oneBall);
//		success = true;
//	}
//	handle = resManager->getHandle(&resources.at(1));
//	if (handle.get()) {
//		Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
//		SDL_Point point = { 60, 1000 };
//		PoolWallPhysicsComponent* physicsComp = new PoolWallPhysicsComponent(point, 1800, 50);
//		GameObject* poolWallOne = new GameObject(new PoolWallInputComponent(), physicsComp, new PoolWallGraphicsComponent(renderer, physicsComp, texture));
//		view->addGameObject(poolWallOne);
//		success = true;
//	}
//	// load the view game objects
//
//	// sort the game objects by their z-values so that we render objects layered correctly
//
//	return success;
//}

View::View(ViewState viewState) {

}

View::View(ViewState viewState, ResourceManager* resManager, PuddleRenderer* renderer) {
	std::vector<Resource> resources;
	switch (viewState) {
	case startView:
		// get the startView resources
		loadStartView(this, resManager, renderer);
		break;
	case debugView:
		loadDebugView(this, resManager, renderer);
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

std::vector<GameObject*>* View::getGameObjects() {
	return &_gameObjects;
}

GameObject* View::getGameObject(uint32 id) {
	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); itr++) {
		if ((*itr)->getID() == id) {
			return *itr;
		}
	}

	// we did not find the object
	printf("Object of id: %d not found.\n", id);
	GameObject* emptyObject = new GameObject();
	return emptyObject;
}