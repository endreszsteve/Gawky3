
#include "LevelBuilder.h"

#include "Effects.h"
#include "Camera.h"
#include "LevelParts.h"








LevelBuilder::LevelBuilder()
{







}


LevelBuilder::~LevelBuilder()
{


	for (UINT i = 0; i < LevelPartsclass.size(); ++i)
	{

		delete LevelPartsclass[i];


	}




}






void LevelBuilder::draw(ID3D11DeviceContext* dc, Camera& camera, ID3DX11EffectTechnique* activeTexTech)
{



	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;


	XMMATRIX view = camera.View();
	XMMATRIX proj = camera.Proj();
	XMMATRIX viewProj = camera.ViewProj();



	for (UINT modelIndex = 0; modelIndex < mLevelPartsInstances.size(); ++modelIndex)
	{
		world = XMLoadFloat4x4(&mLevelPartsInstances[modelIndex].World);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		Effects::BasicFX->SetWorld(world);
		Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BasicFX->SetWorldViewProj(worldViewProj);




		//I do not have shadoews
		//Effects::BasicFX->SetShadowTransform(world*shadowTransform);
		Effects::BasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));

		for (UINT subset = 0; subset < mLevelPartsInstances[modelIndex].Model->SubsetCount; ++subset)
		{
			Effects::BasicFX->SetMaterial(mLevelPartsInstances[modelIndex].Model->Mat[subset]);
			Effects::BasicFX->SetDiffuseMap(mLevelPartsInstances[modelIndex].Model->DiffuseMapSRV[subset]);
			//Effects::BasicFX->SetNormalMap(mModelInstances[modelIndex].Model->NormalMapSRV[subset]);

			activeTexTech->GetPassByIndex(0)->Apply(0, dc);
			mLevelPartsInstances[modelIndex].Model->ModelMesh.Draw(dc, subset);


		}

	}

}





void LevelBuilder::addLevelParts(BasicModelInstance theLevelParts)
{



	mLevelPartsInstances.push_back(theLevelParts);


}



void LevelBuilder::createLevelParts(ID3D11Device* device, TextureMgr& texMgr,
	const std::string& modelFilename,
	const std::wstring& texturePath, FLOAT x, FLOAT y, FLOAT z, int collisionstype, int scale)
{
	LevelParts* newLevelParts;

	newLevelParts = new LevelParts();

	XMMATRIX modelScale = XMMatrixScaling(scale, scale, -scale);
	XMMATRIX modelRot = XMMatrixRotationY(0);
	XMMATRIX modelOffset = XMMatrixTranslation(x, y, z);


	newLevelParts->setModelScale(modelScale);
	newLevelParts->setModelRot(modelRot);
	newLevelParts->setModelOffset(modelOffset);


	anLevelParts = new BasicModel(device, texMgr, modelFilename, texturePath);

	newLevelParts->setModel(anLevelParts);


	BasicModelInstance oneLevelParts;


	theLevelParts.Model = anLevelParts;


	XMStoreFloat4x4(&theLevelParts.World, modelScale*modelRot*modelOffset);


	newLevelParts->setBasicMInstance(theLevelParts);

	oneLevelParts = newLevelParts->getBasicMInstance();



	addLevelParts(theLevelParts);

	LevelPartsclass.push_back(newLevelParts);


	LevelCollisions.push_back(LevelPartsBox);

	newLevelParts->setWorld(theLevelParts.World);


	newLevelParts->setCollisionType(collisionstype);


	///set the scale
	newLevelParts->setScale(scale);



}



void LevelBuilder::CreateBoundingBox()
{


	//
	// Compute scene bounding box.
	//
	XMFLOAT3 minPt(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 maxPt(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);



	for (UINT i = 0; i < mLevelPartsInstances.size(); ++i)
	{
		minPt.x = 0.0f;
		minPt.y = 0.0f;
		minPt.z = 0.0f;

		maxPt.x = 0.0f;
		maxPt.y = 0.0f;
		maxPt.z = 0.0f;

		for (UINT j = 0; j < mLevelPartsInstances[i].Model->BasicVertices.size(); ++j)
		{

			XMFLOAT3 P = mLevelPartsInstances[i].Model->BasicVertices[j].Pos;


			//////multiply all these by 7

			minPt.x = MathHelper::Min(minPt.x, P.x);
			minPt.y = MathHelper::Min(minPt.y, P.y);
			minPt.z = MathHelper::Min(minPt.z, P.z);

			maxPt.x = MathHelper::Max(maxPt.x, P.x);
			maxPt.y = MathHelper::Max(maxPt.y, P.y);
			maxPt.z = MathHelper::Max(maxPt.z, P.z);


		}

		XMMATRIX temp = XMLoadFloat4x4(&mLevelPartsInstances[i].World);

		LevelPartsclass[i]->setWorld(mLevelPartsInstances[i].World);

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

		LevelCollisions[i].collisionType = LevelPartsclass[i]->getCollisionType();

		// sets the scale of the collision box 
		int scale = LevelPartsclass[i]->getScale();

		LevelCollisions[i].Extents.x = LevelCollisions[i].Extents.x * scale;
		LevelCollisions[i].Extents.y = LevelCollisions[i].Extents.y * scale;
		LevelCollisions[i].Extents.z = LevelCollisions[i].Extents.z * scale;

		//// this doesn't work, useless atm
		//LevelPartsBox[i]->setCollisionType();


		LevelPartsclass[i]->setAABB(&LevelCollisions[i]);

	}
}



std::vector<BasicModelInstance> LevelBuilder::getLevelParts()
{

	return mLevelPartsInstances;
}



std::vector <XNA::AxisAlignedBox> LevelBuilder::getLevelPartsCollisions()
{







	return LevelCollisions;
}




void LevelBuilder::update(float dt)
{



	DeltaTime = dt;

	for (int i = 0; i < mLevelPartsInstances.size(); i++)
	{

		LevelPartsclass[i]->update(DeltaTime);

		mLevelPartsInstances[i].World = LevelPartsclass[i]->GetWorld();

		XMMATRIX temp = XMLoadFloat4x4(&mLevelPartsInstances[i].World);
		XMVECTOR Scale;
		XMVECTOR Rotation;
		XMVECTOR Position;

		XMFLOAT3 tempPosition;

		XMMatrixDecompose(&Scale, &Rotation, &Position, temp);
		XMStoreFloat3(&tempPosition, Position);



		LevelCollisions[i].Center = tempPosition;


	}








}


void LevelBuilder::RemovemLevelPartsInstance(int number)
{

	mLevelPartsInstances.erase(mLevelPartsInstances.begin() + number);

	LevelCollisions.erase(LevelCollisions.begin() + number);
	delete(LevelPartsclass[number]);
}