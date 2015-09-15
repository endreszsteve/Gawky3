#ifndef LevelParts_H
#define LevelParts_H


#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"

#include "d3dUtil.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "xnacollision.h"


class BasicModel;
class Camera;



class LevelParts
{
public:
	LevelParts();
	~LevelParts();

	void move(FLOAT dt);

	void update(FLOAT dt);

	///setters
	void setModelScale(XMMATRIX& Scale);
	void setModelRot(XMMATRIX& Rot);
	void setModelOffset(XMMATRIX& Offset);
	void setModel(BasicModel* LevelParts);
	void setBasicMInstance(BasicModelInstance model);
	void setAABB(XNA::AxisAlignedBox* tempLevelPartsBox);
	void setWorld(XMFLOAT4X4 tempLevelPartsWorld);
	void setCollisionType(int theCollisionType);
	void setScale(int theScale);

	///getters
	XMMATRIX getModelScale();
	XMMATRIX getModelRot();
	XMMATRIX getModelOffset();
	BasicModel* getModel();
	BasicModelInstance getBasicMInstance();
	XNA::AxisAlignedBox* getLevelPartsAABB();
	XMFLOAT4X4 GetWorld();
	XMFLOAT3 getLevelPartsPosition();
	int getCollisionType();
	int getScale();


private:


	XMMATRIX modelScale;
	XMMATRIX modelRot;
	XMMATRIX modelOffset;

	XMFLOAT4X4 mLevelPartsWorld;


	XMFLOAT4X4 mLevelPartsStartingWorld;


	XMFLOAT3 mLevelPartsPosition;
	XMFLOAT3 mLevelPartsScale;
	XMFLOAT4 mLevelPartsRotationQuad;
	XMFLOAT4 mLevelPartsRotation;

	BasicModel* anLevelParts;

	BasicModelInstance theLevelParts;

	XNA::AxisAlignedBox* LevelPartsBox;


	XMVECTOR currCharDirection;
	XMVECTOR oldCharDirection;
	XMVECTOR charPosition;
	XMVECTOR moveDirection;

	XMVECTOR LevelPartsForward;
	XMVECTOR LevelPartsRight;
	XMVECTOR LevelPartsUp;

	XMVECTOR direction;
	int collisionType;
	int scale;










};


#endif 