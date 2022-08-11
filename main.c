//
//  main.c
//  Sudoko
//
//  Created by Zain 👻 on 07/10/2021.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                         G   A   M    E              B   O   R   D                               //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int GameBoard[9][9]; //-------->Board from file
int Validation[11];  //--------->For Validation
int GridCheck = 2; //------>3x3 selection

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                      S   T   R   U   C   T                                           //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    //struct to store info of thread
    int row;
    int column;
    
}forCheck;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                            T H R E A D       D E C L E R A T I O N                     //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

pthread_t COL;//---------> Coloumn thread
pthread_t ROW;//---------> Row     thread
pthread_t thread_1, thread_2, thread_3, thread_4, thread_5, thread_6, thread_7, thread_8, thread_9;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                       F U N C T I O N       P R O T O T Y P E S                     //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Declare the thread that checks columns, rows and 3x3 subgrids */
void *colCheck(void *Input);
void *rowCheck(void *Input);
void *squareCheck(void *Input);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                 M A I N           F U N C T I O N                             //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{

    int validator = 0; //-------------->used for validation
        
    if (argc == 1)
    {
            fprintf(stderr,"Usage: ./hw2 <sudoku grid>\n");

            return -1;//IF error
        }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                 R E A D I N G       F I L E                                      //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    FILE *sudoku;
    sudoku = fopen(argv[1], "r");

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 9; j++)
        {
            fscanf(sudoku, "%d ", &GameBoard[i][j]);
        }
    }
            
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////           V A L U E S       T O      S T R U C T      V A R I A B L E S          //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
        forCheck *checkRows = (forCheck *) malloc(sizeof(forCheck));
        checkRows->row = 0;
        checkRows->column = 0;
       
        forCheck *checkCols = (forCheck *) malloc(sizeof(forCheck));
        checkCols->row = 0;
        checkCols->column = 0;
       
        forCheck *one3x3 = (forCheck *) malloc(sizeof(forCheck));
        one3x3->row = 0;
        one3x3->column = 0;
        
        forCheck *two3x3 = (forCheck *) malloc(sizeof(forCheck));
        two3x3->row = 0;
        two3x3->column = 3;
     
        forCheck *three3x3 = (forCheck *) malloc(sizeof(forCheck));
        three3x3->row = 0;
        three3x3->column = 6;
      
        forCheck *four3x3 = (forCheck *) malloc(sizeof(forCheck));
        four3x3->row = 3;
        four3x3->column = 0;
       
        forCheck *five3x3 = (forCheck *) malloc(sizeof(forCheck));
        five3x3->row = 3;
        five3x3->column = 3;
       
        forCheck *six3x3 = (forCheck *) malloc(sizeof(forCheck));
        six3x3->row = 3;
        six3x3->column = 6;
       
        forCheck *seven3x3 = (forCheck *) malloc(sizeof(forCheck));
        seven3x3->row = 6;
        seven3x3->column = 0;
    
        forCheck *eight3x3 = (forCheck *) malloc(sizeof(forCheck));
        eight3x3->row = 6;
        eight3x3->column = 3;
      
        forCheck *nine3x3 = (forCheck *) malloc(sizeof(forCheck));
        nine3x3->row = 6;
        nine3x3->column = 6;
        
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////            V A L U E S    TO  BE    U S E D   IN      PTHREAD  JOIN      //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //return values? this is used for pthread join
        void * rows;
        void * cols;
        void * one;
        void * two;
        void * three;
        void * four;
        void * five;
        void * six;
        void * seven;
        void * eight;
        void * nine;
        
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                 C R E A T I N G         T H R E A D S                    //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        pthread_create(&COL, NULL, colCheck, (void *) checkCols);
        pthread_create(&ROW, NULL, rowCheck, (void *) checkRows);
        pthread_create(&thread_1, NULL, squareCheck, (void *) one3x3);
        pthread_create(&thread_2, NULL, squareCheck, (void *) two3x3);
        pthread_create(&thread_3, NULL, squareCheck, (void *) three3x3);
        pthread_create(&thread_4, NULL, squareCheck, (void *) four3x3);
        pthread_create(&thread_5, NULL, squareCheck, (void *) five3x3);
        pthread_create(&thread_6, NULL, squareCheck, (void *) six3x3);
        pthread_create(&thread_7, NULL, squareCheck, (void *) seven3x3);
        pthread_create(&thread_8, NULL, squareCheck, (void *) eight3x3);
        pthread_create(&thread_9, NULL, squareCheck, (void *) nine3x3);
     
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                 W A I T I N G       T H R E A D S                          //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        pthread_join(COL, &cols);
        pthread_join(ROW, &rows);
        pthread_join(thread_1, &one);
        pthread_join(thread_2, &two);
        pthread_join(thread_3, &three);
        pthread_join(thread_4, &four);
        pthread_join(thread_5, &five);
        pthread_join(thread_6, &six);
        pthread_join(thread_7, &seven);
        pthread_join(thread_8, &eight);
        pthread_join(thread_9, &nine);
        
    
        /* Finally, after all children returns, you can check the status array that is visible to everyone and see if it is valid. You then print out the final checking result*/
    while(validator < 11)
    {

            
            if(Validation[validator] == 1){
        validator++;
            }
            else
            {
                printf("The Sudoku Puzzle is NOT solved. \n");
                exit(0);
            }
            
        }
        printf("The Sudoku Puzzle is solved. \n");
    

    return 0;
}


/*thread code for child checking all columns*/
void *colCheck(void *params)
{
    int i, j;
    
    forCheck * c_worker = (forCheck *) params;
    int beginCol = c_worker->column;
    int beginRow = c_worker->row;

    /*sorting sudoku column; this will put the column into a sorted order and check for duplicates
      if there are duplicates, this section is not valid and returns 0 for the column_worker;*/
    
    for(i = beginCol; i < 9; i++){
     
    int col[9]= {0}; //array to hold sorted column
        
        for(j = beginRow; j < 9; j++){
            
            int val = GameBoard[i][j]; //this specific value in the column
            
            /*if the coresponding array element for the value is 0, it has not been seen and
        we will input this value to the sorted column array. */
            
        if(col[val-1] == 0 && val > 0){
               col[val-1] = val;
        
            }
        /*if the value is not 0, then the value is a duplicate and the sudoku puzzle
          is not solved or valid so the value of column_worker in valid is 0; */
            else{
        Validation[0] = 0;
                pthread_exit(0);
            }
        }
    }
    Validation[0] = 1;
    pthread_exit(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                C H I L D             C H E C K           R O W S                        //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void *rowCheck(void *INPUT)
{

    
    forCheck * ptrCheck = (forCheck *) INPUT;
    int startC = ptrCheck->column;//--------------->start coloumn
    int startR = ptrCheck->row;//--------------->start rows
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                                 S O R T I N G    R O W S                                    //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    for(int i = startC; i < 9; i++)
    {
        
        int row[9]= {0};//------------------>SORTEED ROW
        for(int j = startR; j < 9; j++){
            
            int val = GameBoard[i][j];
            if(row[val-1] == 0 && val > 0)//------------> IF ZERO PUT FROM ROW
            {
                row[val-1] = val;
            }
            else
            {
                Validation[1] = 0;
                pthread_exit(0);
            }
        }
    }
    Validation[1] = 1;
    pthread_exit(0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////         C H I L D             C H E C K     3X3       S U B G R I D S            //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void *squareCheck(void *INPUT)
{
    forCheck * worker = (forCheck *) INPUT;
    int startR = worker->row;
    int startC = worker->column;
    
    
    int square[9] = {0}; //----------->3X3 in 1D array

    for (int i = startR; i < startR + 3; ++i)
    {
        for (int j = startC; j < startC + 3; ++j)
        {
            int temp = GameBoard[i][j];
            if (square[temp-1] == 0 && temp > 0)
            {
                square[temp-1] = temp;
            }
            else
            {
                Validation[GridCheck] = 0;
                GridCheck++;
                pthread_exit(0);
            }
        }
    }
    Validation[GridCheck] = 1;
    GridCheck++;
    pthread_exit(0);
}
