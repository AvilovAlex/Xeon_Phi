// 25600*1024*4/1024/1024/1024/352=0.277 мс - xeon phi
//27 мс - cpu
#include <iostream>
#include <ctime>
#include <omp.h>
#include <cstdlib>

using namespace std;


int main( int argc, char** argv )
{
  // 25600*1024*4/1024/1024/1024/352=0.277 мс
  //28 мс - cpu
  const int rowCount = 25600;
  const int colCount = 1024;
  int * matrix = (int*) malloc(rowCount*colCount*sizeof(int));
  if (matrix==NULL) exit (1);

  srand(0);

  //заполнение матрицы случайными числами
 for(int i = 0; i < rowCount; i++)
    for(int j = 0; j < colCount; j++)
      matrix[i*colCount + j] = colCount-j/*rand() % 100 + 1*/;


  int N = rowCount * colCount;
  int * countTranspArr = (int*) malloc(rowCount*sizeof(int));

  //openmp getwait time
  double start = omp_get_wtime();
  #pragma omp parallel for
  for(int i = 0; i < rowCount; i++){
    int localCount = 0;
    for(int j = 0; j < colCount-1; j++){
      if(matrix[i*colCount + j] > matrix[i*colCount + j + 1])
        localCount++;
    }
    countTranspArr[i] = localCount;
  }

  double elapsedTimeCPU = omp_get_wtime() - start;
  cout << "CPU sum time = " << elapsedTimeCPU*1000 << " ms\n";
  cout << "CPU memory throughput = " << 2*(rowCount*sizeof(int)+N*sizeof(int))/elapsedTimeCPU/1024/1024/1024 << " Gb/s\n";

}
