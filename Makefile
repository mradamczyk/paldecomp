CC=g++
CFLAGS=-std=c++11 -Wall -Wpedantic
LDFLAGS=
SOURCES=gusfield.cpp min_fact.cpp min_fact_with_gaps.cpp
EXECUTABLE=gusfield min_fact min_fact_with_gaps

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $@.cpp -o $@

gen-test:
	mkdir -p dna/
	for i in $$(seq 1 12); do echo $$i; python gen_dna.py $$i > dna/$$i.in; done

clean:
	rm -f *~ $(EXECUTABLE) dna/*
