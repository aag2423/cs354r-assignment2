ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= PhysicsObject.h PhysicsEngine.h Ball.h PlayGround.h BaseApplication.h Assignment2.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= PhysicsObject.cpp PhysicsEngine.cpp Ball.cpp PlayGround.cpp BaseApplication.cpp Assignment2.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
