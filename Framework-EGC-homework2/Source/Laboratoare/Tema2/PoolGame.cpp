#include "PoolGame.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

PoolGame::PoolGame()
{
}

PoolGame::~PoolGame()
{
}

void PoolGame::Init()
{
	{
		engine = new GameEngine();
		engine->player1 = new Player("Player1");
		engine->player2 = new Player("Player2");
		engine->player1->setActive();
	}
	{
		renderCameraTarget = false;
		fovStep = 60;
		zNear = 0.01f;
		zFar = 200.0f;
		orthoStep = 10;
		thirdPerson = false;
	}
	{
		startCorner = glm::vec3(0, 0, 0.71f);
		centerCoordinates = glm::vec3(0, 0, 0);
		allowWhiteMoveBeginning = true;
	}

	{
		offSet = 0.3f;
		offsetHoles = 0.002f;
		bankOffset = 0.07f;
	}

	{
		tableLength = 2.84f;
		tableWidth = 1.42f;
		tableHeight = 0.90f;
	}
	{
		sideWidth = 0.13f;
		sideLength = 1.37f;
		sideHeight = 0.07f;

		side2Width = 0.13f;
		side2Length = tableWidth + 2 * offSet - 0.5f;
		side2Height = 0.07f;
	}
	{
		bankWidth = 0.07f;
		bankLength = 1.37f;
		bankHeight = 0.07f;

		bank2Width = 0.07f;
		bank2Length = tableWidth + 2 * offSet - 0.5f;
		bank2Height = 0.07f;
	}
	{
		holeRadius = 0.125f;
		middleHoleRadius = 0.1f;
		dotRadius = 0.01f;
	}
	{
		cover1Length = 0.25f;
		cover2Length = 0.20f;
		coverGirth = 0.005f;
	}

	{
		ballRadius = 1.0f;
		ballMass = 0.2f;
		ballAcceleration = -0.35f;
	}
	{
		roomLength = 4.0f + tableLength;
		roomWidth = 4.0f + tableWidth;
		roomHeight = 2.7f + tableHeight;
	}

	{
		playableSurfaceWhiteSideOX = centerCoordinates.x - offSet + side2Width + bank2Width;
		playableSurfaceBallsSideOX = centerCoordinates.x + tableLength + offSet - side2Width - bank2Width;
		playableSurfaceRightOZ = startCorner.z + offSet - sideWidth - bankWidth;
		playableSurfaceLeftOZ = startCorner.z - tableWidth - offSet + sideWidth + bankWidth;

		playableSurfaceLength = playableSurfaceBallsSideOX - playableSurfaceWhiteSideOX;
		moveLimitOX = playableSurfaceWhiteSideOX + playableSurfaceLength / 4;
		firstBallPlacement = playableSurfaceWhiteSideOX + 3 * (playableSurfaceLength / 4);
	}

	camera = new Pool::Camera();
	camera->Set(glm::vec3(-1.22f, 1.76f, 1.04f), glm::vec3(1.8f, 0, -offSet), glm::vec3(0, 1, 0));
	camera->SetProjectionMatrix(fovStep, window->props.aspectRatio, zNear, zFar);

	{
		lightPosition = glm::vec3(tableLength / 2, 2.3f, 0);
		lightDirection = glm::vec3(0, -1, 0);
		materialShininess = 32;
		materialKa = 0.75f;
		materialKd = 0.5f;
		materialKs = 0.5f;
		attenuation_factor = 0.2f;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/wood.jpg", GL_REPEAT);
		mapTextures["wood"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/carpet.jpg", GL_REPEAT);
		mapTextures["carpet"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/wallpaper.jpg", GL_REPEAT);
		mapTextures["wallpaper"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/ceiling.jpg", GL_REPEAT);
		mapTextures["ceiling"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/woodCue.jpg", GL_REPEAT);
		mapTextures["woodCue"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/white.png", GL_REPEAT);
		mapTextures["white"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/cloth.jpg", GL_REPEAT);
		mapTextures["cloth"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/black.jpg", GL_REPEAT);
		mapTextures["hole"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/whiteBall.png", GL_REPEAT);
		mapTextures["whiteBall"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/redBall.jpg", GL_REPEAT);
		mapTextures["redBall"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/yellowBall.jpg", GL_REPEAT);
		mapTextures["yellowBall"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Source/Laboratoare/Tema2/Textures/blackBall.jpg", GL_REPEAT);
		mapTextures["blackBall"] = texture;
	}

	{
		Mesh* mesh = new Mesh("Ball");
		mesh->LoadMesh("Source/Laboratoare/Tema2/Objects", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("Cue");
		mesh->LoadMesh("Source/Laboratoare/Tema2/Objects", "cue.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader *shader = new Shader("PoolShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	cout << "\n=========================\nActive player is: " << engine->getActivePlayer()->getName() <<  "\n=========================\n";

	Mesh* room = Object3DHome::CreateRoom("Room", roomLength, roomWidth, roomHeight);
	Mesh* floor = Object3DHome::CreateFloor("Floor", roomLength, roomWidth, roomHeight);
	Mesh* ceiling = Object3DHome::CreateCeiling("Ceiling", roomLength, roomWidth, roomHeight);
	Mesh* poolTable = Object3DHome::CreateFrustum("PoolTable", tableLength, tableWidth, tableHeight);
	Mesh* poolCloth = Object3DHome::CreateCloth("PoolCloth", tableLength, tableWidth, tableHeight);

	Mesh* poolSide1 = Object3DHome::CreateCuboid("PoolSide1", sideLength, sideWidth, sideHeight);
	Mesh* poolSide2 = Object3DHome::CreateCuboid("PoolSide2", sideLength, sideWidth, sideHeight);
	Mesh* poolSide3 = Object3DHome::CreateCuboid("PoolSide3", sideLength, sideWidth, sideHeight);
	Mesh* poolSide4 = Object3DHome::CreateCuboid("PoolSide4", sideLength, sideWidth, sideHeight);
	Mesh* poolSide5 = Object3DHome::CreateCuboid("PoolSide5", side2Length, side2Width, side2Height);
	Mesh* poolSide6 = Object3DHome::CreateCuboid("PoolSide6", side2Length, side2Width, side2Height);

	Mesh* cover1 = Object3DHome::CreateCuboid("Cover1", cover1Length, coverGirth, sideHeight);
	Mesh* cover2 = Object3DHome::CreateCuboid("Cover2", cover1Length - coverGirth, coverGirth, sideHeight);
	Mesh* cover3 = Object3DHome::CreateCuboid("Cover3", cover2Length, coverGirth, sideHeight);
	Mesh* cover4 = Object3DHome::CreateCuboid("Cover4", cover1Length, coverGirth, sideHeight);
	Mesh* cover5 = Object3DHome::CreateCuboid("Cover5", cover1Length, coverGirth, sideHeight);
	Mesh* cover6 = Object3DHome::CreateCuboid("Cover6", cover1Length - coverGirth, coverGirth, sideHeight);
	Mesh* cover7 = Object3DHome::CreateCuboid("Cover7", cover2Length, coverGirth, sideHeight);
	Mesh* cover8 = Object3DHome::CreateCuboid("Cover8", cover1Length, coverGirth, sideHeight);
	Mesh* cover9 = Object3DHome::CreateCuboid("Cover9", cover1Length - coverGirth, coverGirth, sideHeight);
	Mesh* cover10 = Object3DHome::CreateCuboid("Cover10", cover1Length - coverGirth, coverGirth, sideHeight);

	Mesh* dot1 = Object2DPool::CreateDot("Dot1", dotRadius, startCorner);
	Mesh* dot2 = Object2DPool::CreateDot("Dot2", dotRadius, startCorner);
	Mesh* dot3 = Object2DPool::CreateDot("Dot3", dotRadius, startCorner);
	Mesh* dot4 = Object2DPool::CreateDot("Dot4", dotRadius, startCorner);
	Mesh* dot5 = Object2DPool::CreateDot("Dot5", dotRadius, startCorner);
	Mesh* dot6 = Object2DPool::CreateDot("Dot6", dotRadius, startCorner);
	Mesh* dot7 = Object2DPool::CreateDot("Dot7", dotRadius, startCorner);
	Mesh* dot8 = Object2DPool::CreateDot("Dot8", dotRadius, startCorner);
	Mesh* dot9 = Object2DPool::CreateDot("Dot9", dotRadius, startCorner);
	Mesh* dot10 = Object2DPool::CreateDot("Dot10", dotRadius, startCorner);
	Mesh* dot11 = Object2DPool::CreateDot("Dot11", dotRadius, startCorner);
	Mesh* dot12 = Object2DPool::CreateDot("Dot12", dotRadius, startCorner);
	Mesh* dot13 = Object2DPool::CreateDot("Dot13", dotRadius, startCorner);
	Mesh* dot14 = Object2DPool::CreateDot("Dot14", dotRadius, startCorner);
	Mesh* dot15 = Object2DPool::CreateDot("Dot15", dotRadius, startCorner);
	Mesh* dot16 = Object2DPool::CreateDot("Dot16", dotRadius, startCorner);
	Mesh* dot17 = Object2DPool::CreateDot("Dot17", dotRadius, startCorner);
	Mesh* dot18 = Object2DPool::CreateDot("Dot18", dotRadius, startCorner);

	for (int i = 1; i <= 4; i++) {
		auto name = "Hole" + std::to_string(i);
		Hole* hole = new Hole(name, startCorner, holeRadius);
		holes.push_back(hole);
	}

	for (int i = 5; i <= 6; i++) {
		auto name = "Hole" + std::to_string(i);
		Hole* hole = new Hole(name, startCorner, middleHoleRadius);
		holes.push_back(hole);
	}

	for (int i = 1; i <= 4; i++) {
		auto name = "Bank" + std::to_string(i);
		Bank* bank = new Bank(name, startCorner, bankLength, bankWidth, bankHeight, bankOffset);
		banks.push_back(bank);
	}

	for (int i = 5; i <= 6; i++) {
		auto name = "Bank" + std::to_string(i);
		Bank* bank = new Bank(name, startCorner, bank2Length, bank2Width, bank2Height, bankOffset);
		banks.push_back(bank);
	}

	PoolBall * whiteBall = new PoolBall("whiteBall", centerCoordinates, ballRadius, meshes["Ball"], ballType::white, ballMass);
	PoolBall * blackBall = new PoolBall("blackBall", centerCoordinates, ballRadius, meshes["Ball"], ballType::black, ballMass + 0.01f);
	balls.push_back(whiteBall);
	balls.push_back(blackBall);

	ballMassCreator = 2;
	for (int i = 1; i <= 7; i++) {
		auto name = "RedBall" + std::to_string(i);
		PoolBall * ball = new PoolBall(name, centerCoordinates, ballRadius, meshes["Ball"], ballType::red, ballMass + ballMassCreator * 0.01f);
		ballMassCreator++;
		balls.push_back(ball);
	}
	ballMassCreator++;
	for (int i = 1; i <= 7; i++) {
		auto name = "YellowBall" + std::to_string(i);
		PoolBall * ball = new PoolBall(name, centerCoordinates, ballRadius, meshes["Ball"], ballType::yellow, ballMass + ballMassCreator * 0.01f);
		ballMassCreator++;
		balls.push_back(ball);
	}

	cue = new PoolCue("Cue", centerCoordinates, meshes["Cue"]);

	AddMeshToList(cue->getMesh());
	AddMeshToList(poolTable);
	AddMeshToList(poolCloth);
	AddMeshToList(room);
	AddMeshToList(floor);
	AddMeshToList(ceiling);

	AddMeshToList(poolSide1);
	AddMeshToList(poolSide2);
	AddMeshToList(poolSide3);
	AddMeshToList(poolSide4);
	AddMeshToList(poolSide5);
	AddMeshToList(poolSide6);

	AddMeshToList(cover1);
	AddMeshToList(cover2);
	AddMeshToList(cover3);
	AddMeshToList(cover4);
	AddMeshToList(cover5);
	AddMeshToList(cover6);
	AddMeshToList(cover7);
	AddMeshToList(cover8);
	AddMeshToList(cover9);
	AddMeshToList(cover10);

	AddMeshToList(dot1);
	AddMeshToList(dot2);
	AddMeshToList(dot3);
	AddMeshToList(dot4);
	AddMeshToList(dot5);
	AddMeshToList(dot6);
	AddMeshToList(dot7);
	AddMeshToList(dot8);
	AddMeshToList(dot9);
	AddMeshToList(dot10);
	AddMeshToList(dot11);
	AddMeshToList(dot12);
	AddMeshToList(dot13);
	AddMeshToList(dot14);
	AddMeshToList(dot15);
	AddMeshToList(dot16);
	AddMeshToList(dot17);
	AddMeshToList(dot18);

	for (itH = holes.begin(); itH != holes.end(); itH++) {
		Hole* hole = *itH;
		AddMeshToList(hole->getMesh());
	}

	for (it = banks.begin(); it != banks.end(); it++) {
		Bank* bank = *it;
		AddMeshToList(bank->getMesh());
	}

	for (itB = balls.begin(); itB != balls.end(); itB++) {
		PoolBall * ball = *itB;
		AddMeshToList(ball->getMesh());
	}

	perspective = true;
	ortho = false;
}

void PoolGame::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void PoolGame::Update(float deltaTimeSeconds)
{
	ballsAreStill = true;

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.0f, 0, 2.0f);
	RenderMesh(meshes["Room"], shaders["PoolShader"], modelMatrix, mapTextures["wallpaper"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.0f, 0, 2.0f);
	RenderMesh(meshes["Floor"], shaders["PoolShader"], modelMatrix, mapTextures["carpet"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-2.0f, 0, 2.0f);
	RenderMesh(meshes["Ceiling"], shaders["PoolShader"], modelMatrix, mapTextures["ceiling"]);

	modelMatrix = glm::mat4(1);
	RenderMesh(meshes["PoolTable"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);
	RenderMesh(meshes["PoolCloth"], shaders["PoolShader"], modelMatrix, mapTextures["cloth"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f, tableHeight, offSet);
	RenderMesh(meshes["PoolSide1"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f, tableHeight, offSet);
	RenderMesh(meshes["PoolSide2"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f, tableHeight, -(tableWidth + offSet - sideWidth));
	RenderMesh(meshes["PoolSide3"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f, tableHeight, -(tableWidth + offSet - sideWidth));
	RenderMesh(meshes["PoolSide4"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-offSet + side2Width, tableHeight, tableWidth / 2 + 0.05f);
	modelMatrix *= Transform3D::RotateOY((float)M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["PoolSide5"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + offSet, tableHeight, tableWidth / 2 + 0.05f);
	modelMatrix *= Transform3D::RotateOY((float)M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["PoolSide6"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.3f, tableHeight, offSet);
	RenderMesh(meshes["Cover1"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.3f + coverGirth, tableHeight, (tableWidth / 2) + offSet - coverGirth);
	modelMatrix *= Transform3D::RotateOY((float)M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["Cover2"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength / 2 - 0.1f, tableHeight, offSet);
	RenderMesh(meshes["Cover3"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + 0.05f, tableHeight, offSet);
	RenderMesh(meshes["Cover4"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f, tableHeight, (tableWidth / 2) - tableWidth - offSet);
	modelMatrix *= Transform3D::RotateOY((float)M_PI);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["Cover5"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.3f + coverGirth, tableHeight, (tableWidth / 2) - tableWidth - 0.05f);
	modelMatrix *= Transform3D::RotateOY((float)M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["Cover6"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength / 2 + 0.1f, tableHeight, (tableWidth / 2) - tableWidth - offSet);
	modelMatrix *= Transform3D::RotateOY((float)M_PI);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["Cover7"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + offSet, tableHeight, (tableWidth / 2) - tableWidth - offSet);
	modelMatrix *= Transform3D::RotateOY((float)M_PI);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["Cover8"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + offSet - coverGirth, tableHeight, (tableWidth / 2) + 0.05f);
	modelMatrix *= Transform3D::RotateOY(-(float)M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["Cover9"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + offSet - coverGirth, tableHeight, (tableWidth / 2) - tableWidth - offSet + coverGirth);
	modelMatrix *= Transform3D::RotateOY(-(float)M_PI / 2);
	modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
	RenderMesh(meshes["Cover10"], shaders["PoolShader"], modelMatrix, mapTextures["wood"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f + 0.1f, tableHeight + bankHeight + offsetHoles, offSet - (sideWidth / 2));
	RenderMesh(meshes["Dot1"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f + sideLength / 2, tableHeight + bankHeight + offsetHoles, offSet - (sideWidth / 2));
	RenderMesh(meshes["Dot2"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f + sideLength - 0.1f, tableHeight + bankHeight + offsetHoles, offSet - (sideWidth / 2));
	RenderMesh(meshes["Dot3"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f + 0.1f, tableHeight + bankHeight + offsetHoles, offSet - (sideWidth / 2));
	RenderMesh(meshes["Dot4"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f + sideLength / 2, tableHeight + bankHeight + offsetHoles, offSet - (sideWidth / 2));
	RenderMesh(meshes["Dot5"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f + sideLength - 0.1f, tableHeight + bankHeight + offsetHoles, offSet - (sideWidth / 2));
	RenderMesh(meshes["Dot6"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f + 0.1f, tableHeight + bankHeight + offsetHoles, -tableWidth - offSet + sideWidth / 2);
	RenderMesh(meshes["Dot7"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f + sideLength / 2, tableHeight + bankHeight + offsetHoles, -tableWidth - offSet + sideWidth / 2);
	RenderMesh(meshes["Dot8"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-0.05f + sideLength - 0.1f, tableHeight + bankHeight + offsetHoles, -tableWidth - offSet + sideWidth / 2);
	RenderMesh(meshes["Dot9"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f + 0.1f, tableHeight + bankHeight + offsetHoles, -tableWidth - offSet + sideWidth / 2);
	RenderMesh(meshes["Dot10"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f + sideLength / 2, tableHeight + bankHeight + offsetHoles, -tableWidth - offSet + sideWidth / 2);
	RenderMesh(meshes["Dot11"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(1.52f + sideLength - 0.1f, tableHeight + bankHeight + offsetHoles, -tableWidth - offSet + sideWidth / 2);
	RenderMesh(meshes["Dot12"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-offSet + side2Width/2, tableHeight + bankHeight + offsetHoles, 0.05f - side2Length + 0.1f);
	RenderMesh(meshes["Dot13"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-offSet + side2Width / 2, tableHeight + bankHeight + offsetHoles, 0.05f - side2Length/2);
	RenderMesh(meshes["Dot14"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(-offSet + side2Width / 2, tableHeight + bankHeight + offsetHoles, 0.05f - 0.1f);
	RenderMesh(meshes["Dot15"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + offSet - side2Width/2, tableHeight + bankHeight + offsetHoles, 0.05f - side2Length + 0.1f);
	RenderMesh(meshes["Dot16"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + offSet - side2Width / 2, tableHeight + bankHeight + offsetHoles, 0.05f - side2Length / 2);
	RenderMesh(meshes["Dot17"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	modelMatrix = glm::mat4(1);
	modelMatrix *= Transform3D::Translate(tableLength + offSet - side2Width / 2, tableHeight + bankHeight + offsetHoles, 0.05f - 0.1f);
	RenderMesh(meshes["Dot18"], shaders["PoolShader"], modelMatrix, mapTextures["white"]);

	
	

	for (itB = balls.begin(); itB != balls.end(); itB++) {
		PoolBall * b = *itB;
		if (b->getIsMoving())
			ballsAreStill = false;
	}

	for (it = banks.begin(); it != banks.end(); it++) {
		Bank* bank = *it;
		if (bank->getName() == "Bank1") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-0.05f, tableHeight, offSet - sideWidth);
			bank->setTransformCoords(modelMatrix);
			RenderMesh(meshes[bank->getName()], shaders["PoolShader"], modelMatrix, mapTextures["cloth"]);
		}

		if (bank->getName() == "Bank2") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(1.52f, tableHeight, offSet - sideWidth);
			bank->setTransformCoords(modelMatrix);
			RenderMesh(meshes[bank->getName()], shaders["PoolShader"], modelMatrix, mapTextures["cloth"]);
		}

		if (bank->getName() == "Bank3") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-0.05f + bankLength, tableHeight, tableWidth / 2 - (tableWidth + offSet - sideWidth));
			modelMatrix *= Transform3D::RotateOY((float)M_PI);
			modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
			bank->setTransformCoords(modelMatrix);
			RenderMesh(meshes[bank->getName()], shaders["PoolShader"], modelMatrix, mapTextures["cloth"]);

		}

		if (bank->getName() == "Bank4") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(1.52f + bankLength, tableHeight, tableWidth / 2 - (tableWidth + offSet - sideWidth));
			modelMatrix *= Transform3D::RotateOY((float)M_PI);
			modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
			bank->setTransformCoords(modelMatrix);
			RenderMesh(meshes[bank->getName()], shaders["PoolShader"], modelMatrix, mapTextures["cloth"]);

		}

		if (bank->getName() == "Bank5") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-offSet + side2Width, tableHeight, tableWidth / 2 + 0.05f - bank2Length);
			modelMatrix *= Transform3D::RotateOY(-(float)M_PI / 2);
			modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
			bank->setTransformCoords(modelMatrix);
			RenderMesh(meshes[bank->getName()], shaders["PoolShader"], modelMatrix, mapTextures["cloth"]);


		}

		if (bank->getName() == "Bank6") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(tableLength + offSet - side2Width, tableHeight, tableWidth / 2 + 0.05f);
			modelMatrix *= Transform3D::RotateOY((float)M_PI / 2);
			modelMatrix *= Transform3D::Translate(0, 0, -(tableWidth / 2));
			RenderMesh(meshes[bank->getName()], shaders["PoolShader"], modelMatrix, mapTextures["cloth"]);
			bank->setTransformCoords(modelMatrix);
		}
	}

	for (itH = holes.begin(); itH != holes.end(); itH++) {
		Hole* hole = *itH;

		if (hole->getName() == "Hole1") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-offSet + holeRadius, tableHeight + offsetHoles, offSet - holeRadius);
			hole->setTransformCoords(modelMatrix);
			RenderMesh(meshes[hole->getName()], shaders["PoolShader"], modelMatrix, mapTextures["hole"]);
		}

		if (hole->getName() == "Hole2") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(tableLength + offSet - holeRadius, tableHeight + offsetHoles, holeRadius - tableWidth - offSet);
			hole->setTransformCoords(modelMatrix);
			RenderMesh(meshes[hole->getName()], shaders["PoolShader"], modelMatrix, mapTextures["hole"]);
		}

		if (hole->getName() == "Hole3") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(tableLength + offSet - holeRadius, tableHeight + offsetHoles, offSet - holeRadius);
			hole->setTransformCoords(modelMatrix);
			RenderMesh(meshes[hole->getName()], shaders["PoolShader"], modelMatrix, mapTextures["hole"]);
		}

		if (hole->getName() == "Hole4") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-offSet + holeRadius, tableHeight + offsetHoles, holeRadius - tableWidth - offSet);
			hole->setTransformCoords(modelMatrix);
			RenderMesh(meshes[hole->getName()], shaders["PoolShader"], modelMatrix, mapTextures["hole"]);
		}

		if (hole->getName() == "Hole5") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(tableLength / 2, tableHeight + offsetHoles, offSet - middleHoleRadius);
			hole->setTransformCoords(modelMatrix);
			RenderMesh(meshes[hole->getName()], shaders["PoolShader"], modelMatrix, mapTextures["hole"]);
		}

		if (hole->getName() == "Hole6") {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(tableLength / 2, tableHeight + offsetHoles, middleHoleRadius - tableWidth - offSet);
			hole->setTransformCoords(modelMatrix);
			RenderMesh(meshes[hole->getName()], shaders["PoolShader"], modelMatrix, mapTextures["hole"]);
		}

		for (itB = balls.begin(); itB != balls.end(); itB++) {
			if (ballInPocket((*itB), hole)) {
				if (shotStarted) {
					ballWasPotted = (*itB)->getType();
					if ((*itB)->getType() != engine->getActivePlayer()->getPlayerColor() && engine->getActivePlayer()->getPlayerColor() != ballType::notSet) {
						if ((*itB)->getType() != ballType::white && (*itB)->getType() != ballType::black) {
							myBall = (*itB)->getType();
						}
					}
				}
				if(engine->getFirstBallPotted() == ballType::notSet){
					if ((*itB)->getType() != ballType::white && (*itB)->getType() != ballType::black) {
						engine->setFirstBallPotted((*itB)->getType());
					}
				}
				(*itB)->setOnTable(false);
				(*itB)->setInPocket(true);
				Player * pl = engine->getActivePlayer();

				if ((*itB)->getType() == ballType::red) {
					engine->setRedsPotted(engine->getRedsPotted() + 1);
					if (pl->getPlayerColor() == ballType::red && engine->getFoult() == false && !whiteBallPotted && !blackBallPotted) {
						pl->setScore(pl->getScore() + 1);
						cout << "\n===================================\n" 
							<< engine->player1->getName()
							<< " score is: " << engine->player1->getScore() << ", foults commited: " << engine->player1->getFoults()
							<<  endl << engine->player2->getName()
							<< " score is: " << engine->player2->getScore() << ", foults commited: " << engine->player2->getFoults() 
							<< "\n===================================\n";
						shotStarted = false;
					}
				}
				if ((*itB)->getType() == ballType::yellow) {
					engine->setYellowsPotted(engine->getYellowsPotted() + 1);
					if (pl->getPlayerColor() == ballType::yellow && engine->getFoult() == false && !whiteBallPotted && !blackBallPotted) {
						pl->setScore(pl->getScore() + 1);
						cout << "\n===================================\n"
							<< engine->player1->getName()
							<< " score is: " << engine->player1->getScore() << ", foults commited: " << engine->player1->getFoults()
							<< endl << engine->player2->getName()
							<< " score is: " << engine->player2->getScore() << ", foults commited: " << engine->player2->getFoults()
							<< "\n===================================\n";
						shotStarted = false;
					}
				}
				if ((*itB)->getName() == "whiteBall") {
					whiteBallPotted = true;
					engine->setFirstBallPotted(ballType::notSet);
				}
				if ((*itB)->getName() == "whiteBall" && (*itB)->getMovement() == glm::vec2(0)){
					cout << "\n=========================\n" << pl->getName() << " white ball foult! Ball in hand!\n";
					pl->setFoults(pl->getFoults() + 1);
					engine->setFoult(true);
					engine->switchPlayers();
					cout << "Active player is: " << engine->getActivePlayer()->getName() << endl;
					cout << "=========================\n";
					shotStarted = false;
				}
				if ((*itB)->getName() == "blackBall") {
					blackBallPotted = true;
					Player * pl = engine->getActivePlayer();
					switch (pl->getPlayerColor())
					{
					case ballType::red:
						if (engine->getRedsPotted() != 7) {
							cout << pl->getName() << " black ball foult!\n";
							engine->switchPlayers();
							cout << engine->getActivePlayer()->getName() << " wins!\n";
							shotStarted = false;
						}
						else {
							cout << pl->getName() << " wins the game!!!\n\n";	
						}
						break;
					case ballType::yellow:
						if (engine->getYellowsPotted() != 7) {
							cout << pl->getName() << " black ball foult!\n";
							engine->switchPlayers();
							cout << engine->getActivePlayer()->getName() << " wins!\n";
							shotStarted = false;
						}
						else {
							cout << pl->getName() << " wins the game!!!\n\n";
						}
						break;
					case ballType::notSet:
						cout << pl->getName() << " black ball foult!\n";
						engine->switchPlayers();
						cout << engine->getActivePlayer()->getName() << " wins!\n";
						shotStarted = false;
						break;
					}
					cout << "GAME RESTART\n";
					engine->setReset(true);
					engine->setFirstBallPotted(ballType::notSet);
					shotStarted = false;
					engine->player1->setFirstTouched(ballType::notSet);
					engine->player2->setFirstTouched(ballType::notSet);
					engine->player1->setPlayerColor(ballType::notSet);
					engine->player2->setPlayerColor(ballType::notSet);
				}
			}
		}
	}

	switch (engine->getActivePlayer()->getPlayerColor())
	{
	case ballType::red:
		if (engine->getRedsPotted() == 7)
			engine->getActivePlayer()->setBlackBallPlay(true);
		break;
	case ballType::yellow:
		if (engine->getYellowsPotted() == 7)
			engine->getActivePlayer()->setBlackBallPlay(true);
		break;
	}

	if (engine->getActivePlayer()->getPlayerColor() != ballType::notSet) {
		for (int i = 1; i < balls.size(); i++) {
			if (firstHit(balls.at(0), balls[i], deltaTimeSeconds))
				if (engine->getActivePlayer()->getFirstTouched() == ballType::notSet) {
					engine->getActivePlayer()->setFirstTouched(balls[i]->getType());
				}
		}
		if (ballsAreStill && (engine->getActivePlayer()->getFirstTouched() != engine->getActivePlayer()->getPlayerColor()) 
			&& (engine->getActivePlayer()->getFirstTouched() != ballType::notSet) && !whiteBallPotted && !blackBallPotted) {
			if (!engine->getActivePlayer()->getBlackBallPlay()) {
				engine->getActivePlayer()->setFirstTouched(ballType::notSet);
				cout << "\n=====================================\n" << engine->getActivePlayer()->getName() << " wrong ball hit! FOULT! Ball in hand!\n";
				engine->getActivePlayer()->setFoults(engine->getActivePlayer()->getFoults() + 1);
				engine->setFoult(true);
				engine->switchPlayers();
				cout << "Active player is: " << engine->getActivePlayer()->getName() << endl;
				cout << "=====================================\n";
				shotStarted = false;
			}
			else if (engine->getActivePlayer()->getFirstTouched() != ballType::black) {
				engine->getActivePlayer()->setFirstTouched(ballType::notSet);
				cout << "\n=====================================\n" << engine->getActivePlayer()->getName() << " wrong ball hit! FOULT! Ball in hand!\n";
				engine->getActivePlayer()->setFoults(engine->getActivePlayer()->getFoults() + 1);
				engine->setFoult(true);
				engine->switchPlayers();
				cout << "Active player is: " << engine->getActivePlayer()->getName() << endl;
				cout << "=====================================\n";
				shotStarted = false;
			}
		}
	}
	if (shotStarted && ballsAreStill && engine->getFoult() == false && ballWasPotted == ballType::notSet) {
		cout << "\n=============================\n" << engine->getActivePlayer()->getName() << " didn't pot any balls!\n";
		engine->getActivePlayer()->setFirstTouched(ballType::notSet);
		engine->switchPlayers();
		cout << "Active player is: " << engine->getActivePlayer()->getName() << endl;
		cout << "=============================\n";
		shotStarted = false;
	}

	if (shotStarted && ballsAreStill && engine->getFoult() == false && myBall != ballType::notSet && !whiteBallPotted && !blackBallPotted) {
		cout << "\n===================================\n" << engine->getActivePlayer()->getName() << " didn't pot any of his balls!\n";
		engine->getActivePlayer()->setFirstTouched(ballType::notSet);
		engine->switchPlayers();
		cout << "Active player is: " << engine->getActivePlayer()->getName() << endl;
		cout << "==================================\n";
		shotStarted = false;
		myBall = ballType::notSet;
	}

	if (engine->getFoult() == false && ballsAreStill && engine->getActivePlayer()->getPlayerColor() == ballType::notSet 
		&& engine->getFirstBallPotted() != ballType::notSet && !whiteBallPotted && !blackBallPotted) {
		engine->getActivePlayer()->setPlayerColor(engine->getFirstBallPotted());
		if (engine->getFirstBallPotted() == ballType::red) {
			engine->getInActivePlayer()->setPlayerColor(ballType::yellow);
			cout << "\n===========================\n"
				<< engine->getActivePlayer()->getName()
				<< " plays with reds!"
				<< endl << engine->getInActivePlayer()->getName()
				<< " plays with yellows!"
				<< "\n===========================\n";
			shotStarted = false;
		}
		else {
			engine->getInActivePlayer()->setPlayerColor(ballType::red);
			cout << "\n===========================\n"
				<< engine->getActivePlayer()->getName()
				<< " plays with yellows!"
				<< endl << engine->getInActivePlayer()->getName()
				<< " plays with reds!"
				<< "\n===========================\n";
			shotStarted = false;
		}
		
	}

	for (itB = balls.begin(); itB != balls.end(); itB++) {
		if (engine->getReset() && ballsAreStill) {
			(*itB)->setMovement(0, 0);
			(*itB)->setOnTable(true);
			(*itB)->setIsMoving(false);
			(*itB)->setInPocket(false);
			if (itB == balls.end() - 1) {
				engine->setRedsPotted(0);
				engine->setYellowsPotted(0);
				engine->player1->setScore(0);
				engine->player2->setScore(0);
				engine->player1->setBlackBallPlay(false);
				engine->player2->setBlackBallPlay(false);
				engine->player1->setFoults(0);
				engine->player2->setFoults(0);
				engine->player1->setFirstTouched(ballType::notSet);
				engine->player2->setFirstTouched(ballType::notSet);
				engine->player1->setPlayerColor(ballType::notSet);
				engine->player2->setPlayerColor(ballType::notSet);
				engine->setFirstBallPotted(ballType::notSet);
				engine->setReset(false);
				shotStarted = false;
				camera->Set(glm::vec3(-1.22f, 1.76f, 1.04f), glm::vec3(1.8f, 0, -offSet), glm::vec3(0, 1, 0));
				allowWhiteMoveBeginning = true;
				blackBallPotted = false;
			}
		}
		
	}

	for (int i = 0; i < balls.size(); i++) {
		for (int k = 0; k < balls.size(); k++)
			if (i != k) {
				pushBalls(balls[i], balls[k], deltaTimeSeconds);
			}
				
	}

	for (itB = balls.begin(); itB != balls.end(); itB++) {
		PoolBall * b = *itB;
		if (b->getName() == "whiteBall") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(moveLimitOX - b->getRadius(), tableHeight + b->getRadius(), 0);
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);

			if (cueNearBall) {
				if (b->getIsMoving()) {
					
					for (it = banks.begin(); it != banks.end(); it++) {
						Bank* bankToHit = *it;
						if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
							if (bankToHit->getName() == "Bank1") {
								bankToHit->setTriangleSide(triangleSide::leftTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
								bankToHit->setTriangleSide(triangleSide::rightTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
							}
							else if (bankToHit->getName() == "Bank2") {
								bankToHit->setTriangleSide(triangleSide::leftTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
								bankToHit->setTriangleSide(triangleSide::rightTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
							}

							if (get<0>(collisionDetBanks(b, bankToHit))) {
								switch ( get<1>(collisionDetBanks(b, bankToHit)) ) {
								case bankSide::horizontal:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
									b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								case bankSide::cornerLeft:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
									b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, 
										b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								case bankSide::cornerRight:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
									b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
										b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								}
							}

						} else if(bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

							if (get<0>(collisionDetBanks(b, bankToHit))) {
								switch (get<1>(collisionDetBanks(b, bankToHit))) {
								case bankSide::horizontal:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
									b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								case bankSide::cornerLeft:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
									b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
										b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								case bankSide::cornerRight:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
									b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
										b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								}

							}

							if (bankToHit->getName() == "Bank3") {
								bankToHit->setTriangleSide(triangleSide::leftTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
								bankToHit->setTriangleSide(triangleSide::rightTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
							}
							else if (bankToHit->getName() == "Bank4") {
								bankToHit->setTriangleSide(triangleSide::leftTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
								bankToHit->setTriangleSide(triangleSide::rightTriangle);
								if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
									b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
									b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
										b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
								}
							}
						} else if (bankToHit->getName() == "Bank5") {
							if (get<0>(collisionDetBanks(b, bankToHit))) {
								switch (get<1>(collisionDetBanks(b, bankToHit))) {
								case bankSide::horizontal:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
									b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
									break;
								case bankSide::cornerLeft:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
									b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
										b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								case bankSide::cornerRight:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
									b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
										b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								}
							}
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x, 
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							
						} else if (bankToHit->getName() == "Bank6") {
							if (get<0>(collisionDetBanks(b, bankToHit))) {
								switch (get<1>(collisionDetBanks(b, bankToHit))) {
								case bankSide::horizontal:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
									b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
									break;
								case bankSide::cornerLeft:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
									b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
										b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								case bankSide::cornerRight:
									b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
									b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
										b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
									break;
								}

							}
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

					}
					
					b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds);
					b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);
					b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
					
					if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
						b->setIsMoving(false);
						b->setSpeed(glm::vec3(0));
					}
				}
				else {
					b->setSpeed(glm::vec3(cue->getPower()));
					b->setTrajectory(camera->getForward());
					b->setMovementVector(camera->getForward());
					b->setAcceleration(glm::vec3(ballAcceleration));
				}
			}


			if(b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["whiteBall"], true);
			else {
				b->setIsMoving(false);
				if (ballsAreStill) {
					b->setOnTable(true);
					b->setInPocket(false);
					for (itB2 = balls.begin() + 1; itB2 < balls.end(); itB2++) {
						PoolBall * ball = *itB2;
						glm::vec3 futureCoords = glm::vec3(moveLimitOX - b->getRadius(), tableHeight + b->getRadius(), 0);
						float distance = ballInsideBall(futureCoords, ball);
						b->setMovement(0 - ballInsideBall(futureCoords, ball), 0);
						
					}

	
				}	
			}
		}

		if (b->getName() == "RedBall1") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement, tableHeight + b->getRadius(), 0);
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);

			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));
				
				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}
				

			} else { 
				b->setAcceleration(glm::vec3(ballAcceleration)); 
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["redBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "blackBall") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 4 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), 0);
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {

								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["blackBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "RedBall2") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 2 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), - b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["redBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "YellowBall1") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 2 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["yellowBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "YellowBall2") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 4 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), - 2 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));
				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["yellowBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "RedBall3") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 4 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), 2 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["redBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "YellowBall3") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 6 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), -b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["yellowBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "RedBall4") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 6 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["redBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "YellowBall4") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 6 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), 3 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["yellowBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "RedBall5") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 6 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), - 3 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["redBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "RedBall6") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 8 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), 0);
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["redBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "RedBall7") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 8 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), 4 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["redBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "YellowBall5") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 8 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), 2 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["yellowBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "YellowBall6") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 8 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), -2 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["yellowBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

		if (b->getName() == "YellowBall7") {
			modelMatrix = glm::mat4(1);
			b->setRadius(0.04f);
			modelMatrix *= Transform3D::Translate(b->getMovement().x, 0, b->getMovement().y);
			modelMatrix *= Transform3D::Translate(firstBallPlacement + 8 * (b->getRadius() - 0.0055f), tableHeight + b->getRadius(), - 4 * b->getRadius());
			modelMatrix *= Transform3D::Scale(b->getRadius(), b->getRadius(), b->getRadius());
			b->setTransformCoords(modelMatrix);
			if (b->getIsMoving()) {
				for (it = banks.begin(); it != banks.end(); it++) {
					Bank* bankToHit = *it;
					if (bankToHit->getName() == "Bank1" || bankToHit->getName() == "Bank2") {
						if (bankToHit->getName() == "Bank1") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank2") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, -1)));
								b->setMovement(b->getMovement().x, b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}

					}
					else if (bankToHit->getName() == "Bank3" || bankToHit->getName() == "Bank4") {

						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(0, 0, 1)));
								b->setMovement(b->getMovement().x, b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}

						if (bankToHit->getName() == "Bank3") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
						else if (bankToHit->getName() == "Bank4") {
							bankToHit->setTriangleSide(triangleSide::leftTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
							bankToHit->setTriangleSide(triangleSide::rightTriangle);
							if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
								b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
								b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
									b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
							}
						}
					}
					else if (bankToHit->getName() == "Bank5") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 0)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, -1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(1, 0, 1)));
								b->setMovement(b->getMovement().x - get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}
						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}

					}
					else if (bankToHit->getName() == "Bank6") {
						if (get<0>(collisionDetBanks(b, bankToHit))) {
							switch (get<1>(collisionDetBanks(b, bankToHit))) {
							case bankSide::horizontal:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 0)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x, b->getMovement().y);
								break;
							case bankSide::cornerLeft:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, 1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y - get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							case bankSide::cornerRight:
								b->setMovementVector(glm::reflect(b->getMovementVector(), glm::vec3(-1, 0, -1)));
								b->setMovement(b->getMovement().x + get<2>(collisionDetBanks(b, bankToHit)).x,
									b->getMovement().y + get<2>(collisionDetBanks(b, bankToHit)).y);
								break;
							}

						}
						bankToHit->setTriangleSide(triangleSide::leftTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::leftTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
						bankToHit->setTriangleSide(triangleSide::rightTriangle);
						if (get<0>(collisionDetBankSide(b, bankToHit)) && bankToHit->getTriangleSide() == triangleSide::rightTriangle) {
							b->setMovementVector(glm::reflect(b->getMovementVector(), get<2>(collisionDetBankSide(b, bankToHit))));
							b->setMovement(b->getMovement().x + get<1>(collisionDetBankSide(b, bankToHit)).x,
								b->getMovement().y + get<1>(collisionDetBankSide(b, bankToHit)).y);
						}
					}

				}
				b->setPartialPosition(b->getMovementVector() * b->getSpeed() * deltaTimeSeconds
					+ (deltaTimeSeconds * deltaTimeSeconds * b->getAcceleration()) / glm::vec3(2));

				b->setMovement(b->getMovement().x + b->getPartialPosition().x, b->getMovement().y + b->getPartialPosition().z);
				b->setSpeed(b->getSpeed() + b->getAcceleration() * deltaTimeSeconds);

				if (b->getSpeed().x <= 0 || b->getSpeed().y <= 0 || b->getSpeed().z <= 0) {
					b->setIsMoving(false);
					b->setSpeed(glm::vec3(0));
				}


			}
			else {
				b->setAcceleration(glm::vec3(ballAcceleration));
			}

			if (b->getOnTable())
				RenderMesh(meshes[b->getMesh()->GetMeshID()], shaders["PoolShader"], modelMatrix, mapTextures["yellowBall"], true);
			else {
				b->setIsMoving(false);
			}
		}

	}
	
	if (thirdPerson) {
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y, camera->GetTargetPosition().z);
		modelMatrix *= Transform3D::RotateOY(cue -> getCueRotate());
		modelMatrix *= Transform3D::RotateOZ(RADIANS(80));
		modelMatrix *= Transform3D::Translate(0, balls.at(0)->getRadius() + 0.02f, 0);
		cue->setTransformCoords(modelMatrix);
		isCue = 1;
		RenderMesh(meshes[cue->getName()], shaders["PoolShader"], modelMatrix, mapTextures["woodCue"]);
		isCue = 0;
	}
}

void PoolGame::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), camera->getProjectionMatrix());
}

void PoolGame::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, Texture2D *texture, bool isBall)
{
	if (!mesh || !shader || !shader->program)
		return;
	if (isBall) {
	}
	shader->Use();

	glUniform3f(glGetUniformLocation(shader->program, "light_position"), lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(glGetUniformLocation(shader->program, "light_direction"), lightDirection.x, lightDirection.y, lightDirection.z);
	glm::vec3 eyePosition = camera->getPosition();
	glUniform3f(glGetUniformLocation(shader->program, "eye_position"), eyePosition.x, eyePosition.y, eyePosition.z);
	glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
	glUniform1f(glGetUniformLocation(shader->program, "material_ka"), materialKa);
	glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
	glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
	glUniform1f(glGetUniformLocation(shader->program, "attenuation_factor"), attenuation_factor);
	glUniform1i(glGetUniformLocation(shader->program, "isCue"), isCue);
	glUniform1f(glGetUniformLocation(shader->program, "magnitude"), cue->getPower());

	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	texture->BindToTextureUnit(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void PoolGame::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && moveAllowed && !thirdPerson)
	{
		float cameraSpeed = 1.5f;

		if (window->KeyHold(GLFW_KEY_W)){
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(-deltaTime * cameraSpeed);
		}
	}

	if (allowWhiteMoveBeginning && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		PoolBall * b = balls.at(0);

		if (window->KeyHold(GLFW_KEY_W)) {
			if (b->getTransformedCoords().x < moveLimitOX - b->getRadius())
				b->setMovement(b->getMovement().x + deltaTime, b->getMovement().y);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			if (b->getTransformedCoords().z > playableSurfaceLeftOZ + b->getRadius() + 0.02f)
				b->setMovement(b->getMovement().x, b->getMovement().y - deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			if (b->getTransformedCoords().x > playableSurfaceWhiteSideOX + 0.02f + b->getRadius())
				b->setMovement(b->getMovement().x - deltaTime, b->getMovement().y);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			if (b->getTransformedCoords().z < playableSurfaceRightOZ - b->getRadius() - 0.02f)
				b->setMovement(b->getMovement().x, b->getMovement().y + deltaTime);
		}
	}

	if (engine ->getFoult() && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		PoolBall * b = balls.at(0);

		if (window->KeyHold(GLFW_KEY_A)) {
			if (b->getTransformedCoords().x < playableSurfaceBallsSideOX - b->getRadius() - 0.02f)
				b->setMovement(b->getMovement().x + deltaTime, b->getMovement().y);
			for (itB = balls.begin() + 1; itB < balls.end(); itB++) {
				PoolBall * ball = *itB;
				glm::vec3 futureCoords = balls.at(0)->getTransformedCoords();
				futureCoords.x += deltaTime;
				b->setMovement(b->getMovement().x - ballInsideBall(futureCoords, ball), b->getMovement().y );
			}
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			if (b->getTransformedCoords().z > playableSurfaceLeftOZ + b->getRadius() + 0.02f)
				b->setMovement(b->getMovement().x, b->getMovement().y - deltaTime);
			for (itB = balls.begin() + 1; itB < balls.end(); itB++) {
				PoolBall * ball = *itB;
				glm::vec3 futureCoords = balls.at(0)->getTransformedCoords();
				futureCoords.z -= deltaTime;
				b->setMovement(b->getMovement().x , b->getMovement().y + ballInsideBall(futureCoords, ball));
			}
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			if (b->getTransformedCoords().x > playableSurfaceWhiteSideOX + 0.02f + b->getRadius())
				b->setMovement(b->getMovement().x - deltaTime, b->getMovement().y);
			for (itB = balls.begin() + 1; itB < balls.end(); itB++) {
				PoolBall * ball = *itB;
				glm::vec3 futureCoords = balls.at(0)->getTransformedCoords();
				futureCoords.x -= deltaTime;
				b->setMovement(b->getMovement().x + ballInsideBall(futureCoords, ball), b->getMovement().y );
			}
		}

		if (window->KeyHold(GLFW_KEY_W)) {
			if (b->getTransformedCoords().z < playableSurfaceRightOZ - b->getRadius() - 0.02f)
				b->setMovement(b->getMovement().x, b->getMovement().y + deltaTime);
			for (itB = balls.begin() + 1; itB < balls.end(); itB++) {
				PoolBall * ball = *itB;
				glm::vec3 futureCoords = balls.at(0)->getTransformedCoords();
				futureCoords.z += deltaTime;
				b->setMovement(b->getMovement().x, b->getMovement().y - ballInsideBall(futureCoords, ball));
			}
		}
	}

	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && thirdPerson) {
		if (cue->getPower() < 4) {
			cue->setPower(cue->getPower() + 0.04f);
		}
		else
			cue->setPower(0.2f);
	}
}

void PoolGame::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
	if (key == GLFW_KEY_M)
	{
		moveAllowed = true;
	}
	if (key == GLFW_KEY_SPACE && ballsAreStill)
	{
		engine->setFoult(false);
		cueNearBall = true;
		allowWhiteMoveBeginning = false;
		moveAllowed = false;
		thirdPerson = true;
		glm::vec3 ballCoords = balls.at(0)->getTransformedCoords();
		camera->Set(glm::vec3(ballCoords.x - cos(RADIANS(20)) *0.5f, ballCoords.y + 0.2f, ballCoords.z), ballCoords, glm::vec3(0, 1, 0));
		cue->setCueRotate(0);
		cue->setPower(0.2f);
		ballWasPotted = ballType::notSet;
		myBall = ballType::notSet;
		engine->getActivePlayer()->setFirstTouched(ballType::notSet);
		whiteBallPotted = false;
		blackBallPotted = false;
	}
}

void PoolGame::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void PoolGame::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0 && !thirdPerson && moveAllowed) {
			renderCameraTarget = false;
			camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
		}

		if (thirdPerson) {
			camera->RotateThirdPerson_OY(deltaX * sensivityOX);
			cue->setCueRotate(cue->getCueRotate() + deltaX * sensivityOX);
		}

	}
}

void PoolGame::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && thirdPerson) {
		cue->setPower(0.2f);
	}
	// add mouse button press event
}

void PoolGame::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && thirdPerson) {
		balls.at(0)->setIsMoving(true);
		camera->Set(glm::vec3(tableLength / 2, 3.5f, 0), glm::vec3(tableLength / 2, tableHeight, 0), glm::vec3(0, 0, 1));
		thirdPerson = false;
		shotStarted = true;
	}
}

void PoolGame::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void PoolGame::OnWindowResize(int width, int height)
{
}
