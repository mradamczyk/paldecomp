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

# Examples from thesis:

`./max_pal_decomp_gaps_errors.x -d -G 4 -E 3 -L 14 -H -p < examples/ab220944.in | python drawDecomposition.py -d -E 3 -f max-pal-ham`
`./max_pal_decomp_gaps_errors.x -d -G 4 -E 3 -L 14 -p < examples/ab220944.in | python drawDecomposition.py -d -E 3 -f max-pal-edit-1`
`./max_pal_decomp_gaps_errors.x -d -G 4 -E 4 -L 20 -p < examples/ab220944.in | python drawDecomposition.py -d -E 4 -f max-pal-edit-2`

`./max_pal_decomp_gaps_errors.x -d -L 5 -G 2 -E 2 -p < examples/ab286863.in | python drawDecomposition.py -d -E 2 -f not-maximal-max`
`./all_pal_decomp_gaps_errors.x -d -L 5 -G 2 -E 2 -p < examples/ab286863.in | python drawDecomposition.py -d -E 2 -f not-maximal-all`

# Known issues:

https://stackoverflow.com/questions/26185978/macos-wchar-h-file-not-found
