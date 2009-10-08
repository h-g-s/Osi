# Copyright (C) 2009 Stefan Vigerske and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id$

AUTOMAKE_OPTIONS = foreign

########################################################################
#                             libOsiGrb                                 
########################################################################

# Name of the library compiled in this directory.
lib_LTLIBRARIES = libOsiGrb.la

# List all source files for this library, including headers
libOsiGrb_la_SOURCES = OsiGrbSolverInterface.cpp OsiGrbSolverInterface.hpp

# This is for libtool (on Windows)
libOsiGrb_la_LDFLAGS = $(LT_LDFLAGS)

# Here list all include flags, relative to this "srcdir" directory.  This
# "cygpath" stuff is necessary to compile with native compilers on Windows.
# "top_srcdir" refers to the basic directory for the main package that is
# being compiled.
AM_CPPFLAGS = \
	-I`$(CYGPATH_W) $(COINUTILSSRCDIR)/src` \
	-I`$(CYGPATH_W) $(COINUTILSOBJDIR)/inc` \
	-I`$(CYGPATH_W) $(GRBINCDIR)` \
	-I`$(CYGPATH_W) $(srcdir)/..`

# This line is necessary to allow VPATH compilation with MS compilers
# on Cygwin
DEFAULT_INCLUDES = -I. -I`$(CYGPATH_W) $(srcdir)` -I$(top_builddir)/inc

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'install/coin'
includecoindir = $(includedir)/coin
includecoin_HEADERS = OsiGrbSolverInterface.hpp