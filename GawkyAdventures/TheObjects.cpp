
#include "TheObjects.h"

#include "Effects.h"
#include "Camera.h"
#include "Object.h"








TheObjects::TheObjects()
{







}


TheObjects::~TheObjects()
{


	for (UINT i = 0; i < Objectclass.size(); ++i)
	{

		delete Objectclass[i];


	}




}






void TheObjects::draw(ID3D11DeviceContext* dc, Camera& camera, ID3DX11EffectTechnique* activeTexTech)
{



	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;


	XMMATRIX view = camera.View();
	XMMATRIX proj = camera.Proj();
	XMMATRIX viewProj = camera.ViewProj();



	for (UINT modelIndex = 0; modelIndex < mObjectInstances.size(); ++modelIndex)
	{
		world = XMLoadFloat4x4(&mObjectInstances[modelIndex].World);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);




		//I do not have shadoews
		//Effects::BasicFX->SetShadowTransform(world*shadowTransform);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

		for (UINT subset = 0; subset < mObjectInstances[modelIndex].Model->SubsetCount; ++subset)
		{
			Effects::BasicFX->SetMaterial(mObjectInstances[modelIndex].Model->Mat[subset]);
			Effects::BasicFX->SetDiffuseMap(mObjectInstances[modelIndex].Model->DiffuseMapSRV[subset]);
			//Effects::BasicFX->SetNormalMap(mModelInstances[modelIndex].Model->NormalMapSRV[subset]);

			activeTexTech->GetPassByIndex(0)->Apply(0, dc);
			mObjectInstances[modelIndex].Model->ModelMesh.Draw(dc, subset);




		}

	}




}





void TheObjects::addObject(BasicModelInstance theObject)
{



	mObjectInstances.push_back(theObject);


}



void TheObjects::createObject(ID3D11Device* device, TextureMgr& texMgr,
	const std::string& modelFilename,
	const std::wstring& texturePath, FLOAT x, FLOAT y, FLOAT z,  int collisionstype)
{
	Object* newObject;

	newObject = new Object();

	XMMATRIX modelScale = XMMatrixScaling(1.0f, 1.0f, -1.0f);
	XMMATRIX modelRot = XMMatrixRotationY(0);
	XMMATRIX modelOffset = XMMatrixTranslation(x, y, z);


	newObject->setModelScale(modelScale);
	newObject->setModelRot(modelRot);
	newObject->setModelOffset(modelOffset);


	anObject = new BasicModel(device, texMgr, modelFilename, texturePath);

	newObject->setModel(anObject);


	BasicModelInstance oneObject;


	theObject.Model = anObject;


	XMStoreFloat4x4(&theObject.World, modelScale*modelRot*modelOffset);


	newObject->setBasicMInstance(theObject);

	oneObject = newObject->getBasicMInstance();



	addObject(theObject);

	Objectclass.push_back(newObject);


	LevelCollisions.push_back(ObjectBox);

	newObject->setWorld(theObject.World);


	newObject->setCollisionType(collisionstype);



}



void TheObjects::CreateBoundingBox()
{


	//
	// Compute scene bounding box.
	//
	XMFLOAT3 minPt(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 maxPt(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);



	for (UINT i = 0; i < mObjectInstances.size(); ++i)
	{
		minPt.x = 0.0f;
		minPt.y = 0.0f;
		minPt.z = 0.0f;

		maxPt.x = 0.0f;
		maxPt.y = 0.0f;
		maxPt.z = 0.0f;

		for (UINT j = 0; j < mObjectInstances[i].Model->BasicVertices.size(); ++j)
		{

			XMFLOAT3 P = mObjectInstances[i].Model->BasicVertices[j].Pos;


			//////multiply all these by 7

			minPt.x = MathHelper::Min(minPt.x, P.x);
			minPt.y = MathHelper::Min(minPt.y, P.y);
			minPt.z = MathHelper::Min(minPt.z, P.z);

			maxPt.x = MathHelper::Max(maxPt.x, P.x);
			maxPt.y = MathHelper::Max(maxPt.y, P.y);
			maxPt.z = MathHelper::Max(maxPt.z, P.z);


		}

		XMMATRIX temp = XMLoadFloat4x4(&mObjectInstances[i].World);

		Objectclass[i]->setWorld(mObjectInstances[i].World);

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

		LevelCollisions[i].collisionType = Objectclass[i]->getCollisionType();

		LevelCollisions[i].Extents.x = LevelCollisions[i].Extents.x;
		LevelCollisions[i].Extents.y = LevelCollisions[i].Extents.y;
		LevelCollisions[i].Extents.z = LevelCollisions[i].Extents.z;

		//// this doesn't work, useless atm
		ObjectBox.collisionType = 2;


		Objectclass[i]->setAABB(&LevelCollisions[i]);

	}
}



std::vector<BasicModelInstance> TheObjects::getObject()
{

	return mObjectInstances;
}



std::vector <XNA::AxisAlignedBox> TheObjects::getObjectCollisions()
{







	return LevelCollisions;
}




void TheObjects::update(float dt)
{



	DeltaTime = dt;

	for (int i = 0; i < mObjectInstances.size(); i++)
	{

		Objectclass[i]->update(DeltaTime);

		mObjectInstances[i].World = Objectclass[i]->GetWorld();

		XMMATRIX temp = XMLoadFloat4x4(&mObjectInstances[i].World);
		XMVECTOR Scale;
		XMVECTOR Rotation;
		XMVECTOR Position;

		XMFLOAT3 tempPosition;

		XMMatrixDecompose(&Scale, &Rotation, &Position, temp);
		XMStoreFloat3(&tempPosition, Position);



		LevelCollisions[i].Center = tempPosition;


	}








}


void TheObjects::RemovemObjectInstance(int number)
{

	mObjectInstances.erase(mObjectInstances.begin() + number);

	LevelCollisions.erase(LevelCollisions.begin() + number);
	delete(Objectclass[number]);
}