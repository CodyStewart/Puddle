#include <sstream>

#include "poolBall.h"
#include "physics.h"

#define BALL_BOUNCE 1.0f	// coefficient of restitution for Balls
#define WALL_BOUNCE 1.0f	// coefficient of restitution for Walls
#define FRICTION 0.003f

extern PhysicsSystem* physicsSystem;
extern PuddleApp* app;

struct RawImageData;

void PoolBallInputComponent::update() {

}

PoolBallPhysicsComponent::PoolBallPhysicsComponent(Point pos, float radius, float restitution) {
	_linearVelocity = Vec2();
	_angularVelocity = 0.0f;
	_volume = Circle(pos, (int)(radius * UNIT_SIZE / 2));
	_collisionVolume = Circle(pos, (int)(radius * UNIT_SIZE / 2));
	_prevPosition = pos;
	_elasticity = restitution;
	_orientation = Vec3();
}

PoolBallPhysicsComponent::~PoolBallPhysicsComponent() {
	
}

void PoolBallPhysicsComponent::setPosition(Point pos) {
	_volume._position = pos;
	_collisionVolume._position = pos;
}

void PoolBallPhysicsComponent::setOrientation(Vec3 orient) {
	_orientation = orient;
}

void PoolBallPhysicsComponent::setLinearVelocity(Vec2 newVel) {
	this->_linearVelocity._x = newVel._x;
	this->_linearVelocity._y = newVel._y;
	this->_linearVelocity.calculateMagnitude();
}

void PoolBallPhysicsComponent::setAngularVelocity(float newVel) {
	this->_angularVelocity = newVel;
}

float findAngleBetween(Vec2 vec1, Vec2 vec2) {
	float dotVector = vec1.dot(vec2);
	float magnitudeProduct = vec1._length * vec2._length;
	float angle = acosf(dotVector / magnitudeProduct);
	return angle;
}

void PoolBallPhysicsComponent::move(double frametime) {
	if (this->getLinearVelocity()._length > 0.0f) {
		double t = double(UNIT_SIZE) * frametime / double(1000);
		//printf("frametime: %f\n", frametime);
		Vec2 vel = this->getLinearVelocity();
		float pixelsToMoveAlongX = vel._x * t;
		float pixelsToMoveAlongY = vel._y * t;

		this->_volume._position.x += pixelsToMoveAlongX;
		this->_volume._position.y += pixelsToMoveAlongY;
		this->_collisionVolume._position = this->_volume._position;

		// we must also change the x and y values of our orientation based on the linear velocity
		// formula is --- radians = linear distance / radius --- only if we want to tether linear distance with orientation
		// otherwise our orientation is based on angular velocity
		float xRadians = pixelsToMoveAlongX / this->getData().radius;
		float yRadians = pixelsToMoveAlongY / this->getData().radius;

		_orientation._x += xRadians;
		_orientation._y += yRadians;

		if (_orientation._x >= TAU)
			_orientation._x = std::fmod(_orientation._x, TAU);
		else if (_orientation._x <= -(TAU))
			_orientation._x = std::fmod(_orientation._x, TAU);
		if (_orientation._y >= TAU)
			_orientation._y = std::fmod(_orientation._y, TAU);
		else if (_orientation._y <= -(TAU))
			_orientation._y = std::fmod(_orientation._y, TAU);
	}

	if (fabs(this->getAngularVelocity()) > 0.0f) {
		// we only alter the z value of our orientation
		float angVel = this->getAngularVelocity();
		float radius = this->getData().radius;
		float fullRevolution = 2.0f * PI * radius;
		double t = (double)(frametime / double(1000));
		float rotateBy = angVel * UNIT_SIZE * t / fullRevolution;
		_orientation._z += rotateBy;
	}

	//physics->_volume._position.x += static_cast<int>(_velocity._x);
	//physics->_volume._position.y += static_cast<int>(_velocity._y);
	//physics->_collisionVolume._position = physics->_volume._position;
}

void PoolBallPhysicsComponent::resolveCollision(ContactInfo* cInfo) {

	GameObject* objectCollided = cInfo->B;

	const float invMassA = cInfo->A->_InvMass;
	const float invMassB = cInfo->B->_InvMass;

	const float elasticityA = cInfo->A->getElasticity();
	const float elasticityB = cInfo->B->getElasticity();
	const float elasticity = elasticityA * elasticityB;

	// calculate the impulse
	Vec2 n = cInfo->normal;
	Vec2 relativeVel = cInfo->A->getLinearVelocity() - cInfo->B->getLinearVelocity();

	float impulseJNumerator = relativeVel.dot(n) * -(1.0f + elasticity);
	float impulseJDenominator = n.dot(n * (invMassA + invMassB));
	float impulse = impulseJNumerator / impulseJDenominator;
	Vec2 impulseJVecA = n * impulse * invMassA;
	Vec2 impulseJVecB = (n * impulse * invMassB) * -1.0f;

	// apply the linear impulse to us and to the object collided with
	_linearVelocity += impulseJVecA;
	objectCollided->AddLinearImpulse(impulseJVecB);

	// calculate the angular impulse
	Point pos = this->getPosition();
	Vec2 r = (cInfo->ApointWrldSpace - pos.toVec()).getPerp();
	float perpDotOfImpact = impulseJVecA.perpDot(n);
	float radius = this->getData().radius;
	float invTensor = 1.0f / (1.0f * (radius * radius));	// inertia tensor of point mass = Mr^2
	float angImpulseA = impulse * invTensor * perpDotOfImpact; // apply the inverse of the inertia tensor to the angular impulse
	float angImpulseB = angImpulseA * -1.0f;
	_angularVelocity += angImpulseA;	
	objectCollided->AddAngularImpulse(angImpulseB);

	// reposition ourselves
	Vec2 ds = cInfo->BpointWrldSpace - cInfo->ApointWrldSpace;
	const float tA = invMassA / (invMassA + invMassB);
	const float tB = invMassB / (invMassA + invMassB);
	Point APos = cInfo->A->getPosition();
	Point BPos = cInfo->B->getPosition();
	APos += (ds * tA).toPoint();
	BPos -= (ds * tB).toPoint();
	this->setPosition(APos);
	cInfo->B->setPosition(BPos);

	// DEBUG
	//SDL_Renderer* renderer = app->getRenderer()->getRenderer();
	//GameObject* thisObject = app->getObject(_id);
	//int size = _volume._radius * 2;
	//Texture* texture = thisObject->getGraphicsComp()->getTexture();
	//texture->renderScaled(renderer, _volume._position.x - _volume._radius, _volume._position.y - _volume._radius, size, size);
	//SDL_RenderPresent(renderer);

	//}
}

void PoolBallPhysicsComponent::update(std::list<Vec2>* forcesList, std::list<Vec2>* impulsesList, std::list<float>* angularImpulsesList) {

	double frametime = physicsSystem->getThisFrametime() / 10000.0f;

	// apply linear impulses and forces
	Vec2 sumOfForces = Vec2();
	for (auto itr = forcesList->begin(); itr != forcesList->end(); itr++) {
		sumOfForces += *itr;
	}
	for (auto itr = impulsesList->begin(); itr != impulsesList->end(); itr++) {
		sumOfForces += *itr;
	}
	impulsesList->clear();

	_linearVelocity += sumOfForces;
	_prevPosition = _volume._position;

	// apply angular impulses
	float sumOfAngImpulses = 0.0f;
	for (auto itr = angularImpulsesList->begin(); itr != angularImpulsesList->end(); itr++) {
		sumOfAngImpulses += *itr;
	}
	angularImpulsesList->clear();

	_angularVelocity +=  sumOfAngImpulses;

	// apply frictional force
	if (_linearVelocity._length > 0.0f) {
		Vec2 vel = _linearVelocity;
		vel = -(vel);
		vel.normalize();
		vel *= FRICTION;
		_linearVelocity += vel;
		if (_linearVelocity._length > 0.0f && _linearVelocity._length < 0.1f)
			_linearVelocity = Vec2();
		//GameObject* thisObject = app->getObject(_id);
		//if (vel._length > _linearVelocity._length)
		//	thisObject->setVelocity(Vec2());
		//else
		//	thisObject->AddLinearForce(vel);
	}
	if (_angularVelocity != 0.0f) {
		float angVel = _angularVelocity;
		angVel *= -1.0f;
		angVel /= fabs(angVel);
		angVel *= FRICTION;
		_angularVelocity += angVel;
	}

	move(frametime);

	ContactInfo cInfo = ContactInfo();
	GameObject* thisObject = app->getObject(this->_id);
	if (physicsSystem->checkCollisionWithObject(thisObject, &cInfo)) {
		resolveCollision(&cInfo);
	}
}
extern SDL_Window* window;

bool BallSampler::withinSampler(int x, int y) {
	bool ret = false;

	// we need to find out if the coordinates of the byte at x,y are within the radius of the center of the sampler
	int xDelta = _centerOfBall.x - x;
	int yDelta = _centerOfBall.y - y;

	int xDeltaSqr = xDelta * xDelta;
	int yDeltaSqr = yDelta * yDelta;
	int radiusSqr = _radius * _radius;

	if (xDeltaSqr + yDeltaSqr <= radiusSqr + 50)
		ret = true;

	return ret;
}

PoolBallGraphicsComponent::PoolBallGraphicsComponent(PuddleRenderer* renderer, PoolBallPhysicsComponent* physics, ResHandleShrdPtr rawDataHandle) {
	_puddleRenderer = renderer;
	_physics = physics;
	_handle = rawDataHandle;

	Resource res = Resource("ball_ring.png");
	ResHandleShrdPtr ptr = app->getResourceHandle(&res);
	Texture* ringTexture = new Texture(renderer->getRenderer(), ptr->buffer(), ptr->size());
	_ring = ringTexture;

	// create the default texture for the ball by using the raw image data and clipping
	char* imageData = _handle.get()->buffer();
	int size = _handle.get()->size();
	int width = 0;
	int height = 0;
	int depth = 0;

	stbi_uc* newBuffer = stbi_load_from_memory((const stbi_uc*)imageData, size, &width, &height, &depth, 0);
	_imgData.buffer = (char*)newBuffer;
	_imgData.depth = depth;
	_imgData.height = height;
	_imgData.width = width;
	_imgData.size = height * width * depth;
	
	_sampler._centerOfBall = { width / 2, height / 2 };
	_sampler._radius = UNIT_SIZE / 2;
	_sampler._prevOrientation._x = _physics->_orientation._x;
	_sampler._prevOrientation._y = _physics->_orientation._y;
	//char* samples = new char[_imgData.size];
	_sampler._samples = new char[_imgData.size];
	_sampler._width = _imgData.width;
	_sampler._height = _imgData.height;
	_sampler._size = _imgData.size;
	_sampler._depth = _imgData.depth;

	_texture = this->computeOrientedTexture();
}

PoolBallGraphicsComponent::~PoolBallGraphicsComponent() {
	if (_texture) {
		delete _texture;
		_texture = nullptr;
	}
}

/*
	We want to make our sampler center's movement to occupy a circular space rather than a sqaure one
	Take the computed offsets and normalize them across the range 0.0 - sampler radius
*/
void normalizeOffsets(float& xOffset, float& yOffset) {
	if (xOffset == 0.0f || yOffset == 0.0f)
		return;	// no work needs to be done

	// normalize the vector defined by the offsets and multiply the offsets by their normalized values
	// this shrinks the offsets in such a way that the offsets will never exceed the radius and be equivalent in distance to a pure horzizontal or vertical movement
	Vec2 vec = Vec2(xOffset, yOffset);
	vec.normalize();
	xOffset *= fabs(vec._x);
	yOffset *= fabs(vec._y);
}

Texture* PoolBallGraphicsComponent::computeOrientedTexture() {
	float xOrientation = _physics->_orientation._x;
	float yOrientation = _physics->_orientation._y;

	float deltaX = xOrientation - _sampler._prevOrientation._x;
	float deltaY = yOrientation - _sampler._prevOrientation._y;

	/* Compute where on the texture to move center of ball													//
	//	our orientation is in radians, convert to pixels													//
	//	formula is distance = radius * radians																//
	//																										//
	// But first we need to make sure to wrap our sampler appropriately based on orientation				//
	// for the x direction, we deviate directly from the center of the image when orientation				//
	// is between 0 - PI/2 and PI - 3*PI/2 in both + and - directions										//
	// For PI/2 - PI and 3*PI/2 - 2*PI, we start from the opposite edge and work inwards in both + and -	//
	//																										//
	// For y direction we deviate from the center of image for the same orientations as the x				//
	// However, for the other two ranges, we start at the edge in the same direction and move our way back	//
	// towards the center. We MUST make sure to rotate our image 180 degrees when this happens either by	//
	// using the rotation feature or flipping the image														//
	*/																										//
	// take the orientation and mod by PI / 2
	//if (xOrientation > TAU - 0.1f)
	//	int breakOn = 0;
	//float fullMovementRotation = TAU;
	//float percentXRotation = deltaX / fullMovementRotation;
	//int fullMovementPixels = _imgData.width - _sampler._radius * 2;
	//int pixelsToMoveX = percentXRotation * fullMovementPixels;
	//if (_sampler._centerOfBall.x - pixelsToMoveX < _sampler._radius)
	//	_sampler._centerOfBall.x = _imgData.width - _sampler._radius;
	//else if (_sampler._centerOfBall.x - pixelsToMoveX > _imgData.width - _sampler._radius)
	//	_sampler._centerOfBall.x = _sampler._radius;
	//else 
	//	_sampler._centerOfBall.x -= percentXRotation * fullMovementPixels;

	float modXOrientation = std::fmod(xOrientation, PI);
	float percentOfRotationX = modXOrientation / (PI);
	float xTextureOffset = (_imgData.width / 4) * percentOfRotationX;	// with the way we've set up the textures, 1/4 of the image width is as far as we'll go in any direction
	float modYOrientation = std::fmod(yOrientation, PI);
	float percentOfRotationY = modYOrientation / (PI);
	float yTextureOffset = (_imgData.height / 4) * percentOfRotationY;

	//normalizeOffsets(xTextureOffset, yTextureOffset);

	if (xOrientation >= 0.0f && xOrientation < (PI / 2) || xOrientation <= 0.0 && xOrientation > -(PI / 2) || xOrientation >= (PI / 2) && xOrientation < PI || xOrientation <= -(PI / 2) && xOrientation > -PI) {
		_sampler._centerOfBall.x = _imgData.width / 2 - xTextureOffset;
	}
	else if (xOrientation >= PI && xOrientation < (3 * PI / 2) || xOrientation >= (3 * PI / 2) && xOrientation < (2 * PI)) {
		_sampler._centerOfBall.x = 3 * _imgData.width / 4 - xTextureOffset;
	}
	else if (xOrientation <= -PI && xOrientation > -(3 * PI / 2) || xOrientation <= -(3 * PI / 2) && xOrientation > -(2 * PI)) {
		_sampler._centerOfBall.x = _imgData.width / 4 - xTextureOffset;
	}

	if (yOrientation >= 0.0f && yOrientation < (PI / 2) || yOrientation <= 0.0 && yOrientation > -(PI / 2) || yOrientation >= (PI / 2) && yOrientation < PI || yOrientation <= -(PI / 2) && yOrientation > -PI) {
		_sampler._centerOfBall.y = _imgData.height / 2 - yTextureOffset;
	}
	else if (yOrientation >= PI && yOrientation < (3 * PI / 2) || yOrientation >= (3 * PI / 2) && yOrientation < (2 * PI)) {
		_sampler._centerOfBall.y = 3 * _imgData.height / 4 - yTextureOffset;
	}
	else if (yOrientation <= -PI && yOrientation > -(3 * PI / 2) || yOrientation <= -(3 * PI / 2) && yOrientation > -(2 * PI)) {
		_sampler._centerOfBall.y = _imgData.height / 4 - yTextureOffset;
	}

	_sampler._prevOrientation._x = _physics->_orientation._x;
	_sampler._prevOrientation._y = _physics->_orientation._y;

	int* samplePtr = (int*)_sampler._samples;
	int* imgPtr = (int*)_imgData.buffer;
	int sampleCount = 0;

	// clear out the texture
	int transparency = 0x00FFFFFF;
	for (int y = 0; y < _sampler._height; y++) {
		for (int x = 0; x < _sampler._width; x++) {
			*(samplePtr + y * _sampler._width + x) = transparency;
		}
	}

	int xCenterOfImage = _imgData.width / 2;
	int yCenterOfImage = _imgData.height / 2;
	int xDeviation = xCenterOfImage - _sampler._centerOfBall.x;
	int yDeviation = yCenterOfImage - _sampler._centerOfBall.y;

	for (int y = 0; y < _imgData.height; y++) {
		for (int x = 0; x < _imgData.width; x++) {
			if (_sampler.withinSampler(x, y) && sampleCount < _sampler._size) {
				*(samplePtr + (y + yDeviation) * _sampler._width + x + xDeviation) = *(imgPtr + y * _imgData.width + x);
				sampleCount++;
			}
		}
	}

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)_sampler._samples, _sampler._width, _sampler._height, _sampler._depth * 8, _sampler._width * _sampler._depth, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(_puddleRenderer->getRenderer(), surface);
	return new Texture(newTexture);
}

void PoolBallGraphicsComponent::update() {
	// scale the texture
	float percentSamplerOfImage = _sampler._radius * 2 / _imgData.width;
	int size = _physics->_volume._radius * 2 / percentSamplerOfImage;
	float percentRingOfImage = _sampler._radius * 2 / (float)_ring->getWidth();
	int ringSize = _physics->_volume._radius * 2 / percentRingOfImage;

	// convert radians into degrees for z-axis
	float degrees = _physics->_orientation._z * 360.0f / (2.0f * PI);

	delete _texture;
	_texture = this->computeOrientedTexture();
	//_texture->render(_puddleRenderer->getRenderer(), _physics->_volume._position.x - size / 2, _physics->_volume._position.y - size / 2);
	_texture->renderScaledEx(_puddleRenderer->getRenderer(), _physics->_volume._position.x - _imgData.width / 2, _physics->_volume._position.y - _imgData.height / 2, size, size, degrees);
	_ring->renderScaled(_puddleRenderer->getRenderer(), _physics->_volume._position.x - _ring->getWidth() / 2 + 1, _physics->_volume._position.y - _ring->getHeight() / 2, ringSize, ringSize);

	// DEBUG
	float offset = _physics->getData().radius;
	SDL_SetRenderDrawColor(_puddleRenderer->getRenderer(), 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x - offset, _physics->_volume._position.y - offset, _physics->_volume._position.x - offset, _physics->_volume._position.y + offset);
	SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x - offset, _physics->_volume._position.y - offset, _physics->_volume._position.x + offset, _physics->_volume._position.y - offset);
	SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x + offset, _physics->_volume._position.y - offset, _physics->_volume._position.x + offset, _physics->_volume._position.y + offset);
	SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x - offset, _physics->_volume._position.y + offset, _physics->_volume._position.x + offset, _physics->_volume._position.y + offset);

	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), _physics->_volume._position.x, 0, _physics->_volume._position.x, 1050);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), 0, _physics->_volume._position.y, 1900, _physics->_volume._position.y);
	//SDL_RenderDrawLine(_puddleRenderer->getRenderer(), 0, _physics->_volume._position.y + _physics->_volume._radius, 1900, _physics->_volume._position.y + _physics->_volume._radius);
}