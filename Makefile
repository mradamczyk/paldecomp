CC=g++
CFLAGS=-std=c++11 -Wall -Wpedantic
LDFLAGS=
EXECUTABLE=gusfield min_fact min_fact_with_gaps

all: $(EXECUTABLE .cpp) $(EXECUTABLE)
	
$(EXECUTABLE):
	$(CC) $(CFLAGS) $@.cpp -o $@

clean:
	rm *~ $(EXECUTABLE)
