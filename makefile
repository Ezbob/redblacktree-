########
#
# Makefile that finds C++-source files and generates compilation rules for
# use with the G++ compiler.
#
# This makefile relies a specific source tree organization created by the user:
#	- SOURCE_PREFIX: this director contains the different MODULES directories which in turn 
# contains the .cc/.cpp source files for the project
#	- TEST_PREFIX: contains the TEST_MODULES directories which contains the .cc/.cpp source files for 
# the project's tests
#	- INCLUDE_PREFIX: contains headers for the projects 
# 
# Using this source tree organization the following phony targets are availiable:
# 	- all: Default target. Compiles code in SOURCE_PREFIX and creates the EXEC exectutable in
# the BUILD_PREFIX folder under the "bin/" folder
#	- tests: Compiles the test code and creates the TEST_EXEC executable in
# the BUILD_PREFIX folder under the "bin/" folder
#	- run_tests: Relies on the "tests" target and then executes the TEST_EXEC
#	- run: Like run_tests, this target builds the source and runs the main executable
#	- rollout: Creates the source tree organization
#	- clean: Removes the BUILD_PREFIX directory
#	- testcheckdirs: Creates the directory tree for the compilation output from compiling the tests
#	- checkdirs: Creates the directory tree for the compilation output from compiling the EXEC executable
#
########
include buildvars.mk

SUFFIX = .cpp .C .cc .CC .CPP .cxx .cp .c++

BIN_PATH = $(addprefix $(BUILD_PREFIX), bin/)

MODULES_DIR = $(foreach mod, $(MODULES), $(mod)/ )
TEST_MODULES_DIR = $(foreach mod, $(TEST_MODULES), $(mod)/ )

# Artifact Dirs
EXEC_PATH = $(addprefix $(BIN_PATH), $(EXEC))
SRC_DIR = $(addprefix $(SOURCE_PREFIX), $(MODULES_DIR)) $(SOURCE_PREFIX)
BUILD_DIR = $(addprefix $(BUILD_PREFIX), $(addprefix $(COMPILED_DIR), $(MODULES_DIR))) $(addprefix $(BUILD_PREFIX), $(COMPILED_DIR))

# TEST dirs
TEXEC_PATH = $(addprefix $(BIN_PATH), $(TEST_EXEC))
TEST_SRC_DIR = $(addprefix $(TEST_PREFIX), $(TEST_MODULES_DIR)) $(TEST_PREFIX)
TEST_DIR = $(addprefix $(BUILD_PREFIX), $(addprefix $(T_COMPILED_DIR), $(TEST_MODULES_DIR)))

define findsrc
$(strip $(foreach dir, $1, $(foreach format, $(SUFFIX), $(wildcard $(strip $(dir))*$(format)))))
endef

define findobj
$(strip $(foreach suff, $(SUFFIX), $(patsubst %$(suff), %.o, $(filter $2%$(suff), $(foreach name, $3, $(subst $1, $2, $(name)))))))
endef

T_SRC = $(call findsrc, $(TEST_SRC_DIR))
T_OBJ = $(call findobj, $(TEST_PREFIX), $(BUILD_PREFIX)$(T_COMPILED_DIR), $(T_SRC))

SRC = $(call findsrc, $(SRC_DIR))
OBJ = $(call findobj, $(SOURCE_PREFIX), $(BUILD_PREFIX)$(COMPILED_DIR), $(SRC))

VPATH = $(SRC_DIR) $(TEST_SRC_DIR)

.PHONY: all test checkdirs testcheckdirs clean run_tests rollout valgrind valgrind_test

all: checkdirs $(EXEC_PATH)

tests: testcheckdirs $(TEXEC_PATH)

run_tests: tests
	exec $(TEXEC_PATH)

run: all
	exec $(EXEC_PATH)

rollout: $(INCLUDE_PREFIX) $(SRC_DIR) $(TEST_SRC_DIR)

testcheckdirs: checkdirs $(TEST_DIR)

checkdirs: $(BUILD_DIR) $(BIN_PATH)
	
clean:
	rm -rf $(BUILD_PREFIX)

valgrind: all
	valgrind $(VALGRIND_OPTS) $(EXEC_PATH)

valgrind_test: tests 
	valgrind $(VALGRIND_OPTS) $(TEXEC_PATH)

$(EXEC_PATH): $(OBJ)
	$(CXX) -iquote$(INCLUDE_PREFIX) -iquote$(TEMPLATE_PREFIX) $(CXXFLAGS) -o $@ $^  $(LDFLAGS) $(LDLIBS)

$(TEXEC_PATH): $(OBJ) $(T_OBJ)
	$(CXX) -iquote$(INCLUDE_PREFIX) -iquote$(TEMPLATE_PREFIX) $(CXXFLAGS) -o $@ $^  $(LDFLAGS) $(LDLIBS)

## Auto-generating rules

# Stuff in define gets evaled twice so we need double $s

GEN_DIRS = $(BIN_PATH) $(BUILD_DIR) $(TEST_DIR) $(SRC_DIR) $(TEST_SRC_DIR) $(INCLUDE_PREFIX) $(TEMPLATE_PREFIX)
GEN_CPP_RULES = $(BUILD_DIR) $(TEST_DIR)


define make-dirs
$1:
	mkdir -p $$@
endef

define cpp-rule
$1%.o:%$(strip $2)
	$(CXX) -iquote$(INCLUDE_PREFIX) -iquote$(TEMPLATE_PREFIX) $(CXXFLAGS) -c $$< -o $$@ $(LDFLAGS) $(LDLIBS)
endef

# the following line creates all the rules for the subdirectories
$(foreach dir, $(GEN_CPP_RULES), $(foreach format, $(SUFFIX), $(eval $(call cpp-rule, $(dir), $(format))) ))
$(foreach dir, $(GEN_DIRS), $(eval $(call make-dirs, $(dir))))
