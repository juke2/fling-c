#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// gcc main.c -g -O0 -fsanitize=address

enum DIRECTIONS{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NUM_DIRECTIONS
};
typedef struct
{
    int new_x;
    int new_y;
    int num_mins;
    int direction;
    int next_fling;
} to_move_info;
to_move_info next_move_info;

void print_arr_debug(int* x, int len_x){
    printf("\n");
    for(int i = 0; i < len_x; i++){
        printf("%d ",x[i]);
    }
    printf("\n");
    printf("\n");
}


int check_north(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_ypos = 0, adress = 0;
    for(fling = 0; fling < num_fling; fling++){
        other_ypos = fling_pos[fling*3+1];
        if(xpos == fling_pos[fling*3]){
            if(ypos - 1 == other_ypos || skip_close_check){
                return -1;
            }
            if(ypos-1 > other_ypos && xpos == fling_pos[fling*3]){
                num_in_sequence++; /*this could theoretically overflow but at that point I don't really care.*/
                if(min < other_ypos || min == -1){
                    min = other_ypos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }
    printf("num_in_sequence: %d",num_in_sequence);
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = xpos;
        next_move_info.new_y = fling_pos[adress * 3 + 1] +1;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        return NORTH;
    }
    return -1;
}

int check_east(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_xpos = 0, adress = 0;
    for(fling = 0; fling < num_fling; fling++){
        other_xpos = fling_pos[fling*3];
        if(ypos == fling_pos[fling*3+1]){
            if(xpos+ 1 == other_xpos || skip_close_check){
                return -1;
            }
            if(xpos+1 < other_xpos){
                num_in_sequence++;
                if(min > other_xpos || min == -1){
                    min = other_xpos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }
    printf("num_in_sequence: %d",num_in_sequence);
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = fling_pos[adress * 3] - 1;
        next_move_info.new_y = ypos;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        printf("\nMINDEX = %d\n", mindex);
        return EAST;
    }
    return -1;
}

int check_south(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_ypos = 0, adress = 0;
    // printf("CHECK_SOUTH_CALLED!");
    for(fling = 0; fling < num_fling; fling++){
        other_ypos = fling_pos[fling*3+1];
        printf("\nxpos: %d, ypos: %d, other_x_pos: %d, other_y_pos: %d \n", xpos, ypos, fling_pos[fling*3], fling_pos[fling*3+1]);
        if(xpos == fling_pos[fling*3]){
            if(ypos + 1 == other_ypos || skip_close_check){
                printf("EXCEPTION_YPOS_CLAUSE");
                return -1;
            }
            if(ypos + 1 < other_ypos){
                printf("SUCCESS! ypos: %d xpos: %d other_xpos: %d other_ypos: %d", xpos, ypos, fling_pos[fling*3],other_ypos );
                num_in_sequence++; 
                if(min > other_ypos || min == -1){ 
                    min = other_ypos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }
    printf("num_in_sequence: %d",num_in_sequence);
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = xpos;
        next_move_info.new_y = fling_pos[adress * 3 + 1] - 1;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        printf("\n NEW_POS: (%d,%d)", next_move_info.new_x, next_move_info.new_y);
        printf("\nMINDEX = %d\n", mindex);
        return SOUTH;
    }
    return -1;
}

int check_west(int num_fling, int* fling_pos, int xpos, int ypos, int skip_close_check){
    int fling = 0, min = -1, mindex = 0, num_in_sequence = 0, other_xpos = 0, adress = 0;
    for(fling = 0; fling < num_fling; fling++){
        other_xpos = fling_pos[fling*3];
        if(ypos == fling_pos[fling*3+1]){
            if(xpos - 1 == other_xpos || skip_close_check){
                return -1;
            }
            if(xpos-1 > other_xpos){
                num_in_sequence++; 
                if(min < other_xpos || min == -1){
                    min = other_xpos;
                    mindex = fling_pos[fling*3+2];
                    adress = fling;
                }
            }
        }
    }
    printf("num_in_sequence: %d",num_in_sequence);
    if(num_in_sequence >= 1){ 
        next_move_info.new_x = fling_pos[adress * 3] + 1 ;
        next_move_info.new_y = ypos;
        next_move_info.num_mins = num_in_sequence;
        next_move_info.next_fling = mindex;
        printf("\nMINDEX = %d\n", mindex);
        return WEST;
    }
    return -1;
}

int process_direction_movement(int current_fling,int num_fling,int* fling_pos, int** next_pos,int direction){
    int move = 0, changed_pos_list_len = 0, i = 0, j = 0, replaced = 0, total_num_mins = 0, real_ind = 0;
    int* changed_pos_list;
    changed_pos_list = (int*)(malloc(sizeof(int) * next_move_info.num_mins * 3));
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
            printf("ERROR_NO_DIRECTION");
            return -1;
    }
    total_num_mins = next_move_info.num_mins;
    for(move = 0; move < total_num_mins; move++){ // iterate through moves and find next moves and whether those next moves remove a fling
        changed_pos_list[move*3] = next_move_info.new_x; 
        changed_pos_list[move*3+1] = next_move_info.new_y;
        changed_pos_list[move*3+2] = current_fling;
        current_fling = next_move_info.next_fling;
        changed_pos_list_len++;
        directional(num_fling,fling_pos,next_move_info.new_x,next_move_info.new_y,1);
    }
    printf("PRINTING CHANGED POS LIST: ");
    print_arr_debug(changed_pos_list,changed_pos_list_len*3);
    *(next_pos) = (int*)(malloc(sizeof(int) * (num_fling - 1)*3)); // allocate mem for next cycle (based on removed items calculated previously)
    while(i < num_fling){ // iterate through values to copy them over, ignoring any removed.
        replaced = 1;
        for(j=0; j < changed_pos_list_len; j++){ // make sure replaced values are replaced as well.
            if(fling_pos[i*3+2] == changed_pos_list[j*3+2]){
                replaced = 0;
                (*(next_pos))[real_ind*3] = changed_pos_list[j*3];
                (*(next_pos))[real_ind*3+1] = changed_pos_list[j*3+1];
                (*(next_pos))[real_ind*3+2] = changed_pos_list[j*3+2];
                printf("REPLACED");
                real_ind++;
                break;
            } // these values should never be the removed values hopefully? if they are, oh god. panic.
        }
        if( replaced != 0 && (fling_pos[i*3 + 2] != next_move_info.next_fling) ){
            (*(next_pos))[real_ind*3] = fling_pos[i*3];
            (*(next_pos))[real_ind*3+1] = fling_pos[i*3+1];
            (*(next_pos))[real_ind*3+2] = fling_pos[i*3+2];
            printf("TRANSFERED");
            real_ind++;
        }
        printf("%d", next_move_info.next_fling);
        printf("Real_ind: %d ",real_ind/*fling_pos[i*3 + 2] != next_move_info.next_fling*/);
        i++;
    }
    print_arr_debug(*next_pos, 3*(num_fling-1));
    free(changed_pos_list);
    return num_fling - 1; // returns length of resulting array
    

    // ----------------------------IMPORTANT REMINDER----------------------------
    // the list that this function allocates memory too (next_pos/new_pos) MUST be freed later. Currently happens in the solve_fling_recurse_helper function before exiting.
    // it shouldn't need to be freed in cases where no possible fling is found.
}
int check_direction(int current_fling, int num_fling, int* fling_pos, int* results, int direction, int solution_step, int** next_pos){
    int xpos = 0, ypos = 0, result = 0, fling_id = 0;
    xpos = fling_pos[current_fling * 3];
    ypos = fling_pos[current_fling * 3 + 1];
    fling_id = fling_pos[current_fling * 3 + 2];
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
            printf("ERROR_NO_DIRECTION");
            return -1;
    }
    // printf("RESULT: %d", result);

    if(result >= 0){
        printf("FUNC_PROCESS_DIRECTION_MOVEMENT_TRIGGERED");
        results[solution_step*2] = fling_id;
        results[solution_step*2+1] = direction;
        return process_direction_movement(fling_id,num_fling,fling_pos,next_pos,direction);
    }
    return -1;


}

/*
function solve_fling_recurse_helper

helper method to facilitate recursion of solve_fling.
*/
int solve_fling_recurse_helper(int num_fling, int* fling_pos, int* results, int solution_step){
    int fling = 0, direction = 0, new_pos_len = 0, recurse_result = 0;
    if(num_fling == 1){
        // print_arr_debug(fling_pos, num_fling);
        return 0;
    }
    for(fling = 0; fling < num_fling; fling++){
        for(direction = 0; direction < NUM_DIRECTIONS; direction++){
            printf("NUM_FLING: %d Depth: %d. Fling#: %d. Direction: %d\n",num_fling, solution_step,fling,direction);
            // print_arr_debug(fling_pos,num_fling*3);
            int** new_pos = malloc(sizeof(int*)); 
            new_pos_len = check_direction(fling,num_fling,fling_pos,results,direction,solution_step,new_pos);
            if(new_pos_len != -1){
                printf("RESULT FROM CHECK DIRECTION: %d", new_pos_len);
                recurse_result = solve_fling_recurse_helper(new_pos_len,*new_pos,results,solution_step+1);
                printf("-----------new_pos FREED-----------");
                free(*new_pos);
                if(recurse_result == 0){
                    free(new_pos);
                    return 0;
                }
            }
            free(new_pos);
        }
    }
    return -1;
}

/*
process solve_fling:

solves the "fling" puzzle recursively.

params:

(in) int num_fling: Number of flings on the board.
(in) int* fling_pos: Positions of flings on the board. Flings will be labled as 0-x based on index in fling_pos.
(in/out) int* results: contains moves, where 0,1,2,3 is mapped to north,east,south,west respectively. Expected length of (num_flings - 1)*2*sizeOf(int).

return value:
0 - process successful, solution found and in results.
1 - process failed, no solution found.
*/
int solve_fling(int num_fling, int * fling_pos, int * results){
    int flingdex = 0, solving_result = 0;
    printf("Num_fling: %d\n",num_fling);
    int* fling_pos_with_ind = (int*)malloc(sizeof(int) * num_fling * 3);
    for(flingdex = 0; flingdex < num_fling; flingdex++){
        fling_pos_with_ind[flingdex * 3] = fling_pos[flingdex * 2];
        fling_pos_with_ind[flingdex * 3+1] = fling_pos[flingdex * 2+1];
        fling_pos_with_ind[flingdex * 3+2] = flingdex;
    }
    print_arr_debug(fling_pos_with_ind,num_fling * 3);
    solving_result = solve_fling_recurse_helper(num_fling, fling_pos_with_ind, results, 0);
    free(fling_pos_with_ind);
    fling_pos_with_ind = NULL;
    return solving_result;
}

int main(int argc, char **argv)
{
    int result_len = 8;
    printf("Hello World!\n");
    int test_array[10] = {3,3,0,4,3,2,1,0,0,0};
    int* results_test = (int*)(malloc(sizeof(int) * result_len));
    for(int i = 0; i < result_len; i++){
        results_test[i] = -1;
    }
    print_arr_debug(results_test,result_len);
    printf("\nRETURNED RESULT:%d\n", solve_fling(5,test_array,results_test));
    print_arr_debug(test_array,10);
    print_arr_debug(results_test,result_len);
    free(results_test);
    return 0;
}


