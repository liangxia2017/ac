#include $(APPS_ARM)/Rules.elinux
include ../../Rules.elinux
INSTDIR   = $(prefix)/cgi-bin
INSTMODE  = 0755

PROGS     = about.cgi

SRCS      =  about.c

OBJS      = about.o

CFLAGS    += -Werror

all:    $(PROGS)

#CFLAGS   += -I../../library 
#LDLIBS    = -lwebman -lgcc
#LDFLAGS   = -L$(INSTALL_ROOT)/lib


$(PROGS): $(OBJS)
	$(CC)  $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(STRIP) $@
install:        $(PROGS)
	#echo $(INSTDIR)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)

clean:
	rm -rf $(PROGS) *.o core

depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null
