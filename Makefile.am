ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h BattingSimulator.h AI.h

bin_PROGRAMS= BattingSimulator
BattingSimulator_CPPFLAGS= -I$(top_srcdir)
BattingSimulator_SOURCES= BaseApplication.cpp BattingSimulator.cpp AI.cpp
BattingSimulator_CXXFLAGS= -std=c++11 $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS) $(SDL_MIXER_CFLAGS) $(CEGUI_0_CFLAGS)
BattingSimulator_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(SDL_MIXER_LIBS) $(CEGUI_0_LIBS) 

BattingSimulator_LDFLAGS= -lOgreOverlay -lboost_system 


EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
