CC=g++
CFLAGS=-std=c++11 -Wall -Wpedantic
LDFLAGS=
EXECUTABLES=min_fact min_fact_with_gaps max_pal_decomp_gaps_errors

all: $(EXECUTABLES)
	
min_fact: min_fact.cpp
	$(CC) $(CFLAGS) $@.cpp -o $@

min_fact_with_gaps: min_fact_with_gaps.cpp
	$(CC) $(CFLAGS) $@.cpp -o $@

max_pal_decomp_gaps_errors: max_pal_decomp_gaps_errors.cpp
	$(CC) $(CFLAGS) $@.cpp -o $@

gen-test:
	mkdir -p dna/
	for i in $$(seq 1 10); do echo $$i; python gen_dna.py $$i > dna/$$i.in; done
	python gen_random_dna.py 1000000 30 > dna/random.in

s-test: gen-test
	./min_fact_with_gaps -L 4 -G 10 -b < dna/random.in > brute.out 2>/dev/null
	./min_fact_with_gaps -L 4 -G 10 < dna/random.in > fici.out 2>/dev/null
	diff fici.out brute.out

d-test: gen-test
	./min_fact_with_gaps -d -L 4 -G 10 -b < dna/random.in > brute.out 2>/dev/null
	./min_fact_with_gaps -d -L 4 -G 10 < dna/random.in > fici.out 2>/dev/null
	diff fici.out brute.out

clean:
	rm -f *~ $(EXECUTABLES) dna/* fici.out brute.out
