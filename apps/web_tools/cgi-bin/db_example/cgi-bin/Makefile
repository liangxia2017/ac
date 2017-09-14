#include $(APPS_ARM)/Rules.elinux

include ../../Rules.elinux
INSTDIR   = $(prefix)/cgi-bin
INSTMODE  = 0755

PROGS     = db_example.cgi
SRCS      =  db_example.c
OBJS      = db_example.o
CFLAGS    += -Werror
LDFLAGS += -O2 


all:    $(PROGS)

CFLAGS   += -I../../library 
LDLIBS    = -lwebman -lgcc -lsqlite3 -lpthread
#LDFLAGS   += -L$(GR_INSTALL_ROOT)/lib


$(PROGS): $(OBJS)
	$(CC)  $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(STRIP) $@
install:        $(PROGS)
	echo $(INSTDIR)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)

clean:
	rm -rf $(PROGS) *.o core

depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null
