PROJECT := threephase

## DIRECTORIES
SRC_DIR := src
BUILD_DIR := build

## LIBRARIES OUTPUT
LIB_BUILD_DIR := $(BUILD_DIR)/lib
LIB_SHARED := $(LIB_BUILD_DIR)/lib$(PROJECT).so
LIB_STATIC := $(LIB_BUILD_DIR)/lib$(PROJECT).a

## EXECUTABLES OUTPUT
BIN_BUILD_DIR := $(BUILD_DIR)/bin
BIN := $(BIN_BUILD_DIR)/$(PROJECT)

## SOURCES
CXX_SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
HXX_SRCS := $(shell find $(SRC_DIR) -name "*.h")

## OBJECTS
CXX_OBJS := $(addprefix $(BUILD_DIR)/, ${CXX_SRCS:.cpp=.o})
OBJ_BUILD_DIR := $(BUILD_DIR)/src
OBJS := $(CXX_OBJS)

## INCLUDES
INCLUDE_DIRS += ./src

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

###############################################################################
## COMPILER FLAGS

## COMPILER WARNINGS
WARNINGS := -Wall -Wno-sign-compare

## COMPILER FLAGS (DEBUG/RELEASE)
ifeq ($(DEBUG), 1)
	COMMON_FLAGS += -DDEBUG -g -O0
else
	COMMON_FLAGS += -DNDEBUG -O2
endif

## 
COMMON_FLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))

CXXFLAGS = -pthread -fPIC $(COMMON_FLAGS) $(WARNINGS)

LDFLAGS += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(LIBRARIES),-l$(library))

###############################################################################
## TARGETS

$(LIB_SHARED): $(OBJS)
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS)

$(LIB_STATIC): $(OBJS)
	ar rcs $(TARGET) $(OBJS)

all: $(LIB_SHARED) $(LIB_STATIC)

clean:
	rm -f $(OBJS) $(LIB_SHARED) $(LIB_STATIC)
