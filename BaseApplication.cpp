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

int score = 0;
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
    yawPerSec(0),
    gMusic(NULL),
    hosting(false)
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
  /*if(hosting)
    {
        auto rs = *begin(mRoot->getAvailableRenderers());
        mRoot->setRenderSystem(rs);
        rs->setConfigOption("Full Screen", "No");
        rs->setConfigOption("Video Mode", "640 x 480");
        mWindow = mRoot->initialise(true, "TutorialApplication Render Window");
        return true;
    }
    else*/ if(mRoot->showConfigDialog())
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
    for(auto rb : rigidBodies)
    {
        bWorld->removeRigidBody(rb);
        delete rb->getMotionState();
        delete rb->getCollisionShape();
        delete rb;
    }
    for(auto rb : environment)
    {
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
    if(SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        return false;
    for(const std::string& sound : sounds)
        if(not addSound("resource/audio/"+sound)) return false;
     //Load music
    gMusic = Mix_LoadMUS( "resource/audio/happyrock.mp3" );
    if( gMusic == NULL)
        return false;
    Mix_PlayMusic( gMusic, -1 );
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::addSound(const std::string& sound)
{
    mix_chunks.resize(mix_chunks.size()+1);
    return (mix_chunks.back() = Mix_LoadWAV(sound.c_str()));
}
//---------------------------------------------------------------------------
void BaseApplication::closeSound(void)
{
    // shut everything down
    for(Mix_Chunk* chunk : mix_chunks)
        Mix_FreeChunk(chunk);
    mix_chunks.resize(0);
    Mix_Quit();
}
//---------------------------------------------------------------------------
void BaseApplication::playSound(int index)
{
    Mix_PlayChannel(-1, mix_chunks[index], 0);
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

    if (setup())
        mRoot->startRendering();
    // Clean up
    closeSound();
    nMan.close();
    destroyScene();
}
//---------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    bool carryOn;

    std::cout << "Hosting? y/n" << std::endl;
    std::string h;
    while(h.compare("y") and h.compare("n"))
        std::getline(std::cin, h);

    hosting = h.compare("y") == 0;

    carryOn = nMan.initNetManager();
    if(!carryOn) return false;

    if(hosting)
    {
        nMan.addNetworkInfo(PROTOCOL_ALL, NULL, 51215);
        carryOn = nMan.startServer();
        if(!carryOn) return false;
    }
    else
    {
        std::string ip;
        do
        {
            std::cout << "Host IP?" << std::endl;
            nMan.close();
            carryOn = nMan.initNetManager();
            if(!carryOn) return false;
            std::getline(std::cin, ip);
            nMan.addNetworkInfo(PROTOCOL_ALL, ip.c_str(), 51215);
            carryOn = nMan.startClient();
        }
        while(!carryOn);
        std::cout << "Hostname: " << nMan.getHostname() << std::endl;
    }

    carryOn = setupSound();
    if(!carryOn) return false;

    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    carryOn = configure();
    if(!carryOn) return false;

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

    if(mGUI->isStarted && !mGUI->isPaused) {
        bWorld->stepSimulation(BT_TIMESTEP, 10);
        btTransform trans;
        for(auto rb : rigidBodies)
        {
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
        if(batBody)
        {
            auto orient  = batBody->getOrientation();
            batSpinNode->setOrientation(orient.getW(),
                                        orient.getX(),
                                        orient.getY(),
                                        orient.getZ());
        }
        if(!batSwing)
        {
            camSpinNode->yaw(Ogre::Radian(yawPerSec * evt.timeSinceLastFrame));
            Ogre::Vector3 currentMainPos = mainNode->getPosition();
            currentMainPos += cameraVelocity * evt.timeSinceLastFrame;
            mainNode->setPosition(currentMainPos);
        }

        float scale = (4+charge*3/5)*3;
        batNode->setScale(scale,scale,scale);
        for(int i = 0; i < AIObjects.size(); ++i){
            AIObjects[i]->patrol();
        }
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

        mGUI->updateP1Score(score);
        mGUI->updateTimer();
        if(mGUI->timebox_time == 0) {
           mGUI->setGOverScreenVisible(true);
           mGUI->isGameOver = true;
           mGUI->isStarted = false;
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
    /******************************************************************************
     ** CEGUI Handler for key events, do not delete!                             **
     ******************************************************************************/
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown((CEGUI::Key::Scan)arg.key);
    context.injectChar((CEGUI::Key::Scan)arg.text);
    // END CEGUI Handler

    if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

    if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
    {
        mTrayMgr->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_SPACE) {
        if(mGUI->isStarted) {
            if(!mGUI->isPaused) {
                mGUI->setPauseVisible((mGUI->isPaused = true));
            } else {
                mGUI->setPauseVisible((mGUI->isPaused = false));
            }
        }
        if(!mGUI->isStarted && mGUI->multiStarted) {
            mGUI->isStarted = true;
            mGUI->playerMessageVisible(false);
        }
        if(mGUI->isGameOver) {
            mGUI->isGameOver = false;
            mGUI->setGOverScreenVisible(false);
            mGUI->setTimerVisible(false);
            mGUI->setP1ScoreVisible(false);
            mGUI->setP2ScoreVisible(false);
            mGUI->setP3ScoreVisible(false);
            mGUI->setP4ScoreVisible(false);
            mGUI->setTitleScreenVisible(true);
        }
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
    else if (arg.key == OIS::KC_P)   // cycle polygon rendering mode
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
        yawPerSec =  M_PI/4;
    else if (arg.key == OIS::KC_E)
        yawPerSec = -M_PI/4;
    else if (arg.key == OIS::KC_R)
    {
        static int i = -1;
        Ogre::Entity*    ent;
        Ogre::SceneNode* sphereNode;
        Ogre::Vector3 pos = bApp->mainNode->getPosition();
        pos.x += 300;
        pos.y += 1000;
        pos.z -= 100;
        ent = mSceneMgr->createEntity("sphere"+std::to_string(++i), Ogre::SceneManager::PT_SPHERE);
        ent->setMaterialName("Examples/BumpyMetal");
        ent->setCastShadows(true);
        sphereNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("sphere"+std::to_string(++i));
        sphereNode->setPosition(pos);
        sphereNode->setScale(.5, .5, .5);
        Ogre::ParticleSystem* gn = mSceneMgr->createParticleSystem("GN"+std::to_string(++i), "Examples/GreenyNimbus");
        Ogre::SceneNode* particleNode = sphereNode->createChildSceneNode("Particle"+std::to_string(++i));
        particleNode->attachObject(gn);

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
        float v = std::min(1600.0f, 300.0f*charge+100.0f);
        ballBody->setUserPointer(sphereNode);
        ballBody->setRestitution(0.8f);
        bWorld->addRigidBody(ballBody);
        rigidBodies.insert(ballBody);
        balls.push(ballBody);
        if(balls.size() > 3)
        {
            auto ball = balls.front();
            balls.pop();
            rigidBodies.erase(ball);
            auto sn = static_cast<Ogre::SceneNode*>(ball->getUserPointer());
            sn->removeAndDestroyAllChildren();
            mSceneMgr->destroySceneNode(sn);
            bWorld->removeRigidBody(ball);
            delete ball->getMotionState();
            delete ball->getCollisionShape();
            delete ball;
        }
    } else if(arg.key == OIS::KC_M){
        if( Mix_PlayingMusic() == 0 )
                  //Play the music
            Mix_PlayMusic( gMusic, -1 );
        //If music is being played
        else
        {
            //If the music is paused
            if( Mix_PausedMusic() == 1 )
                //Resume the music
                Mix_ResumeMusic();
            //If the music is playing
            else
                //Pause the music
                Mix_PauseMusic();
        }
    }
    //mCameraMan->injectKeyDown(arg);
    
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::keyReleased(const OIS::KeyEvent &arg)
{
    /******************************************************************************
     ** CEGUI Handler for key events, do not delete!                             **
     ******************************************************************************/
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp((CEGUI::Key::Scan)arg.key);
    // END CEGUI KEY HANDLER

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
    /******************************************************************************
     ** CEGUI Handler for mouse movement, do not delete!                         **
     ******************************************************************************/
    CEGUI::System &sys = CEGUI::System::getSingleton();
    sys.getDefaultGUIContext().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    // Scroll wheel.
    if (arg.state.Z.rel)
        sys.getDefaultGUIContext().injectMouseWheelChange(arg.state.Z.rel / 120.0f);
    // END OF CEGUI HANDLER
    camNode->translate(0,0,arg.state.Z.rel*0.25f);
    return true;
}
//---------------------------------------------------------------------------
bool BaseApplication::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    /******************************************************************************
     ** CEGUI Handler for mouse movement, do not delete!                         **
     ******************************************************************************/
    //CEGUI::GUIContext &mPress = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(mGUI->convertButton(id));
    // END OF CEGUI HANDLER
    if(id == OIS::MB_Left && mGUI->isStarted && !mGUI->isPaused)
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
    /******************************************************************************
     ** CEGUI Handler for mouse movement, do not delete!                         **
     ******************************************************************************/
    //CEGUI::GUIContext &mRel = CEGUI::System::getSingleton().getDefaultGUIContext();
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(mGUI->convertButton(id));
    // END OF CEGUI HANDLER
    if(mTrayMgr->injectMouseUp(arg, id)) return true;
    //mCameraMan->injectMouseUp(arg, id);

    if(batCharge and id == OIS::MB_Left and mGUI->isStarted and !mGUI->isPaused)
    {
        batCharge = false;
        if(not batSwing)
        {
            batSwing  = true;
            Ogre::Vector3 pos = mainNode->getPosition();
            btCollisionShape* batShape =  new btBoxShape(btVector3(300.0f,50.0f,50.0f));
            btDefaultMotionState* batMotionState =
                        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                 btVector3(pos.x+200, pos.y, pos.z)));
            btScalar bmass (100.0);
            btVector3 fallInertia(0, 0, 0);
            batShape->calculateLocalInertia(bmass, fallInertia);
            btRigidBody::btRigidBodyConstructionInfo batRBCI(bmass, batMotionState,batShape,fallInertia);
            batBody = new btRigidBody(batRBCI);
            batBody->setUserPointer(batNode);
            batBody->setRestitution(0.8f);
            batBody->setAngularVelocity(btVector3(0, 4+charge*3/5*M_PI, 0));
            bWorld->addRigidBody(batBody);

            batHinge = new btHingeConstraint(*batBody,
                                             btVector3(-200, 0, 0),
                                             btVector3(0,1,0));
            bWorld->addConstraint(batHinge);
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
    bApp->mGUI->tMod += timeStep;
    if(bApp->batCharge)
    {
        bApp->charge += timeStep;
        if(bApp->charge > 5.0f)
            bApp->charge = 5.0f;
    }

    if(bApp->batSwing)
    {
        bApp->swing  += timeStep;
        if(bApp->swing > 1.0f)
        {
            if(bApp->batCharge)
                bApp->playSound(0);
            bApp->charge = 0.0f;
            bApp->swing  = 0.0f;
            bApp->batSwing = false;
            bApp->batSpinNode->setOrientation(0, 0, 0, 1);
            bApp->batSpinNode->yaw(Ogre::Radian(Ogre::Real((float)M_PI)));
            bApp->bWorld->removeConstraint(bApp->batHinge);
            delete bApp->batHinge;
            bApp->bWorld->removeRigidBody(bApp->batBody);
            delete bApp->batBody->getMotionState();
            delete bApp->batBody->getCollisionShape();
            delete bApp->batBody;
            bApp->batBody = nullptr;
        }
    }

    int numManifolds = bApp->bWorld->getDispatcher()->getNumManifolds();
    std::unordered_set<btRigidBody*> toRemove;
    bool ptisys = false;
    for(int i=0;i<numManifolds;++i)
    {
        btPersistentManifold* contactManifold =  bApp->bWorld->getDispatcher()->getManifoldByIndexInternal(i);
        btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
        btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

        bool sphereA;
        bool sphereB;
        bool blockA;
        bool blockB;
        bool groundA;
        bool groundB;
        bool pointA;
        bool pointB;
        bool batA, batB;
        bool ogreA, ogreB;
        ogreA = !static_cast<Ogre::SceneNode*>(obA->getUserPointer())->getName().substr(0,4).compare("ogre");
        ogreB = !static_cast<Ogre::SceneNode*>(obB->getUserPointer())->getName().substr(0,4).compare("ogre");
        sphereA = !static_cast<Ogre::SceneNode*>(obA->getUserPointer())->getName().substr(0,6).compare("sphere");
        sphereB = !static_cast<Ogre::SceneNode*>(obB->getUserPointer())->getName().substr(0,6).compare("sphere");
        blockA  = !static_cast<Ogre::SceneNode*>(obA->getUserPointer())->getName().substr(0,5).compare("block");
        blockB  = !static_cast<Ogre::SceneNode*>(obB->getUserPointer())->getName().substr(0,5).compare("block");
        groundA = !static_cast<Ogre::SceneNode*>(obA->getUserPointer())->getName().substr(0,6).compare("ground");
        groundB = !static_cast<Ogre::SceneNode*>(obB->getUserPointer())->getName().substr(0,6).compare("ground");
        pointA  = !static_cast<Ogre::SceneNode*>(obA->getUserPointer())->getName().substr(0,5).compare("point");
        pointB  = !static_cast<Ogre::SceneNode*>(obB->getUserPointer())->getName().substr(0,5).compare("point");
        batA    = !static_cast<Ogre::SceneNode*>(obA->getUserPointer())->getName().substr(0,3).compare("bat");
        batB    = !static_cast<Ogre::SceneNode*>(obB->getUserPointer())->getName().substr(0,3).compare("bat");
        if(sphereA and (blockB or pointB) or sphereB and (blockA or pointA))
        {
            int numContacts = contactManifold->getNumContacts();
            for(int j=0;j<numContacts;++j)
            {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.getAppliedImpulse()>0.f)
                {

                    bApp->playSound(1);
                    break;
                }
            }
        }
        if(groundA and pointB or groundB and pointA)
        {
            int numContacts = contactManifold->getNumContacts();
            for(int j=0;j<numContacts;++j)
            {
                btManifoldPoint& pt = contactManifold->getContactPoint(j);
                if (pt.getAppliedImpulse()>0.f)
                {
                    // bApp->AIObjects[0]->vulnerable();
                    toRemove.insert(static_cast<btRigidBody*>(pointA ? obA : obB));
                    score += 10;
                    bApp->playSound(3);
                    break;
                }
            }
        }
        if(batA and sphereB or batB and sphereA)
        {
            int numContacts = contactManifold->getNumContacts();
            if(numContacts){
                // ptisys = true;
                // Ogre::ParticleSystem* gn = bApp->mSceneMgr->createParticleSystem("pts", "Examples/PurpleFountain");
                // static_cast<Ogre::SceneNode*>(obA->getUserPointer())->createChildSceneNode("Particle")->attachObject(gn);
                bApp->playSound(2);
            }
            break;
        }
        if(ogreA and sphereB or ogreB and sphereA)
        {
            int numContacts = contactManifold->getNumContacts();
            if (numContacts)
            {
                for(int i = 0; i < bApp->AIObjects.size(); ++i){
                    if(bApp->AIObjects[i]->name == "ogre" ){
                        delete bApp->AIObjects[i];
                        bApp->AIObjects.erase(bApp->AIObjects.begin()+i);
                        score += 50;
                    }
                }
                toRemove.insert(static_cast<btRigidBody*>(ogreA ? obA : obB));
                bApp->playSound(3);
            }
        }
    }
    // if(ptisys)
    //     bApp->mSceneMgr->destroyParticleSystem("pts");
    for(auto rb : toRemove)
    {
        auto sn = static_cast<Ogre::SceneNode*>(rb->getUserPointer());
        sn->removeAndDestroyAllChildren();
        bApp->mSceneMgr->destroySceneNode(sn);
        bApp->rigidBodies.erase(rb);
        bApp->bWorld->removeRigidBody(rb);
        delete rb->getMotionState();
        delete rb->getCollisionShape();
        delete rb;
    }

}
//---------------------------------------------------------------------------
