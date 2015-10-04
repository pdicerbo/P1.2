Report
-------------------

First Part
===========

The solution to first part is provided by **init_script**. This script produces the *plot.png* through the
*plot.py* python script. The program *stream* performs a very large amount of simples operations in order to
check the maximum bandwidth of the system. The program contains several OpenMP directives, thus has been
running with a variable number of threads in a given node of ulysess cluster. Roughly, we expect an increasing
bandwidth as long as the number of threads increases too. This appens because each single thread contributes to
increase the number of data processed per second. However, if the number of threads exceeds 10 the
bandwidth slightly decreases. This results are summarized in *plot.png*.

Second Part
===========

In the second part I have tested the *stream* program in two different configurations with the *numactl* command.

