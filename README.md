# cmathematics
Mathematics library for C and C++

Follows this tutorial playlist on [YouTube](https://www.youtube.com/playlist?list=PLysLvOneEETMjrK5N-PLIYhZKwmxjGs2-).

### Description
This library is a low level mathematics library for C and C++ that implements common mathematical concepts and ideas from algebra, calculus, statistics, and linear algebra. A complete list of implemented functions and concepts can be found [here](#complete-list-of-functions).

List of implemented concepts so far:
1) Big Integers
2) Vectors
3) Matrices

*Note: a good reference book is **Modern Computer Arithmetic** by Richard Brent and Paul Zimmerman*

### Documentation
Full documentation can be found in the header files in the `cmathematics` folder.

### Use
Simply drag the `cmathematics` folder into your C or C++ project, not the include directory, and it can be used as if it were apart of the project.

Example file structure:
```
PROJECT_DIR \
	...
    cmathematics \
    	cmathematics.h
        ...
    ...
    main.c
```

Sample use in `main.c`:
```c
#include "cmathematics/cmathematics.h"
#include "cmathematics/vec.h"

int main() {
	vec v1 = vector(2.0f, 1.0f, 3.4f);
    printVec(v1);
    
    return 0;
}
```

### Complete list of functions
##### Utility
1) Big Integers
    1) Parsing from string
    2) Output to string
    3) Comparison
    4) Addition, subtraction
    5) Multiplication (Long, Karatsuba)
##### Linear Algebra
1) Vectors
	1) Vector-Scalar operations (+, -, *, /, ^)
    2) Vector-Vector operations (+, -, .*, ./, dot, cross)
    3) Magnitude, Squared Magnitude
    4) Normalize
2) Matrices
    1) Matrix-Scalar operations (+, -, *, /)
    2) Matrix-Vector multiplication
    3) Matrix-Matrix multiplication
    4) Matrix operations
        1) Transpose
        2) Elementary row operations
        3) Row Echelon Form/Reduced Row Echelon Form
        4) Splicing
        5) Determinant (through 3 different methods)
        6) Augmentation (vectors and matrices)
        7) Cofactors/adjugates
        8) Matrix Inverses
