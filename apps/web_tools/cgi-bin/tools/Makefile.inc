prefix      =  ./
run_prefix  = 	$(prefix)

CC          = 	gcc 
STRIP	    =   strip
INSTALL     = 	/usr/bin/install
LD          = 	ld
AR          = 	ar
RANLIB      = 	ranlib
CHMODE		=	chmod
MODE		=	0755
CFLAGS      = -O2 -Wformat 
CRYPTO	    = crypto
ENG_VER		=no
LD_DYLIB	=no
MEM_DEBUG   =no
ATH	=yes

SUBDIRS     = 	dummy
ifeq ($(MEM_DEBUG),yes)
	CC+=-DMEMWATCH -DMW_STDIO -DMEM_DEBUG
endif
	
ifeq ($(ENG_VER),yes)
	CC+=-DENG_VER
endif
ifeq ($(LD_DYLIB),no)
	LDFLAGS   += -L../LIB/
endif

#CROSS=/home/athbuild/build/gcc-3.4.4-2.16.1/build_mips/bin/mips-linux-uclibc-
CFLAGS +=  -O2
LDFLAGS +=  -O2
PRODUCT=ap
