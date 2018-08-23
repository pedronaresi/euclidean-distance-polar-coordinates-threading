#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define N 100000

/*
#if DEZ_CINCO
  #define N 100000
#else
  #define N 10000000
#endif
*/

#define MAX_THREADS 1 //8


double *r1, *r2, *teta1, *teta2;

double *result;

void *distancia_radial(void *tid) {
  long thid;
  long i;

  thid = (long) tid;

  for(i=thid; i<N; i+=MAX_THREADS){
      //if (array[i]>maxloc) maxloc = array[i];
      result[thid] = sqrt((pow(r1[i],2) + pow(r2[i],2) - 2*r1[i]*r2[i]*cos(teta1[i]-teta2[i])));
  }

  printf("Thread %ld finalizou.\n", thid);

  pthread_exit(0);
} 

int main(void){
  pthread_t t[MAX_THREADS];
  double maxglobal;
  long i;
  long th;

  struct timeval inicio, final2;
  int tmili;

  r1 = (double*) malloc(N*sizeof(double));
  r2 = (double*) malloc(N*sizeof(double));
  teta1 = (double*) malloc(N*sizeof(double));
  teta2 = (double*) malloc(N*sizeof(double));

  result = (double*) malloc(MAX_THREADS*sizeof(double));

  for(i=0; i<N; i++){
      r1[i] = (double) (rand()%100);
      r2[i] = (double) (rand()%100);
  }

  for(i=0; i<N; i++){
      teta1[i] = (double) (rand()%360);
      teta2[i] = (double) (rand()%360);
  }

  gettimeofday(&inicio, NULL);

  for(th=0; th<MAX_THREADS; th++) {
    pthread_create(&t[th], NULL, distancia_radial, (void *) th);
  }

  //maxglobal = r1[0];
  for(th=0; th<MAX_THREADS; th++) {
    pthread_join(t[th],NULL);
    //if (result[th]>maxglobal) maxglobal = result[th];
  }

  gettimeofday(&final2, NULL);
  tmili = (int) (1000 * (final2.tv_sec - inicio.tv_sec) + (final2.tv_usec - inicio.tv_usec) / 1000);

  printf("\ntempo decorrido para o calculo: %d milisegundos\n", tmili);

  //printf("maior valor=%f\n",maxglobal);

  return 0;
}
