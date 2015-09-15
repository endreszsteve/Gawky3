#ifndef Enemies_H
#define Enemies_H


#include "BasicModel.h"
#include "xnacollision.h"
#include "GameTimer.h"




class BasicModel;
class Camera;
class Enemy;
class GameTimer;

class Enemies
{
public:
	Enemies();

	~Enemies();



	void addEnemy(BasicModelInstance theEnemy);

	void draw(ID3D11DeviceContext* dc, Camera& camera, ID3DX11EffectTechnique* activeTexTech);

	void createEnemy(ID3D11Device* device, TextureMgr& texMgr,
		const std::string& modelFilename,
		const std::wstring& texturePath, FLOAT x, FLOAT y, FLOAT z);


	void CreateBoundingBox();

	void update(float dt);

	std::vector<BasicModelInstance> getEnemy();

	std::vector <XNA::AxisAlignedBox> getEnemyCollisions();
	void RemovemObjectInstance(int number);


	









private:

	std::vector<BasicModelInstance> mEnemyInstances;
	std::vector<Enemy*> enemyclass;
	
	std::vector <XNA::AxisAlignedBox> LevelCollisions;


	BasicModel* anEnemy;
	
	BasicModelInstance theEnemy;
	XNA::AxisAlignedBox EnemyBox;


	GameTimer timer;
	 
	FLOAT DeltaTime;

	Enemy* newEnemy;


};

#endif