include ../../Rules.elinux
INSTDIR   = $(INSTALL_ROOT)/bin
INSTMODE  = 0755

PROGS     = model2sysinfo

SRCS      = model2sysinfo.c

OBJS      = model2sysinfo.o 

all:    $(PROGS)

#CFLAGS   += -I../../library -I../../include
#LDLIBS    = -lwebman 
#LDFLAGS   += -L$(INSTALL_ROOT)/lib

$(PROGS): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@ 
	$(STRIP) $@
install:        $(PROGS)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)

clean:
	rm -rf $(PROGS) *.o core *.log
	rm -rf $(INSTDIR)/$(PROGS) 
depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null
