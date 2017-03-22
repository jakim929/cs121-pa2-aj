#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int **m1;
int **m2;
int **c;

int flexible;
int dimension;


// Only for testing
void printMatrix(int** matrix)
{
  for(int i = 0; i < dimension; i++)
  {
    for(int j = 0; j < dimension; j++)
    {
      printf("%i ", matrix[i][j]);
    }
    printf("\n");
  }
}

// Prints final solution as specified
void PrintDiagonal()
{
  for (int i=0; i < dimension; i++)
  {
    printf("%i\n", c[i][i]);
  }
}

// Add two matrices (NOT IN USE)
void sum(int** a, int** b)
{
   for (int i = 0; i < dimension; i++) {
      for (int j = 0 ; j < dimension; j++) {
         sum[i][j] = a[i][j] + b[i][j];
      }
   }
}

// Subtract Two Matrices (NOT IN USE)
void sub(int** a, int** b)
{
   for (int i = 0; i < dimension; i++) {
      for (int j = 0 ; j < dimension; j++) {
         sum[i][j] = a[i][j] - b[i][j];
      }
   }
}

// For all values of n
void ConvMult() 
{
  for (int i=0; i < dimension; i++)
  {
    for (int j=0; j < dimension; j++)  
    {
      c[i][j]=0;
      for (int k=0; k < dimension; k++)
      {
        c[i][j] = c[i][j]+m1[i][k]*m2[k][j];
      }
    }
  }
}

//for even n value (ONLY WORKS FOR n = 2)
void StrassMult() 
{
  
  int p1 = (m1[0][0] + m1[1][1]) * (m2[0][0] + m2[1][1]);
  int p2 = (m1[1][0] + m1[1][1]) * m2[0][0];
  int p3 = m1[0][0] * (m2[0][1] - m2[1][1]);
  int p4 = m1[1][1] * (m2[1][0] - m2[0][0]);
  int p5 = (m1[0][0] + m1[0][1]) * m2[1][1];
  int p6 = (m1[1][0] - m1[0][0]) * (m2[0][0] + m2[0][1]);
  int p7 = (m1[0][1] - m1[1][1]) * (m2[1][0] + m2[1][1]);
 
  c[0][0] = p1 + p4 - p5 + p7;
  c[0][1] = p3 + p5;
  c[1][0] = p2 + p4;
  c[1][1] = p1 - p2 + p3 + p6;

}


int main(int argc, char *argv[])
{

  flexible = atoi(argv[1]);
  dimension = atoi(argv[2]);
  char* inputfile = argv[3];

  // Initializing Solution Matrix
  // int c[dimension][dimension];

  c = (int**) malloc(dimension * sizeof(int));
  m1 = (int**) malloc(dimension * sizeof(int));
  m2 = (int**) malloc(dimension * sizeof(int));

  for(int i = 0; i < dimension; i++)
  {
    c[i] = (int*) malloc(dimension * sizeof(int));
    m1[i] = (int*) malloc(dimension * sizeof(int));
    m2[i] = (int*) malloc(dimension * sizeof(int));
  }


  FILE * fp = fopen(inputfile, "r");

  if(fp == NULL){
    printf("Unable to open file\n");
    return -1;
  }

  int count = 0;
  int elementNo = dimension * dimension;
  char line[10];
  while(fgets(line, sizeof(line), fp) != NULL)
  {
    // line[strlen(line) - 1] = '\0';
    int i = atoi(line);
    int matrixNo = count / elementNo;
    int matrixElementNo = count % elementNo;
    int matrixRow = matrixElementNo / dimension;
    int matrixColumn = matrixElementNo % dimension;

    if(matrixNo == 0)
    {
      m1[matrixRow][matrixColumn]  = i;
    }else{
      m2[matrixRow][matrixColumn]  = i;
    }
    count++;
  }
  fclose(fp);
  printMatrix(m1);
  printMatrix(m2);
  //ConvMult();
  StrassMult();
  printMatrix(c);
  // PrintDiagonal();


  return 0;
}
