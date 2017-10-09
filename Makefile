
srctree		:= $(if $(KBUILD_SRC),$(KBUILD_SRC),$(CURDIR))
objtree		:= $(CURDIR)
HOSTCC  	= gcc
export HOSTCC
export srctree

include $(srctree)/scripts/Kbuild.include




CC = g++
UI = src/libframebuffer.a
TAR = main
ROOT = $(shell pwd)

INCLUDE = include

CONFIG_FREETYPE = $(shell grep ^CONFIG_FREETYPE .config 2>/dev/null)
CONFIG_FREETYPE := $(subst CONFIG_FREETYPE=,,$(CONFIG_FREETYPE))
CONFIG_FREETYPE := $(subst ",,$(CONFIG_FREETYPE))#")

CONFIG_PNG = $(shell grep ^CONFIG_PNG .config 2>/dev/null)
CONFIG_PNG := $(subst CONFIG_PNG=,,$(CONFIG_PNG))
CONFIG_PNG := $(subst ",,$(CONFIG_PNG))#")

CONFIG_JPEG = $(shell grep ^CONFIG_JPEG .config 2>/dev/null)
CONFIG_JPEG := $(subst CONFIG_JPEG=,,$(CONFIG_JPEG))
CONFIG_JPEG := $(subst ",,$(CONFIG_JPEG))#")

ifeq ($(CONFIG_FREETYPE),y)
FREETYPE += -L$(ROOT)/out/freetype/lib -lfreetype -I $(ROOT)/out/freetype/include/freetype2
else
FREETYPE =
endif

ifeq ($(CONFIG_PNG),y)
PNG += -L$(ROOT)/out/libpng/lib -lpng -I $(ROOT)/out/libpng/include
else
PNG =
endif

ifeq ($(CONFIG_JPEG),y)
JPEG += -Lout/libjpeg/lib -ljpeg -I out/libjpeg/include
else
JPEG =
endif

$(TAR):main.cpp $(UI)
	$(CC) $^ -o $@ -lpthread -lm -I $(INCLUDE) $(FREETYPE) $(PNG) $(JPEG)
	@rm $(UI) -rf
$(UI):
ifeq ($(CONFIG_FREETYPE),y)
	mkdir -p $(ROOT)/out/freetype
	cd $(ROOT)/external/freetype-2.8 && ./configure --prefix=$(ROOT)/out/freetype -exec-prefix=$(ROOT)/out/freetype
	cd $(ROOT)/external/freetype-2.8 && make && make install
endif
ifeq ($(CONFIG_PNG),y)
	mkdir -p $(ROOT)/out/libpng
	cd $(ROOT)/external/libpng-1.6.30 && ./configure --prefix=$(ROOT)/out/libpng -exec-prefix=$(ROOT)/out/libpng
	cd $(ROOT)/external/libpng-1.6.30 && make && make install
endif
ifeq ($(CONFIG_JPEG),y)
	mkdir -p $(ROOT)/out/libjpeg
	cd $(ROOT)/external/jpeg && ./configure --prefix=$(ROOT)/out/libjpeg -exec-prefix=$(ROOT)/out/libjpeg
	cd $(ROOT)/external/jpeg && make && make install
endif
	cd $(ROOT)/src && make

clean:
	cd $(ROOT)/src && make clean
	rm $(ROOT)/out -rf















PHONY += scripts_basic
scripts_basic:
	$(Q)$(MAKE) $(build)=scripts/basic

scripts/basic/%: scripts_basic ;

PHONY += gen_build_files
gen_build_files: $(wildcard $(srctree)/*/*.c) $(wildcard $(srctree)/*/*/*.c)
	$(Q)$(srctree)/scripts/gen_build_files.sh $(srctree) $(objtree)

config: scripts_basic gen_build_files FORCE
	$(Q)mkdir -p include
	$(Q)$(MAKE) $(build)=scripts/kconfig $@

%config: scripts_basic gen_build_files FORCE
	$(Q)mkdir -p include
	$(Q)$(MAKE) $(build)=scripts/kconfig $@



PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
