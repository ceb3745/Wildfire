/*
*  This program is created to simulate a Wildfire using
*  a matrix of structs that contains an array of pointers to
*  the outer array indicating the cell's neighbors.
*
*  Uses status to check if neighbors are on fire and if at least two
*  neighboring trees are on fire that cell catches fire itself.
*
*  Function also handles the situation where on the next cycle
*  any fires from the previous cycle go out.
*
*  Handles the printing and displaying dependant on optional function 
*  arguements  
*
*  @author Carson Bloomingdale || ceb3745
*/

#define SIZE 10
#define INT_MAX 2147483647
#define _BSD_SOURCE
#define MESSAGE "usage: wildfire [-pN] size probability treeDensity proportionBurning\nThe -pN option tells the simulation to print N cycles and stop.\nThe probability is the probability a tree will catch fire."


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include "display.h"

extern int errno;
// create a struct called cell to initialize the board
// create an array to symbolize the neighoring cells in the order as follows:
// N - North, S - South, W - West, E - East
// 0 - NW, 1 - N, 2 - NE, 3 - E, 4 - SE, 5 - S, 6 - SW, 7 - W
// create a integer status that with either be 
// 0 - empty cell, 1 - tree cell, 2 - burning cell, 3- burnt cell

struct cell {
    int status;
};

struct tree {
    struct cell *neighbors[8];
    struct cell self;
};


 //Converts the arguement character string to an int to be checked.
int argToInt( char *str){

    char *p;
    int num = 0;

    errno = 0;
    
    long conv = strtol(str, &p, 10);

    // Check to see that the number inside the long can be converted to an integer
    if (errno != 0 || *p != '\0' || conv > INT_MAX){
        return 0;
    } 
    else{
        // Continue if no error
        num = conv;    
        return num;
    }
    
}

/*
Function that takes in a struct tree and looks at the indices of each neighbor
tree and then decided what to do based on the array. Using if statements I checked
row and column and then gave rules of which neighbors would be defined in each case.
This is major safegaurd against Seg Fals in case of NUL pointers.
*/
int neighbors(struct tree copy, int row, int col, int size){
    int neighbors = 0;
    
    // check for corners
    if (row == 0 && col == 0){
        if (copy.neighbors[3]->status == 2){
          neighbors++;
        }
        if (copy.neighbors[4]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[5]->status == 2){
            neighbors++;
        }
    }
    else if (col == size - 1 && row == size - 1){
        if (copy.neighbors[7]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[0]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[1]->status == 2){
            neighbors++;
        }
    }
    else if (col == 0 && row == size - 1){
        if (copy.neighbors[1]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[2]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[3]->status == 2){
            neighbors++;
        }
    }
    else if (col == size - 1 && row == 0){
        if (copy.neighbors[5]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[6]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[7]->status == 2){
            neighbors++;
        }
    //check for borders
    }
    else if (row == 0){
        if (copy.neighbors[3]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[4]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[5]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[6]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[7]->status == 2){
            neighbors++;
        }        
    }
    else if (col == 0){
        
        if (copy.neighbors[1]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[2]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[3]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[4]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[5]->status == 2){
            neighbors++;
        }  
    }
    else if (col == size - 1){
        
        if (copy.neighbors[0]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[1]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[5]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[6]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[7]->status == 2){
            neighbors++;
        }  
    }
    else if (row == size - 1){
        
        if (copy.neighbors[0]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[1]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[2]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[3]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[7]->status == 2){
            neighbors++;
        }  
    //what to do for any generic cell
    }
    else{
        if (copy.neighbors[0]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[1]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[2]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[3]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[4]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[5]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[6]->status == 2){
            neighbors++;
        }
        if (copy.neighbors[7]->status == 2){
            neighbors++;
        }
    }
    return neighbors;
}
 
/*
Function used to implement the rules of spreading fire. It rotates through the entire array 
to check if such indice is a tree and subsequently check if the neighboring cell is a tree 
on fire. If there is at least two trees on fire then generate the random number x to be compared
to probability. This number x is between 0 and 1 and if that number is smaller than the 
probability, then that cell starts burning. returns the changes made in order to be able to hav
a total count of the changes in the main function. 
*/
int rules(int size, struct tree orig[][size], struct tree treecopy[][size], float prob, int change){
    int row, col;
    int burning = 0;
    
    //loop through all the rows and columns that are trees.
    for ( row = 0; row < size; row++){
    
        for ( col = 0; col < size; col++){
            
            if (orig[row][col].self.status == 1){
            
                //counts burning neighbors
                burning = neighbors(treecopy[row][col], row, col, size);
                
                // if 25% of the neighbors are burning, then check. 
                //We know 25% of the neighbors is equivalent to 2 neighbors.
                if (burning >= 2){
                    float x = rand() % 100;
                    x = x/100;
                    if (x < prob){
                        orig[row][col].self.status = 2; 
                        //count the changes made
                        change++;
                    }
                }
            
            }
            
        }
    
    }
    
    return change;
    
}

/*
Separate function that uses the indices of the treecopy's burning trees to delete the remaining 
burning trees in the original array. This function also returns the number of changes to 
attribute to the total count of changes in the main function. 
*/
int water(int size, struct tree orig[][size], struct tree treecopy[][size], int change, float prob){
    int row, col;
    
    //loop through all of the burning trees in the copy array
    for (row = 0; row < size; row++){
    
        for (col = 0; col < size; col++){
        
            if (treecopy[row][col].self.status == 2){
                
                //change the original array to a burnt tree and increment change
                float x = rand() % 100;
                x = x/100;
                if (x < prob){
                    orig[row][col].self.status = 3; 
                    //count the changes made
                    change++;
                }
            
            }
        
        }
    
    }
    return change;
}

/*
Personalized copy function used for copying my array of structs. Unable to use memcopy 
because it will copy the exact pointers of the neighbors in that array to the copy where
we need to initialize new pointers that point to the copy function for correct implementation.
*/
void copy(int size, struct tree orig[][size], struct tree treecopy[][size]){

    //loop through the copy and orig function and reset the self values and assign the pointers. 
    for (int i=0; i < size; i++){
    
        for (int j=0; j < size; j++){
        
            treecopy[i][j].self.status = orig[i][j].self.status;
            treecopy[i][j].neighbors[0] = &treecopy[i-1][j-1].self;
            treecopy[i][j].neighbors[1] = &treecopy[i-1][j].self;
            treecopy[i][j].neighbors[2] = &treecopy[i-1][j+1].self;
            treecopy[i][j].neighbors[3] = &treecopy[i][j+1].self;
            treecopy[i][j].neighbors[4] = &treecopy[i+1][j+1].self;
            treecopy[i][j].neighbors[5] = &treecopy[i+1][j].self;
            treecopy[i][j].neighbors[6] = &treecopy[i+1][j-1].self;
            treecopy[i][j].neighbors[7] = &treecopy[i][j-1].self;
            
        }
    
    }
    return;
}

int main (int argc, char *argv[]){
    
    //define and initialized variables
    bool onFire = 1;
    int totchange = 0;
    int size = SIZE;
    int gens = -1;
    int row;
    int col;
    int pN = 0;
    long prob;
    long dens;
    long prop;
    
    if (argc == 1){
        printf("%s\n", MESSAGE);
        return 1;
    }
    
    //check to see if the arguement number is 5 for the normal implementation
    if (argc == 5){
    
        size = argToInt(argv[1]);
        prob = argToInt(argv[2]);
        dens = argToInt(argv[3]);
        prop = argToInt(argv[4]);
    
    }
    //check to see if the arguement number is 6 for the pN implementation
    else if (argc == 6){
        
        sscanf(argv[1], "-p%i", &pN);
        size = argToInt(argv[2]);
        prob = argToInt(argv[3]);
        dens = argToInt(argv[4]);
        prop = argToInt(argv[5]);
        onFire = 0;
        
        if (pN < 0){
            fprintf(stderr,"\nThe -pN option was negative.\n %s\n", MESSAGE);
            return EXIT_FAILURE;
        }else if (pN == 0){
            fprintf(stderr,"\nThe -pN option was invalid.\n %s\n", MESSAGE);
            return EXIT_FAILURE;
        }
    
    }
  
    //checks to see if variables and valid and within the range
    if (size == 0 || size > 40 || size < 5) {
        fprintf(stderr, "The size ""(""%i"")"" must be an integer in [5...40].\n%s\n", size, MESSAGE);
        return EXIT_FAILURE;
    }
    if ( prob > 100 || prob < 0){
        fprintf(stderr, "The probability ""(""%ld"")"" must be an integer in [0...100].\n%s\n", prob, MESSAGE);
        return EXIT_FAILURE;
    }
    if ( dens > 100 || dens < 0){
        fprintf(stderr, "The density ""(""%ld"")"" must be an integer in [0...100].\n%s\n", dens, MESSAGE);
        return EXIT_FAILURE;
    }
    if ( prop > 100 || prop < 0){
        fprintf(stderr,"The proportion ""(""%ld"")"" must be an integer in [0...100].\n%s\n", prop, MESSAGE);
        return EXIT_FAILURE;
    }
    
    //converts the numbers to decimals
    float probf = ((float)prob/100);
    float densf = ((float)dens/100);
    float propf = ((float)prop/100);

    //printf("\n %f, %f, %f \n", probf, densf, propf);
    
    //initializes the structs that will contain our statuses
    struct tree forest[size][size];
    struct tree cpyforest[size][size];
    
    //initializes the original struct and assigns the address of the pointers to the outer array
    for (int i = 0; i <= size -1; i++){
        for (int j = 0; j <= size -1; j++){
                forest[i][j].self.status = 0;
                forest[i][j].neighbors[0] = &forest[i-1][j-1].self;
                forest[i][j].neighbors[1] = &forest[i-1][j].self;
                forest[i][j].neighbors[2] = &forest[i-1][j+1].self;
                forest[i][j].neighbors[3] = &forest[i][j+1].self;
                forest[i][j].neighbors[4] = &forest[i+1][j+1].self;
                forest[i][j].neighbors[5] = &forest[i+1][j].self;
                forest[i][j].neighbors[6] = &forest[i+1][j-1].self;
                forest[i][j].neighbors[7] = &forest[i][j-1].self;
        }
    }
    
    //uses the densf variable to convert to number of trees that will be in the system
    float numTree = densf * (size * size);
    
    //assigns trees to random row or column and checks if it is already containing a tree
    for (int k = 1; k <= numTree; k++){
        while(1){
            row = rand();
            row %= size;
            col = rand();
            col %= size;
            if (forest[row][col].self.status == 1){
                continue;
            }
            else{
                forest[row][col].self.status = 1;
                break;
            }
        }
    }
    
    //uses the numTree and propf variable to convert to the number of trees in the 
    //system that are burning 
    float numFire = numTree * propf;
    
    //checks if the randomly generated row or column is a tree and then places a fire there.
    for (int k = 1; k <= numFire; k++){
        while(1){
            row = rand();
            row %= size;
            col = rand();
            col %= size;
            if (forest[row][col].self.status == 1){
                forest[row][col].self.status = 2;
                break;
            }
        }
    }
    
    //checks to see what implemtation is underway. handles the pN implementation.
    if (onFire == 0){
    
        while (gens <= pN - 1) {
            int count = 0;
            int changes = 0;
            //checks to see if this is the first generation and doesn't implement the rules 
            //if so
            if (gens == -1){
                for (row = 0; row<size; row++){
                
                    for (col = 0; col<size; col ++){
                
                    	  if (forest[row][col].self.status == 0){
                            printf(" ");
                        }
                        else if (forest[row][col].self.status == 1){
                            printf("Y");
                        }
                        else if (forest[row][col].self.status == 2){
                            printf("*");
                            count++;
                        }
                        else if (forest[row][col].self.status == 3){
                            printf("_");
                        }
                    }
                    puts(" ");
                }
            }
            //deals with other cases that aren't the first and implements the rules of the game.
            else {
                copy(size, forest, cpyforest);
                
                //counts the changes that are made and adds them together.
                changes = changes + rules(size, forest, cpyforest, probf, changes);
                changes = changes + water(size, forest, cpyforest, changes, probf);
                
                //prints the array
                for (row = 0; row<size; row++){
                
                    for (col = 0; col<size; col ++){
                
                    	  if (forest[row][col].self.status == 0){
                            printf(" ");
                        }
                        else if (forest[row][col].self.status == 1){
                            printf("Y");
                        }
                        else if (forest[row][col].self.status == 2){
                            printf("*");
                            count++;
                        }
                        else if (forest[row][col].self.status == 3){
                            printf("_");
                        }
                    }
                    puts(" ");
                }
                //increments total changes
                totchange = totchange + changes;
            }
            //handles final rounds print message and checks to see if the change was singular 
            //or plural
            if (count == 0){
                //handles print for when all fires are out and checks if the changes are 
                //singular or plural
                if (changes == 1){
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f," 
                           " %i change\n", gens + 1, size, probf, densf, propf, changes);
                    printf("fires are out after %i cumulative changes\n", totchange); 
                    onFire = 0;
                }
                else{
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f,"
                           " %i changes\n", gens + 1, size, probf, densf, propf, changes);
                    printf("fires are out after %i cumulative changes\n", totchange); 
                    onFire = 0;
                }
                gens = pN-1;
            }
            else{
                if(changes == 1){
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f,"
                           " %i change\n", gens + 1, size, probf, densf, propf, changes);
                }
                else{
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f,"
                           " %i changes\n", gens + 1, size, probf, densf, propf, changes);
                } 
            }
            gens++;
        }
    
    }
    //handles the no pN case for looping until all fires are out
    else if (onFire == 1){
    
        while (onFire) {
            clear();
            int count = 0;
            int changes = 0;
            //handles the first generation print, by not implementing the rules. 
            if (gens == -1){
                set_cur_pos(0,0);
                //prints the array
                for (row = 0; row<size; row++){
                
                    for (col = 0; col<size; col ++){
                
                    	  if (forest[row][col].self.status == 0){
                            putchar(' ');
                        }
                        else if (forest[row][col].self.status == 1){
                            putchar('Y');
                        }
                        else if (forest[row][col].self.status == 2){
                            putchar('*');
                            count++;
                        }
                        else if (forest[row][col].self.status == 3){
                            putchar('_');
                        }
                    }
                    puts(" ");
                }
            }
            //handles other situations that aren't the initial.
            else{
                copy(size, forest, cpyforest);
                
                //adds the changes made to the total changes for this round
                changes = changes + rules(size, forest, cpyforest, probf, changes);
                changes = changes + water(size, forest, cpyforest, changes, probf);
                set_cur_pos(0,0);
                
                //prints the array
                for (row = 0; row<size; row++){
                
                    for (col = 0; col<size; col ++){
                
                    	  if (forest[row][col].self.status == 0){
                            putchar(' ');
                        }
                        else if (forest[row][col].self.status == 1){
                            putchar('Y');
                        }
                        else if (forest[row][col].self.status == 2){
                            putchar('*');
                            count++;
                        }
                        else if (forest[row][col].self.status == 3){
                            putchar('_');
                        }
                    }
                    puts(" ");
                }
                
                //increments total changes
                totchange = totchange + changes;
            }
            if (count == 0){
                //handles print for when all fires are out and checks if the changes are 
                //singular or plural
                if (changes == 1){
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f," 
                           " %i change\n", gens + 1, size, probf, densf, propf, changes);
                    printf("fires are out after %i cumulative changes\n", totchange); 
                    onFire = 0;
                }
                else{
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f,"
                           " %i changes\n", gens + 1, size, probf, densf, propf, changes);
                    printf("fires are out after %i cumulative changes\n", totchange); 
                    onFire = 0;
                }
              
            }
            else{
                if(changes == 1){
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f,"
                           " %i change\n", gens + 1, size, probf, densf, propf, changes);
                }
                else{
                    printf("cycle %d, size %i, probability %.2f, density %.2f, proportion %.2f,"
                           " %i changes\n", gens + 1, size, probf, densf, propf, changes);
                } 
            }
            usleep(300000);
            gens++;
        }
    
    }
    
    return 0;
}