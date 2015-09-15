#ifndef TheObjects_H
#define TheObjects_H


#include "BasicModel.h"
#include "xnacollision.h"
#include "GameTimer.h"




class BasicModel;
class Camera;
class Object;
class GameTimer;

class TheObjects
{
public:
	TheObjects();

	~TheObjects();



	void addObject(BasicModelInstance theObject);

	void draw(ID3D11DeviceContext* dc, Camera& camera, ID3DX11EffectTechnique* activeTexTech);

	void createObject(ID3D11Device* device, TextureMgr& texMgr,
		const std::string& modelFilename,
		const std::wstring& texturePath, FLOAT x, FLOAT y, FLOAT z, int collisionstype);


	void CreateBoundingBox();

	void update(float dt);

	void RemovemObjectInstance(int number);

	std::vector<BasicModelInstance> getObject();

	std::vector <XNA::AxisAlignedBox> getObjectCollisions();












private:

	std::vector<BasicModelInstance> mObjectInstances;
	std::vector<Object*> Objectclass;

	std::vector <XNA::AxisAlignedBox> LevelCollisions;

	BasicModel* anObject;

	BasicModelInstance theObject;
	XNA::AxisAlignedBox ObjectBox;


	GameTimer timer;

	FLOAT DeltaTime;

	Object* newObject;

	

	


};

#endif