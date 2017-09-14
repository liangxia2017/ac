# *******************************************************************************************
# * Copyright (c) 2006-7 Laboratorio di Sistemi di Elaborazione e Bioingegneria Informatica *
# *                      Universita' Campus BioMedico - Italy                               *
# *                                                                                         *
# * This program is free software; you can redistribute it and/or modify it under the terms *
# * of the GNU General Public License as published by the Free Software Foundation; either  *
# * version 2 of the License, or (at your option) any later version.                        *
# *                                                                                         *
# * This program is distributed in the hope that it will be useful, but WITHOUT ANY         *
# * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 	    *
# * PARTICULAR PURPOSE. See the GNU General Public License for more details.                *
# *                                                                                         *
# * You should have received a copy of the GNU General Public License along with this       *
# * program; if not, write to the:                                                          *
# * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,                    *
# * MA  02111-1307, USA.                                                                    *
# *                                                                                         *
# * --------------------------------------------------------------------------------------- *
# * Project:  Capwap                                                                        *
# *                                                                                         *
# * Author :  Ludovico Rossi (ludo@bluepixysw.com)                                          *  
# *           Del Moro Andrea (andrea_delmoro@libero.it)                                    *
# *           Giovannini Federica (giovannini.federica@gmail.com)                           *
# *           Massimo Vellucci (m.vellucci@unicampus.it)                                    *
# *	      Donato Capitella (d.capitella@gmail.com)  	                            *
# *******************************************************************************************



CC = gcc 


LDFLAGS = -lpthread -ldl -D_REENTRANT

CFLAGS = -Wall -g -O0 -D_REENTRANT -I./include/ #-DCW_NO_DTLS

CFLAGS   += -I../sqlite/ -I../web_tools/cgi-bin/library
LDFLAGS += -O2  -lsqlite3 -lwebman 

RM = /bin/rm -f 

# list of generated object files for AC. 

CAPWAPS_CLIENT_OBJS = main.o get_db_message.o
 
CAPWAPS_CLIENT_SRCS := $(CAPWAPS_CLIENT_OBJS:.o=.c)

CAPWAPS_CLIENT_DEPS := $(CAPWAPS_CLIENT_OBJS:.o=.d)

# program executables. 

CAPWAPS_CLIENT_NAME = CAPWAPS_CLIENT

.PHONY: deps clean clean_libs libs

# top-level rule, to compile everything. 
all: $(CAPWAPS_CLIENT_NAME)


$(CAPWAPS_CLIENT_NAME): $(CAPWAPS_CLIENT_OBJS) 
	$(CC) $(CAPWAPS_CLIENT_OBJS) $(CC_FLAGS)  $(LDFLAGS) -o $(CAPWAPS_CLIENT_NAME) 

clean: 
	$(RM) $(CAPWAPS_CLIENT_NAME) $(CAPWAPS_CLIENT_OBJS) $(CAPWAPS_CLIENT_DEPS)

clean_deps:
	$(CAPWAPS_CLIENT_DEPS)
	
deps: $(CAPWAPS_CLIENT_SRC)
	$(CC) -MD -E $(AC_SRCS) $(CFLAGS) >/dev/null
	$(CC) -MD -E $(CAPWAPS_CLIENT_SRC) $(CFLAGS) >/dev/null

-include $(CAPWAPS_CLIENT_DEPS)
