# Install

`. ./install.sh`

will install external `sdsl-lite` library

# Install python 2.7 with libraries for drawing decomposition:

`pip install networkx`
`pip install graphviz`

# Install Graphviz

On MacOS:

`brew install graphviz`

# Drawing decomposition:

`./max_pal_decomp_gaps_errors.x -d -p -L 11 -G 2 -E 3 < example.input | python drawDecomposition.py -d -E 3`
