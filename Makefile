SRCDIR      := ./src
BUILDDIR    := ./build
EXE         := prime_gap_finder
CXX_FILES   := \
    prime.cpp \
    main.cpp

CXX         := $(shell clang++ --version >/dev/null 2>&1 && echo 'clang++' || echo 'g++')
LD          := $(CXX)
# -MMD generates a .d dependency file alongside the .o file
CXXFLAGS    := \
    -std=c++11 \
    -Wall -Wextra \
    -I$(SRCDIR) \
    -pthread \
    -MMD \
    $(if $(SIEVE_IMPL),-DSIEVE_IMPL=$(SIEVE_IMPL))
LDFLAGS     := -pthread
LDLIBS      :=

OBJS += $(patsubst %.cpp,$(BUILDDIR)/%.o, $(CXX_FILES))

.PHONY: all debug release clean

all:            release
debug:          build
release:        build
prof:           build
build:          pre-build $(EXE)

# Target-specific variables
debug:          CXXFLAGS += -O0 -ggdb
release:        CXXFLAGS += -O3 -g0 -DNDEBUG
prof:           CXXFLAGS += -O3 -DNDEBUG -pg
prof:           LDFLAGS  += -pg

pre-build:
	@# Creating build and executable path if necessary
	@mkdir -p $(BUILDDIR)

clean:
	find $(BUILDDIR) \( -name '*.o' -or -name '*.d' \) -delete
	$(RM) $(EXE)

$(EXE): $(OBJS)
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJS) $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# Use generated dependency files
-include $(OBJS:%.o=%.d)
