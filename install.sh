git submodule update --init --recursive
./external/sdsl-lite/install.sh .

virtualenv --python=python2.7 v
. v/bin/activate

pip install networkx graphviz

