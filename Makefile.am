ACLOCAL_AMFLAGS= -I m4

noinst_HEADERS= BaseApplication.h BattingSimulator.h NetManager.h AI.h SGUI.h GameObject.h

bin_PROGRAMS= BattingSimulator
BattingSimulator_CPPFLAGS= -I$(top_srcdir)
BattingSimulator_SOURCES= BaseApplication.cpp BattingSimulator.cpp NetManager.cpp AI.cpp SGUI.cpp GameObject.cpp
BattingSimulator_CXXFLAGS= -std=c++11 $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS) $(SDL_MIXER_CFLAGS) $(SDL_NET_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS)
BattingSimulator_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(SDL_MIXER_LIBS) $(SDL_NET_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)
BattingSimulator_LDFLAGS= -lOgreOverlay -lboost_system -R/lusr/lib/cegui-0.8

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
