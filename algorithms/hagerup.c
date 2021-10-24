#include "../la_algorithm.h"

#if LA_ALGORITHM == HAGERUP

    #include "hagerup.h"

    /************************************************************************/
    /* Preprocessing						*/
    /************************************************************************/
    void hagerup_preprocessing(){

        int euler_n = 2 * n - 2;
        height_arr = alloc(euler_n * sizeof(int));
        valley = alloc(euler_n * sizeof(int));
        int* weight = (int*)calloc(euler_n, sizeof(int));   

        int top = 0;

        valley_tuple* S = (valley_tuple*)malloc(((n + 2) / 2) * sizeof(valley_tuple));

        // Adding dummy sential tuple that will never be popped from stack
        S[top].x = 0;
        S[top].low = INT_MIN;
        S[top].high = INT_MAX;

        int x, y = 0;

        
        for(x = 0; x < euler_n; x++){
            height_arr[x] = max_depth - tree[euler_arr[x]].depth;
        }

        for(x = 0; x < euler_n; x++){

            y = height_arr[x];

            while(S[top].low >= y)  top--;

            if(S[top].high >= y){
                valley[x] = x;
                weight[valley[x]]++;

                if(S[top].high > y){
                    top++;
                    S[top].x = x;
                    S[top].low = y;
                    S[top].high = y;
                }
            }
            else{
                while(S[top - 1].high < y)  top--;

                valley[x] = S[top].x;
                weight[valley[x]]++;

                if(S[top - 1].high > y){
                    S[top].high = y;
                }
                else{
                    top--;
                }
            }
        }

        free(S);

        K_prime = ceil( (double)((2 * K) + 2) / (K - 2)  );
        K_max = INT_MAX / (K - 2);

        y_min = 0;
        y_max = max_depth;

        int* right_sight = (int*)calloc((y_max + 2),  sizeof(int));

        ladder = alloc(euler_n * sizeof(int*)); 

        int ladder_h;

        jump = alloc(euler_n * sizeof(int));

        int i;
        unsigned int temp_sum;

        for(x = euler_n - 1; x >= 0; x--){

            y = height_arr[x];
        
            right_sight[y] = x;

            ladder_h = y_max - y;

            if(x > 0 && x < euler_n - 1){
                ladder_h = min(max((K - 1), (K_prime * (weight[x] - 1) - 2)), ladder_h);
            }

            ladder[x] = alloc(ladder_h * sizeof(int));

            // Fill ladder with this points right sight
            for(i = 0; i < ladder_h; i++){
                ladder[x][i] = right_sight[y + i + 1];
            }
    
            if(x == 0){
                jump[x] = 0;
            }
            else{
            
                // Make sure the result doesn't overflow or wrap around
                // FIX THIS SOMETHING LOOKS WRONG; BAD CODING PRACTICES
                if((y > y_max) || (temp_sum = highest_pwrOf2_divides(x)) > K_max){
                    temp_sum = y_max + 1;
                } 
                else if((temp_sum = y + (K - 2) * temp_sum) < y){
                    temp_sum = y_max + 1;
                }

                jump[x] = valley[   right_sight[    min(temp_sum, y_max + 1)  ]   ];
            }   

        }
    }
    /************************************************************************/

    /************************************************************************/
    /* Querying						*/
    /************************************************************************/
    int hagerup_query(int query_node, int query_level){
    
        int x_hat, y_x, p, x, y;

        y = y_max - query_level;

        x = rep_arr[query_node];
        y_x = height_arr[x];

        //if(x >= euler_n || y >= y_max || x < 0) return 0;

        if(y <= y_x) return euler_arr[x];

        if(y < (y_x + K)) return euler_arr[ladder[x][y - y_x - 1]];
        
        p = highest_pwrOf2((y - y_x) / K);

        x_hat = (x / p) * p;

        if(x_hat > 0 && ( x_hat & ((2*p) - 1) ) == 0){
            x_hat -= p;
        }

        return euler_arr[ladder[  jump[x_hat] ][y - height_arr[jump[x_hat]] - 1]];

    }
    /************************************************************************/

    /************************************************************************/
    /* Tree Manipulation						*/
    /************************************************************************/
    void add_hagerup_leaf(int parent){
        printf("Error: Function Not Yet Implemented!\n");
        exit(-1);
    }   

    void remove_hagerup_leaf(int leaf){
        printf("Error: Function Not Yet Implemented!\n");
        exit(-1);
    }
    /************************************************************************/
#endif
