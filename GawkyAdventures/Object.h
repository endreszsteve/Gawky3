#ifndef Object_H
#define Object_H


#include "d3dApp.h"
#include "d3dx11Effect.h"
#include "MathHelper.h"

#include "d3dUtil.h"
#include "TextureMgr.h"
#include "BasicModel.h"
#include "xnacollision.h"


class BasicModel;
class Camera;



class Object
{
public:
	Object();
	~Object();

	void move(FLOAT dt);

	void update(FLOAT dt);

	///setters
	void setModelScale(XMMATRIX& Scale);
	void setModelRot(XMMATRIX& Rot);
	void setModelOffset(XMMATRIX& Offset);
	void setModel(BasicModel* Object);
	void setBasicMInstance(BasicModelInstance model);
	void setAABB(XNA::AxisAlignedBox* tempObjectBox);
	void setWorld(XMFLOAT4X4 tempObjectWorld);
	void setCollisionType(int theCollisionType);

	///getters
	XMMATRIX getModelScale();
	XMMATRIX getModelRot();
	XMMATRIX getModelOffset();
	BasicModel* getModel();
	BasicModelInstance getBasicMInstance();
	XNA::AxisAlignedBox* getObjectAABB();
	XMFLOAT4X4 GetWorld();
	XMFLOAT3 getObjectPosition();
	int getCollisionType();


private:


	XMMATRIX modelScale;
	XMMATRIX modelRot;
	XMMATRIX modelOffset;

	XMFLOAT4X4 mObjectWorld;


	XMFLOAT4X4 mObjectStartingWorld;


	XMFLOAT3 mObjectPosition;
	XMFLOAT3 mObjectScale;
	XMFLOAT4 mObjectRotationQuad;
	XMFLOAT4 mObjectRotation;

	BasicModel* anObject;

	BasicModelInstance theObject;

	XNA::AxisAlignedBox* ObjectBox;


	XMVECTOR currCharDirection;
	XMVECTOR oldCharDirection;
	XMVECTOR charPosition;
	XMVECTOR moveDirection;

	XMVECTOR ObjectForward;
	XMVECTOR ObjectRight;
	XMVECTOR ObjectUp;

	XMVECTOR direction;
	int collisionType;










};


#endif 