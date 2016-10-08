/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
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

#include "BaseApplication.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <macUtils.h>
#endif

//---------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mTrayMgr(0),
    //mCameraMan(0),
    mDetailsPanel(0),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
    mOverlaySystem(0),
    charge(0),
    batCharge(false),
    batSwing(false),
    cameraVelocity(Ogre::Vector3::ZERO),
    yawPerSec(0)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
    m_ResourcePath = "";
#endif
}

//---------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
    if (mTrayMgr) delete mTrayMgr;
    //if (mCameraMan) delete mCameraMan;
    if (mOverlaySystem) delete mOverlaySystem;
    if (collisionConfiguration) delete collisionConfiguration;
    if (dispatcher) delete dispatcher;
    if (overlappingPairCache) delete overlappingPairCache;
    if (solver) delete solver;
    if (bWorld) delete bWorld;

    // Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

//---------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    // Show the configuration dialog and initialise the system.
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg.
    if(mRoot->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise.
        // Here we choose to let the system create a default rendering window by passing 'true'.
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

    // Initialize the OverlaySystem (changed for Ogre 1.9)
    mOverlaySystem = new Ogre::OverlaySystem();
    mSceneMgr->addRenderQueueListener(mOverlaySystem);
}
//---------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    mCamera->setNearClipDistance(5);

    //mCameraMan = new OgreBites::SdkCameraMan(mCamera);   // Create a default camera controller
}
//---------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem(pl);

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

    // Set initial mouse clipping size
    windowResized(mWindow);

    // Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    mInputContext.mKeyboard = mKeyboard;
    mInputContext.mMouse = mMouse;
    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    mTrayMgr->hideCursor();

    // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();

    mRoot->addFrameListener(this);
}
//---------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
    for(int i = 0; i < rigidBodies.size(); i++){
        btRigidBody* rb = rigidBodies.at(i);
        bWorld->removeRigidBody(rb);
        delete rb->getMotionState();
        delete rb->getCollisionShape();
        delete rb;
    }
    for(int i = 0; i < environment.size(); i++){
        btRigidBody* rb = environment.at(i);
        bWorld->removeRigidBody(rb);
        delete rb->getMotionState();
        delete rb->getCollisionShape();
        delete rb;
    }
}
//---------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}
//---------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
void BaseApplication::createBulletSim(void) {
         ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
         collisionConfiguration = new btDefaultCollisionConfiguration();

         ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
         dispatcher = new   btCollisionDispatcher(collisionConfiguration);

         ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
         overlappingPairCache = new btDbvtBroadphase();

         ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
         solver = new btSequentialImpulseConstraintSolver;

         bWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
         bWorld->setGravity(btVector3(0,-100,0));

         bWorld->setInternalTickCallback(bulletCallback);
    }
//---------------------------------------------------------------------------
bool BaseApplication::setupSound(void)
{
    // SDL sound setup
    // the specs, length and buffer of our wav are filled
    // SOUND_PATH in BaseApplication.h defines
    // set the callback function
    wav_spec.callback = audioCallback;
    wav_spec.userdata = NULL;

    /* Open the audio device */
    if(SDL_OpenAudio(&wav_spec, NULL) < 0)
    {
        exit(-1);
    }
    for(const std::string& sound : sounds)
        if(addSound(sound)) return false;
}
//---------------------------------------------------------------------------
void BaseApplication::closeSound(void)
{
        // wait until we're done playing
        while(audio_len > 0)
        {
            SDL_Delay(100);
        }

        // shut everything down
        SDL_CloseAudio();
        for(auto wav_buffer : wav_buffers)
            SDL_FreeWAV(wav_buffer);
}
//---------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{
}
//---------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}
//---------------------------------------------------------------------------
bool BaseApplication::addSound(const std::string& sound)
{
    wav_buffers.resize(wav_buffers.size()+1);
    wav_lengths.resize(wav_lengths.size()+1);
    return SDL_LoadWAV(sound.c_str(), &wav_spec, &wav_buffers.back(), &wav_lengths.back()) == NULL;
}
//---------------------------------------------------------------------------
void BaseApplication::playSound(int index)
{
    audio_pos = wav_buffers[index];
    audio_len = wav_lengths[index];
    SDL_PauseAudio(0);
}
//---------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources_d.cfg";
    mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
#else
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
#endif
#else
#ifndef OGRE_STATIC_LIB
    mResourcesCfg = m_ResourcePath + "resources.cfg";
    mPluginsCfg = m_ResourcePath + "plugins.cfg";
#else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
#endif
#endif

    if (!setup())
        return;
    mRoot->startRendering();
    // Clean up
    closeSound();
    destroyScene();
}
//---------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    bool carryOn;

    carryOn = setupSound();
    if (!carryOn) return false;

    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();
    createCamera();
    createViewports();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();
    // Create Bullet world and hook callback in
    createBulletSim();
    // Create the scene
    createScene();

    createFrameListener();

    return true;
};
//---------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;

    // Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    bWorld->stepSimulation(BT_TIMESTEP, 10);
    btTransform trans;
    for(int i = 0; i < rigidBodies.size(); i++)
    {
        btRigidBody* rb = rigidBodies.at(i);
        rb->getMotionState()->getWorldTransform(trans);
        Ogre::SceneNode *sn = static_cast<Ogre::SceneNode *> (rb->getUserPointer());
        auto origin = trans.getOrigin();
        sn->setPosition(origin.getX(),
                        origin.getY(),
                        origin.getZ());
        auto orient = rb->getOrientation();
        sn->setOrientation(orient.getW(),
                           orient.getX(),
                           orient.getY(),
                           orient.getZ());
    }
    testNode->yaw(Ogre::Radian(yawPerSec * evt.timeSinceLastFrame));
    Ogre::Vector3 currentBallPos = testNode->getPosition();
    currentBallPos += cameraVelocity * evt.timeSinceLastFrame;
    testNode->setPosition(currentBallPos);

    float scale = (4+charge)*7;
    batNode->setScale(scale,scale,scale);

    mTrayMgr->frameRenderingQueued(evt);

    if (!mTrayMgr->isDialogVisible())
    {
        //mCameraMan->frameRenderingQueued(evt);   // If dialog isn't up, then update the camera
        if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
        }
    }


    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
    {
        if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
            mDetailsPanel->show();
        }
        else
        {
            mTrayMgr->removeWidgetFromTray(mDetailsPanel);
            mDetailsPanel->hide();
        }
    }
    else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::TextureFilterOptions tfo;
        unsigned int aniso;

        switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
        {
        case 'B':
            newVal = "Trilinear";
            tfo = Ogre::TFO_TRILINEAR;
            aniso = 1;
            break;
        case 'T':
            newVal = "Anisotropic";
            tfo = Ogre::TFO_ANISOTROPIC;
            aniso = 8;
            break;
        case 'A':
            newVal = "None";
            tfo = Ogre::TFO_NONE;
            aniso = 1;
            break;
        default:
            newVal = "Bilinear";
            tfo = Ogre::TFO_BILINEAR;
            aniso = 1;
        }

        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
        Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
        mDetailsPanel->setParamValue(9, newVal);
    }
    else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
    {
        Ogre::String newVal;
        Ogre::PolygonMode pm;

        switch (mCamera->getPolygonMode())
        {
        case Ogre::PM_SOLID:
            newVal = "Wireframe";
            pm = Ogre::PM_WIREFRAME;
            break;
        case Ogre::PM_WIREFRAME:
            newVal = "Points";
            pm = Ogre::PM_POINTS;
            break;
        default:
            newVal = "Solid";
            pm = Ogre::PM_SOLID;
        }

        mCamera->setPolygonMode(pm);
        mDetailsPanel->setParamValue(10, newVal);
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
    else if (arg.key == OIS::KC_W)
        cameraVelocity.y = 200;
    else if (arg.key == OIS::KC_S)
        cameraVelocity.y = -200;
    else if (arg.key == OIS::KC_A)
        cameraVelocity.x = -200;
    else if (arg.key == OIS::KC_D)
        cameraVelocity.x = 200;
    else if (arg.key == OIS::KC_Q)
        yawPerSec = -M_PI/4;
    else if (arg.key == OIS::KC_E)
        yawPerSec =  M_PI/4;
    //mCameraMan->injectKeyDown(arg);
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::keyReleased(const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_W)
        cameraVelocity.y = 0;
    else if (arg.key == OIS::KC_S)
        cameraVelocity.y = 0;
    else if (arg.key == OIS::KC_A)
        cameraVelocity.x = 0;
    else if (arg.key == OIS::KC_D)
        cameraVelocity.x = 0;
    else if (arg.key == OIS::KC_Q)
        yawPerSec = 0;
    else if (arg.key == OIS::KC_E)
        yawPerSec = 0;
    //mCameraMan->injectKeyUp(arg);
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::mouseMoved(const OIS::MouseEvent &arg)
{
    //batNode->translate(arg.state.X.rel*0.25f, -arg.state.Y.rel*0.25f, 0);
    //mCamera->lookAt(testNode->getPosition());

    // if (mTrayMgr->injectMouseMove(arg)) return true;
    // mCameraMan->injectMouseMove(arg);
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
        batCharge = true;
        if(not batSwing)
            playSound(0);
    }

    //if (mTrayMgr->injectMouseDown(arg, id)) return true;
    //mCameraMan->injectMouseDown(arg, id);
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(mTrayMgr->injectMouseUp(arg, id)) return true;
    //mCameraMan->injectMouseUp(arg, id);

    if(batCharge and id == OIS::MB_Left)
    {
        batCharge = false;
        if(not batSwing)
        {
            batSwing  = true;
            playSound(1);
        }
    }
}
//---------------------------------------------------------------------------
// Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
//---------------------------------------------------------------------------
// Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    // Only close for window that created OIS (the main window in these demos)
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
//---------------------------------------------------------------------------
//callback to have handle on bullet time
void bulletCallback(btDynamicsWorld *world, btScalar timeStep)
{
    if(bApp->batCharge)
    {
        bApp->charge += timeStep;
        if(bApp->charge > 5.0f)
            bApp->charge = 5.0f;
    }

    if(bApp->batSwing)
    {
        bApp->swing  += timeStep;
        Ogre::Entity* ballEnt = static_cast<Ogre::Entity*>(bApp->testNode->getAttachedObject(0));
        if(bApp->swing < 1.0f)
        {
            bApp->batNode->pitch(Ogre::Radian(Ogre::Real(2*3.1415927*timeStep)));
            if(ballEnt->getVisible() and bApp->swing > 0.2f)
            {
                ballEnt->setVisible(false);
                Ogre::Entity*    ent;
                Ogre::SceneNode* sphereNode;
                Ogre::Vector3    direction = bApp->mCamera->getDerivedDirection();
                direction.y = 0;
                direction.normalise();
                static int i = -1;
                Ogre::Vector3 pos = bApp->testNode->getPosition();
                ent = bApp->mSceneMgr->createEntity("sphere"+std::to_string(++i), Ogre::SceneManager::PT_SPHERE);
                ent->setMaterialName("Core/StatsBlockBorder/Up");
                ent->setCastShadows(true);
                sphereNode = bApp->mSceneMgr->getRootSceneNode()->createChildSceneNode();
                sphereNode->setPosition(pos);
                sphereNode->setScale(.25, .25, .25);
                sphereNode->attachObject(ent);

                btCollisionShape* ballShape =  new btSphereShape(25);
                btDefaultMotionState* ballMotionState =
                    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                             btVector3(pos.x, pos.y, pos.z)));
                btScalar bmass (1.0);
                btVector3 fallInertia(0, 0, 0);
                ballShape->calculateLocalInertia(bmass, fallInertia);
                btRigidBody::btRigidBodyConstructionInfo ballRBCI(bmass, ballMotionState,ballShape,fallInertia);
                btRigidBody* ballBody = new btRigidBody(ballRBCI);
                float v = std::min(1600.0f, 300.0f*(bApp->charge)+100.0f);
                ballBody->setLinearVelocity(btVector3(direction.x*v, 0, direction.z*v));
                ballBody->setUserPointer(sphereNode);
                ballBody->setRestitution(0.8f);
                bApp->bWorld->addRigidBody(ballBody);
                bApp->rigidBodies.push_back(ballBody);
            }
        }
        else
        {
            if(bApp->batCharge)
                bApp->playSound(1);
            bApp->charge = 0.0f;
            bApp->swing  = 0.0f;
            bApp->batSwing = false;
            ballEnt->setVisible(true);
        }
    }
}

//---------------------------------------------------------------------------
// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void audioCallback(void *userdata, Uint8 *stream, int len)
{

    if (bApp->audio_len == 0)
        return;

    len = ( len > bApp->audio_len ? bApp->audio_len : len );
    //SDL_memcpy(stream, bApp->audio_pos, len);                  // simply copy from one buffer into the other
    SDL_MixAudio(stream, bApp->audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    bApp->audio_pos += len;
    bApp->audio_len -= len;
}

//---------------------------------------------------------------------------
