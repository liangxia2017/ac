include ../Rules.elinux

STATICLIB     = libwebman.a
DYLIB = libwebman.so
#LIBVER10 =.1.0
#LIBVER1	 =.1
#SRCS= myproc.c prop.c dbinterface.c
SRCS= prop.c

#memwatch.c
ifeq ($(MEM_DEBUG),yes)
	SRCS += memwatch.c
endif	
#$(wildcard *.c)


DCFLAGS   += -Werror -c
CFLAGS   += -Werror  
DYLIBCFLAGS   += -fPIC 
DYLIBLDFLAGS +=-shared -W1,-soname,
ARFLAGS = -r

all:      $(STATICLIB)

STATICPATH=.static
STATICOBJS=$(patsubst %.c,$(STATICPATH)/%.o,$(SRCS))
$(STATICLIB): $(STATICOBJS)
	ar -r $(STATICLIB) $(STATICOBJS)
$(STATICPATH)/%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $<

DYLIBPATH=.shared
DYLIBOBJS=$(patsubst %.c,$(DYLIBPATH)/%.o,$(SRCS))
$(DYLIB): $(DYLIBOBJS)
	$(CC) $(CFLAGS) $(DYLIBLDFLAGS)$@$(LIBVER1) -o $@$(LIBVER10) $(DYLIBOBJS)
#	ln -s $(DYLIB)$(LIBVER10) $(DYLIB)$(LIBVER1)
#	ln -s $(DYLIB)$(LIBVER1) $(DYLIB)
$(DYLIBPATH)/%.o:%.c
	$(CC) $(CFLAGS) $(DYLIBCFLAGS) -c -o $@ $<

install: all
	install -m 755 -d $(GR_INSTALL_ROOT)/lib
	install -m 755 $(DYLIB) $(GR_INSTALL_ROOT)/lib/$(DYLIB)

clean:
	rm -rf $(STATICLIB) $(STATICPATH)/*.o *.a core 
	rm -rf $(DYLIBPATH)/*.o $(DYLIBPATH)*.a core 
	rm -rf $(DYLIB)
	rm -rf $(INSTALL_ROOT)/lib/$(STATICLIB) $(INSTALL_ROOT)/lib/$(DYLIB)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null
# DO NOT DELETE

myproc.o: myproc.h
dbinterface.o:dbinterface.h
postdata.o: postdata.h
prop.o: prop.h postdata.h
parser.o: parser.h
comm_ioctl.o:comm_ioctl.h
print_handle.o:print_handle.h
as_shm.o: as_shm.c
get_running_time.o: get_running_time.c 