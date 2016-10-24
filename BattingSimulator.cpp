/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "BattingSimulator.h"

// globals, extern variables from BaseApplication.h
BaseApplication* bApp;

//---------------------------------------------------------------------------
BattingSimulator::BattingSimulator(void)
{
}
//---------------------------------------------------------------------------
BattingSimulator::~BattingSimulator(void)
{
}
//---------------------------------------------------------------------------
void BattingSimulator::createScene(void)
{
    using namespace Ogre;

    mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
    mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
    Light* light;
    light = mSceneMgr->createLight("MainLight1");
    light->setPosition(199, 199, 199);
    light->setCastShadows(true);
    light->setDiffuseColour(ColourValue(0.2, 0, 0));
    light = mSceneMgr->createLight("MainLight2");
    light->setPosition(20, 80, 50);
    light->setCastShadows(false);
    light->setDiffuseColour(ColourValue(0.4, 0.4, 0.4));

    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

    // Create the ground mesh
    Plane p;
    p.normal = Vector3::UNIT_Y;
    p.d = 0;
    MeshManager::getSingleton().createPlane("FloorPlane",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,100000,100000,1,1,true,1,5,5,Vector3::UNIT_Z);

    // Create the entities, scene nodes, and bullet objects
    Entity*    ent;
    SceneNode* node;
    String matString = "Core/StatsBlockBorder/Up";

    // create non-physical node
    mainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mainNode->setPosition(0, 500, 2000);

    // create rotating focal point for camera
    camSpinNode = mainNode->createChildSceneNode();

    // create non-physical camera node as a child of camSpinNode
    camNode = camSpinNode->createChildSceneNode();
    camNode->attachObject(mCamera);
    mCamera->lookAt(mainNode->getPosition());
    camNode->setPosition(0, 200, 1000);
    mCamera->setPosition(0, 200, 1000);

    // create rotating axis point for bat
    batSpinNode = mainNode->createChildSceneNode();

    // create non-physical bat as a child of batSpinNode
    ent = mSceneMgr->createEntity("bat.mesh");
    ent->setMaterialName(matString);
    ent->setCastShadows(false);
    batNode = batSpinNode->createChildSceneNode("bat");
    batNode->roll(Ogre::Radian(Ogre::Real((float)M_PI/2)));
    batNode->setPosition(0,0,0);
    batNode->setScale(14,14,14);
    batNode->attachObject(ent);

    // create ground
    ent = mSceneMgr->createEntity("floor", "FloorPlane");
    ent->setMaterialName("Examples/Rockwall");
    ent->setCastShadows(false);
    mSceneMgr->getRootSceneNode()->createChildSceneNode("ground")->attachObject(ent);

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0,0,0));
    btScalar mass(0.0);
    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    // lathe - this plane isnt going to be moving so i dont care about setting the motion state.
    // using motionstate is recommended, it provides interpolation capabilities,
    // and only synchronizes 'active' objects.
    btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
                                                                   btVector3(0,10,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRBCI(mass, myMotionState, groundShape,
                                                               btVector3(0, 0, 0));
    groundRBCI.m_friction = 1.0f;
    btRigidBody* groundBody = new btRigidBody(groundRBCI);
    groundBody->setUserPointer(mSceneMgr->getSceneNode("ground"));
    groundBody->setRestitution(0.8f);
    //add the body to the dynamics world
    bWorld->addRigidBody(groundBody);
    environment.insert(groundBody);

    // create target blocks
    for(int i = -4; i < 5; ++i)
        for(int j = 0; j < 7; ++j)
        {
            bool pointBlock = j!=0 and (2+i+j)%2;
            ent = mSceneMgr->createEntity(Ogre::SceneManager::PT_CUBE);
            ent->setMaterialName(pointBlock ? "Examples/10PointBlock" : "Examples/Water3");
            ent->setCastShadows(true);
            node = mSceneMgr->getRootSceneNode()->createChildSceneNode((pointBlock ? "point" : "block")
                                                                       +std::to_string(i)
                                                                       +" "
                                                                       +std::to_string(j));
            node->setPosition(i*100, j*100, -500);
            node->attachObject(ent);

            btCollisionShape* blockShape =  new btBoxShape(btVector3(50.0f,50.0f,50.0f));
            btDefaultMotionState* blockMotionState =
                        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                 btVector3(i*100, j*100+25, -500)));
            btScalar bmass (10.0);
            btVector3 fallInertia(0, 0, 0);
            blockShape->calculateLocalInertia(bmass, fallInertia);
            btRigidBody::btRigidBodyConstructionInfo blockRBCI(bmass, blockMotionState,blockShape,fallInertia);
            btRigidBody* blockBody = new btRigidBody(blockRBCI);
            blockBody->setUserPointer(node);
            blockBody->setRestitution(0.8f);
            bWorld->addRigidBody(blockBody);
            rigidBodies.insert(blockBody);
        }
    ent = mSceneMgr->createEntity("ogrehead.mesh");
    ent->setCastShadows(true);
    node = mSceneMgr->getRootSceneNode()->createChildSceneNode("ogre");
    node->attachObject(ent);
    node->setScale(4,4,4);
    Vector3 pos(0,250,0);
    node->setPosition(pos);
    btCollisionShape* blockShape =  new btBoxShape(btVector3(50.0f,50.0f,50.0f));
    btDefaultMotionState* blockMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                         btVector3(pos.x,pos.y,pos.z)));
    btScalar bmass (100.0);
    btVector3 fallInertia(0, 0, 0);
    blockShape->calculateLocalInertia(bmass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo blockRBCI(bmass, blockMotionState,blockShape,fallInertia);
    btRigidBody* blockBody = new btRigidBody(blockRBCI);
    blockBody->setUserPointer(node);
    blockBody->setRestitution(0.8f);
    bWorld->addRigidBody(blockBody);
    rigidBodies.insert(blockBody);
    AI *ogreAI = new AI(blockBody,"ogre");
    AIObjects.push_back(ogreAI);
}
//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Random seed
        srand(time(NULL));


        // Create application object
        BattingSimulator app;
        bApp = &app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
