#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int **m1;
int **m2;
int **product;

int n0;

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
  printf("\n");

}

// Only for testing
void printOddMatrix(int** matrix)
{
  for(int i = 0; i < dimension+1; i++)
  {
    for(int j = 0; j < dimension+1; j++)
    {
      printf("%i ", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");

}

// Prints final solution as specified
void PrintDiagonal(int** matrix)
{
  for (int i=0; i < dimension; i++)
  {
    printf("%i ", matrix[i][i]);
  }
}


int** allocateN2(int d)
{
  int ** temp = (int**) malloc(d * sizeof(int *));
  for(int i = 0; i < d; i++)
  {
    temp[i] = (int*) malloc(d * sizeof(int));
  }
  return temp;
}

int** allocateN2Init(int d)
{
  int ** temp = (int**) malloc(d * sizeof(int *));
  for(int i = 0; i < d; i++)
  {
    temp[i] = (int*) calloc(d ,sizeof(int));
  }

  return temp;
}

int** allocateAll(int d)
{
  int ** temp = (int**) malloc(d * sizeof(int *));
  for(int i = 0; i < d+1; i++)
  {
    temp[i] = (int*) malloc(d * sizeof(int));
  }
  return temp;
}

int** allocateAllInit(int d)
{
  int ** temp = (int**) malloc(d * sizeof(int *));
  for(int i = 0; i < d+1; i++)
  {
    temp[i] = (int*) malloc(d * sizeof(int));
  }
  for(int i = 0; i < d+1; i++)
  {
    for(int j = 0; j < d+1; j++)
    {
      temp[i][j] = 0;
    }
  }
  return temp;
}

// For all values of n
void ConvMult(int** result, int** matrix1, int** matrix2, int aRow, int aCol, int bRow, int bCol, int size)
{
  for (int i=0; i < size; i++)
  {
    for (int k=0; k < size; k++)
    {
      result[i][k]=0;
      for (int j=0; j < size; j++)
      {
        result[i][j] = result[i][j]+matrix1[aRow + i][aCol + k]*matrix2[bRow + k][bCol + j];
      }
    }
  }
}

void sumMatrices(int** target, int** a, int** b, int targetRow, int targetCol, int aRow, int aCol, int bRow, int bCol, int size)
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      target[targetRow + i][targetCol + j] = a[aRow + i][aCol + j] + b[bRow + i][bCol + j];
    }
  }
}

void subtractMatrices(int** target, int** a, int** b, int targetRow, int targetCol, int aRow, int aCol, int bRow, int bCol, int size)
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      target[targetRow + i][targetCol + j] = a[aRow + i][aCol + j] - b[bRow + i][bCol + j];
    }
  }
}


void copyMatrices(int** target, int** from, int rRow, int rCol, int aRow, int aCol, int size)
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      target[rRow + i][rCol + j] = from[aRow + i][aCol + j];
    }
  }
}


//for even n value
void StrassMult(int** result, int** matrix1, int** matrix2, int aRow, int aCol, int bRow, int bCol, int size)
{
  // if(size == 2)
  // {
  //   int a = matrix1[aRow][aCol];
  //   int b = matrix1[aRow][aCol + 1];
  //   int c = matrix1[aRow + 1][aCol];
  //   int d = matrix1[aRow + 1][aCol + 1];
  //   int e = matrix2[bRow][bCol];
  //   int f = matrix2[bRow][bCol + 1];
  //   int g = matrix2[bRow + 1][bCol];
  //   int h = matrix2[bRow + 1][bCol + 1];
  //
  //   int p1 = a * (f - h);
  //   int p2 = (a + b) * h;
  //   int p3 = (c + d) * e;
  //   int p4 = d * (g - e);
  //   int p5 = (a + d) * (e + h);
  //   int p6 = (b - d) * (g + h);
  //   int p7 = (a - c) * (e + f);
  //
  //   result[0][0] = p5 + p4 - p2 + p6;
  //   result[0][1] = p1 + p2;
  //   result[1][0] = p3 + p4;
  //   result[1][1] = p5 + p1 - p3 - p7;
  //   return;
  // }


  // Fix to accomodate odd numbers
  int sub_size = size/2;


  int a_r, b_r, c_r, d_r, e_r, f_r, g_r, h_r, a_c, b_c, c_c, d_c, e_c, f_c, g_c, h_c;

  a_r = aRow;
  a_c = aCol;
  b_r = a_r;
  b_c = a_c + sub_size;
  c_r = a_r + sub_size;
  c_c = a_c;
  d_r = a_r + sub_size;
  d_c = a_c + sub_size;

  e_r = bRow;
  e_c = bCol;
  f_r = e_r;
  f_c = e_c + sub_size;
  g_r = e_r + sub_size;
  g_c = e_c;
  h_r = e_r + sub_size;
  h_c = e_c + sub_size;



  if(size > n0)
  {
    int** s1 = allocateN2Init(sub_size);
    int** s2 = allocateN2Init(sub_size);
    int** s3 = allocateN2Init(sub_size);

    // printf("Strassen %i!!!\n", size);

    // P1
      // copyMatrices(s1, matrix1, 0, 0, a_r, a_c, sub_size);
      subtractMatrices(s2, matrix2, matrix2, 0, 0, f_r, f_c, h_r, h_c, sub_size);
      StrassMult(s3, matrix1, s2, a_r, a_c, 0, 0, sub_size);

      // AF + BH
      copyMatrices(result, s3, 0 , sub_size, 0, 0, sub_size);
      // CF + DH
      copyMatrices(result, s3, sub_size , sub_size, 0, 0, sub_size);

    // P5
      sumMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, d_r, d_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, e_r, e_c, h_r, h_c, sub_size);
      StrassMult(s3, s1, s2, 0, 0, 0, 0, sub_size);

      // AE + BG
      copyMatrices(result, s3, 0, 0, 0, 0, sub_size );
      // CF + DH
      sumMatrices(result, result, s3, sub_size, sub_size, sub_size, sub_size, 0, 0,  sub_size);


    // P3
      sumMatrices(s1, matrix1, matrix1, 0, 0, c_r, c_c, d_r, d_c, sub_size);
      // copyMatrices(s2, matrix2, 0, 0, e_r, e_c, sub_size);
      StrassMult(s3, s1, matrix2, 0, 0, e_r, e_c, sub_size);

      // CE + DG
      copyMatrices(result, s3, sub_size, 0, 0, 0, sub_size);

      // CF + DH
      subtractMatrices(result, result, s3, sub_size, sub_size, sub_size, sub_size, 0, 0, sub_size);

    // P2
      sumMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, b_r, b_c, sub_size);
      // copyMatrices(s2, matrix2, 0, 0, h_r, h_c, sub_size);
      StrassMult(s3, s1, matrix2, 0, 0, h_r, h_c, sub_size);

      // AE + BG
      subtractMatrices(result, result, s3, 0, 0, 0, 0, 0, 0, sub_size);

      // AF + BH
      sumMatrices(result, result, s3, 0, sub_size, 0, sub_size, 0, 0, sub_size);

    // P4
      // copyMatrices(s1, matrix1, 0, 0, d_r, d_c, sub_size);
      subtractMatrices(s2, matrix2, matrix2, 0, 0, g_r, g_c, e_r, e_c, sub_size);
      StrassMult(s3, matrix1, s2, d_r, d_c, 0, 0, sub_size);

      // AE + BG
      sumMatrices(result, result, s3, 0, 0, 0, 0, 0, 0, sub_size);

      // CE + DG
      sumMatrices(result, result, s3,  sub_size, 0, sub_size, 0, 0, 0, sub_size);

    // P6
      subtractMatrices(s1, matrix1, matrix1, 0, 0, b_r, b_c, d_r, d_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, g_r, g_c, h_r, h_c, sub_size);
      StrassMult(s3, s1, s2, 0, 0, 0, 0, sub_size);

      // AE + BG
      sumMatrices(result, result, s3, 0, 0, 0, 0, 0, 0, sub_size);

    // P7
      subtractMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, c_r, c_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, e_r, e_c, f_r, f_c, sub_size);
      StrassMult(s3, s1, s2, 0, 0, 0, 0, sub_size);

      // CF + DH
      subtractMatrices(result, result, s3, sub_size, sub_size, sub_size, sub_size, 0, 0, sub_size);




  }
  else
  {
    // printf("Normal!!!\n");
    ConvMult(result, matrix1, matrix2, aRow, aCol, bRow, bCol, size);
  }
//


}


int main(int argc, char *argv[])
{

  flexible = atoi(argv[1]);
  dimension = atoi(argv[2]);
  char* inputfile = argv[3];

  n0 = 1;

  // Initializing Solution Matrix
  if (dimension % 2 != 0)
  {
    product = allocateN2Init(dimension+1);
    m1 = allocateN2(dimension+1);
    m2 = allocateN2(dimension+1);
  } else {
    product = allocateN2Init(dimension);
    m1 = allocateN2(dimension);
    m2 = allocateN2(dimension);
  }

  // Intermediate Calculations


  // If flag is 0, read from input file, otherwise fill random
  if(flexible == 0)
  {
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
      int number = atoi(line);
      int matrixNo = count / elementNo;
      int matrixElementNo = count % elementNo;
      int matrixRow = matrixElementNo / dimension;
      int matrixColumn = matrixElementNo % dimension;

      if(matrixNo == 0)
      {
        m1[matrixRow][matrixColumn]  = number;
      }else{
        m2[matrixRow][matrixColumn]  = number;
      }
      count++;
    }
    fclose(fp);

  }else{
    srand(time(NULL));
    for(int i = 0 ; i < dimension; i++)
    {
      for(int j = 0; j < dimension; j++)
      {
        int randomNumber;
        randomNumber = rand() % 10;
        m1[i][j] = randomNumber;

        randomNumber = rand() % 10;
        m2[i][j] = randomNumber;

        // printf("i = %d\n", i );
        // printf("j = %d\n", j);
      }
    }
    if (dimension % 2 != 0)
    {
      for(int i = 0; i < dimension+1; i++)
      {
          m1[dimension][i]=0;
          m2[dimension][i]=0;
          m1[i][dimension]=0;
          m2[i][dimension]=0;
        }

        //printf("%d\n", m1[dimension][dimension]);
      }
  }

  // // For Testing
  // printf("Matrix 1: \n");
  // // if (dimension % 2 != 0){
  // //   printOddMatrix(m1);
  // // } else{
  //   printMatrix(m1);
  // // }
  // // printMatrix(m1);
  // printf("\n");
  // printf("Matrix 2: \n");
  // // if (dimension % 2 != 0){
  // //   printOddMatrix(m2);
  // // } else{
  //   printMatrix(m2);
  // // }

  clock_t b, f;
  //n0 = 128;
  n0 = 64;

  b = clock();

  // Only for testing
  // ConvMult(product, m1, m2, 0, 0, 0, 0, dimension);
  printf("Normal Multiplication: \n");
  // if (dimension % 2 != 0){
  //   printOddMatrix(product);
  // } else{
    PrintDiagonal(product);
  // }
  printf("\n");

  f = clock();
  double convTimeTaken = ((double)(f - b)) / CLOCKS_PER_SEC;

  product = allocateN2Init(dimension);

  b = clock();
  if (dimension % 2 != 0){
    StrassMult(product, m1, m2, 0, 0, 0, 0, dimension+1);
  } else{
    StrassMult(product, m1, m2, 0, 0, 0, 0, dimension);
  }
  printf("Smart Strassen Multiplication: \n");
  // PrintDiagonal(product);
  // if (dimension % 2 != 0){
  //   printOddMatrix(product);
  // } else{
    PrintDiagonal(product);
  // }
  printf("\n");
  f = clock();
  double strassenTimeTaken = ((double)(f - b)) / CLOCKS_PER_SEC;

  printf("Conventional Time: %f\n", convTimeTaken);
  printf("Strassen Time: %f\n", strassenTimeTaken);



  return 0;



}
