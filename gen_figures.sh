./max_pal_decomp_gaps_errors.x -d -G 4 -E 3 -L 14 -H -p < examples/ab220944.in | python drawDecomposition.py -d -H -E 3 -f max-pal-ham
./max_pal_decomp_gaps_errors.x -d -G 4 -E 3 -L 14 -p < examples/ab220944.in | python drawDecomposition.py -d -E 3 -f max-pal-edit-1
./max_pal_decomp_gaps_errors.x -d -G 4 -E 4 -L 20 -p < examples/ab220944.in | python drawDecomposition.py -d -E 4 -f max-pal-edit-2

./max_pal_decomp_gaps_errors.x -d -L 5 -G 2 -E 2 -p < examples/ab286863.in | python drawDecomposition.py -d -E 2 -f not-maximal-max
./all_pal_decomp_gaps_errors.x -d -L 5 -G 2 -E 2 -p < examples/ab286863.in | python drawDecomposition.py -d -E 2 -f not-maximal-all

python ./drawDecomposition.py -d -E 1 < examples/vis-ex.in -f vis-ex

cat examples/eertree-ex2.dot |  neato -Tpdf > eertree-ex2.pdf
cat examples/eertree-ex1.dot |  neato -Tpdf > eertree-ex1.pdf
cat examples/eertree-ex1-step0.dot |  neato -Tpdf > eertree-ex1-step0.pdf
cat examples/eertree-ex1-step1.dot |  neato -Tpdf > eertree-ex1-step1.pdf
cat examples/eertree-ex1-step2.dot |  neato -Tpdf > eertree-ex1-step2.pdf
cat examples/eertree-ex1-step3.dot |  neato -Tpdf > eertree-ex1-step3.pdf
cat examples/eertree-ex1-step4.dot |  neato -Tpdf > eertree-ex1-step4.pdf
cat examples/eertree-ex1-step5.dot |  neato -Tpdf > eertree-ex1-step5.pdf
cat examples/eertree-ex1-step6.dot |  neato -Tpdf > eertree-ex1-step6.pdf
cat examples/eertree-ex1-step7.dot |  neato -Tpdf > eertree-ex1-step7.pdf
