UNIT_TESTS_DIR := test/unit
UNIT_TESTS_BUILD_DIR := $(TMP_DIR)/test/unit
UNIT_TESTS_SRC := $(shell find $(UNIT_TESTS_DIR) -name "*.cpp")
UNIT_TESTS_OBJ := $(addprefix $(UNIT_TESTS_BUILD_DIR)/, $(patsubst $(UNIT_TESTS_DIR)/%,%,$(patsubst %.cpp,%.o,$(UNIT_TESTS_SRC))))
UNIT_TESTS_EXE := $(UNIT_TESTS_BUILD_DIR)/test-all

$(UNIT_TESTS_BUILD_DIR)/%.o: $(UNIT_TESTS_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(UNIT_TESTS_EXE): $(UNIT_TESTS_OBJ) $(OBJS)
	$(CXX) -o $@ -lgtest -lgmock $(LDFLAGS) $(UNIT_TESTS_OBJ) $(OBJS)

unit_test: $(UNIT_TESTS_EXE)
	cd test/mocks && ../../tmp/test/unit/test-all

test: executables unit_test
	for testScript in $(wildcard test/scripts/auto_*); do echo $$testScript; $$testScript; done