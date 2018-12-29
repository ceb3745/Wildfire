commit 92e6c72c281834a7a6e3c438d4e2c839e363dd72
Author: Carson Elizabeth Bloomingdale <ceb3745@cs.rit.edu>
Date:   Mon Feb 26 15:40:53 2018 -0500

    Created my own copy function which allows me to selectively copy
    the original array into the copy array so that the pointee of the
    original array is not simply placed into the copy array. Assigned
    the self values associated with the orig array and then simply
    declare references to itself for the copy array.
    
    Created the function water which uses the indices of the copy array
    to delete any pre existing fires from the original array.
    
    Inserted the final end condition and line and then began to implement
    a change count which seems like an insanely high number.

commit b038a98bf61a680a5c3284776220f9913a501936
Author: Carson Elizabeth Bloomingdale <ceb3745@cs.rit.edu>
Date:   Mon Feb 26 15:23:08 2018 -0500

    Created the project itself and implemented ideas from the write up and
    from the homework 4 on Conway's Game of Life. This time creating
    a struct called cell that contain it's state and then creating an outer
    struct called tree that actually contained a struct of itself and then
    a pointer to all of the neighboring structs situated in a multi-dimensional
    array that is representative of the forest.
    
    Implemented basic data checking for reaching in the parameters. Shot out
    error message in case of out of bounds variables. Then converted variables
    to decimals for further study. Created the function argtoInt which checks
    if it is possible for the arguement to be converted to an int.
    
    I then began counting neighbors and found that I would get a SEG FAL if
    I reached outside of the array bounds, and instead assigned which neighboring
    cells it would check depending on which part of the array it was in.
