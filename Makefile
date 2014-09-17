PROJECT := threephase

## DIRECTORIES
SRC_DIR := src
LIB_SRC_DIR := $(SRC_DIR)/lib
BUILD_DIR := build
DIST_DIR := dist

## SOURCES
CXX_SRC := $(shell find $(LIB_SRC_DIR) -name "*.cpp")
HXX_SRC := $(shell find $(LIB_SRC_DIR) -name "*.h")

## OBJECTS
CXX_OBJS := $(addprefix $(BUILD_DIR)/, $(patsubst $(SRC_DIR)/%,%,$(patsubst %.cpp,%.o,$(CXX_SRC))))
CXX_DEPS := $(CXX_OBJS:.o=.d)

OBJS := $(CXX_OBJS)
OBJS_DIRS := $(dir $(OBJS))

###############################################################################
## DIST

## LIBRARIES OUTPUT
LIB_DIST_DIR := $(DIST_DIR)/lib
LIB_SHARED := $(LIB_DIST_DIR)/lib$(PROJECT).so
LIB_STATIC := $(LIB_DIST_DIR)/lib$(PROJECT).a

## EXECUTABLES OUTPUT
APPS := projector engine
BIN_DIST_DIR := $(DIST_DIR)/bin
EXECUTABLES_DIST := $(addprefix $(BIN_DIST_DIR)/$(PROJECT)-, $(APPS))
EXECUTABLES_OBJS := $(addsuffix .o,$(addprefix $(BUILD_DIR)/, $(APPS)))
EXECUTABLES := $(addprefix $(BUILD_DIR)/, $(APPS))

## HEADERS OUTPUT
HEADERS_DIST_DIR := $(DIST_DIR)/include
HEADERS_DIST := $(addprefix $(HEADERS_DIST_DIR)/, $(patsubst $(LIB_SRC_DIR)/%,%,$(HXX_SRC)))

###############################################################################
## COMPILER

## INCLUDES
INCLUDE_DIRS += $(LIB_SRC_DIR)

## LIBRARIES
LIBRARIES += pcl_common
LIBRARIES += pcl_io
LIBRARIES += pcl_filters
LIBRARIES += pcl_visualization
LIBRARIES += opencv_highgui
LIBRARIES += opencv_imgproc
LIBRARIES += opencv_core
LIBRARIES += opencv_contrib
LIBRARIES += boost_system
LIBRARIES += boost_thread
LIBRARIES += GLEW
LIBRARIES += glfw
LIBRARIES += GL
LIBRARIES += rt

## COMPILER WARNINGS
WARNINGS := -pedantic -Wall -Wextra -c -fmessage-length=0

## COMPILER FLAGS (DEBUG/RELEASE)
ifeq ($(DEBUG), 1)
	COMMON_FLAGS += -DDEBUG -g3 -O0
else
	COMMON_FLAGS += -DNDEBUG -O2
endif

COMMON_FLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
EXTRA_FLAGS += -pthread -fPIC -std=c++11

CXXFLAGS = $(EXTRA_FLAGS) $(COMMON_FLAGS) $(WARNINGS)

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

###############################################################################
## TARGETS

.PHONY: all test clean distclean includes libraries executables default

default: test

## build single object
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@  -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $<

## build executable
$(EXECUTABLES): $(OBJS) $(EXECUTABLES_OBJS)
	mkdir -p $(BIN_DIST_DIR)
	$(CXX) -o $@ $(LDFLAGS) $@.o $(OBJS)

## build shared library
$(LIB_SHARED): $(OBJS)
	mkdir -p $(LIB_DIST_DIR)
	$(CXX) -shared -o $@ $(OBJS) $(LDFLAGS)

## build static library
$(LIB_STATIC): $(OBJS)
	mkdir -p $(LIB_DIST_DIR)
	ar rcs $@ $(OBJS)

## copy headers to dist/include
$(HEADERS_DIST_DIR)/%.h: $(LIB_SRC_DIR)/%.h
	mkdir -p $(dir $@)
	cp $< $@

test:
	@echo $(EXECUTABLES)
	@echo $(EXECUTABLES_DIST)
	@echo $(EXECUTABLES_OBJS)


executables: $(EXECUTABLES)

libraries: $(LIB_STATIC) $(LIB_SHARED)

includes: $(HEADERS_DIST)

## build libraries and executables
all: libraries executables

## build libraries executables and miscellaneus
dist: all includes

## install
install:
	@echo install not implemented

## doc
doc:
	@echo install not implemented

## clean libraries and executables
clean:
	rm -f $(OBJS) $(CXX_DEPS)  $(EXECUTABLES_OBJS)
	rm -f $(LIB_SHARED) $(LIB_STATIC) $(EXECUTABLES_DIST)

## clean everything
distclean: clean
	rm -rf $(DIST_DIR) $(BUILD_DIR)

-include $(CXX_DEPS)
