
# Compiler and Tools
CC := gcc
AR := ar
SOURCE=../source

# DEBUG, DEBUG-ASAN, OPTIMIZE2 or OPTIMIZE3
MODE := OPTIMIZE2

# Automatically find source and header files
SRC := $(shell find $(SOURCE)/Impl -name '*.c')
OBJ := $(patsubst $(SOURCE)/Impl/%.c,./obj/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

# Compiler-Flags
CFLAGS := -I $(SOURCE)/Includes -std=c18 -Wall -Wextra -Werror 
CFLAGS += -Wmissing-prototypes -Werror=missing-prototypes
LFLAGS := -lSDL3 -lSDL3_image

# additional flags in case of different MODE's
ifeq ($(MODE), DEBUG)
	CFLAGS += -g -pg -DDEBUG
	LFLAGS += -g -pg
# flags for activating AddressSanitizer (ASan) for finding memory leaks
else ifeq ($(MODE), DEBUG-ASAN)
	CFLAGS += -g -pg -DDEBUG
	CFLAGS += -fsanitize=address
	LFLAGS += -g -pg
	LFLAGS += -fsanitize=address -static-libasan
# flags for activating C-Optimization Level 2 (-O2) 	
else ifeq ($(MODE), OPTIMIZE2)
	CFLAGS += -O2
	LFLAGS += -O2
# flags for activating C-Optimization Level 3 (-O3)
else ifeq ($(MODE), OPTIMIZE3)
	CFLAGS += -O3
	LFLAGS += -O3
endif

# platform-specific settings
PLATFORM := $(shell uname)
# Linux
ifeq ($(findstring Linux,$(PLATFORM)),Linux)
    OUTPUT := greenr.x
endif
# Apple OS X
ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
    OUTPUT := greenr.x
endif
# Windows MinGW
ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
    OUTPUT := greenr.exe
endif

# Default-Target
.PHONY: all
all: $(OUTPUT)

# Linking
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

# Objects-Files and dependencies
./obj/%.o: $(SOURCE)/Impl/%.c | obj
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM -MT "$@" $(CFLAGS) $< > $(@:.o=.d)

# include dependencies-files
-include $(DEP)

# create object-folder
obj:
	mkdir -p obj

# clean all the binary data
.PHONY: clean
clean:
	@rm -rf obj/* $(OUTPUT)
	@rm -rf obj
	@rm -f gmon.out
