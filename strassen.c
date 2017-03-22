#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int **m1;
int **m2;

int flexible;
int dimension;


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

int main(int argc, char *argv[])
{

  flexible = atoi(argv[1]);
  dimension = atoi(argv[2]);
  char* inputfile = argv[3];

  m1 = (int**) malloc(dimension * sizeof(int));
  m2 = (int**) malloc(dimension * sizeof(int));

  for(int i = 0; i < dimension; i++)
  {
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


  return 0;
}
