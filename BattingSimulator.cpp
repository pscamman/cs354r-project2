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
#include "BaseApplication.h"

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

    /*************************************************************************************
    * Begin GUI Creation Block                                                           *
    * TODO: Pull this into a class                                                       *
    *************************************************************************************/
    // Init GUI System
    CEGUI::OgreRenderer* mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

    // Point CEGUI to default resource locations specified in resources.cfg
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // Create a new window manager and a new window for all the GUI components
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");



    // Create the title screen
    CEGUI::Texture* title_texture;
    CEGUI::BasicImage* title_image;

    CEGUI::Window *title = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/title");
    title->setSize(CEGUI::USize(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
    title->setVisible(true);

    title_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("titlet", "title.jpg", "");
    title_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","titlei"));

    title_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), title_texture->getOriginalDataSize()));
    title_image->setTexture(title_texture);

    sheet->addChild(title);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    title->setProperty("NormalImage", "titlei");

    // Create the image for player 1's score window
    CEGUI::Texture* scorer_texture;
    CEGUI::BasicImage* scorer_image;

    CEGUI::Window *scorer = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/scorer");
    scorer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
    scorer->setVisible(false);

    scorer_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scorert", "scorer.png", "");
    scorer_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scoreri"));

    scorer_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scorer_texture->getOriginalDataSize()));
    scorer_image->setTexture(scorer_texture);

    sheet->addChild(scorer);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scorer->setProperty("NormalImage", "scoreri");

    // Create the text for player 1's score window
    CEGUI::Window *scorer_text = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/ScorerText");
    scorer_text->setText("[colour='FF000000']0");
    scorer_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0))); 
    scorer_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.055f, 0), CEGUI::UDim(0.0f, 0)));
    scorer_text->setProperty("BackgroundEnabled", "false");
    scorer_text->setProperty("FrameEnabled", "false");
    scorer_text->setVisible(false);
    sheet->addChild(scorer_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create the image for player 2's score window
    CEGUI::Texture* scoreb_texture;
    CEGUI::BasicImage* scoreb_image;

    CEGUI::Window *scoreb = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/scoreb");
    scoreb->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scoreb->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1583f, 0), CEGUI::UDim(0.0f, 0)));
    scoreb->setVisible(false);

    scoreb_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scorebt", "scoreb.png", "");
    scoreb_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scorebi"));

    scoreb_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scoreb_texture->getOriginalDataSize()));
    scoreb_image->setTexture(scoreb_texture);

    sheet->addChild(scoreb);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scoreb->setProperty("NormalImage", "scorebi");

    // Create the text for player 2's score window
    CEGUI::Window *scoreb_text = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/ScorebText");
    scoreb_text->setText("[colour='FF000000']0");
    scoreb_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0))); 
    scoreb_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2133f, 0), CEGUI::UDim(0.0f, 0)));
    scoreb_text->setProperty("BackgroundEnabled", "false");
    scoreb_text->setProperty("FrameEnabled", "false");
    scoreb_text->setVisible(false);
    sheet->addChild(scoreb_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create the image for player 3's score window 0.158333333
    CEGUI::Texture* scoreg_texture;
    CEGUI::BasicImage* scoreg_image;

    CEGUI::Window *scoreg = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/scoreg");
    scoreg->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scoreg->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3167f, 0), CEGUI::UDim(0.0f, 0)));
    scoreg->setVisible(false);

    scoreg_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scoregt", "scoreg.png", "");
    scoreg_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scoregi"));

    scoreg_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scoreg_texture->getOriginalDataSize()));
    scoreg_image->setTexture(scoreg_texture);

    sheet->addChild(scoreg);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scoreg->setProperty("NormalImage", "scoregi");

    // Create the text for player 3's score window
    CEGUI::Window *scoreg_text = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/scoregText");
    scoreg_text->setText("[colour='FF000000']0");
    scoreg_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0))); 
    scoreg_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.37167f, 0), CEGUI::UDim(0.0f, 0)));
    scoreg_text->setProperty("BackgroundEnabled", "false");
    scoreg_text->setProperty("FrameEnabled", "false");
    scoreg_text->setVisible(false);
    sheet->addChild(scoreg_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create the image for player 4's score window
    CEGUI::Texture* scorey_texture;
    CEGUI::BasicImage* scorey_image;

    CEGUI::Window *scorey = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/scorey");
    scorey->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorey->setPosition(CEGUI::UVector2(CEGUI::UDim(0.475f, 0), CEGUI::UDim(0.0f, 0)));
    scorey->setVisible(false);

    scorey_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scoreyt", "scorey.png", "");
    scorey_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scoreyi"));

    scorey_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scorey_texture->getOriginalDataSize()));
    scorey_image->setTexture(scorey_texture);

    sheet->addChild(scorey);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scorey->setProperty("NormalImage", "scoreyi");

    // Create the text for player 4's score window
    CEGUI::Window *scorey_text = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/scoreyText");
    scorey_text->setText("[colour='FF000000']0");
    scorey_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0))); 
    scorey_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.53f, 0), CEGUI::UDim(0.0f, 0)));
    scorey_text->setProperty("BackgroundEnabled", "false");
    scorey_text->setProperty("FrameEnabled", "false");
    scorey_text->setVisible(false);
    sheet->addChild(scorey_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // CENTER OF SCREEN LOCATED AT 0.425f, 0.465f
    // Create the main menu
    // Create the option for single player
    CEGUI::Texture* splay_texture_up; 
    CEGUI::Texture* splay_texture_down; 
    CEGUI::Texture* splay_texture_hover; 

    CEGUI::BasicImage* splay_image_up;
    CEGUI::BasicImage* splay_image_down;
    CEGUI::BasicImage* splay_image_hover;

    CEGUI::Window *splay = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/splay");
    splay->setSize(CEGUI::USize(CEGUI::UDim(0.524f, 0), CEGUI::UDim(0.252f, 0))); //<---------------------------------------------- WORKING HERE
    splay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.33f, 0)));
    splay->setVisible(true);

    splay_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("splayt_up", "splay-up.png", "");
    splay_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("splayt_down", "splay-down.png", "");
    splay_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("splayt_hover", "splay-hover.png", "");

    splay_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","splayi_up"));
    splay_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","splayi_down"));
    splay_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","splayi_hover"));

    splay_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), splay_texture_up->getOriginalDataSize()));
    splay_image_up->setTexture(splay_texture_up);
    splay_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), splay_texture_down->getOriginalDataSize()));
    splay_image_down->setTexture(splay_texture_down);
    splay_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), splay_texture_hover->getOriginalDataSize()));
    splay_image_hover->setTexture(splay_texture_hover);

    sheet->addChild(splay);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    splay->setProperty("NormalImage", "splayi_up");
    splay->setProperty("HoverImage", "splayi_hover");
    splay->setProperty("PushedImage", "splayi_down");

    // Create the option for single player
    CEGUI::Texture* mplay_texture_up; 
    CEGUI::Texture* mplay_texture_down; 
    CEGUI::Texture* mplay_texture_hover; 

    CEGUI::BasicImage* mplay_image_up;
    CEGUI::BasicImage* mplay_image_down;
    CEGUI::BasicImage* mplay_image_hover;

    CEGUI::Window *mplay = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/mplay");
    mplay->setSize(CEGUI::USize(CEGUI::UDim(0.490f, 0), CEGUI::UDim(0.252f, 0))); //<---------------------------------------------- WORKING HERE
    mplay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f, 0), CEGUI::UDim(0.50f, 0)));
    mplay->setVisible(true);

    mplay_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("mplayt_up", "mplay-up.png", "");
    mplay_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("mplayt_down", "mplay-down.png", "");
    mplay_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("mplayt_hover", "mplay-hover.png", "");

    mplay_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","mplayi_up"));
    mplay_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","mplayi_down"));
    mplay_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","mplayi_hover"));

    mplay_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), mplay_texture_up->getOriginalDataSize()));
    mplay_image_up->setTexture(mplay_texture_up);
    mplay_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), mplay_texture_down->getOriginalDataSize()));
    mplay_image_down->setTexture(mplay_texture_down);
    mplay_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), mplay_texture_hover->getOriginalDataSize()));
    mplay_image_hover->setTexture(mplay_texture_hover);

    sheet->addChild(mplay);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    mplay->setProperty("NormalImage", "mplayi_up");
    mplay->setProperty("HoverImage", "mplayi_hover");
    mplay->setProperty("PushedImage", "mplayi_down");

    // Create Image for pause menu (We need a separate image for each button state but only 1 here)
    CEGUI::Texture* pause_texture;
    CEGUI::BasicImage* pause_image;

    CEGUI::Window *pause = wmgr.createWindow("TaharezLook/ImageButton", "CEGUIDemo/pause");
    pause->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    pause->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425f, 0), CEGUI::UDim(0.465f, 0)));
    pause->setVisible(false);

    pause_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("pauset", "pause.png", "");
    pause_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","pausei"));

    pause_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), pause_texture->getOriginalDataSize()));
    pause_image->setTexture(pause_texture);

    sheet->addChild(pause);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    pause->setProperty("NormalImage", "pausei");

    // Create the text for the pause window (yes this has to be done separately OMFG)
    CEGUI::Window *pause_text = wmgr.createWindow("TaharezLook/StaticText", "CEGUIDemo/ScoreText");
    pause_text->setText("Pause");
    pause_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0))); 
    pause_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.480f, 0), CEGUI::UDim(0.465f, 0)));
    pause_text->setProperty("BackgroundEnabled", "false");
    pause_text->setProperty("FrameEnabled", "false");
    pause_text->setVisible(false);
    sheet->addChild(pause_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);


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
