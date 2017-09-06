include ./external/sdsl-lite/Make.helper
CXX=$(MY_CXX)
CCLIB=-lsdsl -ldivsufsort -ldivsufsort64
EXECUTABLES=min_pal_fact max_pal_decomp_gaps_errors pal_decomp_gaps test_lgpal

SRC_DIRS=src
BUILD_DIR=build
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CXXFLAGS= $(MY_CXX_FLAGS) $(MY_CXX_OPT_FLAGS) -std=c++11 -DNDEBUG -O3 -MMD -MP $(INC_FLAGS) -I$(INC_DIR) 

SRCS := $(shell find $(SRC_DIRS) -mindepth 2 -name "*.cpp" -or -name "*.hpp")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

SOURCES=$(shell find $(SRC_DIRS) -maxdepth 1 -name "*.cpp" -exec basename {} \;)
EXECUTABLES=$(SOURCES:.cpp=.x)

all: $(EXECUTABLES)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.hpp.o: %.hpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.x: src/%.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(CCLIB) $(LDFLAGS) -L$(LIB_DIR)

gen-test:
	mkdir -p dna/

pal_decomp_gaps-s-test: gen-test
	python gen_random_dna.py 100000 30 > dna/random.in
	for i in $$(seq 1 10); do \
		echo "G: " $$i; \
		./pal_decomp_gaps.x -L 4 -G $$i -b < dna/random.in > brute.out 2>/dev/null; \
		./pal_decomp_gaps.x -L 4 -G $$i < dna/random.in > fast.out 2>/dev/null; \
		diff fast.out brute.out; \
	done;

pal_decomp_gaps-s-big-test: gen-test
	python gen_random_dna.py 10000 200 > dna/random.in
	for i in $$(seq 1 10); do \
		echo "G: " $$i; \
		./pal_decomp_gaps.x -L 7 -G $$i -e < dna/random.in > eertree.out 2>/dev/null; \
		./pal_decomp_gaps.x -L 7 -G $$i < dna/random.in > fast.out 2>/dev/null; \
		diff fast.out eertree.out; \
	done;

pal_decomp_gaps-time: gen-test
	python gen_random_dna.py 1000000 10 > dna/random.in
	echo "G: 3";
	time ./pal_decomp_gaps.x -L 7 -G 3 -e < dna/random.in > eertree.out 2>/dev/null;
	time ./pal_decomp_gaps.x -L 7 -G 3 < dna/random.in > fast.out 2>/dev/null;
	diff fast.out eertree.out;
	python gen_random_dna.py 1000000 10 > dna/random.in
	echo "G: 30";
	time ./pal_decomp_gaps.x -L 7 -G 30 -e < dna/random.in > eertree.out 2>/dev/null;
	time ./pal_decomp_gaps.x -L 7 -G 30 < dna/random.in > fast.out 2>/dev/null;
	diff fast.out eertree.out;
	python gen_random_dna.py 300000 10 > dna/random.in
	echo "G: 300";
	time ./pal_decomp_gaps.x -L 7 -G 300 -e < dna/random.in > eertree.out 2>/dev/null;
	time ./pal_decomp_gaps.x -L 7 -G 300 < dna/random.in > fast.out 2>/dev/null;
	diff fast.out eertree.out;
	python gen_random_dna.py 30000 10 > dna/random.in
	echo "G: 3000";
	time ./pal_decomp_gaps.x -L 7 -G 3000 -e < dna/random.in > eertree.out 2>/dev/null;
	time ./pal_decomp_gaps.x -L 7 -G 3000 < dna/random.in > fast.out 2>/dev/null;
	diff fast.out eertree.out;


pal_decomp_gaps-d-test: gen-test
	for i in $$(seq 1 10); do echo $$i; python gen_dna.py $$i > dna/$$i.in; done
	for i in $$(seq 1 10); do \
		echo "len: " $$i; \
		for g in $$(seq 1 10); do \
			echo "G: " $$g; \
			./pal_decomp_gaps.x -d -L 2 -G $$g -b < dna/$$i.in > brute.out 2>/dev/null; \
			./pal_decomp_gaps.x -d -L 2 -G $$g -e < dna/$$i.in > eertree.out 2>/dev/null; \
			./pal_decomp_gaps.x -d -L 2 -G $$g < dna/$$i.in > fast.out 2>/dev/null; \
			diff fast.out brute.out; \
			diff eertree.out brute.out; \
		done;\
	done;
	python gen_random_dna.py 100000 30 > dna/random.in
	for i in $$(seq 1 10); do \
		echo "G: " $$i; \
		./pal_decomp_gaps.x -d -L 4 -G $$i -b < dna/random.in > brute.out 2>/dev/null; \
		./pal_decomp_gaps.x -d -L 4 -G $$i -e < dna/random.in > eertree.out 2>/dev/null; \
		./pal_decomp_gaps.x -d -L 4 -G $$i < dna/random.in > fast.out 2>/dev/null; \
		diff fast.out brute.out; \
		diff eertree.out brute.out; \
	done;

max_pal_decomp_gaps_errors-s-test: gen-test
	python gen_random_dna.py 100000 18 > dna/random.in
	for i in $$(seq 1 10); do \
		echo "G: " $$i; \
		./max_pal_decomp_gaps_errors.x -L 4 -G $$i -b < dna/random.in > brute.out 2>/dev/null; \
		./max_pal_decomp_gaps_errors.x -L 4 -G $$i < dna/random.in > fast.out 2>/dev/null; \
		diff fast.out brute.out; \
	done;

max_pal_decomp_gaps_errors-d-test: gen-test
	python gen_random_dna.py 10 18 > dna/random.in
	echo "edit";
	for e in $$(seq 0 3); do \
		echo "E: " $$e; \
		for g in $$(seq 0 3); do \
			echo "G: " $$g; \
			./max_pal_decomp_gaps_errors.x -d -L 4 -G $$i -E $$e -b < dna/random.in > brute.out 2>/dev/null; \
			./max_pal_decomp_gaps_errors.x -d -L 4 -G $$i -E $$e < dna/random.in > fast.out 2>/dev/null; \
			diff fast.out brute.out; \
		done;\
	done;
	echo "Hamming";
	for e in $$(seq 0 3); do \
		echo "E: " $$e; \
		for g in $$(seq 0 3); do \
			echo "G: " $$g; \
			./max_pal_decomp_gaps_errors.x -d -h -L 4 -h -G $$i -E $$e -b < dna/random.in > brute.out 2>/dev/null; \
			./max_pal_decomp_gaps_errors.x -d -h -L 4 -h -G $$i -E $$e < dna/random.in > fast.out 2>/dev/null; \
			diff fast.out brute.out; \
		done;\
	done;

.PHONY: clean
clean:
	rm -rf *~ $(BUILD_DIR) $(EXECUTABLES) *.d dna fast.out brute.out eertree.out *.gv *.gv.pdf

deepclean:
	clean
	rm -rf include/ lib/

-include $(DEPS)
	
