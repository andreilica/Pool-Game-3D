#include "ObjectsPool.h"
using namespace std;
Object::Object(){}
Object::~Object(){}

void Object::setTransformCoords(glm::mat4 modelMatrix)
{
	this->modelMatrix = modelMatrix;
}

glm::vec3 Object::getTransformedCoords()
{
	float xp = modelMatrix[0][0] * position.x + modelMatrix[1][0] * position.y + modelMatrix[2][0] * position.z + modelMatrix[3][0];
	float yp = modelMatrix[0][1] * position.x + modelMatrix[1][1] * position.y + modelMatrix[2][1] * position.z + modelMatrix[3][1];
	float zp = modelMatrix[0][2] * position.x + modelMatrix[1][2] * position.y + modelMatrix[2][2] * position.z + modelMatrix[3][2];
	float wp = modelMatrix[0][3] * position.x + modelMatrix[1][3] * position.y + modelMatrix[2][3] * position.z + modelMatrix[3][3];

	xp /= wp;
	yp /= wp;
	zp /= wp;
	return glm::vec3(xp, yp, zp);
}

glm::mat4 Object::getModelMatrix()
{
	return modelMatrix;
}

std::string Object::getName()
{
	return name;
}

Mesh * Object::getMesh()
{
	return objectMesh;
}

glm::vec3 Object::getPosition()
{
	return position;
}

void Object::setPosition(glm::vec3 pos)
{
	position = pos;
}

Bank::Bank(std::string name, glm::vec3 position, float bankLength, float bankWidth, float bankHeight, float offset)
{
	this->name = name;
	this->bankLength = bankLength;
	this->bankWidth = bankWidth;
	this->bankHeight = bankHeight;
	this->position = position;
	this->objectMesh = Object3DHome::CreateBank(name, position, bankLength, bankWidth, bankHeight, offset);
	this->bankOffset = offset;
}

Bank::Bank(){}

Bank::~Bank(){}

float Bank::getOffset()
{
	return bankOffset;
}

glm::vec3 Bank::getDimensions()
{
	return glm::vec3(bankLength, bankWidth, bankHeight);
}

void Bank::setTriangleSide(triangleSide s)
{
	trSide = s;
}

triangleSide Bank::getTriangleSide()
{
	return trSide;
}

Hole::Hole(std::string name, glm::vec3 position, float radius)
{
	this->name = name;
	this->position = position;
	this->radius = radius;
	this->objectMesh = Object2DPool::CreateCircle(name, radius, position);
}

Hole::Hole(){}

Hole::~Hole(){}

float Hole::getRadius()
{
	return radius;
}

PoolBall::PoolBall(std::string name, glm::vec3 position, float radius, Mesh * ballMesh, ballType type, float mass)
{
	this->name = name;
	this->position = position;
	this->ballRadius = radius;
	this->objectMesh = ballMesh;
	this->type = type;
	translateOX = 0;
	translateOZ = 0;
	isMoving = false;
	inPocket = false;
	onTable = true;
	speed = glm::vec3(0.0f);
	partialPosition = glm::vec3(0);
	movementVector = glm::vec3(0);
	acceleration = glm::vec3(0);
	this->mass = mass;
}

PoolBall::PoolBall(){}

PoolBall::~PoolBall(){}

float PoolBall::getRadius()
{
	return ballRadius;
}

void PoolBall::setRadius(float newRadius)
{
	ballRadius = newRadius;
}

glm::vec2 PoolBall::getMovement()
{
	return glm::vec2(translateOX, translateOZ);
}


void PoolBall::setMovement(float tX, float tZ)
{
	translateOX = tX;
	translateOZ = tZ;
}

ballType PoolBall::getType()
{
	return type;
}

void PoolBall::setTrajectory(glm::vec3 vector)
{
	trajectory = normalize(glm::vec3(vector.x, 0, vector.z)) + getTransformedCoords();
}

glm::vec3 PoolBall::getTrajectory()
{
	return trajectory;
}

glm::vec3 PoolBall::getMovementVector()
{
	return movementVector;
}

void PoolBall::setMovementVector(glm::vec3 vec)
{
	movementVector = normalize(glm::vec3(vec.x, 0, vec.z));
}

bool PoolBall::getIsMoving()
{
	return isMoving;
}

void PoolBall::setIsMoving(bool m)
{
	isMoving = m;
}

glm::vec3 PoolBall::getSpeed()
{
	return speed;
}

void PoolBall::setSpeed(glm::vec3 s)
{
	speed = s;
}

glm::vec3 PoolBall::getAcceleration()
{
	return acceleration;
}

void PoolBall::setAcceleration(glm::vec3 s)
{
	this->acceleration = s;
}

glm::vec3 PoolBall::getPartialPosition()
{
	return partialPosition;
}

void PoolBall::setPartialPosition(glm::vec3 p)
{
	partialPosition = p;
}

void PoolBall::setOnTable(bool o)
{
	onTable = o;
}

bool PoolBall::getOnTable()
{
	return onTable;
}

void PoolBall::setInPocket(bool p)
{
	inPocket = p;
}

bool PoolBall::getInPocket()
{
	return inPocket;
}

float PoolBall::getMass()
{
	return mass;
}

PoolCue::PoolCue(std::string name, glm::vec3 position, Mesh * cue)
{
	this->name = name;
	this->position = position;
	this->objectMesh = cue;
	this->cueRotate = 0;
}

PoolCue::~PoolCue(){}

void PoolCue::setCueRotate(float angle)
{
	this->cueRotate = angle;
}

float PoolCue::getCueRotate()
{
	return cueRotate;
}

void PoolCue::setPower(float p)
{
	power = p;
}

float PoolCue::getPower()
{
	return power;
}

bool ballInPocket(PoolBall *ball, Hole* hole) {
	if (!ball->getInPocket()) {
		float holeRadius = hole->getRadius();
		glm::vec3 holeCoords = hole->getTransformedCoords();
		glm::vec3 ballcoords = ball->getTransformedCoords();
		return ((ballcoords.x - holeCoords.x) * (ballcoords.x - holeCoords.x)
			+ (ballcoords.z - holeCoords.z) * (ballcoords.z - holeCoords.z)) < holeRadius * holeRadius;
	}
	return false;
}


float intervalFunction(float value, float min, float max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

std::tuple <bool, bankSide, glm::vec2> collisionDetBanks(PoolBall * ball, Bank * bank)
{
	float nearX, nearZ, centerX, centerZ, bankX, bankZ, distance;
	float radius = ball->getRadius();
	glm::vec2 moveOutBank;
	bankSide side;
	
	
	if (bank->getName() == "Bank1") {
		centerX = ball->getTransformedCoords().x;
		centerZ = ball->getTransformedCoords().z;
		bankX = bank->getTransformedCoords().x;
		bankZ = bank->getTransformedCoords().z - bank->getDimensions().y;
		nearX = intervalFunction(centerX, bankX + bank->getOffset(), bankX + bank->getDimensions().x - bank->getOffset());
		nearZ = intervalFunction(centerZ, bankZ, bankZ + bank->getDimensions().y);

		if (nearX == bankX + bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerLeft;
			moveOutBank.x = nearX - centerX - radius;
		}
		else if (nearX == bankX + bank->getDimensions().x - bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerRight;
			moveOutBank.x = -nearX + centerX - radius;
		}
		else
			side = bankSide::horizontal;
		distance = (centerX - nearX) * (centerX - nearX) + (centerZ - nearZ) * (centerZ - nearZ);
		moveOutBank.y = nearZ - centerZ - radius;

	} else if(bank->getName() == "Bank2"){
		centerX = ball->getTransformedCoords().x;
		centerZ = ball->getTransformedCoords().z;
		bankX = bank->getTransformedCoords().x;
		bankZ = bank->getTransformedCoords().z - bank->getDimensions().y;
		nearX = intervalFunction(centerX, bankX + bank->getOffset(), bankX + bank->getDimensions().x - bank->getOffset());
		nearZ = intervalFunction(centerZ, bankZ, bankZ + bank->getDimensions().y);

		if (nearX == bankX + bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerLeft;
			moveOutBank.x = nearX - centerX - radius;
		}
		else if (nearX == bankX + bank->getDimensions().x - bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerRight;
			moveOutBank.x = -nearX + centerX - radius;
		}
		else
			side = bankSide::horizontal;
		distance = (centerX - nearX) * (centerX - nearX) + (centerZ - nearZ) * (centerZ - nearZ);
		moveOutBank.y = nearZ - centerZ - radius;

	} else if(bank->getName() == "Bank3") {
		centerX = ball->getTransformedCoords().x;
		centerZ = ball->getTransformedCoords().z;
		bankX = bank->getTransformedCoords().x;
		bankZ = bank->getTransformedCoords().z + bank->getDimensions().y;
		
		nearX = intervalFunction(centerX, bankX - bank->getDimensions().x + bank->getOffset(), bankX - bank->getOffset());
		nearZ = intervalFunction(centerZ, bankZ - bank->getDimensions().y, bankZ);

		if (nearX == bankX - bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerLeft;
			moveOutBank.x = -nearX + centerX - radius;
		}
		else if (nearX == bankX - bank->getDimensions().x + bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerRight;
			moveOutBank.x = nearX - centerX - radius;
		}
		else
			side = bankSide::horizontal;
		distance = (centerX - nearX) * (centerX - nearX) + (centerZ - nearZ) * (centerZ - nearZ);
		moveOutBank.y = -nearZ + centerZ - radius;

	} else if(bank->getName() == "Bank4") {
		centerX = ball->getTransformedCoords().x;
		centerZ = ball->getTransformedCoords().z;
		bankX = bank->getTransformedCoords().x;
		bankZ = bank->getTransformedCoords().z + bank->getDimensions().y;

		nearX = intervalFunction(centerX, bankX - bank->getDimensions().x + bank->getOffset(), bankX - bank->getOffset());
		nearZ = intervalFunction(centerZ, bankZ - bank->getDimensions().y, bankZ);

		if (nearX == bankX - bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerLeft;
			moveOutBank.x = -nearX + centerX - radius;
		}
		else if (nearX == bankX - bank->getDimensions().x + bank->getOffset() && nearZ == bankZ) {
			side = bankSide::cornerRight;
			moveOutBank.x = nearX - centerX - radius;
			
		}
		else
			side = bankSide::horizontal;
		distance = (centerX - nearX) * (centerX - nearX) + (centerZ - nearZ) * (centerZ - nearZ);
		moveOutBank.y = -nearZ + centerZ - radius;

	} else if (bank->getName() == "Bank5") {
		centerX = ball->getTransformedCoords().x;
		centerZ = ball->getTransformedCoords().z;
		bankX = bank->getTransformedCoords().x + bank->getDimensions().y;
		bankZ = bank->getTransformedCoords().z;

		nearX = intervalFunction(centerX, bankX - bank->getDimensions().x, bankX);
		nearZ = intervalFunction(centerZ, bankZ + bank->getOffset(), bankZ + bank->getDimensions().x - bank->getOffset());

		if (nearX == bankX && nearZ == bankZ + bank->getOffset()) {
			side = bankSide::cornerLeft;
			moveOutBank.y = nearZ - centerZ - radius;
		}
		else if (nearX == bankX && nearZ == bankZ + bank->getDimensions().x - bank->getOffset()) {
			side = bankSide::cornerRight;
			moveOutBank.y = -nearZ + centerZ - radius;
		}
		else
			side = bankSide::horizontal;
		distance = (centerX - nearX) * (centerX - nearX) + (centerZ - nearZ) * (centerZ - nearZ);
		moveOutBank.x = centerX - nearX - radius;
	} else if (bank->getName() == "Bank6") {
		centerX = ball->getTransformedCoords().x;
		centerZ = ball->getTransformedCoords().z;
		bankX = bank->getTransformedCoords().x - bank->getDimensions().y;
		bankZ = bank->getTransformedCoords().z;

		nearX = intervalFunction(centerX, bankX, bankX + bank->getDimensions().y);
		nearZ = intervalFunction(centerZ, bankZ - bank->getDimensions().x + bank->getOffset(), bankZ - bank->getOffset());

		if (nearX == bankX && nearZ == bankZ - bank->getOffset()) {
			side = bankSide::cornerLeft;
			moveOutBank.y = -nearZ + centerZ - radius;
		}
		else if (nearX == bankX && nearZ == bankZ - bank->getDimensions().x + bank->getOffset()) {
			side = bankSide::cornerRight;
			moveOutBank.y = nearZ - centerZ - radius;
		}
		else
			side = bankSide::horizontal;
		distance = (centerX - nearX) * (centerX - nearX) + (centerZ - nearZ) * (centerZ - nearZ);
		moveOutBank.x = nearX - centerX - radius;
	}
	
	if (distance < radius * radius)
		return std::tuple <bool, bankSide, glm::vec2>(true, side, moveOutBank);
	else
		return std::tuple <bool, bankSide, glm::vec2>(false, side, moveOutBank);
}


float ballInsideBall(glm::vec3 futureCoords, PoolBall * ball2) {
	float distance = glm::length(futureCoords - ball2->getTransformedCoords());
	if (distance <= 2 * ball2->getRadius()) {
		return 2 * ball2->getRadius() - distance;
	}
	return 0.0f;
}
std::tuple <bool, glm::vec2, glm::vec3> collisionDetBankSide(PoolBall * ball, Bank * bank) {
	float radius, dx, dz, distance;
	float epsilon = 0.005f;
	float breakage = 0.015f;
	glm::vec3 segX1, segX2, bankCorner, normal, ballCenter;
	radius = ball->getRadius();
	ballCenter = glm::vec3(ball->getTransformedCoords().x, 0, ball->getTransformedCoords().z);
	glm::vec2 toMoveBall;
	if (bank->getName() == "Bank1" || bank->getName() == "Bank2") {
		if (bank->getTriangleSide() == triangleSide::leftTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x + bank->getOffset() - breakage, 0, bankCorner.z - bank->getDimensions().y + breakage);
			segX2 = glm::vec3(bankCorner.x, 0, bankCorner.z);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;

			normal = glm::normalize(glm::vec3(dx, 0, -dz));
			distance = dot(ballCenter - segX2, normal);
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon));
		}

		if (bank->getTriangleSide() == triangleSide::rightTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x + bank->getDimensions().x - bank->getOffset() + breakage, 0, bankCorner.z - bank->getDimensions().y + breakage);
			segX2 = glm::vec3(bankCorner.x + bank->getDimensions().x, 0, bankCorner.z);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;

			normal = glm::normalize(glm::vec3(dx, 0, -dz));
			distance = dot(ballCenter - segX2, normal);
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon));
		}
	} else if (bank->getName() == "Bank3" || bank->getName() == "Bank4") {
		if (bank->getTriangleSide() == triangleSide::leftTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x - bank->getOffset() + breakage, 0, bankCorner.z + bank->getDimensions().y - breakage);
			segX2 = glm::vec3(bankCorner.x , 0, bankCorner.z);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;

			normal = glm::normalize(glm::vec3(dx, 0, -dz));
			distance = dot(ballCenter - segX2, normal);
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon));
		}

		if (bank->getTriangleSide() == triangleSide::rightTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x - bank->getDimensions().x + bank->getOffset() - breakage, 0, bankCorner.z + bank->getDimensions().y - breakage);
			segX2 = glm::vec3(bankCorner.x - bank->getDimensions().x, 0, bankCorner.z);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;

			normal = glm::normalize(glm::vec3(dx, 0, -dz));
			distance = dot(ballCenter - segX2, normal);
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon));
		}
	}
	else if (bank->getName() == "Bank5") {
		if (bank->getTriangleSide() == triangleSide::leftTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x + bank->getDimensions().y - breakage, 0, bankCorner.z + bank->getOffset() - breakage);
			segX2 = glm::vec3(bankCorner.x, 0, bankCorner.z);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;
			normal = glm::normalize(glm::vec3(-dx, 0, dz));
			distance = dot(ballCenter - segX2, normal);
			
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon)) ;
		}

		if (bank->getTriangleSide() == triangleSide::rightTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x + bank->getDimensions().y - breakage, 0, bankCorner.z + bank->getDimensions().x - bank->getOffset() + breakage);
			segX2 = glm::vec3(bankCorner.x, 0, bankCorner.z + bank->getDimensions().x);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;
			normal = glm::normalize(glm::vec3(-dx, 0, dz));
			distance = dot(ballCenter - segX2, normal);
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon)) ;
		}
	} else if (bank->getName() == "Bank6") {
		if (bank->getTriangleSide() == triangleSide::leftTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x - bank->getDimensions().y + breakage, 0, bankCorner.z - bank->getOffset() + breakage);
			segX2 = glm::vec3(bankCorner.x, 0, bankCorner.z);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;
			normal = glm::normalize(glm::vec3(-dx, 0, dz));
			distance = dot(ballCenter - segX2, normal);
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon));
		}

		if (bank->getTriangleSide() == triangleSide::rightTriangle) {
			bankCorner = bank->getTransformedCoords();
			segX1 = glm::vec3(bankCorner.x - bank->getDimensions().y + breakage, 0, bankCorner.z - bank->getDimensions().x + bank->getOffset() - breakage);
			segX2 = glm::vec3(bankCorner.x, 0, bankCorner.z - bank->getDimensions().x);
			dz = segX2.z - segX1.z;
			dx = segX2.x - segX1.x;
			normal = glm::normalize(glm::vec3(-dx, 0, dz));
			distance = dot(ballCenter - segX2, normal);
			toMoveBall = glm::vec2(normal.x * (max(0.0f, radius - distance) + epsilon), normal.z * (max(0.0f, radius - distance) + epsilon));
		}
	}
	if (distance < radius && dot(ballCenter - segX1, segX2 - segX1) >= 0 && dot(ballCenter - segX2, segX1 - segX2) >= 0)
		return std::tuple <bool, glm::vec2, glm::vec3>(true, toMoveBall, normal);
	else
		return std::tuple <bool, glm::vec2, glm::vec3>(false, toMoveBall, normal);

}


bool ballCollision(PoolBall * ball1, PoolBall * ball2, float deltaTime) {
	double distanceBetweenBalls, radius1, radius2, auxDist, vecToCenter2Length, perpendicularToMoveVec, radiusSum, radiusSumSquare, thirdSide;
	float distanceToTravel;
	glm::vec3 ball1Coords, ball2Coords, moveVectorNormalized, ball1Speed, vecToCenter2, moveVector;
	radius1 = ball1->getRadius();
	radius2 = ball2->getRadius();
	radiusSum = radius1 + radius2;
	ball1Speed = ball1->getSpeed();

	ball1Coords = ball1->getTransformedCoords();
	ball2Coords = ball2->getTransformedCoords();

	if (ball1->getIsMoving() && !ball2->getInPocket()) {

		moveVector = ball1->getPartialPosition() - ball2->getPartialPosition();
		moveVectorNormalized = glm::normalize(moveVector);
		
		distanceBetweenBalls = glm::length(ball2Coords - ball1Coords);
		distanceBetweenBalls -= radiusSum;

		if (glm::length(moveVector) < distanceBetweenBalls)
			return false;

		vecToCenter2 = ball2Coords - ball1Coords;
		auxDist = dot(moveVectorNormalized, vecToCenter2);
		if (auxDist <= 0)
			return false;

		vecToCenter2Length = glm::length(vecToCenter2);
		perpendicularToMoveVec = (vecToCenter2Length * vecToCenter2Length) - (auxDist * auxDist);
		radiusSumSquare = radiusSum * radiusSum;

		if (perpendicularToMoveVec >= radiusSumSquare)
			return false;

		thirdSide = radiusSumSquare - perpendicularToMoveVec;

		if (thirdSide < 0)
			return false;

		distanceToTravel = (float)( auxDist - sqrt(thirdSide));

		if (glm::length(moveVector) < distanceToTravel)
			return false;


		ball1->setPartialPosition(ball1->getPartialPosition() * (glm::length(glm::normalize(moveVector) * distanceToTravel * deltaTime) / glm::length(ball1->getPartialPosition())));
		ball2->setPartialPosition(ball2->getPartialPosition() * (glm::length(glm::normalize(moveVector) * distanceToTravel * deltaTime) / glm::length(ball1->getPartialPosition())));
		return true;
	}
	return false;
}

void pushBalls(PoolBall * ball1, PoolBall * ball2, float deltaTime) {
	glm::vec3 moveVec1, moveVec2, centerVector, newV1, newV2;
	float mass1 = ball1->getMass(), mass2 = ball2->getMass(), deltaMag, lossFactor;
	moveVec1 = ball1->getPartialPosition();
	moveVec2 = ball2->getPartialPosition();
	lossFactor = 0.78f;

	if (ballCollision(ball1, ball2, deltaTime)){
		if (mass1 > mass2) {
			float k = mass1;
			mass1 = mass2;
			mass2 = k;
		}
		centerVector = glm::normalize(ball1->getTransformedCoords() - ball2->getTransformedCoords());
		float length1 = dot(moveVec1, centerVector);
		float length2 = dot(moveVec2, centerVector);
		deltaMag = (2.0f * (length1 - length2)) / (mass1 + mass2);
		newV1 = moveVec1 - deltaMag * mass2 * centerVector;
		newV2 = moveVec2 + deltaMag * mass1 * centerVector;
		ball1->setMovementVector(glm::normalize(newV1));
		ball2->setMovementVector(glm::normalize(newV2));

		if (ball1->getIsMoving() && ball2->getIsMoving()) {
			glm::vec3 avgSpeed = (ball1->getSpeed() + ball2->getSpeed()) / glm::vec3(2);
			ball1->setSpeed(glm::vec3(avgSpeed));
			ball2->setSpeed(glm::vec3(avgSpeed));
		}
		else if(ball2->getSpeed() == glm::vec3(0))
			ball2->setSpeed(ball1->getSpeed() * lossFactor);
		
		ball1->setPartialPosition(newV1 * ball1->getSpeed() * deltaTime);
		ball2->setPartialPosition(newV2 * ball2->getSpeed() * deltaTime);
		
		if (!ball2->getIsMoving()) {
			ball2->setIsMoving(true);
		}
		
	}

	
}

Player::Player(std::string name)
{
	this->name = name;
	score = 0;
	active = false;
	playerColor = ballType::notSet;
	firstTouched = ballType::notSet;
	foultsCommited = 0;
	blackBallPlay = false;
}

Player::Player(){}

Player::~Player(){}

bool Player::getStatus()
{
	return active;
}

void Player::setActive()
{
	active = true;
}

void Player::setInactive()
{
	active = false;
}

void Player::setScore(int s)
{
	score = s;
}

int Player::getScore()
{
	return score;
}

void Player::setFoults(int f)
{
	foultsCommited = f;
}

int Player::getFoults()
{
	return foultsCommited;
}

string Player::getName()
{
	return name;
}

ballType Player::getPlayerColor()
{
	return playerColor;
}

void Player::setPlayerColor(ballType c)
{
	playerColor = c;
}

ballType Player::getFirstTouched()
{
	return firstTouched;
}

void Player::setFirstTouched(ballType f)
{
	firstTouched = f;
}

bool Player::getBlackBallPlay()
{
	return blackBallPlay;
}

void Player::setBlackBallPlay(bool b)
{
	blackBallPlay = b;
}

GameEngine::GameEngine(){
	yellowBallsPotted = 0;
	redBallsPotted = 0;
	resetGame = false;
	firstBallPotted = ballType::notSet;
}

GameEngine::~GameEngine(){}

void GameEngine::setFoult(bool f)
{
	foult = f;
}

bool GameEngine::getFoult()
{
	return foult;
}

void GameEngine::switchPlayers()
{
	if (player1->getStatus() && !player2->getStatus()) {
		player1->setInactive();
		player2->setActive();
	}
	else if (!player1->getStatus() && player2->getStatus()) {
		player1->setActive();
		player2->setInactive();
	}
}

Player * GameEngine::getActivePlayer()
{
	if (player1->getStatus())
		return player1;
	else
		return player2;
}

Player * GameEngine::getInActivePlayer()
{
	if (!player1->getStatus())
		return player1;
	else
		return player2;
}

void GameEngine::setReset(bool r)
{
	resetGame = r;
}

bool GameEngine::getReset()
{
	return resetGame;
}

void GameEngine::setRedsPotted(int r)
{
	redBallsPotted = r;
}

void GameEngine::setYellowsPotted(int y)
{
	yellowBallsPotted = y;
}

int GameEngine::getRedsPotted()
{
	return redBallsPotted;
}

int GameEngine::getYellowsPotted()
{
	return yellowBallsPotted;
}

void GameEngine::setFirstBallPotted(ballType b)
{
	firstBallPotted = b;
}

ballType GameEngine::getFirstBallPotted()
{
	return firstBallPotted;
}



bool firstHit(PoolBall * ball1, PoolBall * ball2, float deltaTime) {
	double distanceBetweenBalls, radius1, radius2, auxDist, vecToCenter2Length, perpendicularToMoveVec, radiusSum, radiusSumSquare, thirdSide;
	float distanceToTravel;
	glm::vec3 ball1Coords, ball2Coords, moveVectorNormalized, ball1Speed, vecToCenter2, moveVector;
	radius1 = ball1->getRadius();
	radius2 = ball2->getRadius();
	radiusSum = radius1 + radius2;
	ball1Speed = ball1->getSpeed();

	ball1Coords = ball1->getTransformedCoords();
	ball2Coords = ball2->getTransformedCoords();

	if (ball1->getIsMoving() && !ball2->getInPocket()) {

		moveVector = ball1->getPartialPosition() - ball2->getPartialPosition();
		moveVectorNormalized = glm::normalize(moveVector);

		distanceBetweenBalls = glm::length(ball2Coords - ball1Coords);
		distanceBetweenBalls -= radiusSum;

		if (glm::length(moveVector) < distanceBetweenBalls)
			return false;

		vecToCenter2 = ball2Coords - ball1Coords;
		auxDist = dot(moveVectorNormalized, vecToCenter2);
		if (auxDist <= 0)
			return false;

		vecToCenter2Length = glm::length(vecToCenter2);
		perpendicularToMoveVec = (vecToCenter2Length * vecToCenter2Length) - (auxDist * auxDist);
		radiusSumSquare = radiusSum * radiusSum;

		if (perpendicularToMoveVec >= radiusSumSquare)
			return false;

		thirdSide = radiusSumSquare - perpendicularToMoveVec;

		if (thirdSide < 0)
			return false;

		distanceToTravel = (float)(auxDist - sqrt(thirdSide));

		if (glm::length(moveVector) < distanceToTravel)
			return false;

		return true;
	}
	return false;
}