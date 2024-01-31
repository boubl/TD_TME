BUILD_DIR := ./build
SRC_DIR := ./src

INC_DIRS := ./include /opt/homebrew/include/
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

LIB_DIRS := ./lib /opt/homebrew/lib/
LIB_DIR_FLAGS := $(addprefix -L,$(LIB_DIRS))

LIBS := cini SDL2 SDL2_ttf
LIBS_FLAGS := $(addprefix -l,$(LIBS))

# Find all the C files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIR) -name '*.c')

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%)

CFLAGS := $(INC_FLAGS) $(LIB_DIR_FLAGS) $(LIBS_FLAGS)

# Build all targets.
all: cini $(OBJS)

.PHONY: cini
cini: ./lib/libcini.a ./include/cini.h

./include/cini.h: ./lib/libcini.a

./lib/libcini.a:
	$(MAKE) -C ./libcini2/

# Build step for C source
$(BUILD_DIR)/%: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) clean -C ./libcini2/


