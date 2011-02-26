# This is my course project for "Introduction to Parallel Programming", winter 2010 @ FMI Sofia.

Right now, it implements a parallel modification of the O(n<sup>3</sup>) Warshaw algorithm for finding the transitive closure of the graph in O(n) time on n<sup>2</sup> processors.

Run it like this
<code>mpiexec -n 16 somename.exe 4 "input4.txt" "output4.txt"</code>