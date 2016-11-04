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
	Ogre::Vector3 speed;
	btRigidBody* btBody;
	Ogre::Vector3 originalPos;
public:
	bool vulnerability;
	AI(btRigidBody*);
	~AI(void);
	void patrol(void);
	void vulnerable(bool);
};
#endif 