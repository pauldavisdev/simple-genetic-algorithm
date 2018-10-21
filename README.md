# Simple Genetic Algorithm

Simple genetic algorithm which uses either roulette wheel or tournament selection, single-point crossover, and bit-wise mutation.

### Build

`make main`

### Run

Uses tournament selection as default.

`./main`

To use roulette wheel selection use command option -rw.

`./main -rw`

Plots graph showing max fitness across generations using [gnuplot](http://www.gnuplot.info/ "gnuplot").