ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= Target.h PhysicsObject.h PhysicsEngine.h Ball.h PlayGround.h Player.h Game.h BaseApplication.h Assignment2.h Sound.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= Target.cpp PhysicsObject.cpp PhysicsEngine.cpp Ball.cpp PlayGround.cpp Player.cpp Game.cpp BaseApplication.cpp Assignment2.cpp Sound.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(SDL_mixer_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(SDL_mixer_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
