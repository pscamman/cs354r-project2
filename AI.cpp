#include "AI.h"
AI::AI(btRigidBody* obj)
{
	btBody = obj;
	speed.x = 80;
	speed.y = 0;
	speed.z = 80;
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
	if(pos.x >= 500)
		speed.x = -80;
	if(pos.x <= -500)
		speed.x = 80;
	if(pos.z >= 500)
		speed.z = -80;
	if(pos.z <= -500)
		speed.z = 80;
}