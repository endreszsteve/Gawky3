
#include "Enemy.h"
#include "Effects.h"


#include "Camera.h"





Enemy::Enemy() : mEnemyPosition(0.0f, 0.0f, 0.0f), mEnemyScale(3.0f, 3.0f, 3.0f), mEnemyRotation(0.0f, 0.0f, 0.0f, 1.0f), mEnemyRotationQuad(0.0f, 0.0f, 0.0f, 0.0f)
{



	///initialize player
	XMVECTOR S = XMLoadFloat3(&mEnemyScale);
	XMVECTOR P = XMLoadFloat3(&mEnemyPosition);
	XMVECTOR Q = XMLoadFloat4(&mEnemyRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mEnemyRotation);
	XMStoreFloat4x4(&mEnemyWorld, XMMatrixAffineTransformation(S, rot, Q, P));


	currCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	oldCharDirection = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	charPosition = XMVectorSet(0.0f, 3.0f, 0.0f, 0.0f);
	EnemyForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	EnemyRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);


	direction = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	

}


Enemy::~Enemy()
{
}

//setters
void Enemy::setModelScale(XMMATRIX& Scale)

{
		
	modelScale = Scale;	

}

void Enemy::setModelRot(XMMATRIX& Rot)
{
	modelRot = Rot;

}
void Enemy::setModelOffset(XMMATRIX& Offset)
{


	modelOffset = Offset;

}

void Enemy::setModel(BasicModel* Enemy)
{

	anEnemy = Enemy;

}

void Enemy::setBasicMInstance(BasicModelInstance model)
{

	theEnemy = model;
}

void Enemy::setAABB(XNA::AxisAlignedBox* tempEnemyBox)
{

	EnemyBox = tempEnemyBox;
}


void Enemy::setWorld(XMFLOAT4X4 tempEnemyWorld)
{


	mEnemyWorld = tempEnemyWorld;


	XMMATRIX tempWorld = XMLoadFloat4x4(&mEnemyWorld);


	XMVECTOR Scale;
	XMVECTOR Position;
	XMVECTOR Rotation;


	XMMatrixDecompose(&Scale, &Rotation, &Position, tempWorld);

	XMStoreFloat3(&mEnemyPosition, Position);

	XMStoreFloat3(&mEnemyScale, Scale);

	XMStoreFloat4(&mEnemyRotation, Rotation);





	XMVECTOR S = XMLoadFloat3(&mEnemyScale);
	XMVECTOR P = XMLoadFloat3(&mEnemyPosition);
	XMVECTOR Q = XMLoadFloat4(&mEnemyRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mEnemyRotation);
	




	XMStoreFloat4x4(&mEnemyStartingWorld, XMMatrixAffineTransformation(S, rot, Q, P));

	






	
	

}




//getters
XMMATRIX Enemy::getModelScale()
{

	return modelScale;
}
XMMATRIX Enemy::getModelRot()
{


	return modelRot;
}
XMMATRIX Enemy::getModelOffset()
{


	return modelOffset;
}

BasicModel* Enemy::getModel()
{

	return anEnemy;
}

BasicModelInstance Enemy::getBasicMInstance()
{


	return theEnemy;
}

XNA::AxisAlignedBox* Enemy::getEnemyAABB()
{


	return EnemyBox;
}

XMFLOAT4X4 Enemy::GetWorld()
{
	

	return mEnemyWorld;
}


XMFLOAT3 Enemy::getEnemyPosition()
{


	return mEnemyPosition;
}




void Enemy::update(FLOAT dt)
{
	
	move(dt);
	
	XMVECTOR S = XMLoadFloat3(&mEnemyScale);
	XMVECTOR P = XMLoadFloat3(&mEnemyPosition);
	XMVECTOR Q = XMLoadFloat4(&mEnemyRotationQuad);
	XMVECTOR rot = XMLoadFloat4(&mEnemyRotation);
	XNA::AxisAlignedBox tempEnemyBox;

	tempEnemyBox.Center = mEnemyPosition;
	
	EnemyBox = &tempEnemyBox;


	XMStoreFloat4x4(&mEnemyWorld, XMMatrixAffineTransformation(S, rot, Q, P));

}



void Enemy::move(FLOAT dt)
{

	XMMATRIX startingworldMatrix = XMLoadFloat4x4(&mEnemyStartingWorld);
	XMVECTOR S;
	XMVECTOR P;
	XMVECTOR Q;
	
	XMMatrixDecompose(&S, &Q, &P, startingworldMatrix);	
	
	
	XMVECTOR enemyPosition = XMLoadFloat3(&mEnemyPosition);

	FLOAT tempY = XMVectorGetY(direction);

	FLOAT newPos = XMVectorGetZ(enemyPosition);
	FLOAT oldPos = XMVectorGetZ(P);

	if (newPos >= oldPos + 10)
	{
		direction = XMVectorSet(0.0f, 0.0f, -1.f, 0.0f);


	}
	else if (newPos <= oldPos - 10)
	{

		direction = XMVectorSet(0.0f, 0.0f, 1.f, 0.0f);

	}
	
	
	

	//enemyPosition += tempAdd;

	//XMStoreFloat3(&mEnemyPosition, enemyPosition);
	
	XMMATRIX worldMatrix = XMLoadFloat4x4(&mEnemyWorld);
	XMVECTOR r = XMLoadFloat3(&mEnemyPosition);



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
	charPosition = XMVector3TransformCoord(charPosition, worldMatrix);



	///// rotate the character
	float destDirLength = 10.0f * dt;



	currCharDirection = (oldCharDirection)+(direction * destDirLength);
	currCharDirection = XMVector3Normalize(currCharDirection);


	// get the angle 
	float charDirAngle = XMVectorGetX(XMVector3AngleBetweenNormals(XMVector3Normalize(currCharDirection), XMVector3Normalize(EnemyForward)));

	charDirAngle = -charDirAngle;

	if (XMVectorGetY(XMVector3Cross(currCharDirection, EnemyRight)) > 0.0f)
	{
		charDirAngle = -charDirAngle;
	}



	float speed = 15.0f * dt;
	XMVECTOR tY = XMVectorSet(0.0f, tempY, 0.0f, 0.0f);

	direction = XMVectorSetY(direction, tempY);
	direction = direction * speed;
	charPosition = charPosition + direction;

	
	charPosition += tY;
	

	XMMATRIX rotationMatrix;

	XMMATRIX previousWorld = worldMatrix;



	XMMATRIX Translation = XMMatrixTranslation(XMVectorGetX(charPosition), XMVectorGetY(charPosition), XMVectorGetZ(charPosition));
	rotationMatrix = XMMatrixRotationY(charDirAngle - 3.14159265f);		// Subtract PI from angle so the character doesn't run backwards



	worldMatrix = rotationMatrix * Translation;


	XMMatrixDecompose(&S, &Q, &P, worldMatrix);

	//XMStoreFloat3(&mEnemyScale, S);
	XMStoreFloat3(&mEnemyPosition, P);
	XMStoreFloat4(&mEnemyRotationQuad, Q);

	

	XMStoreFloat4x4(&mEnemyWorld, worldMatrix);







}
