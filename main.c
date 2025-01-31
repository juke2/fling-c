
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// gcc main.c -g -O0 -fsanitize=address

/*
enum DIRECTIONS:

Reference for direction encoding.

members:

NORTH - Northern direction - 0
EAST - Eastern direction - 1
SOUTH - Southern direction - 2
WEST - Western direction - 3
NUM_DIRECTIONS - Total number of directions - 4

*/
enum DIRECTIONS{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NUM_DIRECTIONS
};

/*
struct to_move_info:

Container for shared values used mainly for function process_direction_movement. 

members:

int to_move_info.new_x: new x position after movement is calculated.
int to_move_info.new_y: new y position after movement is calculated.
int to_move_info.num_mins: number of flings found in the path of the currently moving fling.
int to_move_info.next_fling: ID of the next fling that will move. 

*/
typedef struct
{
    int new_x;
    int new_y;
    int num_mins;
    int next_fling;
} to_move_info;
to_move_info next_move_info;


/*
procedure print_arr_debug:

Prints input array.

params:

(in) int* x: Array to print.
(in) int len_x: Length of array.

*/
void print_arr_debug(int* x, int len_x){
    printf("\n");
    for(int i = 0; i < len_x; i++){
        printf("%d ",x[i]);
    }
    printf("\n");
    printf("\n");
}

/*
function check_north:

Checks if movement is possible in the northern direction.

params:

(in) int num_fling: Total number of flings
(in) int* fling_pos: Positions of flings on the board and their IDs. Will be in format [x_i,y_i,i] for i in [0,num_fling) 
(in) int xpos: x position of fling
(in) int ypos: y position of fling
(in) int skip_close_check: flag that determines whether to check if a fling is right next to the fling that is to be moved. 

return value:

DIRECTION_NORTH - Movement successful.
-1 - Movement failed. Only happens if direction input is invalid.
*/
int check_north(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_ypos = 0, adress = 0;

    // iterate through every fling
    for(fling = 0; fling < num_fling; fling++){
        other_ypos = fling_pos[fling*3+1];

        // check if the xpos positions are the same
        if(xpos == fling_pos[fling*3]){ 

            // fail if a fling is in the way of movement and the fling isn't already moving
            if(ypos - 1 == other_ypos && !skip_close_check){
                return -1;
            }

            // succeed if the ypos is north of the current ypos and increment num_in_sequence
            if(ypos-1 > other_ypos && xpos == fling_pos[fling*3]){
                num_in_sequence++; /*this could theoretically overflow but at that point I don't really care.*/

                // store fling as min if it is the closest fling
                if(min < other_ypos || min == -1){
                    min = other_ypos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }

    // if there are flings in the way, succeed
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = xpos;
        next_move_info.new_y = fling_pos[adress * 3 + 1] +1;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        return NORTH;
    }

    //otherwise fail
    return -1;
}

/*
function check_east:

Checks if movement is possible in the eastern direction.

params:

(in) int num_fling: Total number of flings
(in) int* fling_pos: Positions of flings on the board and their IDs. Will be in format [x_i,y_i,i] for i in [0,num_fling) 
(in) int xpos: x position of fling
(in) int ypos: y position of fling
(in) int skip_close_check: flag that determines whether to check if a fling is right next to the fling that is to be moved. 

return value:

DIRECTION_EAST - Movement successful.
-1 - Movement failed. Only happens if direction input is invalid.
*/
int check_east(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_xpos = 0, adress = 0;

    // iterate through every fling
    for(fling = 0; fling < num_fling; fling++){
        other_xpos = fling_pos[fling*3];

        // check if the ypos positions are the same
        if(ypos == fling_pos[fling*3+1]){

            // fail if a fling is in the way of movement and the fling isn't already moving
            if(xpos+ 1 == other_xpos && !skip_close_check){
                return -1;
            }

            // succeed if the xpos is east of the current xpos and increment num_in_sequence
            if(xpos+1 < other_xpos){
                num_in_sequence++; /*this could theoretically overflow but at that point I don't really care.*/

                // store fling as min if it is the closest fling
                if(min > other_xpos || min == -1){
                    min = other_xpos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }

    // if there are flings in the way, succeed
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = fling_pos[adress * 3] - 1;
        next_move_info.new_y = ypos;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        return EAST;
    }

    //otherwise fail
    return -1;
}

/*
function check_south:

Checks if movement is possible in the southern direction.

params:

(in) int num_fling: Total number of flings
(in) int* fling_pos: Positions of flings on the board and their IDs. Will be in format [x_i,y_i,i] for i in [0,num_fling) 
(in) int xpos: x position of fling
(in) int ypos: y position of fling
(in) int skip_close_check: flag that determines whether to check if a fling is right next to the fling that is to be moved. 

return value:

DIRECTION_SOUTH - Movement successful.
-1 - Movement failed. Only happens if direction input is invalid.
*/
int check_south(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_ypos = 0, adress = 0;

    // iterate through every fling
    for(fling = 0; fling < num_fling; fling++){
        other_ypos = fling_pos[fling*3+1];

        // check if the xpos positions are the same
        if(xpos == fling_pos[fling*3]){

            // fail if a fling is in the way of movement and the fling isn't already moving
            if(ypos + 1 == other_ypos && !skip_close_check){
                return -1;
            }

            // succeed if the ypos is south of the current ypos and increment num_in_sequence
            if(ypos + 1 < other_ypos){
                num_in_sequence++; /*this could theoretically overflow but at that point I don't really care.*/

                // store fling as min if it is the closest fling
                if(min > other_ypos || min == -1){ 
                    min = other_ypos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }

    // if there are flings in the way, succeed
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = xpos;
        next_move_info.new_y = fling_pos[adress * 3 + 1] - 1;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        return SOUTH;
    }

    //otherwise fail
    return -1;
}

/*
function check_west:

Checks if movement is possible in the western direction.

params:

(in) int num_fling: Total number of flings
(in) int* fling_pos: Positions of flings on the board and their IDs. Will be in format [x_i,y_i,i] for i in [0,num_fling) 
(in) int xpos: x position of fling
(in) int ypos: y position of fling
(in) int skip_close_check: flag that determines whether to check if a fling is right next to the fling that is to be moved. 

return value:

DIRECTION_WEST - Movement successful.
-1 - Movement failed. Only happens if direction input is invalid.
*/
int check_west(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_xpos = 0, adress = 0;

    // iterate through every fling
    for(fling = 0; fling < num_fling; fling++){
        other_xpos = fling_pos[fling*3];

        // check if the ypos positions are the same
        if(ypos == fling_pos[fling*3+1]){

            // fail if a fling is in the way of movement and the fling isn't already moving
            if(xpos - 1 == other_xpos && !skip_close_check){
                return -1;
            }

            // succeed if the ypos is west of the current ypos and increment num_in_sequence
            if(xpos-1 > other_xpos){
                num_in_sequence++; 

                // store fling as min if it is the closest fling
                if(min < other_xpos || min == -1){
                    min = other_xpos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }

    // if there are flings in the way, succeed
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = fling_pos[adress * 3] + 1 ;
        next_move_info.new_y = ypos;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        return WEST;
    }
    return -1;
}


/*
function process_direction_movement:

Checks a given direction to see if a move in that direction would be valid and how many other flings are in the way of that move.

params:

(in) int current_fling: ID of current fling in fling_pos.
(in) int num_fling: Total number of flings
(in) int* fling_pos: Positions of flings on the board and their IDs. Will be in format [x_i,y_i,i] for i in [0,num_fling) 
(in) int direction: Direction to check. mapped the same as results.
(in/out) int** next_pos: Pointer to an array containing the next board positions. Unallocated when passed in, allocated assuming direction input is valid.

return value:

x > 0 - New value for num_fling next recursion.
-1 - Process failed. Only happens if direction input is invalid.
*/
int process_direction_movement(int current_fling,int num_fling,int* fling_pos, int** next_pos,int direction){
    int move = 0, changed_pos_list_len = 0, i = 0, j = 0, replaced = 0, total_num_mins = 0, real_ind = 0;

    // initialize list for any moved
    int* changed_pos_list;
    changed_pos_list = (int*)(malloc(sizeof(int) * next_move_info.num_mins * 3));

    // select direction function based on input direction
    int(*directional)(int, int*, int, int, int);
    switch(direction){ // find direction to check
        case NORTH:
            directional = check_north;
            break;
        case EAST:
            directional = check_east;
            break;
        case SOUTH:
            directional = check_south;
            break;
        case WEST:
            directional = check_west;
            break;
        default:
            return -1;
    }

    // iterate through the flings in the path of the current_fling's movement and process the resulting position changes.
    total_num_mins = next_move_info.num_mins;
    for(move = 0; move < total_num_mins; move++){ // iterate through moves and find next moves and whether those next moves remove a fling
        changed_pos_list[move*3] = next_move_info.new_x; 
        changed_pos_list[move*3+1] = next_move_info.new_y;
        changed_pos_list[move*3+2] = current_fling;
        current_fling = next_move_info.next_fling;
        changed_pos_list_len++;
        directional(num_fling,fling_pos,next_move_info.new_x,next_move_info.new_y,1);
    }

    // allocate memory the pointer created earlier for the next recurse
    *(next_pos) = (int*)(malloc(sizeof(int) * (num_fling - 1)*3)); 

    // iterate through old values to transfer into next_pos
    while(i < num_fling){
        replaced = 1; // flag that denotes whether a value has been replaced from changed_pos_list

        // look through changed_pos_list to see if the old value is replaced by a value in changed pos list
        for(j=0; j < changed_pos_list_len; j++){ 
            if(fling_pos[i*3+2] == changed_pos_list[j*3+2]){
                replaced = 0;
                (*(next_pos))[real_ind*3] = changed_pos_list[j*3];
                (*(next_pos))[real_ind*3+1] = changed_pos_list[j*3+1];
                (*(next_pos))[real_ind*3+2] = changed_pos_list[j*3+2];
                real_ind++;
                break;
            }
        }

        // if a value isn't changed and isn't the last fling in the chain (which is removed) then copy it over.
        if( replaced != 0 && (fling_pos[i*3 + 2] != next_move_info.next_fling) ){
            (*(next_pos))[real_ind*3] = fling_pos[i*3];
            (*(next_pos))[real_ind*3+1] = fling_pos[i*3+1];
            (*(next_pos))[real_ind*3+2] = fling_pos[i*3+2];
            real_ind++;
        }
        i++;
    }

    // free mem and return the length of the resulting array
    free(changed_pos_list);
    return num_fling - 1; 
    

    // ----------------------------IMPORTANT REMINDER----------------------------
    // the list that this function allocates memory too (next_pos/new_pos) MUST be freed later. Currently happens in the solve_fling_recurse_helper function before exiting.
    // it shouldn't need to be freed in cases where no possible fling is found.
}

/*
function check_direction:

Checks a given direction to see if a move in that direction would be valid and how many other flings are in the way of that move.

params:

(in) int current_fling_index: Index of current fling in fling_pos.
(in) int num_fling: Total number of flings
(in) int* fling_pos: Positions of flings on the board and their IDs. Will be in format [x_i,y_i,i] for i in [0,num_fling) 
(in/out) int* results: Contains moves, where 0,1,2,3 is mapped to north,east,south,west respectively. Expected length of (num_flings - 1)*2*sizeOf(int).
(in) int direction: Direction to check. mapped the same as results.
(in) int solution_step: Depth of recursion. each time a move is made this value is incremented. corrisponds to the index in results.  
(in/out) int** next_pos: Pointer to an array containing the next board positions. Unallocated when passed in, allocated if movement is possible.

return value:

x > 0 - New value for num_fling next recursion.
-1 - Process failed, movement not possible.


*/
int check_direction(int current_fling_index, int num_fling, int* fling_pos, int* results, int direction, int solution_step, int** next_pos){
    int xpos = 0, ypos = 0, result = 0, fling_id = 0;

    // cache these vals so we don't have to pass the actual array to the direction functions
    xpos = fling_pos[current_fling_index * 3];
    ypos = fling_pos[current_fling_index * 3 + 1];
    fling_id = fling_pos[current_fling_index * 3 + 2];

    // select direction function and store result
    switch (direction){
        case NORTH:
            result = check_north(num_fling,fling_pos,xpos,ypos,0);
            break;
        case EAST:
            result = check_east(num_fling,fling_pos,xpos,ypos,0);
            break;
        case SOUTH:
            result = check_south(num_fling,fling_pos,xpos,ypos,0);
            break;
        case WEST:
            result = check_west(num_fling,fling_pos,xpos,ypos,0);
            break;
        default:
            return -1;
    }

    // if the result is a success, store the move in results and process resulting movement
    // note that because it is a depth first search and we have a guarenteed number of steps for a solution, we never need to clear or cache the value of results.
    // this is because in the event of a success, results will have been set to the correct values by all of the previous correct moves.
    if(result >= 0){
        results[solution_step*2] = fling_id;
        results[solution_step*2+1] = direction;
        return process_direction_movement(fling_id,num_fling,fling_pos,next_pos,direction);
    }

    // if the direction check fails, fail as well
    return -1;


}

/*
process solve_fling_recurse_helper:

Helper method to facilitate recursion of solve_fling.

params:

(in) int num_fling: Number of flings on the board.
(in) int* fling_pos: Positions of flings on the board and their IDs. Will be in format [x_i,y_i,i] for i in [0,num_fling) 
(in/out) int* results: Contains moves, where 0,1,2,3 is mapped to north,east,south,west respectively. Expected length of (num_flings - 1)*2*sizeOf(int).
(in) int solution_step: Depth of recursion. each time a move is made this value is incremented. corrisponds to the index in results. 

return value:

0 - Process successful, solution found and in results.
-1 - Process failed, no solution found.

*/
int solve_fling_recurse_helper(int num_fling, int* fling_pos, int* results, int solution_step){
    int fling = 0, direction = 0, new_pos_len = 0, recurse_result = 0;

    // if num_fling is 1, return as we have the solution.
    if(num_fling == 1){
        return 0;
    }

    //iterate through all flings and all directions
    for(fling = 0; fling < num_fling; fling++){
        for(direction = 0; direction < NUM_DIRECTIONS; direction++){

            // malloc this double pointer every iteration because it needs to be unique for each thread,
            // even if that thread doesn't actually allocate memory to it
            int** new_pos = malloc(sizeof(int*)); 

            // check if the fling can move in the given direction
            new_pos_len = check_direction(fling,num_fling,fling_pos,results,direction,solution_step,new_pos);

            // if the movement succeeds, recurse
            if(new_pos_len != -1){
                recurse_result = solve_fling_recurse_helper(new_pos_len,*new_pos,results,solution_step+1);

                // direction check succeeding guarentees that this memory is allocated
                // so free it after recursions that depend on it have failed/succeeded
                free(*new_pos);

                // return successful state if returned by recurse
                if(recurse_result == 0){
                    free(new_pos);
                    return 0;
                }
            }

            // free afformentioned pointer allocated every iteration... every iteration
            free(new_pos); 
        }
    }
    return -1;
}

/*
process solve_fling:

Solves the "fling" puzzle recursively.

params:

(in) int num_fling: Number of flings on the board.
(in) int* fling_pos: Positions of flings on the board, in format [x_i,y_i] for i in [0,num_fling) . Flings will be labled as 0-x based on index in fling_pos.
(in/out) int* results: contains moves, where 0,1,2,3 is mapped to north,east,south,west respectively. Expected length of (num_flings - 1)*2*sizeOf(int).

return value:

0 - Process successful, solution found and in results.
-1 - Process failed, no solution found.
*/
int solve_fling(int num_fling, int * fling_pos, int * results){
    int flingdex = 0, solving_result = 0;

    // allocate memory for and generate new arrays with IDs along with position
    int* fling_pos_with_ind = (int*)malloc(sizeof(int) * num_fling * 3);
    for(flingdex = 0; flingdex < num_fling; flingdex++){
        fling_pos_with_ind[flingdex * 3] = fling_pos[flingdex * 2];
        fling_pos_with_ind[flingdex * 3+1] = fling_pos[flingdex * 2+1];
        fling_pos_with_ind[flingdex * 3+2] = flingdex;
    }

    // recurse until solved and return result
    solving_result = solve_fling_recurse_helper(num_fling, fling_pos_with_ind, results, 0);
    free(fling_pos_with_ind);
    fling_pos_with_ind = NULL;
    return solving_result;
}
/*
process test_solve_fling:

Tests process solve_fling with a multitude of test cases. Expects data in the following format:

"##x_1y_1x_2y_2..." where # is any character (ignored in parsing) and x_i y_i are the xpos and ypos of each fling. 
This format is based upon the way data is stored in Peter Antoine's fling made in pico-8.
This implementation of fling can be found at https://www.lexaloffle.com/bbs/?pid=85165.

params:

(in) char* data_path: Path to input file with data
(in) int MAX_FLING: Maximum number of flings to check. Any level with num_flings > MAX_FLING will not be checked.
(in) int BUFFER_SIZE: Number of ints to include in buffer.

return value:

0 - Process successful.
-1 - Input buffer overflow.
*/
int test_solve_fling(char * data_path, int MAX_FLING, int BUFFER_SIZE){
    int c = 0, index = 0, input_index = 0, result = 0, level_counter = 0; 

    // allocate memory and open file
    int* buffer = (int*) (malloc(sizeof(int) * BUFFER_SIZE));
    FILE* input_file = fopen(data_path, "r");

    // read file per char
    while((c = getc(input_file)) != EOF){

        /// if char isn't a seperator, convert to number and store in buffer
        if((char)(c) != ';'){
            buffer[index] = (int)(c - '0'); // convert to numerical
            index++;

            // if buffer overflows, close file and return -1
            if(index >= BUFFER_SIZE ){
                printf("ERROR: BUFFER OVERFLOW");
                free(buffer);
                fclose(input_file);
                return -1;
            }
        }

        // if char is a seperator, parse data in buffer to test 
        else{
            level_counter++; 
            int* input_buff = malloc(sizeof(int) * (index-2));
            int* result_array = malloc(sizeof(int) * (index - 4));

            // read buffer (skipping first two because of how the original data is formatted) and put into input
            for(input_index = 2; input_index < index; input_index++){
                input_buff[input_index-2] = buffer[input_index]; 
            }  

            // test and print result if the number of flings in the test is < MAX_FLING
            if((index - 2)/2 <= MAX_FLING){
                result = solve_fling((index - 2)/2, input_buff, result_array);
                    printf("Result: %d Results Array: ", result);
                    for(int i = 0; i < index - 4; i++){
                        printf("%d ",result_array[i]);
                    }
                    printf("num_fling = %d level = %d", (index - 2)/2, level_counter);
                    printf("\n");

            }

            // free mem and reset index for next iteration
            free(input_buff);
            free(result_array);
            index = 0;
        }
    }

    // free buffer and close input_file
    free(buffer);
    fclose(input_file);
    return 0;
}

int dedicated_fling_test(){
int data_index = 0, buff_ind = 0, solution_ind = 0, num_fling = 0, iterator = 0, result = 0, arrays_similar = 0;

// define const data and solutions. data is taken from fling_test_data.txt, solutions are done manually.
const int LEN_DATA = 57; // these are updated manually because I hate myself
const char* input_data = "013236;013464;02316536;02323536;02244474;05316263732465;";
const char* solutions = "02;01;0201;1012;0101;5053022321;";
const int BUFFER_SIZE = 300;
int* buffer = (int*) (malloc(sizeof(int) * BUFFER_SIZE));

// iterate through data
for(data_index = 0; data_index < LEN_DATA; data_index++){

    // take input and put into buffer if it isn't the seperator
    char c = input_data[data_index];
    if(c != ';'){
        buffer[buff_ind] = (int)(c - '0');
        buff_ind++;
    }

    // process if it is the seperator
    else{
        arrays_similar = 1;
        int* input_buff = malloc(sizeof(int) * (buff_ind-2));
        int* result_array = malloc(sizeof(int) * (buff_ind - 4));
        int* expected_result_array = malloc(sizeof(int) * (buff_ind - 4));
        num_fling = (buff_ind - 2)/2;

        // read buffer into input
        for(iterator = 2; iterator < buff_ind; iterator++){
            input_buff[iterator-2] = buffer[iterator]; 
        }  

        // initialize results and expected results
        for(iterator = 0; iterator < buff_ind - 4; iterator++){
            result_array[iterator] = -1;
            expected_result_array[iterator] = (int)(solutions[solution_ind] - '0');
            solution_ind++;
        }

        // solve and compare results
        result = solve_fling((buff_ind - 2)/2, input_buff, result_array);
        for(iterator = 0; iterator < buff_ind - 4; iterator++){
            if(result_array[iterator] != expected_result_array[iterator]){
                arrays_similar = 0;
            }
        }

        // print results
        if(arrays_similar){
            printf("Result: %d | \e[0;32m",result);
            for(iterator = 0; iterator < buff_ind - 4; iterator++){
                printf("%d ",result_array[iterator]);
            }
            printf(" = ");
            for(iterator = 0; iterator < buff_ind - 4; iterator++){
                printf("%d ",expected_result_array[iterator]);
            }
            printf("\e[0m\n");
        }
        else{
            printf("Result: %d | \e[0;31m",result);
            for(iterator = 0; iterator < buff_ind - 4; iterator++){
                printf("%d ",result_array[iterator]);
            }
            printf(" != ");
            for(iterator = 0; iterator < buff_ind - 4; iterator++){
                printf("%d ",expected_result_array[iterator]);
            }
            printf("\e[0m\n");
        }
        free(input_buff);
        free(result_array);
        free(expected_result_array);
        solution_ind++; // need to increment to pass semicolon
        buff_ind = 0;
    }
        
}
return 0;
}


int main(int argc, char **argv)
{
    // current test cases aren't guarenteed to be solvable unfortunately. Haven't gone through to mark them, but all < 8 flings are solvable.
    test_solve_fling("fling_test_data.txt",10,300);
    dedicated_fling_test();
    return 0;
}