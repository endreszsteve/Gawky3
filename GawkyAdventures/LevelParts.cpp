
#include "LevelParts.h"
#include "Effects.h"
#include "Camera.h"





LevelParts::LevelParts() : mLevelPartsPosition(0.0f, 0.0f, 0.0f), mLevelPartsScale(1.0f, 1.0f, 1.0f), mLevelPartsRotation(0.0f, 0.0f, 0.0f, 1.0f), mLevelPartsRotationQuad(0.0f, 0.0f, 0.0f, 0.0f)
{



	///initialize player
	XMVECTOR S = XMLoadFloat3(&mLevelPartsScale);
	XMVECTOR P = XMLoadFloat3(&mLevelPartsPosition);
	XMVECTOR Q = XMLoadFloat4(&mLevelPartsRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mLevelPartsRotation);
	XMStoreFloat4x4(&mLevelPartsWorld, XMMatrixAffineTransformation(S, rot, Q, P));


	currCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	oldCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	charPosition = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	LevelPartsForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	LevelPartsRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


	direction = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);


}


LevelParts::~LevelParts()
{
}

//setters
void LevelParts::setModelScale(XMMATRIX& Scale)

{

	modelScale = Scale;

}

void LevelParts::setModelRot(XMMATRIX& Rot)
{
	modelRot = Rot;

}
void LevelParts::setModelOffset(XMMATRIX& Offset)
{


	modelOffset = Offset;

}

void LevelParts::setModel(BasicModel* LevelParts)
{

	anLevelParts = LevelParts;

}

void LevelParts::setBasicMInstance(BasicModelInstance model)
{

	theLevelParts = model;
}

void LevelParts::setAABB(XNA::AxisAlignedBox* tempLevelPartsBox)
{

	LevelPartsBox = tempLevelPartsBox;
}

void LevelParts::setCollisionType(int theCollisionType)
{

	collisionType = theCollisionType;

}

void LevelParts::setWorld(XMFLOAT4X4 tempLevelPartsWorld)
{


	mLevelPartsWorld = tempLevelPartsWorld;


	XMMATRIX tempWorld = XMLoadFloat4x4(&mLevelPartsWorld);


	XMVECTOR Scale;
	XMVECTOR Position;
	XMVECTOR Rotation;


	XMMatrixDecompose(&Scale, &Rotation, &Position, tempWorld);

	XMStoreFloat3(&mLevelPartsPosition, Position);

	XMStoreFloat3(&mLevelPartsScale, Scale);

	XMStoreFloat4(&mLevelPartsRotation, Rotation);





	XMVECTOR S = XMLoadFloat3(&mLevelPartsScale);
	XMVECTOR P = XMLoadFloat3(&mLevelPartsPosition);
	XMVECTOR Q = XMLoadFloat4(&mLevelPartsRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mLevelPartsRotation);

	XMStoreFloat4x4(&mLevelPartsStartingWorld, XMMatrixAffineTransformation(S, rot, Q, P));

}

void LevelParts::setScale(int theScale)
{
	scale = theScale;

}



//getters
XMMATRIX LevelParts::getModelScale()
{

	return modelScale;
}
XMMATRIX LevelParts::getModelRot()
{


	return modelRot;
}
XMMATRIX LevelParts::getModelOffset()
{


	return modelOffset;
}

BasicModel* LevelParts::getModel()
{

	return anLevelParts;
}

BasicModelInstance LevelParts::getBasicMInstance()
{


	return theLevelParts;
}

XNA::AxisAlignedBox* LevelParts::getLevelPartsAABB()
{


	return LevelPartsBox;
}

XMFLOAT4X4 LevelParts::GetWorld()
{


	return mLevelPartsWorld;
}


XMFLOAT3 LevelParts::getLevelPartsPosition()
{


	return mLevelPartsPosition;
}



int LevelParts::getCollisionType()
{

	return collisionType;
}

int LevelParts::getScale()
{

	return scale;
}




void LevelParts::update(FLOAT dt)
{

	//move(dt);

	XMVECTOR S = XMLoadFloat3(&mLevelPartsScale);
	XMVECTOR P = XMLoadFloat3(&mLevelPartsPosition);
	XMVECTOR Q = XMLoadFloat4(&mLevelPartsRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mLevelPartsRotation);
	XNA::AxisAlignedBox tempLevelPartsBox;

	tempLevelPartsBox.Center = mLevelPartsPosition;

	LevelPartsBox = &tempLevelPartsBox;


	XMStoreFloat4x4(&mLevelPartsWorld, XMMatrixAffineTransformation(S, rot, Q, P));

}

