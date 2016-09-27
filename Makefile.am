ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h BattingSimulator.h

bin_PROGRAMS= BattingSimulator
BattingSimulator_CPPFLAGS= -I$(top_srcdir)
BattingSimulator_SOURCES= BaseApplication.cpp BattingSimulator.cpp
BattingSimulator_CXXFLAGS= -std=c++11 $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS)
BattingSimulator_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS)
BattingSimulator_LDFLAGS= -lOgreOverlay -lboost_system

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
