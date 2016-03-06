# RandomHomology
> Random Filtration of a GIC. Outputs a sequence of collapses which can be used to 
calculate homology (via SimPers).

This software creates a 'random' filtration of Graph Induced Complex. To create the 
filtration a preordering is designated to the N-Dimensional input point set via Morton 
Codes. This ordering 
allows us to subsample the point set and still respect the underlying density of the 
data. The output is a file containing a sequence of collapses which can be used with the 
GIC to compute 
homology.

# Installation
```
echo "TODO";
```

# Dependencies
- ANN
- Boost
- GLUT
- OpenGL

# References
 - [Graph Induced Complex](http://web.cse.ohio-state.edu/~tamaldey/GIC/gic.html) Tamal K. Dey, Fengtao Fan, Yusu Wang.
 - [Morton Codes](http://www.forceflow.be/2013/10/07/morton-encodingdecoding-through-bit-interleaving-implementations/) Jeroen Baert
 - [Z-Order Curve](https://en.wikipedia.org/wiki/Z-order_curve) - Wikipedia
 - [SimPers](http://web.cse.ohio-state.edu/~tamaldey/SimpPers/SimpPers-software/)

# License
[![CC 
BY-NC-ND](https://licensebuttons.net/l/by-nc-nd/4.0/88x31.png)](https://creativecommons.org/licenses/)

