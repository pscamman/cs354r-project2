#include "SGUI.h"

SGUI::SGUI() {
    isStarted = false;
    isPaused = false;
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

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
    title->setVisible(true);

    title_texture = &CEGUI::System::getSingleton().getRenderer()->createTexture("titlet", "title.jpg", "");
    title_image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage","titlei"));

    title_image->setArea(CEGUI::Rectf(CEGUI::Vector2f(0.0f, 0.0f), title_texture->getOriginalDataSize()));
    title_image->setTexture(title_texture);

    sheet->addChild(title);
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

    title->setProperty("Image", "titlei");

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

    // Set up multiplayer button
    mplay = wmgr->createWindow("TaharezLook/ImageButton", "CEGUIDemo/mplay");
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

    // Create the GUI for player 1's score window
    scorer = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/scorer");
    scorer->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorer->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0), CEGUI::UDim(0.0f, 0)));
    scorer->setProperty("BackgroundEnabled", "false");
    scorer->setProperty("FrameEnabled", "false");
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

    // Create the GUI for player 4's score window
    scoreg = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/scoreg");
    scoreg->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scoreg->setPosition(CEGUI::UVector2(CEGUI::UDim(0.3167f, 0), CEGUI::UDim(0.0f, 0)));
    scoreg->setProperty("BackgroundEnabled", "false");
    scoreg->setProperty("FrameEnabled", "false");
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

    scorey = wmgr->createWindow("TaharezLook/StaticImage", "CEGUIDemo/scorey");
    scorey->setSize(CEGUI::USize(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.07, 0)));
    scorey->setPosition(CEGUI::UVector2(CEGUI::UDim(0.475f, 0), CEGUI::UDim(0.0f, 0)));
    scorey->setProperty("BackgroundEnabled", "false");
    scorey->setProperty("FrameEnabled", "false");
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
}

void SGUI::setTitleScreenVisible(bool isVisible) {
	title->setVisible(isVisible);
	splay->setVisible(isVisible);
	mplay->setVisible(isVisible);
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

bool SGUI::singlePlay(const CEGUI::EventArgs &e) {
	splay->setVisible(false);
	splay->setEnabled(false);
	mplay->setVisible(false);
	mplay->setEnabled(false);
	title->setVisible(false);
	title->setEnabled(false);
	scorer->setVisible(true);
	scorer_text->setVisible(true);
    isStarted = true;
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
	return true;
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