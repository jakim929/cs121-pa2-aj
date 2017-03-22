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

// Prints final solution as specified
void PrintDiagonal(int** matrix)
{
  for (int i=0; i < dimension; i++)
  {
    printf("%i\n", matrix[i][i]);
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
    temp[i] = (int*) malloc(d * sizeof(int));
  }
  for(int i = 0; i < d; i++)
  {
    for(int j = 0; j < d; j++)
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
      // result[i][j]=0;
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


void copyMatrices(int** target, int** from, int aRow, int aCol, int size)
{
  for(int i = 0; i < size; i++)
  {
    for(int j = 0; j < size; j++)
    {
      target[i][j] = from[aRow + i][aCol + j];
    }
  }
}


//for even n value
void StrassMult(int** result, int** matrix1, int** matrix2, int aRow, int aCol, int bRow, int bCol, int size)
{
  if(size == 2)
  {
    int a = matrix1[aRow][aCol];
    int b = matrix1[aRow][aCol + 1];
    int c = matrix1[aRow + 1][aCol];
    int d = matrix1[aRow + 1][aCol + 1];
    int e = matrix2[bRow][bCol];
    int f = matrix2[bRow][bCol + 1];
    int g = matrix2[bRow + 1][bCol];
    int h = matrix2[bRow + 1][bCol + 1];

    int p1 = a * (f - h);
    int p2 = (a + b) * h;
    int p3 = (c + d) * e;
    int p4 = d * (g - e);
    int p5 = (a + d) * (e + h);
    int p6 = (b - d) * (g + h);
    int p7 = (a - c) * (e + f);

    result[0][0] = p5 + p4 - p2 + p6;
    result[0][1] = p1 + p2;
    result[1][0] = p3 + p4;
    result[1][1] = p5 + p1 - p3 - p7;
    return;
  }


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

  int** p1 = allocateN2(sub_size);
  int** p2 = allocateN2(sub_size);
  int** p3 = allocateN2(sub_size);
  int** p4 = allocateN2(sub_size);
  int** p5 = allocateN2(sub_size);
  int** p6 = allocateN2(sub_size);
  int** p7 = allocateN2(sub_size);

  int** s1 = allocateN2(sub_size);
  int** s2 = allocateN2(sub_size);

  if(n0 > size)
  {
    // FIX to get rid of unnecessary copying
    // P1
      copyMatrices(s1, matrix1, a_r, a_c, sub_size);
      subtractMatrices(s2, matrix2, matrix2, 0, 0, f_r, f_c, h_r, h_c, sub_size);
      StrassMult(p1, s1, s2, 0, 0, 0, 0, sub_size);

    // P2
      sumMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, b_r, b_c, sub_size);
      copyMatrices(s2, matrix2, h_r, h_c, sub_size);
      StrassMult(p2, s1, s2, 0, 0, 0, 0, sub_size);


    // P3
      sumMatrices(s1, matrix1, matrix1, 0, 0, c_r, c_c, d_r, d_c, sub_size);
      copyMatrices(s2, matrix2, e_r, e_c, sub_size);
      StrassMult(p3, s1, s2, 0, 0, 0, 0, sub_size);


    // P4
      copyMatrices(s1, matrix1, d_r, d_c, sub_size);
      subtractMatrices(s2, matrix2, matrix2, 0, 0, g_r, g_c, e_r, e_c, sub_size);
      StrassMult(p4, s1, s2, 0, 0, 0, 0, sub_size);

    // P5
      sumMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, d_r, d_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, e_r, e_c, h_r, h_c, sub_size);
      StrassMult(p5, s1, s2, 0, 0, 0, 0, sub_size);

    // P6
      subtractMatrices(s1, matrix1, matrix1, 0, 0, b_r, b_c, d_r, d_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, g_r, g_c, h_r, h_c, sub_size);
      StrassMult(p6, s1, s2, 0, 0, 0, 0, sub_size);

    // P7
      subtractMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, c_r, c_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, e_r, e_c, f_r, f_c, sub_size);
      StrassMult(p7, s1, s2, 0, 0, 0, 0, sub_size);
  }
  else
  {
    // FIX to get rid of unnecessary copying
    // P1
      copyMatrices(s1, matrix1, a_r, a_c, sub_size);
      subtractMatrices(s2, matrix2, matrix2, 0, 0, f_r, f_c, h_r, h_c, sub_size);
      ConvMult(p1, s1, s2, 0, 0, 0, 0, sub_size);

    // P2
      sumMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, b_r, b_c, sub_size);
      copyMatrices(s2, matrix2, h_r, h_c, sub_size);
      ConvMult(p2, s1, s2, 0, 0, 0, 0, sub_size);


    // P3
      sumMatrices(s1, matrix1, matrix1, 0, 0, c_r, c_c, d_r, d_c, sub_size);
      copyMatrices(s2, matrix2, e_r, e_c, sub_size);
      ConvMult(p3, s1, s2, 0, 0, 0, 0, sub_size);


    // P4
      copyMatrices(s1, matrix1, d_r, d_c, sub_size);
      subtractMatrices(s2, matrix2, matrix2, 0, 0, g_r, g_c, e_r, e_c, sub_size);
      ConvMult(p4, s1, s2, 0, 0, 0, 0, sub_size);

    // P5
      sumMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, d_r, d_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, e_r, e_c, h_r, h_c, sub_size);
      ConvMult(p5, s1, s2, 0, 0, 0, 0, sub_size);

    // P6
      subtractMatrices(s1, matrix1, matrix1, 0, 0, b_r, b_c, d_r, d_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, g_r, g_c, h_r, h_c, sub_size);
      ConvMult(p6, s1, s2, 0, 0, 0, 0, sub_size);

    // P7
      subtractMatrices(s1, matrix1, matrix1, 0, 0, a_r, a_c, c_r, c_c, sub_size);
      sumMatrices(s2, matrix2, matrix2, 0, 0, e_r, e_c, f_r, f_c, sub_size);
      ConvMult(p7, s1, s2, 0, 0, 0, 0, sub_size);
  }
// Combine Results
// FIX make more efficient, don't do it one by one
// AE + BG
  sumMatrices(result, p5, p4, 0, 0, 0, 0, 0, 0, sub_size);
  subtractMatrices(result, result, p2, 0, 0, 0, 0, 0, 0, sub_size);
  sumMatrices(result,result,  p6, 0, 0, 0, 0, 0, 0, sub_size);

// AF + BH
  sumMatrices(result, p1, p2, 0 , sub_size, 0, 0, 0, 0, sub_size);

// CE + DG
  sumMatrices(result, p3, p4, sub_size , 0, 0, 0, 0, 0, sub_size);

// CF + DH
  sumMatrices(result, p5, p1, sub_size , sub_size, 0, 0, 0, 0, sub_size);
  subtractMatrices(result, result, p3, sub_size, sub_size, sub_size, sub_size, 0, 0, sub_size);
  subtractMatrices(result, result, p7, sub_size, sub_size, sub_size, sub_size, 0, 0, sub_size);




}


int main(int argc, char *argv[])
{

  flexible = atoi(argv[1]);
  dimension = atoi(argv[2]);
  char* inputfile = argv[3];

  // Initializing Solution Matrix
  product = allocateN2Init(dimension);
  m1 = allocateN2(dimension);
  m2 = allocateN2(dimension);
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
      }
    }
  }

  printf("Matrix 1: \n");
  printMatrix(m1);
  printf("\n");
  printf("Matrix 2: \n");
  printMatrix(m2);
  

  clock_t b, f;
  n0 = 256;

  b = clock();
  // ConvMult(product, m1, m2, 0, 0, 0, 0, dimension);
  printf("Normal Multiplication: \n");
  // PrintDiagonal(product);
  printMatrix(product);
  printf("Not in use alone \n");
  printf("\n");
  f = clock();
  double convTimeTaken = ((double)(f - b)) / CLOCKS_PER_SEC;

  b = clock();
  StrassMult(product, m1, m2, 0, 0, 0, 0, dimension);
  printf("Strassen Multiplication: \n");
  // PrintDiagonal(product);
  printMatrix(product);
  printf("\n");
  f = clock();
  double strassenTimeTaken = ((double)(f - b)) / CLOCKS_PER_SEC;

  printf("Conventional Time: %f\n", convTimeTaken);
  printf("Strassen Time: %f\n", strassenTimeTaken);

  // PrintDiagonal();


  return 0;



}
