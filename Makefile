CC=g++
CXX=g++
CFLAGS=-std=c++11 -Wall -Wpedantic
EXECUTABLES=min_pal_fact max_pal_decomp_gaps_errors pal_decomp_gaps

all: $(EXECUTABLES)

SRC_DIRS ?= src
BUILD_DIR ?= build
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.hpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
$DEPS := $(OBJS:.o=.d)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.hpp.o: %.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

min_pal_fact: $(OBJS) src/min_pal_fact.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) src/min_pal_fact.cpp -o $@

pal_decomp_gaps: $(OBJS) src/pal_decomp_gaps.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) src/pal_decomp_gaps.cpp -o $@

max_pal_decomp_gaps_errors: $(OBJS) src/max_pal_decomp_gaps_errors.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) src/max_pal_decomp_gaps_errors.cpp -o $@

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
	rm -rf *~ $(BUILD_DIR) $(EXECUTABLES) dna/* fici.out brute.out

-include $(DEPS)
	
