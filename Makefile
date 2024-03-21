UNAME_S := $(shell uname -s)

BUILD_DIR := ./build
SRC_DIR := ./src
INC_DIRS := ./include
LIB_DIRS := ./libcini3/lib/
NEED_CINI := cini

ifeq ($(UNAME_S), Linux) #LINUX
OS := Linux
INC_DIRS += /usr/include/
# check if a native cini is on the computer
ifneq ("$(wildcard /usr/include/cini.h)","")
NEED_CINI :=
endif
endif

ifeq ($(UNAME_S), Darwin) #APPLE
OS := macOS
INC_DIRS += /opt/homebrew/include/
LIB_DIRS += /opt/homebrew/lib/
endif

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIB_DIR_FLAGS := $(addprefix -L,$(LIB_DIRS))

LIBS := cini m SDL2# we don't care about including everything for everything, the linker will optimize things for us

LIBS_FLAGS := $(addprefix -l,$(LIBS))

# Find all the C files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIR) -name '*.c')

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%)

CFLAGS := -Wall -Wl,-rpath,$(realpath ./libcini3/lib/) $(INC_FLAGS) $(LIB_DIR_FLAGS) $(LIBS_FLAGS)

# Build all targets.
.PHONY: all
all: $(NEED_CINI) $(OBJS)

.PHONY: cini
cini: libcini3/include/cini.h libcini3/lib/libcini.so
	@mkdir -p include/ lib/
	@cp libcini3/lib/libcini.so lib
	@cp libcini3/include/cini.h include

libcini3/include/cini.h: libcini3/lib/libcini.so

libcini3/lib/libcini.so:
	$(MAKE) -C ./libcini3/

# Build step for C source
$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $< $(CFLAGS)
ifeq ($(UNAME_S), Darwin)
	install_name_tool -change libcini.so $(realpath ./libcini3/lib/libcini.so) $@
endif

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: rebuild
rebuild: clean
	$(MAKE) clean -C ./libcini3/
	$(MAKE) all

.PHONY: copy_cini
copy_cini: libcini3/lib/libcini.so
	ln -s "$(realpath ./libcini3/libcini.so)" "$(realpath ./)"

.PHONY: install
install: libcini3/lib/libcini.so
	cp libcini3/include/cini.h /usr/local/include/
	cp libcini3/lib/libcini.so /usr/local/lib/

.PHONY: uninstall
uninstall: /usr/local/lib/libcini.so /usr/local/include/cini.h
	rm /usr/local/lib/libcini.so
	rm /usr/local/include/cini.h
