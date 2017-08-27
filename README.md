# Install Graphviz

On MacOS:

`brew install cmake graphviz`

# Install

`. ./install.sh`

will install external `sdsl-lite` library and python virtualenv with required libraries (`networkx` and `graphviz`).

# Drawing decomposition:

Make sure your virtualenv is active:
`. v/bin/activate`

Make and Run:

`make`

`./max_pal_decomp_gaps_errors.x -d -p -L 11 -G 2 -E 3 < example.input | python drawDecomposition.py -d -E 3 -f outputFile`

# Known issues:

https://stackoverflow.com/questions/26185978/macos-wchar-h-file-not-found
