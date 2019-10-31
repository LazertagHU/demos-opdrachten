#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES += DisplayTaak.cpp InputControlTaak.cpp KeypadTaak.cpp PlayerInfo.cpp runGameTaak.cpp SendTask.cpp TransferHitsControlTaak.cpp

# header files in this project
HEADERS += ButtonTaak.hpp DisplayTaak.hpp hit.hpp InputControlTaak.hpp InputListener.hpp KeypadListener.hpp KeypadTaak.hpp msg_decoder.hpp pause_listener.hpp PlayerInfo.hpp runGameTaak.hpp SendTask.hpp TransferHitsControlTaak.hpp weapon.hpp

# other places to look for files for this project
SEARCH  :=

# set REATIVE to the next higher directory 
# and defer to the Mak defile.due there
RELATIVE := $(RELATIVE)../
include $(RELATIVE)Makefile.due
