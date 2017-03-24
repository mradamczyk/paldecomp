# Install Graphviz

On MacOS:

`brew install graphviz`

# Install

`. ./install.sh`

will install external `sdsl-lite` library and python virtualenv with required libraries (`networkx` and `graphviz`).

# Drawing decomposition:

Make sure your virtualenv is active:
`. v/bin/activate`

Run:
`./max_pal_decomp_gaps_errors.x -d -p -L 11 -G 2 -E 3 < example.input | python drawDecomposition.py -d -E 3`
