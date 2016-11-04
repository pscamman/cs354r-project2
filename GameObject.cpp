#include "GameObject.h"

GameObject::GameObject(Ogre::SceneNode* s,btRigidBody* b):
sn(s),
rb(b)
{
  name = s->getName();
  hasAI =false;
  point = 0;
} 

GameObject::~GameObject(void){
  delete sn;
  delete rb;
  delete ai;
}

void GameObject::attachAI(AI* a){
  ai = a;
  hasAI = true;
}

int GameObject::getPoint(void){
  return point;
}

void GameObject::setPoint(int pt){
  point = pt;
}

std::string GameObject::getName(void){
  return name;
}

Ogre::Vector3 GameObject::getPosition(void){
  return sn->getPosition();
}

void GameObject::updateAI(){
  if(hasAI)
    ai->patrol();
}

void GameObject::attachParticleSystem(Ogre::ParticleSystem* ps){
  ptr = ps;
  sn->createChildSceneNode()->attachObject(ptr);
}

void GameObject::removeParticleSystem(void){
  sn->removeAndDestroyAllChildren();
  ptr = NULL;

}