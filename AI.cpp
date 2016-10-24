#include "AI.h"
AI::AI(btRigidBody* obj, std::string n)
{
	name = n;
	btBody = obj;
	speed.x = 80;
	speed.y = 0;
	speed.z = 80;
	vulnerability = true;
}

AI::~AI(void)
{
}

void AI::patrol()
{
	auto sn = static_cast<Ogre::SceneNode*>(btBody->getUserPointer());
	Ogre::Vector3 pos = sn->getPosition();
	btBody->setLinearVelocity(btVector3(speed.x,speed.y,speed.z));
	// std::cout<< pos << std::endl;
	if(pos.x >= 600)
		speed.x = -80;
	if(pos.x <= -400)
		speed.x = 80;
	if(pos.z >= 600)
		speed.z = -80;
	if(pos.z <= -400)
		speed.z = 80;
}

void AI::vulnerable(){
	auto sn = static_cast<Ogre::SceneNode*>(btBody->getUserPointer());
	auto en = static_cast<Ogre::Entity*>(sn->getAttachedObject(name));
	en->setMaterialName("Examples/BumpyMetal");
	vulnerability = true;
}