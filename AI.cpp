#include "AI.h"
AI::AI(btRigidBody* obj)
{
	btBody = obj;
	speed.x = -80;
	speed.y = 0;
	speed.z = 0;
	vulnerability = true;
	originalPos = static_cast<Ogre::SceneNode*>(btBody->getUserPointer())->getPosition();
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
	if(pos.x <= originalPos.x-800 && pos.z > originalPos.z-800){
		speed.z = -80;
		speed.x = 0;
	}else if(pos.z <= originalPos.z-800 && pos.x < originalPos.x+800){
		speed.x = 80;
		speed.z = 0;
	}else if(pos.x >= originalPos.x+800 && pos.z < originalPos.z + 400){
		speed.x = 0;
		speed.z = 80;
	}else if(pos.z >= originalPos.z+400 && pos.x > originalPos.x+ 800){
		speed.x = -80;
		speed.z = 0;
	}
	
}

void AI::vulnerable(bool vul){
	vulnerability = vul;
}
