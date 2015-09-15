#ifndef LevelBuilder_H
#define LevelBuilder_H


#include "BasicModel.h"
#include "xnacollision.h"
#include "GameTimer.h"




class BasicModel;
class Camera;
class LevelParts;
class GameTimer;

class LevelBuilder
{
public:
	LevelBuilder();

	~LevelBuilder();



	void addLevelParts(BasicModelInstance theLevelParts);

	void draw(ID3D11DeviceContext* dc, Camera& camera, ID3DX11EffectTechnique* activeTexTech);

	void createLevelParts(ID3D11Device* device, TextureMgr& texMgr,
		const std::string& modelFilename,
		const std::wstring& texturePath, FLOAT x, FLOAT y, FLOAT z, int collisionstype, int scale);


	void CreateBoundingBox();

	void update(float dt);

	void RemovemLevelPartsInstance(int number);

	std::vector<BasicModelInstance> getLevelParts();

	std::vector <XNA::AxisAlignedBox> getLevelPartsCollisions();



private:

	std::vector<BasicModelInstance> mLevelPartsInstances;
	std::vector<LevelParts*> LevelPartsclass;

	std::vector <XNA::AxisAlignedBox> LevelCollisions;

	BasicModel* anLevelParts;

	BasicModelInstance theLevelParts;
	XNA::AxisAlignedBox LevelPartsBox;


	GameTimer timer;

	FLOAT DeltaTime;

	LevelParts* newLevelParts;






};

#endif