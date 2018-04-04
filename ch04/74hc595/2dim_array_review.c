// How to dynamically allocate a 2D array
#include <stdio.h>
#include <stdlib.h>
#define ROW 8
#define COL 8

char mat[ROW][COL]={     {0,0,0,0,0,0,0,0},
                         {0,1,1,0,0,1,1,0},
                         {1,1,1,1,1,1,1,1},
                         {1,1,1,1,1,1,1,1},
                         {1,1,1,1,1,1,1,1},
                         {0,1,1,1,1,1,1,0},
                         {0,0,1,1,1,1,0,0},
                         {0,0,0,1,1,0,0,0}};

void method1(); // Using a single pointer
void method2(); // Using an array of pointers
void method3(); // Using pointer to a pointer
void method4(); // Using double pointer and one malloc call for all rows

int main()
{
    int i, j;

    printf("[mat]\n");

    for(i = 0 ; i < ROW ; i++){
        for(j = 0 ; j < COL ; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }

    method1();    method2();    method3();    method4();

    return 0;
}



// Using a single pointer
void method1()
{
    int i, j;
    char *matcopy;

    matcopy = (char*)malloc(ROW * COL * sizeof(char));

    for(i = 0 ; i < ROW ; i++)
        for(j = 0 ; j < COL ; j++)
            *(matcopy + (i * COL) + j) = mat[i][j];

    printf("\nmethod1\n");

    for(i = 0 ; i < ROW ; i++){
        for(j = 0 ; j < COL ; j++)
            printf("%d ", *(matcopy + (i * COL) + j));
        printf("\n");
    }
    free(matcopy);
}



// Using an array of pointers
void method2()
{
    int i, j;
    char *matcopy[ROW];

    for(i = 0 ; i < ROW ; i++)
        matcopy[i] = (char*)malloc(COL * sizeof(char));

    for(i = 0 ; i < ROW ; i++)
        for(j = 0 ; j < COL ; j++)
//          *(matcopy[i] + j) = mat[i][j];
            matcopy[i][j] = mat[i][j];

    printf("\nmethod2\n");

    for(i = 0 ; i < ROW ; i++){
        for(j = 0 ; j < COL ; j++)
//          printf("%d ", *(matcopy[i]+j));
            printf("%d ", matcopy[i][j]);
        printf("\n");
    }
}


// Using pointer to a pointer
void method3()
{
    int i, j;
    char **matcopy;

    matcopy = (char**)malloc(ROW * sizeof(char*));

    for(i = 0 ; i < ROW ; i++)
//      *(matcopy+i) = (char*)malloc(COL * sizeof(char));
        matcopy[i] = (char*)malloc(COL * sizeof(char));

    for(i = 0 ; i < ROW ; i++)
        for(j = 0 ; j < COL ; j++)
//          *(*(matcopy+i) + j) = mat[i][j];
            matcopy[i][j] = mat[i][j];

    printf("\nmethod3\n");

    for(i = 0 ; i < ROW ; i++){
        for(j = 0 ; j < COL ; j++)
//          printf("%d ", *(*(matcopy + i) + j));
            printf("%d ", matcopy[i][j]);
        printf("\n");
    }
}

// Using double pointer and one malloc call for all rows
void method4()
{
    int i, j;
    char **matcopy;

    matcopy = (char**)malloc(ROW * sizeof(char*));

//  *(matcopy + 0) = (char*)malloc(ROW * COL * sizeof(char));
    matcopy[0] = (char*)malloc(ROW * COL * sizeof(char));

    for(i = 0 ; i < ROW ; i++)
//      *(matcopy + i) = *matcopy + COL * i;
        matcopy[i] = *matcopy + COL * i;

    for(i = 0 ; i < ROW ; i++)
        for(j = 0 ; j < COL ; j++)
//          *(*(matcopy + i) + j) = mat[i][j];
            matcopy[i][j] = mat[i][j];

    printf("\nmethod4\n");

    for(i = 0 ; i < ROW ; i++){
        for(j = 0 ; j < COL ; j++)
//          printf("%d ", *(*(matcopy + i) + j));
            printf("%d ", matcopy[i][j]);
        printf("\n");
    }
}


