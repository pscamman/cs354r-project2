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

#include "TutorialApplication.h"

Spheres balls;

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
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

    // Create the meshes
    Plane p;
    p.normal = Vector3::UNIT_Y;
    p.d = 200;
    MeshManager::getSingleton().createPlane("FloorPlane",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,400,400,1,1,true,1,5,5,Vector3::UNIT_Z);

    p.normal = Vector3::NEGATIVE_UNIT_Y;
    p.d = 200;
    MeshManager::getSingleton().createPlane("CeilPlane",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,400,400,1,1,true,1,5,5,Vector3::NEGATIVE_UNIT_Z);

    p.normal = Vector3::UNIT_Z;
    p.d = 200;
    MeshManager::getSingleton().createPlane("WallPlaneN",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    p.normal = Vector3::NEGATIVE_UNIT_X;
    p.d = 200;
    MeshManager::getSingleton().createPlane("WallPlaneE",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    p.normal = Vector3::NEGATIVE_UNIT_Z;
    p.d = 200;
    MeshManager::getSingleton().createPlane("WallPlaneS",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    p.normal = Vector3::UNIT_X;
    p.d = 200;
    MeshManager::getSingleton().createPlane("WallPlaneW",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p,400,400,1,1,true,1,5,5,Vector3::UNIT_Y);

    // Create the entities
    SceneNode* rootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    Entity*    ent;
    String matString = "Core/StatsBlockBorder/Up";

    ent = mSceneMgr->createEntity("floor", "FloorPlane");
    ent->setMaterialName(matString);
    ent->setCastShadows(false);
    rootNode->attachObject(ent);

    ent = mSceneMgr->createEntity("ceil", "CeilPlane");
    ent->setMaterialName(matString);
    ent->setCastShadows(false);
    rootNode->attachObject(ent);

    ent = mSceneMgr->createEntity("wallN", "WallPlaneN");
    ent->setMaterialName(matString);
    ent->setCastShadows(false);
    rootNode->attachObject(ent);

    ent = mSceneMgr->createEntity("wallE", "WallPlaneE");
    ent->setMaterialName(matString);
    ent->setCastShadows(false);
    rootNode->attachObject(ent);

    ent = mSceneMgr->createEntity("wallS", "WallPlaneS");
    ent->setMaterialName(matString);
    ent->setCastShadows(false);
    rootNode->attachObject(ent);

    ent = mSceneMgr->createEntity("wallW", "WallPlaneW");
    ent->setMaterialName(matString);
    ent->setCastShadows(false);
    rootNode->attachObject(ent);

    // ent = mSceneMgr->createEntity("ogrehead.mesh");
    // ogreNode->attachObject(ent);

    SceneNode* sphereNode;
    for(int i = 0; i < 16; ++i) {
        ent = mSceneMgr->createEntity("sphere" + std::to_string(i), Ogre::SceneManager::PT_SPHERE);
        ent->setMaterialName(matString);
        ent->setCastShadows(true);
        sphereNode = rootNode->createChildSceneNode();
        sphereNode->setPosition(-90+60*(i%4), 150, -90+60*(i/4));
        sphereNode->setScale(.38, .38, .38);
        sphereNode->attachObject(ent);
        balls.add(::Sphere(sphereNode, -180, 180, -180, 180, -180, 180));
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

        // Create application object
        TutorialApplication app;

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
