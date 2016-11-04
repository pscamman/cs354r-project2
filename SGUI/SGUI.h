#pragma once

#ifndef __SGUI_h_
#define __SGUI_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include <iostream>
class SGUI
{
public:
    CEGUI::System*          mSys;                                   // System for handling mosue events
    bool                    isPaused;                               // Used to determine whether the game is paused or not
    bool                    isStarted;                              // Used to determine whether the game is started or not
    SGUI();                                                         // Init all GUI components
    void setTitleScreenVisible(bool isVisible);                     // Sets the title screen and all components as visible or invisible
    void setP1ScoreVisible(bool isVisible);                         // Sets player 1's score window and text as visible or invisible
    void setP2ScoreVisible(bool isVisible);                         // Sets player 2's score window and text as visible or invisible
    void setP3ScoreVisible(bool isVisible);                         // Sets player 3's score window and text as visible or invisible
    void setP4ScoreVisible(bool isVisible);                         // Sets player 4's score window and text as visible or invisible
    void setPauseVisible(bool isVisible);                           // Sets the pause menu and text as visible or invisible
    void updateP1Score(int newScore);                               // Sets the score for player 1
    void updateP2Score(int newScore);                               // Sets the score for player 2
    void updateP3Score(int newScore);                               // Sets the score for player 3
    void updateP4Score(int newScore);                               // Sets the score for player 4
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);  // Convert from OIS mouse event to CEGUI mouse event
private:
    CEGUI::OgreRenderer*    mRenderer;              // Renderer for GUI system (helps draw components)
    CEGUI::WindowManager*   wmgr;                  // Window manager used to place components on the screen
    CEGUI::Window*          sheet;                  // The GUI window
    CEGUI::Texture*         title_texture;          // Used to hold the image for the title screen as a texture
    CEGUI::BasicImage*      title_image;            // Itermediate variable used to convert png file to texture
    CEGUI::Window*          title;                  // GUI Component representing the title screen image 
    CEGUI::Texture*         splay_texture_up;       // Used to hold the image for the up-state of the single play button
    CEGUI::Texture*         splay_texture_down;     // Used to hold the image for the down-state of the single play button
    CEGUI::Texture*         splay_texture_hover;    // Used to hold the image for the hover-state of the single play button
    CEGUI::BasicImage*      splay_image_up;         // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      splay_image_down;       // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      splay_image_hover;      // Itermediate variable used to convert png file to texture
    CEGUI::Window*          splay;                  // GUI Component representing the single player button
    CEGUI::Texture*         mplay_texture_up;       // Used to hold the image for the up-state of the multi play button
    CEGUI::Texture*         mplay_texture_down;     // Used to hold the image for the down-state of the multi play button
    CEGUI::Texture*         mplay_texture_hover;    // Used to hold the image for the hover-state of the multi play button
    CEGUI::BasicImage*      mplay_image_up;         // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      mplay_image_down;       // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      mplay_image_hover;      // Itermediate variable used to convert png file to texture
    CEGUI::Window*          mplay;                  // GUI Component represeting the multi player button
    CEGUI::Texture*         scorer_texture;         // Used to hold the image for player 1's score window as a texture
    CEGUI::BasicImage*      scorer_image;           // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scorer;                 // GUI Component representing player 1's score window
    CEGUI::Window*          scorer_text;            // GUI Component representing the text in player 1's score window
    CEGUI::Texture*         scoreb_texture;         // Used to hold the image for player 2's score window as a texture
    CEGUI::BasicImage*      scoreb_image;           // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scoreb;                 // GUI Component representing player 2's score window
    CEGUI::Window*          scoreb_text;            // GUI Component representing the text in player 2's score window
    CEGUI::Texture*         scoreg_texture;         // Used to hold the image for player 3's score window as a texture
    CEGUI::BasicImage*      scoreg_image;           // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scoreg;                 // GUI Component representing player 3's score window
    CEGUI::Window*          scoreg_text;            // GUI Component representing the text in player 3's score window
    CEGUI::Texture*         scorey_texture;         // Used to hold the image for player 4's score window as a texture
    CEGUI::BasicImage*      scorey_image;           // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scorey;                 // GUI Component representing player 4's score window
    CEGUI::Window*          scorey_text;            // GUI Component representing the text in player 4's score window
    CEGUI::Texture*         pause_texture;          // Used to hold the image for the pause window as a texture
    CEGUI::BasicImage*      pause_image;            // Itermediate variable used to convert png file to texture
    CEGUI::Window*          pause;                  // GUI Component representing the pause window
    CEGUI::Window*          pause_text;             // GUI Component representing the text in the pause window

    bool                    singlePlay(const CEGUI::EventArgs &e);  // event for the singlePlayerButton
};

#endif // #ifndef __Sphere_h_