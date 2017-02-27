include ./external/sdsl-lite/Make.helper
CXX=$(MY_CXX)
CCLIB=-lsdsl -ldivsufsort -ldivsufsort64
EXECUTABLES=min_pal_fact max_pal_decomp_gaps_errors pal_decomp_gaps test_lgpal

SRC_DIRS=src
BUILD_DIR=build
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CXXFLAGS= $(MY_CXX_FLAGS) $(MY_CXX_OPT_FLAGS) -std=c++11 -DNDEBUG -O3 -MMD -MP $(INC_FLAGS) -I$(INC_DIR) -L$(LIB_DIR)

SRCS := $(shell find $(SRC_DIRS) -mindepth 2 -name "*.cpp" -or -name "*.hpp")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

SOURCES=$(shell find $(SRC_DIRS) -name "*.cpp" -depth 1 -exec basename {} \;)
EXECUTABLES=$(SOURCES:.cpp=.x)

all: $(EXECUTABLES)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.hpp.o: %.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.x: src/%.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(CCLIB) $(LDFLAGS)

gen-test:
	mkdir -p dna/
	for i in $$(seq 1 10); do echo $$i; python gen_dna.py $$i > dna/$$i.in; done
	python gen_random_dna.py 1000000 30 > dna/random.in

s-test: gen-test
	./build/min_fact_with_gaps -L 4 -G 10 -b < dna/random.in > brute.out 2>/dev/null
	./build/min_fact_with_gaps -L 4 -G 10 < dna/random.in > fici.out 2>/dev/null
	diff fici.out brute.out

d-test: gen-test
	./build/min_fact_with_gaps -d -L 4 -G 10 -b < dna/random.in > brute.out 2>/dev/null
	./build/min_fact_with_gaps -d -L 4 -G 10 < dna/random.in > fici.out 2>/dev/null
	diff fici.out brute.out

.PHONY: clean
clean:
	rm -rf *~ $(BUILD_DIR) $(EXECUTABLES) *.d dna/* fici.out brute.out

deepclean:
	clean
	rm -rf include/ lib/

-include $(DEPS)
	
