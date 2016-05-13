# Makefile: Aramorpher
# Author: David Edelstein, amadan@users.sourceforge.net
#
# This is a simple Makefile to build the Aramorpher executable. You must change the BOOSTDIR and BOOSTLIB variables
# to where the Boost include directories and lib directories, respectively, are installed on your system.
# You will need to do your own editing to build Aramorpher static or dynamic lib files -- I may add this later.
#
# I make no guarantees about how well this Makefile will work on other systems. Please feel free to email me with
# any improvements, which I will add to the Sourceforge release for the benefit of other users.

CPP	   = g++
CFLAGS =	-O3 -Wall -fmessage-length=0
INCLUDES = -I. -I$(ARAMORPHERDIR) -I$(UTF8DIR) -I$(BOOSTDIR)
LD_LIBS = -L$(BOOSTLIB)
LIBS = -lboost_regex

# executable file
EXE= Aramorpher

# other file basenames
APP= AramorpherApp
BAMA= Aramorpher
DICT= AramorpherDictionary
UTILS= AramorpherUtils
COMPAT= CompatTable
BUCKW= arabic_buckwalter
UTF8= arabic_utf8
CP1256= arabic_cp1256
ISO88596= arabic_8859_6

# EXTENSIONS
EXEXT= #.exe
OBJEXT= .o
SRCEXT= .cpp

# executable
EXEC =	$(EXE)$(EXTEXT)

# header-only source files
HDRONLY= AramorpherDefs.h ArabicEncodings.h

# list of all source files that have duplicate header/source names
SRC= $(BAMA)$(SRCEXT) \
$(DICT)$(SRCEXT) \
$(UTILS)$(SRCEXT) \
$(COMPAT)$(SRCEXT) \
$(UTF8)$(SRCEXT) \
$(CP1256)$(SRCEXT) \
$(ISO88596)$(SRCEXT) \
$(BUCKW)$(SRCEXT)

# Object files
OBJS = $(APP)$(OBJEXT) \
$(BAMA)$(OBJEXT) \
$(DICT)$(OBJEXT) \
$(UTILS)$(OBJEXT) \
$(COMPAT)$(OBJEXT) \
$(UTF8)$(OBJEXT) \
$(CP1256)$(OBJEXT) \
$(ISO88596)$(OBJEXT) \
$(BUCKW)$(OBJEXT)

# location of source files
BOOSTDIR= /usr/include/boost
ARAMORPHERDIR = ../
UTF8DIR= $(ARAMORPHERDIR)utf8
VPATH= $(ARAMORPHERDIR)
# location of boost lib
BOOSTLIB= /usr/local/lib

all:	$(EXEC)

$(EXEC):	$(OBJS)
	$(CPP) $(FLAGS) $(INCLUDES) $(OBJS) $(LD_LIBS) $(LIBS) -o $(EXEC)

# app file w/main has different header/source bases
$(APP)$(OBJEXT) : $(APP)$(SRCEXT) $(HDRONLY)
	$(CPP) $(CFLAGS) -c $(APP)$(SRCEXT)

%$(OBJEXT) : %$(SRCEXT) %.h $(HDRONLY)
	$(CPP) $(CFLAGS) -c $(SRC)

clean:
	@rm -f $(OBJS) $(EXEC)
