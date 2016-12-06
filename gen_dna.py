import itertools
import sys

if __name__ == '__main__':
    for s in (''.join(i) for i in itertools.product("ACGT",repeat=int(sys.argv[1]))):
        print s
