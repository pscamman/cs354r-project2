ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h BattingSimulator.h

bin_PROGRAMS= BattingSimulator
Project1_CPPFLAGS= -I$(top_srcdir)
Project1_SOURCES= BaseApplication.cpp BattingSimulator.cpp
Project1_CXXFLAGS= -std=c++11 $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
Project1_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)
Project1_LDFLAGS= -lOgreOverlay -lboost_system

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
