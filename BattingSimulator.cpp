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

//---------------------------------------------------------------------------
BattingSimulator::BattingSimulator(void)
{
}
//---------------------------------------------------------------------------
BattingSimulator::~BattingSimulator(void)
{
    // bWorld->removeRigidBody(ballBody);
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
    // light = mSceneMgr->createLight("MainLight3");
    // light->setPosition(199, 199, -199);
    // light->setCastShadows(true);
    // light->setDiffuseColour(ColourValue(0.8, 0, 0));
    // light = mSceneMgr->createLight("MainLight4");
    // light->setPosition(-199, 199, -199);
    // light->setCastShadows(true);
    // light->setDiffuseColour(ColourValue(0.8, 0, 0));

    mCamera->setPosition(Vector3(600, 300, 400));
    mCamera->lookAt(Vector3(50, 0, -50));

    // // Create the meshes
    // Plane p;
    // p.normal = Vector3::UNIT_Y;
    // p.d = 200;
    // MeshManager::getSingleton().createPlane("FloorPlane",
    //     ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //     p,1500,1500,1,1,true,1,5,5,Vector3::UNIT_Z);

    // p.normal = Vector3::NEGATIVE_UNIT_Y;
    // p.d = 200;
    // MeshManager::getSingleton().createPlane("CeilPlane",
    //     ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //     p,400,400,1,1,true,1,5,5,Vector3::NEGATIVE_UNIT_Z);

    // p.normal = Vector3::UNIT_Z;
    // p.d = 200;
    // MeshManager::getSingleton().createPlane("WallPlaneN",
    //     ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //     p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    // p.normal = Vector3::NEGATIVE_UNIT_X;
    // p.d = 200;
    // MeshManager::getSingleton().createPlane("WallPlaneE",
    //     ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //     p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    // p.normal = Vector3::NEGATIVE_UNIT_Z;
    // p.d = 200;
    // MeshManager::getSingleton().createPlane("WallPlaneS",
    //     ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //     p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    // p.normal = Vector3::UNIT_X;
    // p.d = 200;
    // MeshManager::getSingleton().createPlane("WallPlaneW",
    //     ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
    //     p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    // // Create the entities
    // SceneNode* rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    // Entity*    ent;
    // String matString = "Core/StatsBlockBorder/Up";

    // ent = mSceneMgr->createEntity("floor", "FloorPlane");
    // ent->setMaterialName(matString);
    // ent->setCastShadows(false);
    // rootNode->attachObject(ent);

    // ent = mSceneMgr->createEntity("ceil", "CeilPlane");
    // ent->setMaterialName(matString);
    // ent->setCastShadows(false);
    // rootNode->attachObject(ent);

    // ent = mSceneMgr->createEntity("wallN", "WallPlaneN");
    // ent->setMaterialName(matString);
    // ent->setCastShadows(false);
    // rootNode->attachObject(ent);

    // ent = mSceneMgr->createEntity("wallE", "WallPlaneE");
    // ent->setMaterialName(matString);
    // ent->setCastShadows(false);
    // rootNode->attachObject(ent);

    // ent = mSceneMgr->createEntity("wallS", "WallPlaneS");
    // ent->setMaterialName(matString);
    // ent->setCastShadows(false);
    // rootNode->attachObject(ent);

    // ent = mSceneMgr->createEntity("wallW", "WallPlaneW");
    // ent->setMaterialName(matString);
    // ent->setCastShadows(false);
    // rootNode->attachObject(ent);

    // ent = mSceneMgr->createEntity("ogrehead.mesh");
    // ogreNode->attachObject(ent);
    // SceneNode* sphereNode;
    // ent = mSceneMgr->createEntity("sphere", Ogre::SceneManager::PT_SPHERE);
    // ent->setMaterialName(matString);
    // ent->setCastShadows(true);
    // sphereNode = rootNode->createChildSceneNode();
    // sphereNode->setPosition(0, 0, 0);
    // sphereNode->setScale(.38, .38, .38);
    // sphereNode->attachObject(ent);
    // balls.add(::Sphere(sphereNode, -180, 180, -180, 180, -180, 180));



     ///create a few basic rigid bodies
     // start with ground plane, 1500, 1500
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0,-2,0));    
    btScalar mass(0.0);
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    // lathe - this plane isnt going to be moving so i dont care about setting the motion state
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,10,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(mass, myMotionState, groundShape, btVector3(0, 0, 0));    
    btRigidBody* body = new btRigidBody(groundRigidBodyCI);
    //add the body to the dynamics world
    //having seg fault
    std::cout << bWorld << std::endl;
    bWorld->addRigidBody(body);
    rigidBodies.push_back(body);

    // create a ball
    btCollisionShape* ballShape =  new btSphereShape(1);
    btDefaultMotionState* ballMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
    btScalar bmass (1.0);
        btVector3 fallInertia(0, 0, 0);
        ballShape->calculateLocalInertia(bmass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo ballRBCI(bmass, ballMotionState,ballShape,fallInertia);
    btRigidBody* ballBody = new btRigidBody(ballRBCI);
    bWorld->addRigidBody(ballBody);
    rigidBodies.push_back(ballBody);

}
//---------------------------------------------------------------------------

// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len)
{

    if (audio_len ==0)
        return;

    len = ( len > audio_len ? audio_len : len );
    //SDL_memcpy (stream, audio_pos, len);                  // simply copy from one buffer into the other
    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    audio_pos += len;
    audio_len -= len;
}

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

        /* Start playing */
        SDL_PauseAudio(0);

        // wait until we're don't playing
        while(audio_len > 0)
        {
            SDL_Delay(100);
        }

        // shut everything down
        SDL_CloseAudio();
        SDL_FreeWAV(wav_buffer);

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

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
