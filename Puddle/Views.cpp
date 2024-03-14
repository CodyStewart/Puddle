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
		SDL_Point position;
		attribute = attribute->NextSibling();
		const char* xVal = attribute->FirstChild()->FirstChild()->Value();
		const char* yVal = attribute->FirstChild()->NextSibling()->FirstChild()->Value();
		int xPos = std::stoi(xVal);
		int yPos = std::stoi(yVal);
		position.x = xPos;
		position.y = yPos;

		// get the radius
		float radius;
		attribute = attribute->NextSibling();
		const char* radiusText = attribute->FirstChild()->Value();
		radius = std::stof(radiusText);

		// instantiate the object using the above data
		ResHandleShrdPtr handle = resManager->getHandle(&objectResourceName);
		if (handle.get()) {
			Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
			PoolBallPhysicsComponent* physicsComp = new PoolBallPhysicsComponent(position, radius);
			GameObject* poolBallObject = new GameObject(new PoolBallInputComponent(), physicsComp, new PoolBallGraphicsComponent(renderer, physicsComp, texture));
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
		SDL_Point position;
		attribute = attribute->NextSibling();
		const char* xVal = attribute->FirstChild()->FirstChild()->Value();
		const char* yVal = attribute->FirstChild()->NextSibling()->FirstChild()->Value();
		int xPos = std::stoi(xVal);
		int yPos = std::stoi(yVal);
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

		ResHandleShrdPtr handle = resManager->getHandle(&objectResourceName);
		if (handle.get()) {
			Texture* texture = new Texture(renderer->getRenderer(), handle.get()->buffer(), handle.get()->size());
			PoolWallPhysicsComponent* physicsComp = new PoolWallPhysicsComponent(position, width, height);
			GameObject* poolWallObject = new GameObject(new PoolWallInputComponent(), physicsComp, new PoolWallGraphicsComponent(renderer, physicsComp, texture));
			view->addGameObject(poolWallObject);
		}

		poolWall = poolWall->NextSibling();
	}

	//const tinyxml2::XMLNode* node = poolBallNode->FirstChild();
	//const tinyxml2::XMLNode* node2 = node->FirstChild();
	//const char* tex = node2->Value();

	// load the PoolWalls

	//viewElement = viewElement->NextSiblingElement();

	// sort the game objects by their z-values so that we render objects layered correctly

	

	//Resource circleText("blackOutlineCircle.png");
	//Resource RectTexture("blackOutlineRect.png");
	//resources.push_back(circleText);
	//resources.push_back(RectTexture);
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
		loadStartView(this, resManager, renderer);
		// load the resources and instantiate the game objects
		//loadView(resources, this, resManager, renderer);
		break;
	case debugView:
		loadDebugView(this, resManager, renderer);
		//loadView(resources, this, resManager, renderer);
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