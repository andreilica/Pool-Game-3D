#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Object3DHome.h"
#include "Object2DPool.h"
#include "Transform3D.h"
#include "ObjectsPool.h"

class PoolGame : public SimpleScene
{
	public:
		PoolGame();
		~PoolGame();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, Texture2D* texture = NULL, bool isBall = false);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Pool::Camera *camera;
		std::unordered_map<std::string, Texture2D*> mapTextures;
		bool renderCameraTarget, moveAllowed = false;
		bool perspective, ortho;

		float roomLength, roomWidth, roomHeight;
		float fovStep, zNear, zFar;
		float tableWidth, tableHeight, tableLength;
		float sideWidth, sideLength, sideHeight;
		float side2Width, side2Length, side2Height;
		float bankWidth, bankLength, bankHeight;
		float bank2Width, bank2Length, bank2Height;
		float offSet, offsetHoles, bankOffset;
		float ballMass, ballMassCreator, ballAcceleration;
		float middleHoleRadius, holeRadius;
		float coverGirth, cover1Length, cover2Length;
		float ballRadius, dotRadius;
		float playableSurfaceWhiteSideOX, playableSurfaceBallsSideOX;
		float playableSurfaceLeftOZ, playableSurfaceRightOZ;
		float playableSurfaceLength, moveLimitOX, firstBallPlacement;
		bool allowWhiteMoveBeginning, thirdPerson, cueNearBall = false, ballsAreStill = true;
		ballType ballWasPotted = ballType::notSet;
		ballType myBall = ballType::notSet;
		bool whiteBallPotted = false;
		bool blackBallPotted = false;
		bool shotStarted = false;
		bool doNotSwitch = false;
		double orthoStep;
		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		float materialKd, materialKs, materialKa;
		float attenuation_factor;
		int isCue = 0;
		glm::vec3 startCorner, centerCoordinates;

		std::vector<Bank*> banks;
		std::vector<Bank*>::iterator it;

		std::vector<Hole*> holes;
		std::vector<Hole*>::iterator itH;

		std::vector<PoolBall*> balls;
		std::vector<PoolBall*>::iterator itB, itB2;

		PoolCue * cue;
		GameEngine * engine;
};
