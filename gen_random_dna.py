import itertools
import sys
import random

if __name__ == '__main__':
    for i in xrange(int(sys.argv[1])):
        print ''.join([random.choice('ACGT') for n in xrange(int(sys.argv[2]))])

