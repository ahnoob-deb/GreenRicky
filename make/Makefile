
# Compiler und Tools
CC := gcc
AR := ar
SOURCE=../source

# DEBUG, DEBUG-ASAN, OPTIMIZE2 or OPTIMIZE3
MODE := OPTIMIZE2

# Quell- und Header-Dateien automatisch finden
SRC := $(shell find $(SOURCE)/Impl -name '*.c')
OBJ := $(patsubst $(SOURCE)/Impl/%.c,./obj/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

# Compiler-Flags
CFLAGS := -I $(SOURCE)/Includes -std=c18 -Wall -Wextra -Werror 
CFLAGS += -Wmissing-prototypes -Werror=missing-prototypes
LFLAGS := -lSDL3 -lSDL3_image

ifeq ($(MODE), DEBUG)
	CFLAGS += -g -pg -DDEBUG
	LFLAGS += -g -pg
else ifeq ($(MODE), DEBUG-ASAN)
	CFLAGS += -g -pg -DDEBUG
	CFLAGS += -fsanitize=address
	LFLAGS += -g -pg
	LFLAGS += -fsanitize=address -static-libasan
else ifeq ($(MODE), OPTIMIZE2)
	CFLAGS += -O2
	LFLAGS += -O2
else ifeq ($(MODE), OPTIMIZE3)
	CFLAGS += -O3
	LFLAGS += -O3
endif

# Plattformabhängige Einstellungen
PLATFORM := $(shell uname)
ifeq ($(findstring Linux,$(PLATFORM)),Linux)
    OUTPUT := greenr.x
endif
ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
    OUTPUT := greenr.x
endif
ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
    OUTPUT := greenr.exe
endif

# Default-Target
.PHONY: all
all: $(OUTPUT)

# Linken
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

# Objektdateien und Abhängigkeitsdateien
./obj/%.o: $(SOURCE)/Impl/%.c | obj
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM -MT "$@" $(CFLAGS) $< > $(@:.o=.d)

# Abhängigkeitsdateien einbinden
-include $(DEP)

# Objektverzeichnis anlegen
obj:
	mkdir -p obj

# Clean-Target
.PHONY: clean
clean:
	rm -rfv obj/* $(OUTPUT)
	rm -rfv obj
	rm -fv gmon.out
