UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
OS := Linux
endif

ifeq ($(UNAME_S), Darwin) #APPLE
OS := macOS
endif

ifeq ($(OS), Windows_NT) #WINDOWS
OS := Windows
endif

BUILD_DIR := ./build
SRC_DIR := ./src
INC_DIRS := ./include
LIB_DIRS := ./lib

ifeq ($(OS), macOS)
INC_DIRS += /opt/homebrew/include/
LIB_DIRS += /opt/homebrew/lib/
endif

ifeq ($(OS), Linux)
INC_DIRS += /usr/include/
LIB_DIRS += /usr/lib/x86_64-linux-gnu
endif

# for windows the SDL has to be installed manually in the lib and include folders

INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIB_DIR_FLAGS := $(addprefix -L,$(LIB_DIRS))

LIBS := cini SDL2 SDL2_ttf m #we don't care about including everything for everything
LIBS_FLAGS := $(addprefix -l,$(LIBS))

# Find all the C files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIR) -name '*.c')

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%)

CFLAGS := -Wall $(INC_FLAGS) $(LIB_DIR_FLAGS) $(LIBS_FLAGS)

# Build all targets.
all: cini $(OBJS)

.PHONY: cini
cini: ./lib/libcini.a ./include/cini.h

./include/cini.h: ./lib/libcini.a

./lib/libcini.a:
	$(MAKE) -C ./libcini3/

# Build step for C source
$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) clean -C ./libcini3/
