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

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

//my includes
#include <OgrePlane.h>
#include <OgreVector3.h>
#include <cmath>         /* abs, pow    */
#include <string>        /* to_string   */
#include <stdlib.h>      /* srand, rand */
#include <time.h>        /* time        */
#include <vector>

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
    virtual void createResourceListener(void);
    virtual void loadResources(void);
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

    Ogre::OverlaySystem*        mOverlaySystem;

    // OgreBites
    OgreBites::InputContext     mInputContext;
    OgreBites::SdkTrayManager*	mTrayMgr;
    OgreBites::SdkCameraMan*    mCameraMan;     	// Basic camera controller
    OgreBites::ParamsPanel*     mDetailsPanel;   	// Sample details panel
    bool                        mCursorWasVisible;	// Was cursor visible before dialog appeared?
    bool                        mShutDown;

    //OIS Input devices
    OIS::InputManager*          mInputManager;
    OIS::Mouse*                 mMouse;
    OIS::Keyboard*              mKeyboard;

    // Added for Mac compatibility
    Ogre::String                 m_ResourcePath;

#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader m_StaticPluginLoader;
#endif
};

//---------------------------------------------------------------------------

class Sphere {

    typedef Ogre::Real      Real;
    typedef Ogre::SceneNode SceneNode;

    private:
        SceneNode*  _sn;

        Real        _vx;
        Real        _vy;
        Real        _vz;

        Real        _xmin;
        Real        _xmax;
        Real        _ymin;
        Real        _ymax;
        Real        _zmin;
        Real        _zmax;

        Real        _t;

        void bounce(const Real& min, const Real& max, Real& vel, const Real& pos) {
            if(pos < min and vel < 0 or
               pos > max and vel > 0)
                vel *= -.95;
        }

        void xbounce() {
            bounce(_xmin, _xmax, _vx, _sn->_getDerivedPosition().x);
        }

        void ybounce() {
            bounce(_ymin, _ymax, _vy, _sn->_getDerivedPosition().y);
        }

        void zbounce() {
            bounce(_zmin, _zmax, _vz, _sn->_getDerivedPosition().z);
        }

        void move() {
            _sn->translate(_vx*.0001, _vy*.0001, _vz*.0001);
            Real ypos = _sn->_getDerivedPosition().y;
            if(ypos < _ymin)
                _sn->translate(0, (_ymin-ypos)*.0001, 0);

            if(abs(ypos-_ymin) < 1.0 and abs(_vy) < .1) {
                _vx = _vx*.9999;
                _vy = 0;
                _vz = _vz*.9999;
            }
        }

    public:
        Sphere(SceneNode* node, Real xmin, Real xmax, Real ymin, Real ymax, Real zmin, Real zmax)
            : _sn(node),
              _xmin(xmin),
              _xmax(xmax),
              _ymin(ymin),
              _ymax(ymax),
              _zmax(zmax),
              _zmin(zmin) {
            _vx = (rand()%2*2-1)*rand()%160 + 40;
            _vy = (rand()%2*2-1)*rand()%160 + 40;
            _vz = (rand()%2*2-1)*rand()%160 + 40;
            _t  = 0;
        }

        void update(const Real& dt) {
            _t += dt;
            while(_t > .0001) {
                xbounce();
                ybounce();
                zbounce();

                _vy -= .1;

                move();

                _t -= .0001;
            }
        }

    void collision(Sphere& s2) {
        auto vec1 = this->_sn->_getDerivedPosition();
        auto vec2 =    s2._sn->_getDerivedPosition();
        if(vec1.distance(vec2) < 36.0 and
                ((vec2.x - vec1.x) * (s2._vx - this->_vx) < 0 or
                 (vec2.y - vec1.y) * (s2._vy - this->_vy) < 0 or
                 (vec2.z - vec1.z) * (s2._vz - this->_vz) < 0)) {
            std::swap(this->_vx, s2._vx);
            std::swap(this->_vy, s2._vy);
            std::swap(this->_vz, s2._vz);
        }
    }
};

class Spheres {
    private:
        std::vector<Sphere> _balls;

    public:
        void add(const Sphere& ball) {
            _balls.push_back(ball);
        }

        void update(const Ogre::Real& dt) {
            auto e = _balls.end();
            for(auto b1 = _balls.begin(); b1 != e; ++b1) {
                for(auto b2 = b1+1; b2 != e; ++b2)
                    b1->collision(*b2);
                b1->update(dt);
        }
    }
};

extern Spheres balls;

//---------------------------------------------------------------------------

#endif // #ifndef __BaseApplication_h_

//---------------------------------------------------------------------------