libdir = $(plugindir)
USE_NLS = yes

lib_LTLIBRARIES        = libmodplugxmms.la

INCLUDES = $(LIBMODPLUG_CFLAGS) -I.. -I../../..
libmodplugxmms_la_LDFLAGS  = -module -avoid-version -lstdc++
libmodplugxmms_la_SOURCES  = plugin.cpp modplugxmms.cpp
libmodplugxmms_la_LIBADD   = ./archive/libarchive.la ./gui/libgui.la $(LIBMODPLUG_LIBS)

SUBDIRS = archive gui

noinst_HEADERS = modplugxmms.h plugin.h stddefs.h
