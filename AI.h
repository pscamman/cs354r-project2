#ifndef _AI_h_
#define _AI_h_
#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
#include <OgreEntity.h>

struct Vector3d {
	int x, y, z;
}typedef vector3d;

class AI{
protected:
	vector3d speed;
	btRigidBody* btBody;
public:
	std::string name;
	bool vulnerability;
	AI(btRigidBody*, std::string);
	~AI(void);
	void patrol(void);
	void vulnerable(void);
};
#endif 