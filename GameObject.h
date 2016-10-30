#ifndef _GameObject_h_
#define _GameObject_h_
#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include "AI.h"


class GameObject{
protected:
	// Entity* ent;
	Ogre::SceneNode *sn;
	std::string name;
	btRigidBody* rb;
	// Ogre::Vector3 pos;
	int point;
public:
	bool hasAI;
	AI* ai;
	GameObject(Ogre::SceneNode*, btRigidBody*);
	~GameObject(void);
	void attachAI(AI*);
	std::string getName(void);
	int getPoint(void);
	void setPoint(int);
	Ogre::Vector3 getPosition(void);
	void updateAI(void);
};

#endif