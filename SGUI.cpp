#include "SGUI.h"

bool octetValid(std::string octet);

SGUI::SGUI() {
    tMod = 0;
    timebox_time = 60;
    isStarted = false;
    isPaused = false;
    isGameOver = false;
    multiStarted = false;
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    currentAddress = "";
    nFlags = -1;

	// Point CEGUI to default resource locations specified in resources.cfg
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // Create window manager and default window
    wmgr = &CEGUI::WindowManager::getSingleton();
    sheet = wmgr->createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    // Create title screen
    title = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/title");
    title->setSize(CEGUI::USize(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
    title->setProperty("Disabled", "true");
    title->setVisible(true);

    title_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("titlet", "title.jpg", "");
    title_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","titlei"));

    title_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), title_texture->getOriginalDataSize()));
    title_image->setTexture(title_texture);

    sheet->addChild(title);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    title->setProperty("Image", "titlei");

    // Create gover screen
    gover = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/gover");
    gover->setSize(CEGUI::USize(CEGUI::UDim(1.0, 0), CEGUI::UDim(1.0, 0)));
    gover->setProperty("Disabled", "true");
    gover->setVisible(false);

    gover_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("govert", "gover.jpg", "");
    gover_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","goveri"));

    gover_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), gover_texture->getOriginalDataSize()));
    gover_image->setTexture(gover_texture);

    sheet->addChild(gover);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    gover->setProperty("Image", "goveri");

    // Set up Single Player Button
    splay = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/splay");
    splay->setSize(CEGUI::USize(CEGUI::UDim(0.524f, 0), CEGUI::UDim(0.252f, 0))); //<---------------------------------------------- WORKING HERE
    splay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0), CEGUI::UDim(0.33f, 0)));
    splay->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::singlePlay, this));
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
    splay->setProperty("DisabledImage", "splayi_up");

    // Set up multiplayer button
    mplay = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/mplay");
    mplay->setSize(CEGUI::USize(CEGUI::UDim(0.490f, 0), CEGUI::UDim(0.252f, 0))); //<---------------------------------------------- WORKING HERE
    mplay->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f, 0), CEGUI::UDim(0.50f, 0)));
    mplay->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::multiPlay, this));
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
    mplay->setProperty("DisabledImage", "mplayi_up");

    // Create the GUI for player 1's score window
    scorer = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/scorer");
    scorer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
    scorer->setProperty("BackgroundEnabled", "false");
    scorer->setProperty("FrameEnabled", "false");
    scorer->setProperty("Disabled", "true");
    scorer->setVisible(false);

    scorer_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scorert", "scorer.png", "");
    scorer_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scoreri"));

    scorer_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scorer_texture->getOriginalDataSize()));
    scorer_image->setTexture(scorer_texture);

    sheet->addChild(scorer);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scorer->setProperty("Image", "scoreri");

    // Create the text for player 1's score window
    scorer_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/ScorerText");
    scorer_text->setText("[colour='FF000000']0");
    scorer_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorer_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.055f, 0), CEGUI::UDim(0.0f, 0)));
    scorer_text->setProperty("AlwaysOnTop", "true");
    scorer_text->setProperty("BackgroundEnabled", "false");
    scorer_text->setProperty("FrameEnabled", "false");
    scorer_text->setVisible(false);
    sheet->addChild(scorer_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create GUI for player 2's score window
    scoreb = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/scoreb");
    scoreb->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scoreb->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1583f, 0), CEGUI::UDim(0.0f, 0)));
    scoreb->setProperty("BackgroundEnabled", "false");
    scoreb->setProperty("FrameEnabled", "false");
    scoreb->setProperty("Disabled", "true");
    scoreb->setVisible(false);

    scoreb_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scorebt", "scoreb.png", "");
    scoreb_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scorebi"));

    scoreb_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scoreb_texture->getOriginalDataSize()));
    scoreb_image->setTexture(scoreb_texture);

    sheet->addChild(scoreb);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scoreb->setProperty("Image", "scorebi");

    // Create the text for player 2's score window
    scoreb_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/ScorebText");
    scoreb_text->setText("[colour='FF000000']0");
    scoreb_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scoreb_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2133f, 0), CEGUI::UDim(0.0f, 0)));
    scoreb_text->setProperty("AlwaysOnTop", "true");
    scoreb_text->setProperty("BackgroundEnabled", "false");
    scoreb_text->setProperty("FrameEnabled", "false");
    scoreb_text->setVisible(false);
    sheet->addChild(scoreb_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create the GUI for player 3's score window
    scoreg = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/scoreg");
    scoreg->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scoreg->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3167f, 0), CEGUI::UDim(0.0f, 0)));
    scoreg->setProperty("BackgroundEnabled", "false");
    scoreg->setProperty("FrameEnabled", "false");
    scoreg->setProperty("Disabled", "true");
    scoreg->setVisible(false);

    scoreg_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scoregt", "scoreg.png", "");
    scoreg_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scoregi"));

    scoreg_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scoreg_texture->getOriginalDataSize()));
    scoreg_image->setTexture(scoreg_texture);

    sheet->addChild(scoreg);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scoreg->setProperty("Image", "scoregi");

    // Create the text for player 3's score window
    scoreg_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/scoregText");
    scoreg_text->setText("[colour='FF000000']0");
    scoreg_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scoreg_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.37167f, 0), CEGUI::UDim(0.0f, 0)));
    scoreg_text->setProperty("AlwaysOnTop", "true");
    scoreg_text->setProperty("BackgroundEnabled", "false");
    scoreg_text->setProperty("FrameEnabled", "false");
    scoreg_text->setVisible(false);
    sheet->addChild(scoreg_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create graphic for player 4's score window
    scorey = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/scorey");
    scorey->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorey->setPosition(CEGUI::UVector2(CEGUI::UDim(0.475f, 0), CEGUI::UDim(0.0f, 0)));
    scorey->setProperty("BackgroundEnabled", "false");
    scorey->setProperty("FrameEnabled", "false");
    scorey->setProperty("Disabled", "true");
    scorey->setVisible(false);

    scorey_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("scoreyt", "scorey.png", "");
    scorey_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","scoreyi"));

    scorey_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), scorey_texture->getOriginalDataSize()));
    scorey_image->setTexture(scorey_texture);

    sheet->addChild(scorey);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    scorey->setProperty("Image", "scoreyi");

    // Create the text for player 4's score window
    scorey_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/scoreyText");
    scorey_text->setText("[colour='FF000000']0");
    scorey_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorey_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.53f, 0), CEGUI::UDim(0.0f, 0)));
    scorey_text->setProperty("AlwaysOnTop", "true");
    scorey_text->setProperty("BackgroundEnabled", "false");
    scorey_text->setProperty("FrameEnabled", "false");
    scorey_text->setVisible(false);
    sheet->addChild(scorey_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create graphic for the timer window
    timebox = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/timebox");
    timebox->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    timebox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.85f, 0), CEGUI::UDim(0.0f, 0)));
    timebox->setProperty("BackgroundEnabled", "false");
    timebox->setProperty("FrameEnabled", "false");
    timebox->setProperty("Disabled", "true");
    timebox->setVisible(false);

    timebox_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("timeboxt", "timer.png", "");
    timebox_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","timeboxi"));

    timebox_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), timebox_texture->getOriginalDataSize()));
    timebox_image->setTexture(timebox_texture);

    sheet->addChild(timebox);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    timebox->setProperty("Image", "timeboxi");

    // Create the text for the timer
    timebox_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/timeboxText");
    timebox_text->setText("[colour='FF000000']1:00");
    timebox_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    timebox_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.905f, 0), CEGUI::UDim(0.0f, 0)));
    timebox_text->setProperty("AlwaysOnTop", "true");
    timebox_text->setProperty("BackgroundEnabled", "false");
    timebox_text->setProperty("FrameEnabled", "false");
    timebox_text->setVisible(false);
    sheet->addChild(timebox_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create Image for pause menu (We need a separate image for each button state but only 1 here)
    pause = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/pause");
    pause->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    pause->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425f, 0), CEGUI::UDim(0.465f, 0)));
    pause->setProperty("BackgroundEnabled", "false");
    pause->setProperty("FrameEnabled", "false");
    pause->setVisible(false);

    pause_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("pauset", "pause.png", "");
    pause_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","pausei"));

    pause_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), pause_texture->getOriginalDataSize()));
    pause_image->setTexture(pause_texture);

    sheet->addChild(pause);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    pause->setProperty("Image", "pausei");

    // Create the text for the pause window (yes this has to be done separately OMFG)
    pause_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/ScoreText");
    pause_text->setText("Pause");
    pause_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    pause_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.480f, 0), CEGUI::UDim(0.465f, 0)));
    pause_text->setProperty("AlwaysOnTop", "true");
    pause_text->setProperty("BackgroundEnabled", "false");
    pause_text->setProperty("FrameEnabled", "false");
    pause_text->setVisible(false);
    sheet->addChild(pause_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    /********************************************************************************************************************
     * Build Multi-player connect window                                                                                *
     ********************************************************************************************************************/
    multi_con = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/multi_con");
    multi_con->setSize(CEGUI::USize(CEGUI::UDim(0.24, 0), CEGUI::UDim(0.2, 0)));
    multi_con->setPosition(CEGUI::UVector2(CEGUI::UDim(0.38f, 0), CEGUI::UDim(0.40f, 0)));
    multi_con->setProperty("BackgroundEnabled", "false");
    multi_con->setProperty("FrameEnabled", "false");
    multi_con->setProperty("AlwaysOnTop", "true");
    multi_con->setProperty("Disabled", "true");
    multi_con->setVisible(false);

    multi_con_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("multi_cont", "multi-connect.png", "");
    multi_con_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","multi_coni"));

    multi_con_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), multi_con_texture->getOriginalDataSize()));
    multi_con_image->setTexture(multi_con_texture);

    sheet->addChild(multi_con);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    multi_con->setProperty("Image", "multi_coni");

    // Create the host button
    host = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/host");
    host->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.04f, 0)));
    host->setPosition(CEGUI::UVector2(CEGUI::UDim(0.395, 0), CEGUI::UDim(0.49f, 0)));
    host->setProperty("AlwaysOnTop", "true");
    host->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::hosting, this));
    host->setEnabled(false);
    host->setVisible(false);

    host_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("hostt_up", "host-up.png", "");
    host_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("hostt_down", "host-down.png", "");
    host_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("hostt_hover", "host-hover.png", "");

    host_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","hosti_up"));
    host_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","hosti_down"));
    host_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","hosti_hover"));

    host_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), host_texture_up->getOriginalDataSize()));
    host_image_up->setTexture(host_texture_up);
    host_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), host_texture_down->getOriginalDataSize()));
    host_image_down->setTexture(host_texture_down);
    host_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), host_texture_hover->getOriginalDataSize()));
    host_image_hover->setTexture(host_texture_hover);

    sheet->addChild(host);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    host->setProperty("NormalImage", "hosti_up");
    host->setProperty("HoverImage", "hosti_hover");
    host->setProperty("PushedImage", "hosti_down");

    // Create the join button
    join = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/join");
    join->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.04f, 0)));
    join->setPosition(CEGUI::UVector2(CEGUI::UDim(0.505f, 0), CEGUI::UDim(0.49f, 0)));
    join->setProperty("AlwaysOnTop", "true");
    join->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::joining, this));
    join->setEnabled(false);
    join->setVisible(false);

    join_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("joint_up", "join-up.png", "");
    join_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("joint_down", "join-down.png", "");
    join_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("joint_hover", "join-hover.png", "");

    join_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","joini_up"));
    join_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","joini_down"));
    join_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","joini_hover"));

    join_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), join_texture_up->getOriginalDataSize()));
    join_image_up->setTexture(join_texture_up);
    join_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), join_texture_down->getOriginalDataSize()));
    join_image_down->setTexture(join_texture_down);
    join_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), join_texture_hover->getOriginalDataSize()));
    join_image_hover->setTexture(join_texture_hover);

    sheet->addChild(join);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    join->setProperty("NormalImage", "joini_up");
    join->setProperty("HoverImage", "joini_hover");
    join->setProperty("PushedImage", "joini_down");

    // Create the close button for the multi-player connect window
    multi_con_ex = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/multi_con_ex");
    multi_con_ex->setSize(CEGUI::USize(CEGUI::UDim(0.014f, 0), CEGUI::UDim(0.021f, 0)));
    multi_con_ex->setPosition(CEGUI::UVector2(CEGUI::UDim(0.588f, 0), CEGUI::UDim(0.425f, 0)));
    multi_con_ex->setProperty("AlwaysOnTop", "true");
    multi_con_ex->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::multi_con_ex_click, this));
    multi_con_ex->setEnabled(false);
    multi_con_ex->setVisible(false);

    multi_con_ex_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("multi_con_ext_up", "close-up.png", "");
    multi_con_ex_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("multi_con_ext_down", "close-down.png", "");
    multi_con_ex_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("multi_con_ext_hover", "close-hover.png", "");

    multi_con_ex_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","multi_con_exi_up"));
    multi_con_ex_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","multi_con_exi_down"));
    multi_con_ex_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","multi_con_exi_hover"));

    multi_con_ex_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), multi_con_ex_texture_up->getOriginalDataSize()));
    multi_con_ex_image_up->setTexture(multi_con_ex_texture_up);
    multi_con_ex_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), multi_con_ex_texture_down->getOriginalDataSize()));
    multi_con_ex_image_down->setTexture(multi_con_ex_texture_down);
    multi_con_ex_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), multi_con_ex_texture_hover->getOriginalDataSize()));
    multi_con_ex_image_hover->setTexture(multi_con_ex_texture_hover);

    sheet->addChild(multi_con_ex);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    multi_con_ex->setProperty("NormalImage", "multi_con_exi_up");
    multi_con_ex->setProperty("HoverImage", "multi_con_exi_hover");
    multi_con_ex->setProperty("PushedImage", "multi_con_exi_down");

    /********************************************************************************************************************
     * Build Client-Host connect window                                                                                 *
     ********************************************************************************************************************/
    clihos_con = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/clihos_con");
    clihos_con->setSize(CEGUI::USize(CEGUI::UDim(0.347f, 0), CEGUI::UDim(0.3815f, 0)));
    clihos_con->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3265f, 0), CEGUI::UDim(0.3092f, 0)));
    clihos_con->setProperty("BackgroundEnabled", "false");
    clihos_con->setProperty("FrameEnabled", "false");
    clihos_con->setProperty("AlwaysOnTop", "true");
    clihos_con->setProperty("Disabled", "true");
    clihos_con->setVisible(false);

    clihos_con_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("clihos_cont", "client-host-connect.png", "");
    clihos_con_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","clihos_coni"));

    clihos_con_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), clihos_con_texture->getOriginalDataSize()));
    clihos_con_image->setTexture(clihos_con_texture);

    sheet->addChild(clihos_con);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    clihos_con->setProperty("Image", "clihos_coni");

    // Create Editable Text Object for entering the IP Address and also for displaying the IP address if hosting
    clihos_IP = wmgr->createWindow("TaharezLook/Editbox", "CEGUIDemo/clihos_IP");
    clihos_IP->setSize(CEGUI::USize(CEGUI::UDim(0.305, 0), CEGUI::UDim(0.04, 0)));
    clihos_IP->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3473f, 0), CEGUI::UDim(0.3800f, 0)));
    clihos_IP->setProperty("AlwaysOnTop", "true");
    clihos_IP->setVisible(false);
    sheet->addChild(clihos_IP);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create description text to display messages to the user
    clihos_desc = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/clihos_desc");
    clihos_desc->setText("Multiline test");
    clihos_desc->setSize(CEGUI::USize(CEGUI::UDim(0.305, 0), CEGUI::UDim(0.188, 0)));
    clihos_desc->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3473f, 0), CEGUI::UDim(0.4275f, 0)));
    clihos_desc->setProperty("AlwaysOnTop", "true");
    clihos_desc->setProperty("Font", "Arial-5");
    clihos_desc->setProperty("VertFormatting", "TopAligned");
    clihos_desc->setProperty("HorzFormatting", "WordWrapLeftAligned");
    clihos_desc->setVisible(false);
    sheet->addChild(clihos_desc);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // Create the connect button for the client version of the window
    connect = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/connect");
    connect->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.04f, 0)));
    connect->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.623f, 0)));
    connect->setProperty("AlwaysOnTop", "true");
    connect->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::connecting, this));
    connect->setEnabled(false);
    connect->setVisible(false);

    connect_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("connectt_up", "connect-up.png", "");
    connect_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("connectt_down", "connect-down.png", "");
    connect_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("connectt_hover", "connect-hover.png", "");

    connect_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","connecti_up"));
    connect_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","connecti_down"));
    connect_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","connecti_hover"));

    connect_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), connect_texture_up->getOriginalDataSize()));
    connect_image_up->setTexture(connect_texture_up);
    connect_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), connect_texture_down->getOriginalDataSize()));
    connect_image_down->setTexture(connect_texture_down);
    connect_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), connect_texture_hover->getOriginalDataSize()));
    connect_image_hover->setTexture(connect_texture_hover);

    sheet->addChild(connect);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    connect->setProperty("NormalImage", "connecti_up");
    connect->setProperty("HoverImage", "connecti_hover");
    connect->setProperty("PushedImage", "connecti_down");

    // Create the OK button for the host version of the window
    ok = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/ok");
    ok->setSize(CEGUI::USize(CEGUI::UDim(0.1f, 0), CEGUI::UDim(0.04f, 0)));
    ok->setPosition(CEGUI::UVector2(CEGUI::UDim(0.45, 0), CEGUI::UDim(0.623f, 0)));
    ok->setProperty("AlwaysOnTop", "true");
    ok->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::starting, this));
    ok->setEnabled(false);
    ok->setVisible(false);

    ok_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("okt_up", "ok-up.png", "");
    ok_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("okt_down", "ok-down.png", "");
    ok_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("okt_hover", "ok-hover.png", "");

    ok_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","oki_up"));
    ok_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","oki_down"));
    ok_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","oki_hover"));

    ok_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), ok_texture_up->getOriginalDataSize()));
    ok_image_up->setTexture(ok_texture_up);
    ok_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), ok_texture_down->getOriginalDataSize()));
    ok_image_down->setTexture(ok_texture_down);
    ok_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), ok_texture_hover->getOriginalDataSize()));
    ok_image_hover->setTexture(ok_texture_hover);

    sheet->addChild(ok);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    ok->setProperty("NormalImage", "oki_up");
    ok->setProperty("HoverImage", "oki_hover");
    ok->setProperty("PushedImage", "oki_down");

    // Add close button for the window
    clihos_con_ex = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/clihos_con_ex");
    clihos_con_ex->setSize(CEGUI::USize(CEGUI::UDim(0.014f, 0), CEGUI::UDim(0.021f, 0)));
    clihos_con_ex->setPosition(CEGUI::UVector2(CEGUI::UDim(0.638f, 0), CEGUI::UDim(0.338f, 0)));
    clihos_con_ex->setProperty("AlwaysOnTop", "true");
    clihos_con_ex->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SGUI::multi_con_ex_click, this));
    clihos_con_ex->setEnabled(false);
    clihos_con_ex->setVisible(false);

    clihos_con_ex_texture_up = &CEGUI::System::getSingleton().getRenderer()->createTexture("clihos_con_ext_up", "close-up.png", "");
    clihos_con_ex_texture_down = &CEGUI::System::getSingleton().getRenderer()->createTexture("clihos_con_ext_down", "close-down.png", "");
    clihos_con_ex_texture_hover = &CEGUI::System::getSingleton().getRenderer()->createTexture("clihos_con_ext_hover", "close-hover.png", "");

    clihos_con_ex_image_up = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","clihos_con_exi_up"));
    clihos_con_ex_image_down = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","clihos_con_exi_down"));
    clihos_con_ex_image_hover = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","clihos_con_exi_hover"));

    clihos_con_ex_image_up->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), clihos_con_ex_texture_up->getOriginalDataSize()));
    clihos_con_ex_image_up->setTexture(clihos_con_ex_texture_up);
    clihos_con_ex_image_down->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), clihos_con_ex_texture_down->getOriginalDataSize()));
    clihos_con_ex_image_down->setTexture(clihos_con_ex_texture_down);
    clihos_con_ex_image_hover->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), clihos_con_ex_texture_hover->getOriginalDataSize()));
    clihos_con_ex_image_hover->setTexture(clihos_con_ex_texture_hover);

    sheet->addChild(clihos_con_ex);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    clihos_con_ex->setProperty("NormalImage", "clihos_con_exi_up");
    clihos_con_ex->setProperty("HoverImage", "clihos_con_exi_hover");
    clihos_con_ex->setProperty("PushedImage", "clihos_con_exi_down");

    // Starting server text
    starting_server_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/starting_server_text");
    starting_server_text->setText("Starting Server...");
    starting_server_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    starting_server_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.445f, 0), CEGUI::UDim(0.465f, 0)));
    starting_server_text->setProperty("AlwaysOnTop", "true");
    starting_server_text->setProperty("BackgroundEnabled", "false");
    starting_server_text->setProperty("FrameEnabled", "false");
    starting_server_text->setVisible(false);
    sheet->addChild(starting_server_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // connecting text
    connecting_text = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/connecting_text");
    connecting_text->setText("Connecting...");
    connecting_text->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    connecting_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.455f, 0), CEGUI::UDim(0.465f, 0)));
    connecting_text->setProperty("AlwaysOnTop", "true");
    connecting_text->setProperty("BackgroundEnabled", "false");
    connecting_text->setProperty("FrameEnabled", "false");
    connecting_text->setVisible(false);
    sheet->addChild(connecting_text);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    // message to the player
    player_message = wmgr->createWindow("TaharezLook/StaticText", "CEGUIDemo/player_message");
    player_message->setText("Press \"SPACE\" to start game");
    player_message->setSize(CEGUI::USize(CEGUI::UDim(0.21, 0), CEGUI::UDim(0.07, 0)));
    player_message->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3898f, 0), CEGUI::UDim(0.465f, 0)));
    player_message->setProperty("AlwaysOnTop", "true");
    player_message->setVisible(false);
    sheet->addChild(player_message);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);
}

void SGUI::setTitleScreenVisible(bool isVisible) {
	title->setVisible(isVisible);
	splay->setVisible(isVisible);
    splay->setEnabled(isVisible);
	mplay->setVisible(isVisible);
    mplay->setEnabled(isVisible);
    resetP1Pos();
    resetP2Pos();
    resetP3Pos();
    resetP4Pos();
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();

}

void SGUI::playerMessageVisible(bool isVisible) {
    player_message->setVisible(isVisible);
}

void SGUI::setP1ScoreVisible(bool isVisible) {
	scorer->setVisible(isVisible);
	scorer_text->setVisible(isVisible);
}

void SGUI::setP2ScoreVisible(bool isVisible) {
	scoreb->setVisible(isVisible);
	scoreb_text->setVisible(isVisible);
}

void SGUI::setP3ScoreVisible(bool isVisible) {
	scoreg->setVisible(isVisible);
	scoreg_text->setVisible(isVisible);
}

void SGUI::setP4ScoreVisible(bool isVisible) {
	scorey->setVisible(isVisible);
	scorey_text->setVisible(isVisible);
}

void SGUI::setPauseVisible(bool isVisible) {
	pause->setVisible(isVisible);
	pause_text->setVisible(isVisible);
}

void SGUI::setServerStartVisible(bool isVisible) {
    pause->setVisible(isVisible);
    starting_server_text->setVisible(isVisible);
}

void SGUI::setConnectingVisible(bool isVisible) {
    pause->setVisible(isVisible);
    connecting_text->setVisible(isVisible);
}

void SGUI::updateP1Score(int newScore) {
	scorer_text->setText("[colour='FF000000']"+std::to_string(newScore));
}

void SGUI::updateP2Score(int newScore) {
	scoreb_text->setText("[colour='FF000000']"+std::to_string(newScore));
}

void SGUI::updateP3Score(int newScore) {
	scoreg_text->setText("[colour='FF000000']"+std::to_string(newScore));
}

void SGUI::updateP4Score(int newScore) {
	scorey_text->setText("[colour='FF000000']"+std::to_string(newScore));
}

void SGUI::resetP1Pos() {
    scorer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
    scorer_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.055f, 0), CEGUI::UDim(0.0f, 0)));
}

void SGUI::resetP2Pos() {
    scoreb->setPosition(CEGUI::UVector2(CEGUI::UDim(0.1583f, 0), CEGUI::UDim(0.0f, 0)));
    scoreb_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2133f, 0), CEGUI::UDim(0.0f, 0)));
}

void SGUI::resetP3Pos() {
    scoreg->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3167f, 0), CEGUI::UDim(0.0f, 0)));
    scoreg_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.37167f, 0), CEGUI::UDim(0.0f, 0)));
}

void SGUI::resetP4Pos() {
    scorey->setPosition(CEGUI::UVector2(CEGUI::UDim(0.475f, 0), CEGUI::UDim(0.0f, 0)));
    scorey_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.53f, 0), CEGUI::UDim(0.0f, 0)));
}

void SGUI::updateIPAddress(std::string newIP) {
    currentAddress = newIP;
    clihos_IP->setText(newIP);
}

void SGUI::setTimerVisible(bool isVisible) {
    timebox->setVisible(isVisible);
    timebox_text->setVisible(isVisible);
}
void SGUI::setTimer(int seconds) {
    std::string nTime = "";
    if(seconds % 60 < 10)
        nTime = "[colour='FF000000']"+std::to_string(seconds/60)+":0"+std::to_string(seconds%60);
    else
        nTime = "[colour='FF000000']"+std::to_string(seconds/60)+":"+std::to_string(seconds%60);
    timebox_text->setText(nTime);
    timebox_time = seconds;
}

void SGUI::updateTimer() {
    std::string nTime = "";
    if(tMod > 2 && timebox_time > 0) {
        tMod -= 2;
        timebox_time -= 1;
        if(timebox_time % 60 < 10)
            nTime = "[colour='FF000000']"+std::to_string(timebox_time/60)+":0"+std::to_string(timebox_time%60);
        else
            nTime = "[colour='FF000000']"+std::to_string(timebox_time/60)+":"+std::to_string(timebox_time%60);
        timebox_text->setText(nTime);
    }
}

void SGUI::setGOverScreenVisible(bool isVisible) {
    gover->setVisible(isVisible);
    setTimerVisible(!isVisible);
    scorer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425f, 0), CEGUI::UDim(0.5f, 0)));
    scorer_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.48f, 0), CEGUI::UDim(0.5f, 0)));
    scoreb->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425f, 0), CEGUI::UDim(0.578f, 0)));
    scoreb_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.48f, 0), CEGUI::UDim(0.578f, 0)));
    scoreg->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425f, 0), CEGUI::UDim(0.657f, 0)));
    scoreg_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.48f, 0), CEGUI::UDim(0.657f, 0)));
    scorey->setPosition(CEGUI::UVector2(CEGUI::UDim(0.425f, 0), CEGUI::UDim(0.735f, 0)));
    scorey_text->setPosition(CEGUI::UVector2(CEGUI::UDim(0.48f, 0), CEGUI::UDim(0.735f, 0)));
}

void SGUI::setMultiConVisible(bool isVisible) {
    join->setVisible(isVisible);
    join->setEnabled(isVisible);
    host->setVisible(isVisible);
    host->setEnabled(isVisible);
    multi_con->setVisible(isVisible);
    multi_con_ex->setVisible(isVisible);
    multi_con_ex->setEnabled(isVisible);
}

void SGUI::setHostVisible(bool isVisible) {
    clihos_con->setVisible(isVisible);
    clihos_IP->setVisible(isVisible);
    clihos_IP->setEnabled(isVisible);
    clihos_desc->setVisible(isVisible);
    ok->setVisible(isVisible);
    ok->setEnabled(isVisible);
    clihos_con_ex->setVisible(isVisible);
    clihos_con_ex->setEnabled(isVisible);
}

void SGUI::setClientVisible(bool isVisible) {
    clihos_con->setVisible(isVisible);
    clihos_IP->setVisible(isVisible);
    clihos_IP->setEnabled(isVisible);
    clihos_desc->setVisible(isVisible);
    connect->setVisible(isVisible);
    connect->setEnabled(isVisible);
    clihos_con_ex->setVisible(isVisible);
    clihos_con_ex->setEnabled(isVisible);
}

bool SGUI::singlePlay(const CEGUI::EventArgs &e) {
	splay->setVisible(false);
	splay->setEnabled(false);
	mplay->setVisible(false);
	mplay->setEnabled(false);
	title->setVisible(false);
	title->setEnabled(false);
	scorer->setVisible(true);
    setTimerVisible(true);
    setTimer(60);
	scorer_text->setVisible(true);
    isStarted = true;
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	return true;
}

bool SGUI::multiPlay(const CEGUI::EventArgs &e) {
    splay->setEnabled(false);
    mplay->setEnabled(false);
    setMultiConVisible(true);
    return true;
}

bool SGUI::multi_con_ex_click(const CEGUI::EventArgs &e) {
    splay->setEnabled(true);
    mplay->setEnabled(true);
    setMultiConVisible(false);
    setHostVisible(false);
    setClientVisible(false);
}

bool SGUI::hosting(const CEGUI::EventArgs &e) {
    splay->setEnabled(false);
    mplay->setEnabled(false);
    setServerStartVisible(true);
    setMultiConVisible(false);
    clihos_desc->setText("Host program is now started, please give the IP address displayed above to other players you want to join");
    clihos_IP->setProperty("ReadOnly", "true");
    nFlags = 1;
}

bool SGUI::joining(const CEGUI::EventArgs &e) {
    splay->setEnabled(false);
    mplay->setEnabled(false);
    setClientVisible(true);
    setMultiConVisible(false);
    clihos_desc->setText("Please enter the IP address of the game you wish to join in the space above.");
    clihos_IP->setProperty("ReadOnly", "false");
    nFlags = 0;
}

bool SGUI::connecting(const CEGUI::EventArgs &e) {
    std::string address = clihos_IP->getText().c_str();
    int marker = 0;
    int octet = 0;
    if(!strcmp(address.c_str(), "")) {
        clihos_desc->setText("Malformed IP Address! Address cannot be blank!");
    }
    while(marker < address.length()) {
        if(octet < 3) {
            if(address[marker+1]=='.') {
                if(octetValid(address.substr(marker, 1))) {
                    marker += 2;
                    octet++;
                } else {
                    marker = 1000;
                }
            } else if(address[marker+2]=='.') {
                if(octetValid(address.substr(marker, 2))) {
                    marker += 3;
                    octet++;
                } else {
                    marker = 1000;
                }
            } else if(address[marker+3]=='.') {
                if(octetValid(address.substr(marker, 3))) {
                    marker += 4;
                    octet++;
                } else {
                    marker = 1000;
                }
            } else {
                marker = 1000;
                clihos_desc->setText("Malformed IP Address! Check address and try again!");
            }
        } else {
            if(octetValid(address.substr(marker, address.length()-marker))) {
                octet++;
                marker = address.length();
            } else {
                marker = 1000;
            }
        }
    }
    if(marker < 1000 && octet == 4) {
        currentAddress = clihos_IP->getText().c_str();
        setConnectingVisible(true);
        setClientVisible(false);
    } else {
        clihos_desc->setText("Malformed IP Address! Check address and try again!");
    }
}

CEGUI::MouseButton SGUI::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

bool SGUI::starting(const CEGUI::EventArgs &e) {
    setHostVisible(false);
    setTimerVisible(true);
    setTitleScreenVisible(false);
    playerMessageVisible(true);
    setP1ScoreVisible(true);
    multiStarted = true;
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
}

bool octetValid(std::string octet) {
    int len = octet.length();
    if(len > 3) {
        return false;
    }
    for(int i = 0; i < len; i++) {
        if(octet[i] < '0' || octet[i] > '9') {
            return false;
        }
    }

    int oct = stoi(octet);
    if(oct < 0 || oct > 255) {
        return false;
    }
    return true;
}
