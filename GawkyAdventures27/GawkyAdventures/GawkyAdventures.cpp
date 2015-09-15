

#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Camera.h"
#include "Sky.h"
#include "Player.h"
#include "xnacollision.h"
#include "d3dUtil.h"

#include "TextureMgr.h"
#include "BasicModel.h"
#include "RenderStates.h"
#include "Enemies.h"
#include "TheObjects.h"
#include "LevelBuilder.h"





class Game : public D3DApp
{
public:
	Game(HINSTANCE hInstance);
	~Game();

	bool Init(HINSTANCE hInstance);
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	//void stuf()





	///////////player movement
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);

	void Strafe(float d);
	void Walk( float dt, XMVECTOR direction);



	


	
	////////////////////////////////////////Player functions
	XMFLOAT3  GetPlayerPos();

	void UpdatePlayer(XMFLOAT4X4& m);




	

	///////////////////////////////////////////////////////


	//DeltaTime getter
	float Game::getDeltaTime();
////////////////////

private:
	
	void BuildSkullGeometryBuffers();
	void BuildPlayerGeometryBuffers();




	Sky* mSky;


	////////////////////////////////////////Player
	ID3D11Buffer* mPlayerVB;
	ID3D11Buffer* mPlayerIB;


	XMFLOAT3 mPlayerPosition;
	XMFLOAT3 mPlayerScale;	
	XMFLOAT4 mPlayerRotationQuad;
	XMFLOAT4 mPlayerRotation;



	XMFLOAT3 mOPlayerPosition;
	XMFLOAT3 mOPlayerScale;
	XMFLOAT4 mOPlayerRotationQuad;
	XMFLOAT4 mOPlayerRotation;

	float PlayerRotation;


	XMMATRIX mRotation;
	XMFLOAT4X4 playerRotation;

	int mPlayerVertexOffset;
	UINT mPlayerIndexOffset;
	UINT mPlayerIndexCount;

	//textures
	ID3D11ShaderResourceView* mPlayerMapSRV;
	Material mPlayerMat;


	XMFLOAT4X4 mPlayerTexTransform;
	XMFLOAT4X4 mPlayerWorld;

	XMVECTOR PlayerForward;
	XMVECTOR PlayerRight;
	XMVECTOR PlayerUp;
	

	XMVECTOR currCharDirection;
	XMVECTOR oldCharDirection; 
	XMVECTOR charPosition;
	XMVECTOR moveDirection;


	XMVECTOR tripDirection;
	XMVECTOR tripDistance;



	// Bounding box of the Player
	XNA::AxisAlignedBox mPlayerBox;



	///////////// the Players states
	bool isAlive;
	bool isImmune;
	bool isTripping;

	bool hitFeet;
	bool onGround;
	bool fellOffMap;
	


	//////////////////////////////jumping variables
	bool isJump;
	bool isFalling;
	bool hitHead;


	
	int currentObject;
	FLOAT currGround;
	
	XMVECTOR startJumpPos;
	XMVECTOR Jump;
	



	//////////////////////////////////////////////////////////////////////////////////////////	





	///////////////////////////////////////////



	ID3D11Buffer* mSkullVB;
	ID3D11Buffer* mSkullIB;

	ID3D11Buffer* mSkySphereVB;
	ID3D11Buffer* mSkySphereIB;





	///////lighting
	DirectionalLight mDirLights[4];





	//////////skull
	
	XMFLOAT4X4 mSkullWorld;
	UINT mSkullIndexCount;

	Material mSkullMat;

	XNA::AxisAlignedBox mSkullBox;



	UINT mLightCount;

	Camera mCam;
	


	//mouse
	POINT mLastMousePos;



	/// DeltaTime

	// The DeltaTime's velocity vector

	void addDeltaTime(float dt);
	

	XMFLOAT3 DeltaTime;
	
	float DeltaTimeF;




	
	
	
	
	
	/////// OBJ Model files
	TextureMgr mTexMgr;


	std::vector<BasicModelInstance> mModelInstances;
	std::vector <XNA::AxisAlignedBox> LevelCollisions;


	BasicModel* Ground;
	BasicModel* Platform;
	BasicModel* vineCube;
	BasicModel* sandbox;
	BasicModel* sTree;

	BasicModel* lTree;
	BasicModel* fence;
	BasicModel* fence2;
	BasicModel* cattail;

	BasicModel* treeTrunk;
	BasicModel* treeTop;
	BasicModel* houseBack;
	BasicModel* houseSide;
	BasicModel* houseRoof;
	


	/// ground collisions
	
	XNA::AxisAlignedBox mLModelBox, Platform1Box, Platform2Box, Platform3Box, Platform4Box, Platform5Box, vineCubeBox, sandboxBox
		, sTree1Box, sTree2Box, sTree3Box, lTreeBox, fence1Box, fence2Box, fence3Box, fence4Box, cattailBox1, cattailBox2, cattailBox3, cattailBox4, cattailBox5, treeTrunkBox, treeTopBox
		,houseBackBox, houseSide1Box, houseSide2Box, houseRoofBox;



	
	Enemies* theEnemies;
	TheObjects* Objects;

	LevelBuilder* Level1;



};






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	

	Game theApp(hInstance);

	

	if (!theApp.Init(hInstance))
		return 0;

	return theApp.Run();
}


Game::Game(HINSTANCE hInstance)
	: D3DApp(hInstance), mSky(0),
	 mSkullVB(0), mSkullIB(0),
	
	mSkullIndexCount(0), mLightCount(3), mPlayerVB(0), mPlayerIB(), mPlayerMapSRV(0),
	 mPlayerPosition(0.0f, 2.0f, 0.0f), mPlayerRotationQuad(0.0f, 0.0f, 0.0f, 0.0f),
	mPlayerScale(1.0f, 1.0f, 1.0f), mPlayerRotation(0.0f, 0.0f, 0.0f, 1.0f), DeltaTimeF(0.0f), isJump(0), onGround(false), Ground(0), Platform(0)
	, isFalling(false), hitHead(false), hitFeet(false), currGround(2.0f), fellOffMap(false), mOPlayerPosition(0.0f, 2.0f, 0.0f), mOPlayerRotationQuad(0.0f, 0.0f, 0.0f, 0.0f),
	mOPlayerScale(1.0f, 1.0f, 1.0f), mOPlayerRotation(0.0f, 0.0f, 0.0f, 1.0f), isAlive(true), isImmune(false), isTripping(false) 
	
{
	mMainWndCaption = L"Adventures of Gawky";


	////////////////player movement
	currCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	oldCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	charPosition = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	PlayerForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	PlayerRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	
	////send player information to the camera
	mCam.getPlayerPos(mPlayerPosition);
	mCam.playerInfo(PlayerForward, PlayerRight, PlayerUp);

	
	///initialize player
	XMVECTOR S = XMLoadFloat3(&mPlayerScale);
	XMVECTOR P = XMLoadFloat3(&mPlayerPosition);
	XMVECTOR Q = XMLoadFloat4(&mPlayerRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mPlayerRotation);
	XMStoreFloat4x4(&mPlayerWorld, XMMatrixAffineTransformation(S, rot, Q, P));


	tripDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	


	PlayerRotation = 0.00000001;
	mLastMousePos.x = 0;
	mLastMousePos.y = 0;


	Jump = XMVectorSet(0.0f, 5.0f, 0.0f, 0.0f);

	mCam.SetPosition(0.0f, 2.0f, -20.0f);

	

	XMMATRIX skullScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);	
	XMMATRIX skullRotation = XMMatrixRotationY(0);	
	XMMATRIX skullOffset = XMMatrixTranslation(0.0f, 20.0f, 40.0f);

	

	XMStoreFloat4x4(&mSkullWorld, XMMatrixMultiply(skullScale * skullRotation, skullOffset));

	mDirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	mDirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	mDirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(-0.5f, -1.9f, -1.57735f);



	mPlayerMat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mPlayerMat.Diffuse = XMFLOAT4(0.7f, 0.70f, 0.70f, 1.0f);
	mPlayerMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);
	mPlayerMat.Reflect = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);



	mSkullMat.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mSkullMat.Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mSkullMat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 16.0f);
	mSkullMat.Reflect = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
}

Game::~Game()
{
	SafeDelete(mSky);

	SafeDelete(Platform);

	SafeDelete(Ground);
	SafeDelete(sandbox);
	SafeDelete(sTree);
	SafeDelete(fence);
	SafeDelete(fence2);
	SafeDelete(lTree);
	SafeDelete(vineCube);
	SafeDelete(cattail);
	SafeDelete(treeTrunk);
	SafeDelete(treeTop);
	SafeDelete(houseBack);
	SafeDelete(houseSide);
	SafeDelete(houseRoof);


	ReleaseCOM(mPlayerVB);
	ReleaseCOM(mPlayerIB);
	ReleaseCOM(mPlayerMapSRV);





	ReleaseCOM(mSkullVB);
	ReleaseCOM(mSkullIB);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool Game::Init(HINSTANCE hInstance)
{
	if (!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);

	mTexMgr.Init(md3dDevice);




	mSky = new Sky(md3dDevice, L"Textures//sunsetcube1024.dds", 5000.0f);






	//// load  the level models

	Ground = new BasicModel(md3dDevice, mTexMgr, "Models\\Ground.obj", L"Textures\\");
	Platform = new BasicModel(md3dDevice, mTexMgr, "Models\\Platform2.obj", L"Textures\\");
	vineCube = new BasicModel(md3dDevice, mTexMgr, "Models\\vinebox.obj", L"Textures\\");

	sandbox = new BasicModel(md3dDevice, mTexMgr, "Models\\sandbox.obj", L"Textures\\");
	sTree = new BasicModel(md3dDevice, mTexMgr, "Models\\sTree.obj", L"Textures\\");
	fence = new BasicModel(md3dDevice, mTexMgr, "Models\\Fence.obj", L"Textures\\");
	fence2 = new BasicModel(md3dDevice, mTexMgr, "Models\\Fence2.obj", L"Textures\\");
	cattail = new BasicModel(md3dDevice, mTexMgr, "Models\\cattail.obj", L"Textures\\");

	treeTop = new BasicModel(md3dDevice, mTexMgr, "Models\\treetop.obj", L"Textures\\");
	treeTrunk = new BasicModel(md3dDevice, mTexMgr, "Models\\treetrunk.obj", L"Textures\\");

	houseSide = new BasicModel(md3dDevice, mTexMgr, "Models\\houseside.obj", L"Textures\\");
	houseBack = new BasicModel(md3dDevice, mTexMgr, "Models\\houseback.obj", L"Textures\\");
	houseRoof = new BasicModel(md3dDevice, mTexMgr, "Models\\houseroof.obj", L"Textures\\");

	
	
	theEnemies = new Enemies();
	Objects = new TheObjects();
	Level1 = new LevelBuilder();

	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", -80.0f, 7.0f, 78.0f);
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", 60.0f, 3.0f, 70.0f);
	theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\simpleenemy.obj", L"Textures\\", 0.0f, 3.0f, 25.0f);

	Objects->createObject(md3dDevice, mTexMgr, "Models\\branch.obj", L"Textures\\", 60.0f, 0.25f, 55.0f, 2);
	
	Objects->createObject(md3dDevice, mTexMgr, "Models\\Orange.obj", L"Textures\\", 80.0f, 30.0f, 20.0f, 3);
	Objects->createObject(md3dDevice, mTexMgr, "Models\\Orange.obj", L"Textures\\", 20.0f, 4.0f, 20.0f, 3);
	Objects->createObject(md3dDevice, mTexMgr, "Models\\Orange.obj", L"Textures\\", -80.0f, 10.0f, -60.0f, 3);
	Objects->createObject(md3dDevice, mTexMgr, "Models\\branch.obj", L"Textures\\", 0.0f, 0.25f, 20.0f, 2);

	
	//Level1->createLevelParts(md3dDevice, mTexMgr, "Models\\sandbox.obj", L"Textures\\", 0.0f, 4.0f, 40.0f, 0,6);
	


	theEnemies->CreateBoundingBox();
	Objects->CreateBoundingBox();
	Level1->CreateBoundingBox();
	


	BasicModelInstance theGround, thePlatform1, thePlatform2, thePlatform3, thePlatform4, thePlatform5, theVineCube, thesandbox, thesTree1, 
		thesTree2, thesTree3, thelTree, theFence1, theFence2, theFence3, theFence4, theCattail1, theCattail2, theCattail3, theCattail4, theCattail5, theTreeTrunk, theTreeTop,
		theHouseSide1, theHouseSide2, theHouseBack, theHouseRoof;


	theGround.Model = Ground;
	thePlatform1.Model = Platform;
	thePlatform2.Model = Platform;
	thePlatform3.Model = Platform;
	thePlatform4.Model = Platform;
	thePlatform5.Model = Platform;
	theVineCube.Model = vineCube;
	thesandbox.Model = sandbox;
	thesTree1.Model = sTree;
	thesTree2.Model = sTree;
	thesTree3.Model = sTree;
	theFence1.Model = fence;
	theFence2.Model = fence;
	theFence3.Model = fence2;
	theFence4.Model = fence2;
	theCattail1.Model = cattail;
	theCattail2.Model = cattail;
	theCattail3.Model = cattail;
	theCattail4.Model = cattail;
	theCattail5.Model = cattail;
	theTreeTrunk.Model = treeTrunk;
	theTreeTop.Model = treeTop;

	theHouseSide1.Model = houseSide;
	theHouseSide2.Model = houseSide;
	theHouseBack.Model = houseBack;
	theHouseRoof.Model = houseRoof;

	XMMATRIX modelScale = XMMatrixScaling(7.0f, 7.0f, -7.0f);
	XMMATRIX modelRot = XMMatrixRotationY(1.5707963268f);
	XMMATRIX modelOffset = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	//set the position of the level models

	XMStoreFloat4x4(&theGround.World, modelScale*modelRot*modelOffset);
	
	modelOffset = XMMatrixTranslation(-10.8317f * 7, 0.32939f * 7, 3.88198f * 7);

	XMStoreFloat4x4(&thePlatform1.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(-10.8317f * 7, 1.3f * 7, 7.26075f * 7);
	XMStoreFloat4x4(&thePlatform2.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(-10.8317f * 7, 0.32939f * 7, 10.63794f * 7);
	XMStoreFloat4x4(&thePlatform3.World, modelScale*modelRot*modelOffset);


	modelOffset = XMMatrixTranslation(10.8317f * 7, 0.32939f * 7, 10.8317f * 7);
	XMStoreFloat4x4(&thePlatform4.World, modelScale*modelRot*modelOffset);
	
	modelOffset = XMMatrixTranslation(10.8317f * 7, 1.3f * 7, 7.26075f * 7);
	XMStoreFloat4x4(&thePlatform5.World, modelScale*modelRot*modelOffset);

	modelRot = XMMatrixRotationY(3.1415926536f);
	modelOffset = XMMatrixTranslation(10.8317f * 7, 2.0f * 7, 2.26075f * 7);
	XMStoreFloat4x4(&theVineCube.World, modelScale*modelRot*modelOffset);
	
	modelRot = XMMatrixRotationY(1.5707963268f);

	

	modelOffset = XMMatrixTranslation(-8.0f * 7, 2.2f * 7, 12.4f * 7);
	XMStoreFloat4x4(&thesTree1.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(-8.0f * 7, 2.2f * 7, 10.4f * 7);
	XMStoreFloat4x4(&thesTree2.World, modelScale*modelRot*modelOffset);


	modelOffset = XMMatrixTranslation(-8.0f * 7, 2.2f * 7, 8.4f * 7);
	XMStoreFloat4x4(&thesTree3.World, modelScale*modelRot*modelOffset);

	
	modelRot = XMMatrixRotationY(0);
	modelOffset = XMMatrixTranslation(13.0f * 7, 1.0f * 7, 0.0f * 7);
	XMStoreFloat4x4(&theFence1.World, modelScale*modelRot*modelOffset);


	modelOffset = XMMatrixTranslation(-13.0f * 7, 1.0f * 7, 0.0f * 7);
	XMStoreFloat4x4(&theFence2.World, modelScale*modelRot*modelOffset);

	
	modelOffset = XMMatrixTranslation(0.0f * 7, 1.0f * 7, 13.0f * 7);
	XMStoreFloat4x4(&theFence3.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(0.0f * 7, 1.0f * 7, -13.0f * 7);
	XMStoreFloat4x4(&theFence4.World, modelScale*modelRot*modelOffset);

	modelRot = XMMatrixRotationY(1.5707963268f);

	modelOffset = XMMatrixTranslation(+12.0f * 7, 0.8f * 7, -6.8f * 7);
	XMStoreFloat4x4(&theCattail1.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(11.0f * 7, 0.8f * 7, -6.8f * 7);
	XMStoreFloat4x4(&theCattail2.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(9.75f * 7, 0.8f * 7, -6.8f * 7);
	XMStoreFloat4x4(&theCattail3.World, modelScale*modelRot*modelOffset);



	modelOffset = XMMatrixTranslation(8.5f * 7, 0.8f * 7, -6.8f * 7);
	XMStoreFloat4x4(&theCattail4.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(7.25f * 7, 0.8f * 7, -6.8f * 7);
	XMStoreFloat4x4(&theCattail5.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(0.0f * 7, 2.0f * 7, 6.8f * 7);
	XMStoreFloat4x4(&theTreeTrunk.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(0.0f * 7, 8.8f * 7, 6.8f * 7);
	XMStoreFloat4x4(&theTreeTop.World, modelScale*modelRot*modelOffset);


	modelRot = XMMatrixRotationY(3.1415926536);
	modelOffset = XMMatrixTranslation(6.2f * 7, 2.0f * 7, -10.0f * 7);
	XMStoreFloat4x4(&theHouseSide1.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(1.0f * 7, 2.0f * 7, -10.0f * 7);
	XMStoreFloat4x4(&theHouseSide2.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(3.5f * 7, 2.0f * 7, -11.80f * 7);
	XMStoreFloat4x4(&theHouseBack.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(3.5f * 7, 4.7f * 7, -11.5f * 7);
	XMStoreFloat4x4(&theHouseRoof.World, modelScale*modelRot*modelOffset);

	modelOffset = XMMatrixTranslation(-8.699f * 7, 0.2f * 7, -9.72867f * 7);
	XMStoreFloat4x4(&thesandbox.World, modelScale*modelRot*modelOffset);


	

	//drawn object
	mModelInstances.push_back(theGround);
	mModelInstances.push_back(thePlatform1);
	mModelInstances.push_back(thePlatform2);
	mModelInstances.push_back(thePlatform3);
	mModelInstances.push_back(thePlatform4);
	mModelInstances.push_back(thePlatform5);
	mModelInstances.push_back(theVineCube);
	mModelInstances.push_back(thesandbox);
	mModelInstances.push_back(thesTree1);
	mModelInstances.push_back(thesTree2);
	mModelInstances.push_back(thesTree3);
	mModelInstances.push_back(theFence1);
	mModelInstances.push_back(theFence2);
	mModelInstances.push_back(theFence3);
	mModelInstances.push_back(theFence4);
	mModelInstances.push_back(theCattail1);
	mModelInstances.push_back(theCattail2);
	mModelInstances.push_back(theCattail3);
	mModelInstances.push_back(theCattail4);
	mModelInstances.push_back(theCattail5);
	mModelInstances.push_back(theTreeTrunk);
	mModelInstances.push_back(theTreeTop);
	mModelInstances.push_back(theHouseSide1);
	mModelInstances.push_back(theHouseSide2);
	mModelInstances.push_back(theHouseBack);
	mModelInstances.push_back(theHouseRoof);
	


	///hitbox
	LevelCollisions.push_back(mLModelBox);
	LevelCollisions.push_back(Platform1Box);
	LevelCollisions.push_back(Platform2Box);
	LevelCollisions.push_back(Platform3Box);
	LevelCollisions.push_back(Platform4Box);
	LevelCollisions.push_back(Platform5Box);
	LevelCollisions.push_back(vineCubeBox);
	LevelCollisions.push_back(sandboxBox);
	LevelCollisions.push_back(sTree1Box);
	LevelCollisions.push_back(sTree2Box);
	LevelCollisions.push_back(sTree3Box);
	LevelCollisions.push_back(fence1Box);
	LevelCollisions.push_back(fence2Box);
	LevelCollisions.push_back(fence3Box);
	LevelCollisions.push_back(fence4Box);
	LevelCollisions.push_back(cattailBox1);
	LevelCollisions.push_back(cattailBox2);
	LevelCollisions.push_back(cattailBox3);
	LevelCollisions.push_back(cattailBox4);
	LevelCollisions.push_back(cattailBox5);
	LevelCollisions.push_back(treeTrunkBox);
	LevelCollisions.push_back(treeTopBox);
	LevelCollisions.push_back(houseSide1Box);
	LevelCollisions.push_back(houseSide2Box);
	LevelCollisions.push_back(houseBackBox);
	LevelCollisions.push_back(houseRoofBox);




	//
	// Compute scene bounding box.
	//
	XMFLOAT3 minPt(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 maxPt(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	for (UINT i = 0; i < mModelInstances.size(); ++i)
	{
		minPt.x = 0.0f;
		minPt.y = 0.0f;
		minPt.z = 0.0f;

		maxPt.x = 0.0f;
		maxPt.y = 0.0f;
		maxPt.z = 0.0f;

		for (UINT j = 0; j < mModelInstances[i].Model->BasicVertices.size(); ++j)
		{

			XMFLOAT3 P = mModelInstances[i].Model->BasicVertices[j].Pos;


			//////multiply all these by 7

			minPt.x = MathHelper::Min(minPt.x, P.x);
			minPt.y = MathHelper::Min(minPt.y, P.y);
			minPt.z = MathHelper::Min(minPt.z, P.z);

			maxPt.x = MathHelper::Max(maxPt.x, P.x);
			maxPt.y = MathHelper::Max(maxPt.y, P.y);
			maxPt.z = MathHelper::Max(maxPt.z, P.z);

		
		}

		XMMATRIX temp =	XMLoadFloat4x4(&mModelInstances[i].World);

		XMVECTOR Scale;
		XMVECTOR Position;
		XMVECTOR Rotation;


		XMMatrixDecompose(&Scale, &Rotation, &Position, temp);

		XMFLOAT3 tempPos;
		XMStoreFloat3(&tempPos, Position);

		LevelCollisions[i].Center = tempPos;


		LevelCollisions[i].Extents = XMFLOAT3(0.5f*(maxPt.x - minPt.x),
			0.5f*(maxPt.y - minPt.y),
			0.5f*(maxPt.z - minPt.z));

		LevelCollisions[i].Extents.x = LevelCollisions[i].Extents.x * 7;
		LevelCollisions[i].Extents.y = LevelCollisions[i].Extents.y * 7;
		LevelCollisions[i].Extents.z = LevelCollisions[i].Extents.z * 7;
		LevelCollisions[i].collisionType = 0;


	}


	std::vector <XNA::AxisAlignedBox> temp;
	temp = Objects->getObjectCollisions();
	


	for (UINT i = 0; i < temp.size(); i++)
	{

		LevelCollisions.push_back(temp[i]);


	}

	
	temp = theEnemies->getEnemyCollisions();
	for (UINT i = 0; i < temp.size(); i++)
	{

		LevelCollisions.push_back(temp[i]);


	}


	////////////


	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice,
		L"Textures/WoodCrate02.dds", 0, 0, &mPlayerMapSRV, 0));


	BuildSkullGeometryBuffers();
	BuildPlayerGeometryBuffers();


	return true;
}

void Game::OnResize()
{
	D3DApp::OnResize();

	mCam.SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}

void Game::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Silver));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	md3dImmediateContext->IASetInputLayout(InputLayouts::Basic32);
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;





	mCam.UpdateViewMatrix();

	XMMATRIX view = mCam.View();
	XMMATRIX proj = mCam.Proj();
	XMMATRIX viewProj = mCam.ViewProj();

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);


	Effects::BasicFX->SetEyePosW(mCam.GetPosition());
	Effects::BasicFX->SetCubeMap(mSky->CubeMapSRV());



	// Figure out which technique to use.  Skull does not have texture coordinates,
	// so we need a separate technique for it, and not every surface is reflective,
	// so don't pay for cubemap look up.

	ID3DX11EffectTechnique* activeTexTech = Effects::BasicFX->Light1TexTech;
	ID3DX11EffectTechnique* activeReflectTech = Effects::BasicFX->Light1TexReflectTech;
	ID3DX11EffectTechnique* activeSkullTech = Effects::BasicFX->Light1ReflectTech;
	switch (mLightCount)
	{
	case 1:
		activeTexTech = Effects::BasicFX->Light1TexTech;
		activeReflectTech = Effects::BasicFX->Light1TexReflectTech;
		activeSkullTech = Effects::BasicFX->Light1ReflectTech;
		break;
	case 2:
		activeTexTech = Effects::BasicFX->Light2TexTech;
		activeReflectTech = Effects::BasicFX->Light2TexReflectTech;
		activeSkullTech = Effects::BasicFX->Light2ReflectTech;
		break;
	case 3:
		activeTexTech = Effects::BasicFX->Light3TexTech;
		activeReflectTech = Effects::BasicFX->Light3TexReflectTech;
		activeSkullTech = Effects::BasicFX->Light3ReflectTech;
		break;
	}

	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;



	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX toTexSpace(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);



	//
	// Draw the grid, cylinders, and box without any cubemap reflection.
	// 
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTexTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{

		

		//draw the enemies
		theEnemies->draw(md3dImmediateContext, mCam, activeTexTech);

		//draw the objects
		Objects->draw(md3dImmediateContext, mCam, activeTexTech);
		//draw Level
		Level1->draw(md3dImmediateContext, mCam, activeTexTech);




		for (UINT modelIndex = 0; modelIndex < mModelInstances.size(); ++modelIndex)
		{
			world = XMLoadFloat4x4(&mModelInstances[modelIndex].World);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
		
			
			
			
			//I do not have shadoews
			//Effects::BasicFX->SetShadowTransform(world*shadowTransform);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

			for (UINT subset = 0; subset < mModelInstances[modelIndex].Model->SubsetCount; ++subset)
			{
				Effects::BasicFX->SetMaterial(mModelInstances[modelIndex].Model->Mat[subset]);
				Effects::BasicFX->SetDiffuseMap(mModelInstances[modelIndex].Model->DiffuseMapSRV[subset]);
				//Effects::BasicFX->SetNormalMap(mModelInstances[modelIndex].Model->NormalMapSRV[subset]);

				activeTexTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
				mModelInstances[modelIndex].Model->ModelMesh.Draw(md3dImmediateContext, subset);
			}
		}	






		//draw player
		//mPlayer->drawPlayer(md3dImmediateContext, mCam);



		UINT stride = sizeof(Vertex::Basic32);
		UINT offset = 0;

		





		ID3DX11EffectTechnique* activeTech = Effects::BasicFX->Light3TexTech;

		D3DX11_TECHNIQUE_DESC techDesc;
		activeTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			md3dImmediateContext->IASetVertexBuffers(0, 1, &mPlayerVB, &stride, &offset);
			md3dImmediateContext->IASetIndexBuffer(mPlayerIB, DXGI_FORMAT_R32_UINT, 0);

			// Draw the Player
			XMMATRIX world = XMLoadFloat4x4(&mPlayerWorld);
			XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
			XMMATRIX worldViewProj = world*view*proj;

			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMLoadFloat4x4(&mPlayerTexTransform));
			Effects::BasicFX->SetMaterial(mPlayerMat);
			Effects::BasicFX->SetDiffuseMap(mPlayerMapSRV);

			activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
			md3dImmediateContext->DrawIndexed(mPlayerIndexCount, mPlayerIndexOffset, mPlayerVertexOffset);
		}


	

		



	}

	//draw Duck Model
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		// Draw the skull.

		md3dImmediateContext->IASetVertexBuffers(0, 1, &mSkullVB, &stride, &offset);
		md3dImmediateContext->IASetIndexBuffer(mSkullIB, DXGI_FORMAT_R32_UINT, 0);

		world = XMLoadFloat4x4(&mSkullWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);
		Effects::BasicFX->SetMaterial(mSkullMat);

		activeSkullTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSkullIndexCount, 0, 0);
	}


	mSky->Draw(md3dImmediateContext, mCam);




	// restore default states, as the SkyFX changes them in the effect file.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);

	HR(mSwapChain->Present(0, 0));
}




///the mouse functions are not being used
void Game::OnMouseDown(WPARAM btnState, int x, int y)
{

	//theEnemies->createEnemy(md3dDevice, mTexMgr, "Models\\Hat.obj", L"Textures\\", x, 4.0f, y);

	SetCapture(mhMainWnd);
}

void Game::OnMouseUp(WPARAM btnState, int x, int y)
{
		ReleaseCapture();
}

void Game::OnMouseMove(WPARAM btnState, int x, int y)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////



void Game::BuildSkullGeometryBuffers()
{
	std::ifstream fin("Models/skull.txt");

	if (!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	XMFLOAT3 vMin(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMax(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	//XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	//XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
	std::vector<Vertex::Basic32> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;

		XMFLOAT3 P = vertices[i].Pos;

		vMin.x = MathHelper::Min(vMin.x, P.x);
		vMin.y = MathHelper::Min(vMin.y, P.y);
		vMin.z = MathHelper::Min(vMin.z, P.z);

		vMax.x = MathHelper::Max(vMax.x, P.x);
		vMax.y = MathHelper::Max(vMax.y, P.y);
		vMax.z = MathHelper::Max(vMax.z, P.z);
	}

	mSkullBox.Center = XMFLOAT3(0.5f*(vMin.x + vMax.x),
		0.5f*(vMin.y + vMax.y),
		0.5f*(vMin.z + vMax.z));

	mSkullBox.Extents = XMFLOAT3(0.5f * (vMax.x - vMin.x),
		0.5f*(vMax.y -vMin.y),
		0.5f*(vMax.z - vMin.z));


	mSkullBox.Center = XMFLOAT3(0.0f, 1.5f, 40.0f);

	
	



	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	mSkullIndexCount = 3 * tcount;
	std::vector<UINT> indices(mSkullIndexCount);
	for (UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mSkullVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * mSkullIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mSkullIB));
}


void Game::BuildPlayerGeometryBuffers()

{



	GeometryGenerator::MeshData thePlayer;

	GeometryGenerator geoGen;
	geoGen.CreateBox(4.0f, 4.0f, 4.0f, thePlayer);
	


	///////////////bounding 
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);




	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mPlayerVertexOffset = 0;

	// Cache the index count of each object.
	mPlayerIndexCount = thePlayer.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mPlayerIndexOffset = 0;

	UINT totalVertexCount = thePlayer.Vertices.size();

	UINT totalIndexCount = mPlayerIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::Basic32> vertices(totalVertexCount);

	UINT k = 0;
	for (size_t i = 0; i < thePlayer.Vertices.size(); ++i, ++k)
	{
		
		vertices[i].Pos = thePlayer.Vertices[i].Position;
		vertices[i].Normal = thePlayer.Vertices[i].Normal;
		vertices[i].Tex = thePlayer.Vertices[i].TexC;
		XMVECTOR P = XMLoadFloat3(&vertices[i].Pos);


		

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}


	XMStoreFloat3(&mPlayerBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mPlayerBox.Extents, 0.5f*(vMax - vMin));

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mPlayerVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), thePlayer.Indices.begin(), thePlayer.Indices.end());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mPlayerIB));




}


///////////////////player movement


void Game::Walk(float dt, XMVECTOR direction)
{
	
	int collobject = 0;
	int collEnemy = 0;
	


	
XMMATRIX worldMatrix = XMLoadFloat4x4(&mPlayerWorld);	
	XMVECTOR r = XMLoadFloat3(&mPlayerPosition);
	FLOAT tempY = XMVectorGetY(direction);



	/// if the character is tripping, push him in the direction he was going until he goes 10 spaces
	if (isTripping == true && isImmune == false)
	{
		startJumpPos = r;
		isImmune = true;
		tripDirection = direction;
		tripDistance = direction;

	}
	else if (isTripping == true && isImmune == true)
	{

		direction = tripDirection;
		tripDistance = XMVectorAdd(tripDistance, tripDirection);

		//XMVector4Normalize(tripDistance);
		FLOAT tempZ = XMVectorGetZ(tripDistance);
		FLOAT tempX = XMVectorGetZ(tripDistance);

		if (tempZ < 0)
		{

			tempZ = tempZ * -1;
		}

		if (tempX < 0)
		{

			tempX = tempX * -1;
		}

		FLOAT totalDistance = tempZ + tempY;

		

		if (totalDistance > 15.0f || totalDistance < -15.0f)
		{

			isImmune = false;
			isTripping = false;
		}

		








	}




	///if the player is on the ground, they are allowed to jump,  if not they cannot, no flying allowed
	if (onGround == true && tempY > 0.5f)
	{
		//Don't want to normalize the jump
		onGround = false;			
		startJumpPos = r;
		isJump = true;
		hitFeet = false;

	}



	if (onGround == false && isJump == true)
	{

		FLOAT startPos = XMVectorGetY(startJumpPos);
		FLOAT currPos = XMVectorGetY(r);

		tempY = 0.1f;
		
		if (currPos > startPos + 10.0f)
		{

			isJump = false;
			isFalling = true;


		}
		else if (hitHead == true)
		{
			isFalling = true;
			isJump = false;
			

		}



	}
	
	
	// Normalize our destinated direction vector
	direction = XMVector3Normalize(direction);

	direction = XMVectorSetY(direction, 0.0f);
	

	///// the direction the player is going to move
	moveDirection = direction;
	/////character spinning make it more smooth
	if (XMVectorGetX(XMVector3Dot(direction, oldCharDirection)) == -1)
	{
		oldCharDirection += XMVectorSet(0.01f, 0.0f, 0.0f, 0.0f);
	}
	
	///////get characters position in world space
	charPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);	
	charPosition = XMVector3TransformCoord(charPosition,worldMatrix);
	
	
	
	///// rotate the character
	float destDirLength = 10.0f * dt;
	

	
		currCharDirection = (oldCharDirection)+(direction * destDirLength);
		currCharDirection = XMVector3Normalize(currCharDirection);


	// get the angle 
	float charDirAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMVector3Normalize(currCharDirection), XMVector3Normalize(PlayerForward)));
	
	
	if (XMVectorGetY(XMVector3Cross(currCharDirection, PlayerForward)) > 0.0f)
	{
		charDirAngle = -charDirAngle;
	}
	
	
	
	float speed = 30.0f * dt;
	XMVECTOR tY = XMVectorSet(0.0f, tempY, 0.0f, 0.0f);

	direction = XMVectorSetY(direction,tempY);
	direction = direction * speed;
	charPosition = charPosition + direction;

	charPosition += tY;

	

	XMMATRIX rotationMatrix;

	XMMATRIX previousWorld = worldMatrix;

	

	XMMATRIX Translation = XMMatrixTranslation(XMVectorGetX(charPosition), XMVectorGetY(charPosition), XMVectorGetZ(charPosition));
	rotationMatrix = XMMatrixRotationY(charDirAngle - 3.14159265f);		// Subtract PI from angle so the character doesn't run backwards

	
	
	worldMatrix =  rotationMatrix * Translation;



	/////////////////////////////////////////////////////////////  v collision system

	for (UINT i = 1; i < LevelCollisions.size(); ++i)
	{


	


		XMStoreFloat3(&mPlayerBox.Center, charPosition);
		

		// collisions with things in the world
		XMVECTOR diff;



		mSkullBox;

		mPlayerBox;//1

		LevelCollisions[i].Center;
		LevelCollisions[i].Extents;

		int it = 0;


		FLOAT tRight = 0;
		FLOAT tUp = 0;
		FLOAT tForward = 0;
		bool toRight = false;
		bool Above = false;
		bool inFront = false;


		///values just to check values

		float LX = LevelCollisions[i].Center.x;
		float LY = LevelCollisions[i].Center.y;
		float LZ = LevelCollisions[i].Center.z;

		float LXE = LevelCollisions[i].Extents.x;
		float LYE = LevelCollisions[i].Extents.y;
		float LZE = LevelCollisions[i].Extents.z;



		/////////////fooling around with some collision idea's

		//Player is to the left 
		if (mPlayerBox.Center.x <= LevelCollisions[i].Center.x)
		{
			tRight = (LevelCollisions[i].Center.x - LevelCollisions[i].Extents.x) - (mPlayerBox.Center.x + mPlayerBox.Extents.x);


		}
		//player is to the right of the object
		if (mPlayerBox.Center.x >= LevelCollisions[i].Center.x)
		{

			tRight = (mPlayerBox.Center.x - mPlayerBox.Extents.x) - (LevelCollisions[i].Center.x + LevelCollisions[i].Extents.x);
			toRight = true;
		}





		///player is behind object
		if (mPlayerBox.Center.z <= LevelCollisions[i].Center.z)
		{
			tForward = (LevelCollisions[i].Center.z - LevelCollisions[i].Extents.z) - (mPlayerBox.Center.z + mPlayerBox.Extents.z);
		}
		//player is infront of object
		if (mPlayerBox.Center.z >= LevelCollisions[i].Center.z)
		{
			tForward = (mPlayerBox.Center.z - mPlayerBox.Extents.z) - (LevelCollisions[i].Center.z + LevelCollisions[i].Extents.z);
			inFront = true;
		}


		///player is below the object
		if (mPlayerBox.Center.y <= LevelCollisions[i].Center.y)
		{
			tUp = ((LevelCollisions[i].Center.y - LevelCollisions[i].Extents.y) - (mPlayerBox.Center.y + mPlayerBox.Extents.y)); //+ LevelCollisions[i].Extents.y);
		}
		//player is above the object
		if (mPlayerBox.Center.y >= LevelCollisions[i].Center.y)
		{
			tUp = ((mPlayerBox.Center.y - mPlayerBox.Extents.y) - (LevelCollisions[i].Center.y + LevelCollisions[i].Extents.y)); //- LevelCollisions[i].Extents.y);
			Above = true;

		}



		FLOAT oldPos = XMVectorGetY(oldCharDirection);
		FLOAT curPos = XMVectorGetY(currCharDirection);
		
		
		
		
		
		
		if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f && LevelCollisions[i].collisionType == 2)
		{

			
			tripDirection = direction;
			isTripping = true;
		
			break;
		}
		else if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f && LevelCollisions[i].collisionType == 3)
		{

			LevelCollisions.erase(LevelCollisions.begin() + i);
			Objects->RemovemObjectInstance(collobject);
		
			break;

		}
		else if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f && LevelCollisions[i].collisionType == 1 && isImmune == true)
		{

			LevelCollisions.erase(LevelCollisions.begin() + i);
			theEnemies->RemovemObjectInstance(collEnemy);


			break;
		}
		

		else if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f && isJump == true && LevelCollisions[i].collisionType == 0)
		{
			int t = 3;

			// these tell me where the player is verse the object it collided with
			inFront;
			Above;
			toRight;

			
			XMVECTOR S;
			XMVECTOR P;
			XMVECTOR Q;

			hitHead = true;


			XMMatrixDecompose(&S, &Q, &P, previousWorld);
			
			XMStoreFloat3(&mPlayerScale, S);
			XMStoreFloat3(&mPlayerPosition, P);
			XMStoreFloat4(&mPlayerRotationQuad, Q);

			break;



		}

		///if player lands on an object for the first time
		else if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f && Above == true && hitFeet == false && LevelCollisions[i].collisionType == 0)
		{
			int t = 3;

			hitFeet = true;
			//P = XMVectorSubtract(P, moveDirection);
			inFront;
			Above;
			toRight;

			
			XMVECTOR S;
			XMVECTOR P;
			XMVECTOR Q;

			currentObject = i;



			XMMatrixDecompose(&S, &Q, &P, previousWorld);

			FLOAT tempDirection = XMVectorGetY(direction);
			XMVECTOR tempDir = XMVectorSet(0.0f, tempDirection - 0.01f, 0.0f, 0.0f);

			P -= tempDir;
			currGround = XMVectorGetY(P);
			currGround += 0.01f;
			XMStoreFloat3(&mPlayerScale, S);
			XMStoreFloat3(&mPlayerPosition, P);
			XMStoreFloat4(&mPlayerRotationQuad, Q);

			break;

		}
		else if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f && Above == true && LevelCollisions[i].collisionType == 0)
		{
			int t = 3;

			//hitFeet = true;
			inFront;
			Above;
			toRight;


			XMVECTOR S;
			XMVECTOR P;
			XMVECTOR Q;



			XMMatrixDecompose(&S, &Q, &P, previousWorld);

			FLOAT tempDirection = XMVectorGetY(direction);
			XMVECTOR tempDir = XMVectorSet(0.0f, tempDirection - 0.01f, 0.0f, 0.0f);

			P -= tempDir;
		
			XMStoreFloat3(&mPlayerScale, S);
			XMStoreFloat3(&mPlayerPosition, P);
			XMStoreFloat4(&mPlayerRotationQuad, Q);

			break;



		}

		else if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f  && LevelCollisions[i].collisionType == 0)
		{
			int t = 3;
			
			
			inFront;
			Above;
			toRight;

			oldCharDirection = currCharDirection;
			XMVECTOR S;
			XMVECTOR P;
			XMVECTOR Q;



			XMMatrixDecompose(&S, &Q, &P, previousWorld);
			
		

			XMStoreFloat3(&mPlayerScale, S);
			XMStoreFloat3(&mPlayerPosition, P);
			XMStoreFloat4(&mPlayerRotationQuad, Q);
			
			break;



		}
		else if (tRight <= 0.0f && tUp <= 0.0f && tForward <= 0.0f  && LevelCollisions[i].collisionType == 1 && isImmune == false)
		{


		isAlive = false;

		break;


		}
		else if (isAlive == true)
		{
			// Set the characters old direction


			

			XMVECTOR S;
			XMVECTOR P;
			XMVECTOR Q;



			XMMatrixDecompose(&S, &Q, &P, worldMatrix);
		
			XMStoreFloat3(&mPlayerScale, S);
			XMStoreFloat3(&mPlayerPosition, P);
			XMStoreFloat4(&mPlayerRotationQuad, Q);

		}



		oldCharDirection = currCharDirection;

		if (LevelCollisions[i].collisionType == 2 || LevelCollisions[i].collisionType == 3)
		{


			collobject++;
		}

		if (LevelCollisions[i].collisionType == 1 || LevelCollisions[i].collisionType == 1)
		{


			collEnemy++;
		}

	}
	////////////////////////////////////////////////////////////// ^ collisions

//	for (UINT i = 0; i < 2; ++i)
//	{


	//	if (i == 1)
	//	{
	///		currentObject = 0;

	//	}

		FLOAT tRight = 0;
		FLOAT tUp = 0;
		FLOAT tForward = 0;
		bool toRight = false;
		bool Above = false;
		bool inFront = false;


		//Player is to the left 
		if (mPlayerBox.Center.x <= LevelCollisions[currentObject].Center.x)
		{
			tRight = (LevelCollisions[currentObject].Center.x - LevelCollisions[currentObject].Extents.x) - (mPlayerBox.Center.x + mPlayerBox.Extents.x);
		}
		//player is to the right of the object
		if (mPlayerBox.Center.x >= LevelCollisions[currentObject].Center.x)
		{

			tRight = (mPlayerBox.Center.x - mPlayerBox.Extents.x) - (LevelCollisions[currentObject].Center.x + LevelCollisions[currentObject].Extents.x);
			toRight = true;
		}

		///player is behind object
		if (mPlayerBox.Center.z <= LevelCollisions[currentObject].Center.z)
		{
			tForward = (LevelCollisions[currentObject].Center.z - LevelCollisions[currentObject].Extents.z) - (mPlayerBox.Center.z + mPlayerBox.Extents.z);
		}
		//player is infront of object
		if (mPlayerBox.Center.z >= LevelCollisions[currentObject].Center.z)
		{
			tForward = (mPlayerBox.Center.z - mPlayerBox.Extents.z) - (LevelCollisions[currentObject].Center.z + LevelCollisions[currentObject].Extents.z);
			inFront = true;
		}

		if (tRight > 0.0f || tForward > 0.0f)
		{
			isFalling = true;
			hitFeet = false;
			onGround = false;
			if (currentObject == 0)
			{
				fellOffMap = true;
			}

		}


//	}


	



}










//////////////////////////////////////////////////////updates

void Game::UpdateScene(float dt)
{
	
	addDeltaTime(dt);
	moveDirection = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	theEnemies->update(dt);

	

	//////updates the enemy collisions as they move

	std::vector <XNA::AxisAlignedBox> temp;
	temp = theEnemies->getEnemyCollisions();
	
	
	int j = 0;
	
	for (UINT i = LevelCollisions.size() - temp.size(); i < LevelCollisions.size(); i++, j++)
	{

		LevelCollisions[i].Center = temp[j].Center ;


	}

	/////////////////////////////

	

	XMVECTOR desiredCharDir = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);


	XMVECTOR playerPos = XMLoadFloat3(&mPlayerPosition);


	XMVECTOR camRight = XMLoadFloat3(&mCam.GetRight());
	XMVECTOR camForward = XMLoadFloat3(&mCam.GetLook());
	XMVECTOR camUp = XMLoadFloat3(&mCam.GetUp());

	XMVECTOR multiply = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);

	camUp = XMVectorAdd(camUp, multiply);
	
	
	
	bool jumpChar = false;


	bool moveChar = false;


	if (GetAsyncKeyState('W') & 0x8000)
	{
		desiredCharDir += (camForward);

		moveChar = true;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		desiredCharDir += -(camForward);

		moveChar = true;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		desiredCharDir += (camRight);

		moveChar = true;
	}



	if (GetAsyncKeyState('D') & 0x8000)
	{
		desiredCharDir += -(camRight);

		moveChar = true;
	}


	if (GetAsyncKeyState('Q') & 0x8000)
	{
		float dy = 1.5 * dt;
		mCam.RotateY(-dy);


	}

	if (GetAsyncKeyState('E') & 0x8000)
	{

		float dy = 1.5 * dt;
		mCam.RotateY(dy);


	}


	if (GetAsyncKeyState('R') & 0x8000)
	{
		float dy = 0.25 * dt;
		mCam.Pitch(dy);


	}

	if (GetAsyncKeyState('F') & 0x8000)
	{

		float dy = 0.25 * dt;
		mCam.Pitch(-dy);


	}


	if (onGround == true)
	{


	


		if (GetAsyncKeyState('J') & 0x8000)
		{
			desiredCharDir += camUp;
			
			moveChar = true;
		}

	}


	XMVECTOR addGravity = XMVectorSet(0.0f, -30 * DeltaTimeF, 0.0f, 0.0f);

	XMFLOAT3 tGrav;
	XMStoreFloat3(&tGrav, addGravity);

	XMVECTOR tGravity = XMLoadFloat3(&tGrav);

	if (onGround == true)
	{

	}
	else if (onGround == false)
	{

		desiredCharDir += addGravity;
	}

	moveDirection = desiredCharDir;





	//		
	// Switch the number of lights based on key presses.
	//
	if (GetAsyncKeyState('0') & 0x8000)

	{
		mLightCount = 0;

		
	}
	if (GetAsyncKeyState('1') & 0x8000)
		mLightCount = 1;

	if (GetAsyncKeyState('2') & 0x8000)
		mLightCount = 2;

	if (GetAsyncKeyState('3') & 0x8000)
		mLightCount = 3;



	////send player information to the camera
	mCam.getPlayerPos(mPlayerPosition);
	mCam.playerInfo(PlayerForward, PlayerRight, PlayerUp);

	mCam.moveCam();




	/*
	if (moveChar == true)
	{

		moveDirection = desiredCharDir;
		Walk(dt, desiredCharDir);

	}
	*/
	Walk(dt, desiredCharDir);

	UpdatePlayer(mPlayerWorld);

	
	


}






void Game::UpdatePlayer(XMFLOAT4X4& m)
{

	XMVECTOR S = XMLoadFloat3(&mPlayerScale);
	XMVECTOR P = XMLoadFloat3(&mPlayerPosition);
	XMVECTOR Q = XMLoadFloat4(&mPlayerRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mPlayerRotation);



	XMVECTOR OS = XMLoadFloat3(&mOPlayerScale);
	XMVECTOR OP = XMLoadFloat3(&mOPlayerPosition);
	XMVECTOR OQ = XMLoadFloat4(&mOPlayerRotationQuad);
	XMVECTOR Orot = XMLoadFloat4(&mOPlayerRotation);

	mPlayerBox.Center = mPlayerPosition;


	if (hitFeet == true)
	{
		isJump = false;
		isFalling = false;
		onGround = true;

	}

	if (isFalling == true || isJump == true)
	{
		currGround = 2.0f;

	}
	if (fellOffMap == true)
	{

		currGround = -500.0f;

	}





	if (mPlayerPosition.y < currGround)
	{

	P = XMVectorSetY(P, currGround);
		hitHead = false;
		onGround = true;

	}


	if (isAlive == true)
	{

		XMStoreFloat3(&mPlayerBox.Center, P);
		XMStoreFloat3(&mPlayerPosition, P);


		XMStoreFloat4x4(&mPlayerWorld, XMMatrixAffineTransformation(S, rot, Q, P));
	}
	else if (isAlive == false)
	{

		mPlayerBox.Center = mOPlayerPosition;

		XMStoreFloat3(&mPlayerBox.Center, P);
		XMStoreFloat3(&mOPlayerPosition, P);

		mPlayerRotation = mOPlayerRotation;
		mPlayerPosition = mOPlayerPosition;
		mPlayerRotationQuad = mOPlayerRotationQuad;

		XMStoreFloat4x4(&mPlayerWorld, XMMatrixAffineTransformation(OS, Orot, OQ, OP));

		isAlive = true;
	}

	
}







void Game::addDeltaTime(float dt)
{
	
	
	DeltaTimeF = dt;
	

}

float Game::getDeltaTime()
{


	return DeltaTimeF;
}


