#pragma once
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "Object3DHome.h"
#include "Object2DPool.h"
#include <tuple>

enum class ballType { white, red, yellow, black, notSet};
enum class bankSide { horizontal, oblique, cornerLeft, cornerRight };
enum class triangleSide {leftTriangle, rightTriangle};

class Player {
public:
	Player(std::string name);
	Player();
	~Player();
	bool getStatus();
	void setActive();
	void setInactive();
	void setScore(int s);
	int getScore();
	void setFoults(int f);
	int getFoults();
	std::string getName();
	ballType getPlayerColor();
	void setPlayerColor(ballType c);
	ballType getFirstTouched();
	void setFirstTouched(ballType f);
	bool getBlackBallPlay();
	void setBlackBallPlay(bool b);
private:
	ballType playerColor;
	ballType firstTouched;
	std::string name;
	int score;
	bool active;
	int foultsCommited;
	bool blackBallPlay;

};

class GameEngine {
public:
	GameEngine();
	~GameEngine();
	Player* player1;
	Player* player2;
	void setFoult(bool f);
	bool getFoult();
	void switchPlayers();
	Player * getActivePlayer();
	Player * getInActivePlayer();
	void setReset(bool r);
	bool getReset();
	void setRedsPotted(int r);
	void setYellowsPotted(int y);
	int getRedsPotted();
	int getYellowsPotted();
	void setFirstBallPotted(ballType b);
	ballType getFirstBallPotted();
private:
	bool foult;
	bool resetGame;
	int redBallsPotted, yellowBallsPotted;
	ballType firstBallPotted;
};

class Object {
public:
	Object();
	~Object();
	void setTransformCoords(glm::mat4 modelMatrix);
	glm::vec3 getTransformedCoords();
	glm::mat4 getModelMatrix();
	std::string getName();
	Mesh* getMesh();
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
protected:
	std::string name;
	glm::vec3 position;
	Mesh* objectMesh;
	glm::mat4 modelMatrix;
};


class Bank: public Object{
public:
	Bank(std::string name, glm::vec3 position, float bankLength, float bankWidth, float bankHeight, float offset);
	Bank();
	~Bank();
	float getOffset();
	glm::vec3 getDimensions();
	void setTriangleSide(triangleSide s);
	triangleSide getTriangleSide();
private:
	float bankLength, bankWidth, bankHeight;
	float bankOffset;
	triangleSide trSide;
};

class Hole : public Object {
public:
	Hole(std::string name, glm::vec3 position, float radius);
	Hole();
	~Hole();
	float getRadius();
	protected:
		float radius;
};

class PoolBall : public Object {
public:
	PoolBall(std::string name, glm::vec3 position, float radius, Mesh * ballMesh, ballType type, float mass);
	PoolBall();
	~PoolBall();
	float getRadius();
	void setRadius(float newRadius);
	glm::vec2 getMovement();
	void setMovement(float tX, float tZ);
	ballType getType();
	void setTrajectory(glm::vec3 vector);
	glm::vec3 getTrajectory();
	glm::vec3 getMovementVector();
	void setMovementVector(glm::vec3 vec);
	bool getIsMoving();
	void setIsMoving(bool m);
	glm::vec3 getSpeed();
	void setSpeed(glm::vec3 s);
	glm::vec3 getAcceleration();
	void setAcceleration(glm::vec3 s);
	glm::vec3 getPartialPosition();
	void setPartialPosition(glm::vec3 p);
	void setOnTable(bool o);
	bool getOnTable();
	void setInPocket(bool p);
	bool getInPocket();
	float getMass();
protected:
	float mass;
	float ballRadius;
	float translateOX, translateOZ;
	ballType type;
	glm::vec3 trajectory;
	glm::vec3 movementVector;
	glm::vec3 partialPosition;
	bool isMoving, onTable, inPocket;
	glm::vec3 speed;
	glm::vec3 acceleration;
};

class PoolCue : public Object {
public:
	PoolCue(std::string name, glm::vec3 position, Mesh * cue);
	~PoolCue();
	void setCueRotate(float angle);
	float getCueRotate();
	void setPower(float p);
	float getPower();
protected:
	float cueRotate;
	float power;
};


bool ballInPocket(PoolBall *ball, Hole* hole);
std::tuple <bool, bankSide, glm::vec2> collisionDetBanks(PoolBall * ball, Bank * bank);
std::tuple <bool, glm::vec2, glm::vec3> collisionDetBankSide(PoolBall * ball, Bank * bank);
bool ballCollision(PoolBall * ball1, PoolBall * ball2, float deltaTime);
void pushBalls(PoolBall * ball1, PoolBall * ball2, float deltaTime);
float ballInsideBall(glm::vec3 futureCoords, PoolBall * ball2);
bool firstHit(PoolBall * ball1, PoolBall * ball2, float deltaTime);