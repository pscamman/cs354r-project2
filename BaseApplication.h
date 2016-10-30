/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.h
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

#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#ifndef BT_TIMESTEP
#define BT_TIMESTEP 1.0f/100.0f
#endif

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <btBulletDynamicsCommon.h>
// ------------- our includes ------------
// ogre
#include <OgrePlane.h>
#include <OgreVector3.h>

// stl
#include <cmath>         /* abs, pow, sqrt  */
#include <string>        /* to_string       */
#include <stdlib.h>      /* srand, rand     */
#include <time.h>        /* time            */
#include <queue>         /* queue           */
#include <unordered_set> /* unordered_set   */
#include <vector>        /* vector          */
#include <cassert>

// SDL
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include "GameObject.h"
#include "AI.h"
// ---------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OIS/OISEvents.h>
#  include <OIS/OISInputManager.h>
#  include <OIS/OISKeyboard.h>
#  include <OIS/OISMouse.h>

#  include <OGRE/SdkTrays.h>
#  include <OGRE/SdkCameraMan.h>
#else
#  include <OISEvents.h>
#  include <OISInputManager.h>
#  include <OISKeyboard.h>
#  include <OISMouse.h>

#  include <SdkTrays.h>
#  include <SdkCameraMan.h>
#endif

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// D3D10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

//---------------------------------------------------------------------------

class BaseApplication : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener, OgreBites::SdkTrayListener
{
public:
    BaseApplication(void);
    virtual ~BaseApplication(void);

    virtual void go(void);

protected:
    virtual bool setup();
    virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void createScene(void) = 0; // Override me!
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual bool setupSound(void);
    virtual void closeSound(void);
    virtual bool addSound(const std::string& sound);
    virtual void playSound(int index);
    virtual void createResourceListener(void);
    virtual void loadResources(void);
    virtual void createBulletSim(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    // Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);

    Ogre::Root*                 mRoot;
    Ogre::Camera*               mCamera;
    Ogre::SceneManager*         mSceneMgr;
    Ogre::RenderWindow*         mWindow;
    Ogre::String                mResourcesCfg;
    Ogre::String                mPluginsCfg;

    Ogre::SceneNode* mainNode;
    Ogre::SceneNode* camSpinNode;
    Ogre::SceneNode* camNode;
    Ogre::SceneNode* batSpinNode;
    Ogre::SceneNode* batNode;
    btRigidBody*     batBody;
    btHingeConstraint* batHinge;

    float charge;
    float swing;
    bool  batCharge;
    bool  batSwing;
    Ogre::Vector3 cameraVelocity;
    Ogre::Real    yawPerSec;

    Ogre::OverlaySystem*        mOverlaySystem;

    // OgreBites
    OgreBites::InputContext     mInputContext;
    OgreBites::SdkTrayManager*	mTrayMgr;
    //OgreBites::SdkCameraMan*    mCameraMan;     	// Basic camera controller
    OgreBites::ParamsPanel*     mDetailsPanel;   	// Sample details panel
    bool                        mCursorWasVisible;	// Was cursor visible before dialog appeared?
    bool                        mShutDown;

    //OIS Input devices
    OIS::InputManager*          mInputManager;
    OIS::Mouse*                 mMouse;
    OIS::Keyboard*              mKeyboard;

    // Added for Mac compatibility
    Ogre::String                 m_ResourcePath;
    // OgreBullet
    btDefaultCollisionConfiguration*        collisionConfiguration;
    btCollisionDispatcher*                  dispatcher;
    btBroadphaseInterface*                  overlappingPairCache;
    btSequentialImpulseConstraintSolver*    solver;
    btDiscreteDynamicsWorld*                bWorld;
    std::unordered_set<btRigidBody*> rigidBodies;
    std::unordered_set<btRigidBody*> environment;
    std::queue<btRigidBody*>         balls;
    // btRigidBody* batRigidBody;

    std::vector<std::string> sounds = {"Bell.wav",
                                       "crane_bump.wav",
                                       "punch1.wav",
                                       "wow.wav"};
    std::vector<Mix_Chunk*>     mix_chunks;  // the chunks of audio
    Mix_Music *gMusic = NULL;

    std::vector<GameObject*> gameObjects;

#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif

public:
    friend void bulletCallback(btDynamicsWorld *world, btScalar timeStep);
};

//---------------------------------------------------------------------------

void bulletCallback(btDynamicsWorld *world, btScalar timeStep);

//---------------------------------------------------------------------------

extern BaseApplication* bApp;

//---------------------------------------------------------------------------

#endif // #ifndef __BaseApplication_h_

//---------------------------------------------------------------------------
