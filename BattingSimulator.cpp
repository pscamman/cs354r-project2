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
Uint8*        audio_pos;  // global pointer to the audio buffer to be played
Uint32        audio_len;  // remaining length of the sample we have to play
Uint8*        wav_buffer; // buffer containing our audio file
Uint32        wav_length; // length of our sample
SDL_AudioSpec wav_spec;   // the specs of our piece of music

Ogre::SceneNode* testNode;

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

    mCamera->setPosition(Vector3(0, 500, 500));

    // Create the ground mesh
    Plane p;
    p.normal = Vector3::UNIT_Y;
    p.d = 0;
    MeshManager::getSingleton().createPlane("FloorPlane",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,10000,10000,1,1,true,1,5,5,Vector3::UNIT_Z);

    // Create the entities, scene nodes, and bullet objects
    Entity*    ent;
    SceneNode* node;
    String matString = "Core/StatsBlockBorder/Up";

    // create non-physical mouse interactive object
    ent = mSceneMgr->createEntity("MOUSE_TEST", Ogre::SceneManager::PT_SPHERE);
    ent->setMaterialName(matString);
    ent->setCastShadows(true);
    testNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    testNode->setPosition(0, 500, 0);
    testNode->setScale(.19, .19, .19);
    testNode->attachObject(ent);

    // create ground
    ent = mSceneMgr->createEntity("floor", "FloorPlane");
    ent->setMaterialName("Examples/Rockwall");
    ent->setCastShadows(false);
    mSceneMgr->getRootSceneNode()->createChildSceneNode("Ground")->attachObject(ent);

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
    btRigidBody* groundBody = new btRigidBody(groundRBCI);
    groundBody->setUserPointer(mSceneMgr->getSceneNode("Ground"));
    groundBody->setRestitution(0.8f);
    //add the body to the dynamics world
    bWorld->addRigidBody(groundBody);
    environment.push_back(groundBody);

    // create target blocks
    for(int i = -2; i < 3; ++i)
        for(int j = 0; j < 6; ++j)
        {
            ent = mSceneMgr->createEntity(Ogre::SceneManager::PT_CUBE);
            ent->setMaterialName(matString);
            ent->setCastShadows(true);
            node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
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
            rigidBodies.push_back(blockBody);
        }
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

        // SDL sound setup
        // the specs, length and buffer of our wav are filled
        // SOUND_PATH in BaseApplication.h defines
        if(SDL_LoadWAV(SOUND_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL )
        {
            return 1;
        }
        // set the callback function
        wav_spec.callback = my_audio_callback;
        wav_spec.userdata = NULL;
        // set our global static variables
        audio_pos = wav_buffer; // copy sound buffer
        audio_len = wav_length; // copy file length

        /* Open the audio device */
        if(SDL_OpenAudio(&wav_spec, NULL) < 0)
        {
            exit(-1);
        }

        // Create application object
        BattingSimulator app;

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

        // wait until we're done playing
        while(audio_len > 0)
        {
            SDL_Delay(100);
        }

        // shut everything down
        SDL_CloseAudio();
        SDL_FreeWAV(wav_buffer);

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
