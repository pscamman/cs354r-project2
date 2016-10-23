#ifndef _AI_h_
#define _AI_h_
#include <btBulletDynamicsCommon.h>
#include <Ogre.h>
struct Vector3d {
	int x, y, z;
}typedef vector3d;

class AI{
protected:
	vector3d speed;
	btRigidBody* btBody;
public:
	AI(btRigidBody*);
	~AI(void);
	void patrol(void);
};
#endif 