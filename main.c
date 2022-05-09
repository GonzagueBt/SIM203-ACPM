// Compilation:
//   icc -std=c99 -qopenmp -mkl main.c citiesReader.c
//   icc -std=c99 -qopenmp -mkl -qopt-report=1 -qopt-report-annotate=html main.c citiesReader.c
// Execution:
//   ./a.out
// Visualisation:
// python visualisation.py

#include "citiesReader.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <omp.h>
#include <mkl.h>

#ifndef R
  #define R 6378
#endif

#ifndef M_PI
  #define M_PI 3.14159265359
#endif

float distance(float v1lon,float v1lat, float v2lon, float v2lat){
  return R*acosf(sinf(v1lat*(M_PI/180))*sinf(v2lat*(M_PI/180)) + cosf(v1lon*(M_PI/180) - v2lon*(M_PI/180))*cosf(v1lat*(M_PI/180))*cosf(v2lat*(M_PI/180)));
}


int main() {

//-----------------------------------------------------------------
//--- READING cities
//-----------------------------------------------------------------

  int popMin;
  int dep;
  bool isDep;
  char* write = "w";
  printf("Minimal population? ");
  scanf("%i", &popMin);
  printf("By Departement ?(yes=0/no=1) ");
  scanf("%i", &dep);

  // Nettoyage des fichiers (dernière execution) pour ne pas avoir des incohérences ou erreurs de compilation
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  fclose(fileOut);
  fileOut = fopen("resuGraphDep.dat", "w");
  fclose(fileOut);
  fileOut = fopen("resuCities.dat", "w");
  fclose(fileOut);
  fileOut = fopen("resuCitiesDep.dat", "w");
  fclose(fileOut);
  if(dep==0){
    isDep=true;
    write = "a";
    
  }
  else isDep=false;

//-----------------------------------------------------------------
//--- COMPUTING graph
//-----------------------------------------------------------------

  int* voisin;
  ListOfCities* cities;
  float sizeNetwork = 0;
  int index = 0;
  int LOOP = 1;
  int FLOP = 0;

  // timing: start
  const double timeBegin = dsecnd();

  for(int t=0; t<LOOP; t++){
    printf("LOOP %i\n", t);
    if(isDep){                // Variante à poids minimal par Département
      cities = citiesReader(popMin,1,0);              //réseau (1)
      voisin = Prim(cities);
      citiesWriter(write, voisin, cities->number, index, false);
      //// inlined sans modification ////
      sizeNetwork = network_size(cities, voisin, cities->number);
      //// vectorisé sans modification ////
      FLOP+=cities->number+2*pow(cities->number,2);
      for(int i=1; i<=95; i++){                       //réseau (2)
        cities = citiesReader(popMin, 2,i);          
        voisin = Prim(cities);
        citiesWriter(write, voisin, cities->number, index, true);
        index += cities->number;
        //// inlined sans modification ////
        sizeNetwork += network_size(cities, voisin, cities->number);
        FLOP += cities->number+2*pow(cities->number,2);
      }
    }
    else{                   // Global 
      cities = citiesReader(popMin,0,0);
      //// inlined sans modification ////
      voisin  = Prim(cities);
      //// vectorisé sans modification ////
      citiesWriter(write, voisin, cities->number, index, false);
      //// inlined sans modification ////
      sizeNetwork = network_size(cities, voisin, cities->number);
      index = cities->number;
      FLOP+=cities->number+2*pow(cities->number,2);
    }
  }
  // timing: end
  const double timeEnd = dsecnd();

  double timeTotal = (double)(timeEnd-timeBegin)/LOOP;
  double debit1 = FLOP/timeTotal;          //débit arithmétique
  
  if(isDep){
    printf("---- Version poids minimal par departements ----\n");
  }else{
    printf("---- Version classique ----\n");
  }
  printf("taille du réseau  : %f villes\n", sizeNetwork);
  printf("Nombre de villes   : %i\n", index);
  printf("Temps d'exécution : %f s\n",timeTotal);
  printf("débit arithmétique : %f FLOP/s\n", debit1);
//-----------------------------------------------------------------
//--- DEALLOCATE arrays
//-----------------------------------------------------------------

  free(cities->name);
  free(cities->pop);
  free(cities->lon);
  free(cities->lat);
  free(cities);

  return 0;
}



int* Prim(ListOfCities* cities){
  // variables //
  bool* dansS = malloc(cities->number*sizeof(bool));
  int*  voisin = malloc(cities->number*sizeof(int));
  float*  dist = malloc(cities->number*sizeof(float));
  dansS[0] = true;
  dist[0] = 0;

  int sizeS = 1;
  // Initialisation //
  //// vectorisé sans modification ////
  for(int i=1; i<cities->number; i++){
    dansS[i] = false;
    voisin[i] = 0;
    dist[i] = distance(cities->lon[0], cities->lat[0],cities->lon[i], cities->lat[i]);
  }

  // Itérations //
  while(sizeS < cities->number-1){
    int index;
    float distM = FLT_MAX;
    //// unrolled sans modification ////
    for(int i=1; i<cities->number; i++){
      if(dansS[i]==false && dist[i]<distM){
        index = i;
        distM = dist[i];
      }
    }
    dansS[index] = true;
    //// vectorisé sans modification ////
    for(int j=0; j<cities->number; j++){
      if(j==index) continue;
      distM = distance(cities->lon[index], cities->lat[index], cities->lon[j], cities->lat[j]);
      if(dansS[j]==false && dist[j]> distM){
        dist[j]= distM;
        voisin[j] = index;
      }
    }
    sizeS++;
  }
  free(dansS);
  free(dist);
  return voisin;
}


void citiesWriter(const char* write, int* voisin, int number, int index, bool dep){
  FILE* fileOut = NULL;
  char* namefile = "resuGraph.dat";
  if(dep){
    namefile = "resuGraphDep.dat";
  }
  fileOut = fopen(namefile, write);
  for(int i=1; i<number; i++){
    fprintf(fileOut, "%i %i\n", (i+index), (voisin[i]+index));
  }
  fclose(fileOut);
}

void citiesWriterDep(const char* write, int* voisin, int number, int index){
  FILE* fileOut = NULL;
  fileOut = fopen("resuGraphDep.dat", write);
  for(int i=1; i<number; i++){
    fprintf(fileOut, "%i %i\n", (i+index), (voisin[i]+index));
  }
  fclose(fileOut);
}


float network_size(ListOfCities* cities, int* voisin, int n){
  float S=0.0;
  int v;
  //// vectorisé sans modification ////
  for(int k=1; k<n; k++){
    v=voisin[k];
    S+=distance(cities->lon[k], cities->lat[k], cities->lon[v], cities->lat[v]);
  }
  return S;
}
