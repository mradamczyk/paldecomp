CC=g++
CFLAGS=-std=c++11 -Wall -Wpedantic
LDFLAGS=
SOURCES=gusfield.cpp min_fact.cpp min_fact_trimmed.cpp min_fact_with_gaps.cpp
EXECUTABLE=gusfield min_fact min_fact_trimmed min_fact_with_gaps

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(SOURCES)
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
	rm -f *~ $(EXECUTABLE) dna/* fici.out brute.out
