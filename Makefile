.DEFAULT_GOAL := all

####################################################
#                                                  #  
# Stuff that grabs sys info, runs make in parallel #  
#                                                  #  
####################################################

#? Detect PLATFORM from uname if not set
PLATFORM ?= $(shell uname -s || echo unknown)
PLATFORM_LC := $(shell echo $(PLATFORM) | tr '[:upper:]' '[:lower:]')

#? Pull in platform specific source files and get thread count
ifeq ($(PLATFORM_LC),linux)
  THREADS := $(shell getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1)
else ifeq ($(PLATFORM_LC),freebsd)
  THREADS := $(shell getconf NPROCESSORS_ONLN 2>/dev/null || echo 1)
else ifeq ($(PLATFORM_LC),darwin)
  THREADS := $(shell sysctl -n hw.ncpu || echo 1)
else
$(error $(shell printf "\033[1;91mERROR: \033[97mUnsupported platform ($(PLATFORM))\033[0m"))
endif

#? Use all CPU cores (will only be set if using Make 4.3+)
MAKEFLAGS := --jobs=$(THREADS)
ifeq ($(THREADS),1)
  override THREADS := auto
endif

####################################################
#                                                  #  
#  Actual Makefile Dependencies and Stuff          #
#                                                  #     
####################################################

SRC_DIR = src
OBJDIR = bin
BIN = music-gen

ECHOF = echo -e
ECHO = echo
CXX = clang++
CXXFLAGS = -std=c++20 -Werror -Wall -Wextra -Wstrict-aliasing -pedantic -Wunreachable-code

SRCS = $(shell find $(SRC_DIR) -name "*.cpp") 

OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))
DEPS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.d)) 

.PHONY: all
all: $(BIN)

.PHONY: all-slow
all-slow: THREADS=1
all-slow: $(BIN)

.PHONY: debug
debug: CXXFLAGS += -DDEBUG -g
debug: $(BIN)

.PHONY: debug-slow
debug-slow: CXXFLAGS += -DDEBUG -g
debug-slow: THREADS=1
debug-slow: $(BIN)

.PHONY: format
format:
	@./format.sh

$(BIN): $(OBJS)
	@$(ECHO) Linking $@
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(OBJS:.o=.d)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -Iinclude -MMD -MF $(OBJDIR)/$*.d -c $< -o $@

.PHONY: clean
clean:
	@$(ECHO) Removing all generated files
	@$(RM) -f $(OBJS) $(BIN) $(DEPS)
