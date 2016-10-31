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
#include <string>
class SGUI
{
public:
    CEGUI::System*          mSys;                                   // System for handling mosue events
    bool                    isPaused;                               // Used to determine whether the game is paused or not
    bool                    isGameOver;                             // Used to determine whether the game has ended or not
    bool                    isStarted;                              // Used to determine whether the game is started or not
    bool                    multiStarted;                           // used to determine whether a multiplayer game is ready to be started or not
    std::string             currentAddress;                         // Used to hold the IP Address for the server program (or the game to connect to in the clients case)
    int                     nFlags;                                 // Flag used to determine who is hosting and who is the client HOST = 1, CLIENT = 0 UNSTARTED = -1
    int                     timebox_time;                           // holds the numeric time for the timer
    float                   tMod;                                   // time modifier used to correct for differences between bullet's timeStep and actual time (in seconds)
    SGUI();                                                         // Init all GUI components
    void setTitleScreenVisible(bool isVisible);                     // Sets the title screen and all components as visible or invisible
    void setGOverScreenVisible(bool isVisible);                     // Sets the game over screen as visible or invisible
    void setPauseVisible(bool isVisible);                           // Sets the pause menu and text as visible or invisible
    void playerMessageVisible(bool isVisible);                      // sets the player message visible or invisible
    void setMultiConVisible(bool isVisible);                        // Sets the multi-player connect window visible/invisible
    void setConnectingVisible(bool isVisible);                      // Sets the connecting dialog box visible or invisible
    void setHostVisible(bool isVisible);                            // Set the client/host connectivity window visible or invisible as host
    void setClientVisible(bool isVisible);                          // Set the client/host connectivity window visible or invisible as client
    void setP1ScoreVisible(bool isVisible);                         // Sets player 1's score window and text as visible or invisible
    void setP2ScoreVisible(bool isVisible);                         // Sets player 2's score window and text as visible or invisible
    void setP3ScoreVisible(bool isVisible);                         // Sets player 3's score window and text as visible or invisible
    void setP4ScoreVisible(bool isVisible);                         // Sets player 4's score window and text as visible or invisible
    void resetP1Pos();                                              // Resets player 1's score window on the screen
    void resetP2Pos();                                              // Resets player 2's score window on the screen
    void resetP3Pos();                                              // Resets player 3's score window on the screen
    void resetP4Pos();                                              // Resets player 4's score window on the screen
    void setTimerVisible(bool isVisible);                           // Sets the timer window visible or invisible
    void setTimer(int seconds);                                     // Sets the tiemr with a new value
    void updateTimer();                                             // updates the timer based on how many seconds have passed since the last time this function was called
    void updateP1Score(int newScore);                               // Sets the score for player 1
    void updateP2Score(int newScore);                               // Sets the score for player 2
    void updateP3Score(int newScore);                               // Sets the score for player 3
    void updateP4Score(int newScore);                               // Sets the score for player 4
    void updateIPAddress(std::string newIP);                        // upadates currentAddress and also sets
    void setServerStartVisible(bool isVisible);                     // Sets the starting server... dialog box visible or invisible
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);  // Convert from OIS mouse event to CEGUI mouse event
private:
    CEGUI::OgreRenderer*    mRenderer;                      // Renderer for GUI system (helps draw components)
    CEGUI::WindowManager*   wmgr;                           // Window manager used to place components on the screen
    CEGUI::Window*          sheet;                          // The GUI window
    CEGUI::Texture*         title_texture;                  // Used to hold the image for the title screen as a texture
    CEGUI::BasicImage*      title_image;                    // Itermediate variable used to convert png file to texture
    CEGUI::Window*          title;                          // GUI Component representing the title screen image 
    CEGUI::Texture*         splay_texture_up;               // Used to hold the image for the up-state of the single play button
    CEGUI::Texture*         splay_texture_down;             // Used to hold the image for the down-state of the single play button
    CEGUI::Texture*         splay_texture_hover;            // Used to hold the image for the hover-state of the single play button
    CEGUI::BasicImage*      splay_image_up;                 // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      splay_image_down;               // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      splay_image_hover;              // Itermediate variable used to convert png file to texture
    CEGUI::Window*          splay;                          // GUI Component representing the single player button
    CEGUI::Texture*         mplay_texture_up;               // Used to hold the image for the up-state of the multi play button
    CEGUI::Texture*         mplay_texture_down;             // Used to hold the image for the down-state of the multi play button
    CEGUI::Texture*         mplay_texture_hover;            // Used to hold the image for the hover-state of the multi play button
    CEGUI::BasicImage*      mplay_image_up;                 // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      mplay_image_down;               // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      mplay_image_hover;              // Itermediate variable used to convert png file to texture
    CEGUI::Window*          mplay;                          // GUI Component represeting the multi player button
    CEGUI::Texture*         scorer_texture;                 // Used to hold the image for player 1's score window as a texture
    CEGUI::BasicImage*      scorer_image;                   // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scorer;                         // GUI Component representing player 1's score window
    CEGUI::Window*          scorer_text;                    // GUI Component representing the text in player 1's score window
    CEGUI::Texture*         scoreb_texture;                 // Used to hold the image for player 2's score window as a texture
    CEGUI::BasicImage*      scoreb_image;                   // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scoreb;                         // GUI Component representing player 2's score window
    CEGUI::Window*          scoreb_text;                    // GUI Component representing the text in player 2's score window
    CEGUI::Texture*         scoreg_texture;                 // Used to hold the image for player 3's score window as a texture
    CEGUI::BasicImage*      scoreg_image;                   // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scoreg;                         // GUI Component representing player 3's score window
    CEGUI::Window*          scoreg_text;                    // GUI Component representing the text in player 3's score window
    CEGUI::Texture*         scorey_texture;                 // Used to hold the image for player 4's score window as a texture
    CEGUI::BasicImage*      scorey_image;                   // Itermediate variable used to convert png file to texture
    CEGUI::Window*          scorey;                         // GUI Component representing player 4's score window
    CEGUI::Window*          scorey_text;                    // GUI Component representing the text in player 4's score window
    CEGUI::Texture*         pause_texture;                  // Used to hold the image for the pause window as a texture
    CEGUI::BasicImage*      pause_image;                    // Itermediate variable used to convert png file to texture
    CEGUI::Window*          pause;                          // GUI Component representing the pause window
    CEGUI::Window*          pause_text;                     // GUI Component representing the text in the pause window
    CEGUI::Texture*         multi_con_texture;              // Used to hold the image for the multi-player connect window as a texture
    CEGUI::BasicImage*      multi_con_image;                // Itermediate variable used to convert png file to texture
    CEGUI::Window*          multi_con;                      // GUI Component representing the Multi-player connect window
    CEGUI::Texture*         host_texture_up;                // Used to hold the image for the up-state of the host button
    CEGUI::Texture*         host_texture_down;              // Used to hold the image for the down-state of the host button
    CEGUI::Texture*         host_texture_hover;             // Used to hold the image for the hover-state of the host button
    CEGUI::BasicImage*      host_image_up;                  // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      host_image_down;                // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      host_image_hover;               // Itermediate variable used to convert png file to texture
    CEGUI::Window*          host;                           // GUI Component representing the host button
    CEGUI::Texture*         join_texture_up;                // Used to hold the image for the up-state of the join button
    CEGUI::Texture*         join_texture_down;              // Used to hold the image for the down-state of the join button
    CEGUI::Texture*         join_texture_hover;             // Used to hold the image for the hover-state of the join button
    CEGUI::BasicImage*      join_image_up;                  // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      join_image_down;                // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      join_image_hover;               // Itermediate variable used to convert png file to texture
    CEGUI::Window*          join;                           // GUI Component representing the join button
    CEGUI::Texture*         multi_con_ex_texture_up;        // Used to hold the image for the up-state of the multi play window's exit button
    CEGUI::Texture*         multi_con_ex_texture_down;      // Used to hold the image for the down-state of the multi play window's exit button
    CEGUI::Texture*         multi_con_ex_texture_hover;     // Used to hold the image for the hover-state of the multi play window's exit button
    CEGUI::BasicImage*      multi_con_ex_image_up;          // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      multi_con_ex_image_down;        // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      multi_con_ex_image_hover;       // Itermediate variable used to convert png file to texture
    CEGUI::Window*          multi_con_ex;                   // GUI Component representing the Multi-player connect window's exit button
    CEGUI::Texture*         clihos_con_texture;             // Used to hold the image for the multi-player connect window as a texture
    CEGUI::BasicImage*      clihos_con_image;               // Itermediate variable used to convert png file to texture
    CEGUI::Window*          clihos_con;                     // GUI Component representing the Multi-player connect window
    CEGUI::Window*          clihos_IP;                      // GUI Component for getting and obtaining the IP address from the user
    CEGUI::Window*          clihos_desc;                    // GUI Component for displaying information for the user
    CEGUI::Texture*         connect_texture_up;             // Used to hold the image for the up-state of the connect button
    CEGUI::Texture*         connect_texture_down;           // Used to hold the image for the down-state of the connect button
    CEGUI::Texture*         connect_texture_hover;          // Used to hold the image for the hover-state of the connect button
    CEGUI::BasicImage*      connect_image_up;               // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      connect_image_down;             // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      connect_image_hover;            // Itermediate variable used to convert png file to texture
    CEGUI::Window*          connect;                        // GUI Component representing the connect button
    CEGUI::Texture*         ok_texture_up;                  // Used to hold the image for the up-state of the ok button
    CEGUI::Texture*         ok_texture_down;                // Used to hold the image for the down-state of the ok button
    CEGUI::Texture*         ok_texture_hover;               // Used to hold the image for the hover-state of the ok button
    CEGUI::BasicImage*      ok_image_up;                    // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      ok_image_down;                  // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      ok_image_hover;                 // Itermediate variable used to convert png file to texture
    CEGUI::Window*          ok;                             // GUI Component representing the ok button
    CEGUI::Texture*         clihos_con_ex_texture_up;       // Used to hold the image for the up-state of the clihos play window's exit button
    CEGUI::Texture*         clihos_con_ex_texture_down;     // Used to hold the image for the down-state of the clihos play window's exit button
    CEGUI::Texture*         clihos_con_ex_texture_hover;    // Used to hold the image for the hover-state of the clihos play window's exit button
    CEGUI::BasicImage*      clihos_con_ex_image_up;         // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      clihos_con_ex_image_down;       // Itermediate variable used to convert png file to texture
    CEGUI::BasicImage*      clihos_con_ex_image_hover;      // Itermediate variable used to convert png file to texture
    CEGUI::Window*          clihos_con_ex;                  // GUI Component representing the clihos-player connect window's exit button
    CEGUI::Window*          starting_server_text;           // GUI Component representing the starting server... info box
    CEGUI::Window*          connecting_text;                // GUI Component representing the connecting... info box
    CEGUI::Window*          player_message;                 // GUI component telling player how to start the game
    CEGUI::Texture*         timebox_texture;                // Used to hold the image for the timer window as a texture
    CEGUI::BasicImage*      timebox_image;                  // Itermediate variable used to convert png file to texture
    CEGUI::Window*          timebox;                        // GUI Component representing the timer window
    CEGUI::Window*          timebox_text;                   // GUI Component representing the text in the timer window
    CEGUI::Texture*         gover_texture;                  // Used to hold the image for the gover screen as a texture
    CEGUI::BasicImage*      gover_image;                    // Itermediate variable used to convert png file to texture
    CEGUI::Window*          gover;                          // GUI Component representing the gover screen image 

    bool                    singlePlay(const CEGUI::EventArgs &e);          // callback function for the Single play button's click event
    bool                    multiPlay(const CEGUI::EventArgs &e);           // callback function for the Multi-play button's click event
    bool                    multi_con_ex_click(const CEGUI::EventArgs &e);  // callback function for the Multiplayer Connect window's close button's click event
    bool                    hosting(const CEGUI::EventArgs &e);             // callback function for the Multiplayer Connect window's host option
    bool                    joining(const CEGUI::EventArgs &e);             // callback function for the Multiplayer Connect window's join option
    bool                    connecting(const CEGUI::EventArgs &e);          // callback function for the Client/Host Connect window's connect option
    bool                    starting(const CEGUI::EventArgs &e);            // callback function for the player message button
};

#endif // #ifndef __Sphere_h_