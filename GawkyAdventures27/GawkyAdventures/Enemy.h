#ifndef Enemy_H
#define Enemy_H


#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"

#include "d3dUtil.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "xnacollision.h"


class BasicModel;
class Camera;



class Enemy
{
public:
	Enemy();
	~Enemy();

	void move(FLOAT dt);

	void update(FLOAT dt);

	///setters
	void setModelScale(XMMATRIX& Scale);
	void setModelRot(XMMATRIX& Rot);
	void setModelOffset(XMMATRIX& Offset);
	void setModel(BasicModel* Enemy);
	void setBasicMInstance(BasicModelInstance model);
	void setAABB(XNA::AxisAlignedBox* tempEnemyBox);
	void setWorld(XMFLOAT4X4 tempEnemyWorld);
	
	///getters
	XMMATRIX getModelScale();
	XMMATRIX getModelRot();
	XMMATRIX getModelOffset();
	BasicModel* getModel();
	BasicModelInstance getBasicMInstance();
	XNA::AxisAlignedBox* getEnemyAABB();
	XMFLOAT4X4 GetWorld();
	XMFLOAT3 getEnemyPosition();


private:
	

	XMMATRIX modelScale;
	XMMATRIX modelRot;
	XMMATRIX modelOffset;

	XMFLOAT4X4 mEnemyWorld;


	XMFLOAT4X4 mEnemyStartingWorld;


	XMFLOAT3 mEnemyPosition;
	XMFLOAT3 mEnemyScale;
	XMFLOAT4 mEnemyRotationQuad;
	XMFLOAT4 mEnemyRotation;

	BasicModel* anEnemy;

	BasicModelInstance theEnemy;

	XNA::AxisAlignedBox* EnemyBox;

	
	XMVECTOR currCharDirection;
	XMVECTOR oldCharDirection;
	XMVECTOR charPosition;
	XMVECTOR moveDirection;

	XMVECTOR EnemyForward;
	XMVECTOR EnemyRight;
	XMVECTOR EnemyUp;

	XMVECTOR direction;

	
	

	
	

	


};


#endif 