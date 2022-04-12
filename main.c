// Compilation:
//   icc -std=c99 main.c citiesReader.c
// Execution:
//   ./a.out

#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <float.h>

#ifndef R
  #define R 6378
#endif

float distance(float v1lon,float v1lat, float v2lon, float v2lat){
  return R*acos(sin(v1lat)*sin(v2lat) + cos(v1lon - v2lon)*cos(v1lat)*cos(v2lat));
}

bool dansS(int size, char** S, char * name){
  for(int i=0; i<size; i++){
     if(strcmp(S[i],name) == 0) return true;
  }
  return false;
}

int main() {

//-----------------------------------------------------------------
//--- READING cities
//-----------------------------------------------------------------

  int popMin;
  printf("Minimal population? ");
  scanf("%i", &popMin);

  ListOfCities* cities;
  cities = citiesReader(popMin);

  // ... just to check! This line can be removed.
  for(int i=0; i<cities->number; i++){
    printf("%i %s %i %f %f\n", cities->dep[i], cities->name[i], cities->pop[i], cities->lon[i], cities->lat[i]);
  }

//-----------------------------------------------------------------
//--- COMPUTING graph
//-----------------------------------------------------------------

  // variables //
  bool* dansS = malloc(cities->number*sizeof(bool));
  int*  voisin = malloc(cities->number*sizeof(int));
  float*  dist = malloc(cities->number*sizeof(float));
  dansS[0] = true;
  int sizeS = 1;
  // Initialisation //
  for(int i=1; i<cities->number; i++){
    dansS[i] = false;
    voisin[i] = 0;
    dist[i] = distance(cities->lon[0], cities->lat[0],cities->lon[i], cities->lat[i]);
  }

  // Itérations //
  while(sizeS < cities->number-1){
    int index;
    float distM = FLT_MAX;
    for(int i=1; i<cities->number; i++){
      if(dansS[i]==false && dist[i]<distM){
        index = i;
        distM = dist[i];
      }
    }

    sizeS++;
  }



  // Écriture du graphe (chaque ligne correspond à une arête)
  // !!! Ci-dessous, on écrit le graphe complet pour l'exemple
  // !!! Vous devez modifier cette commande pour écrire le graphe obtenu avec Prim


  FILE* fileOut = NULL;
  fileOut = fopen("resuGraph.dat", "w");
  for(int i=0; i<cities->number; i++){
    for(int j=0; j<i; j++){
      fprintf(fileOut, "%i %i\n", i, j);
    }
  }
  fclose(fileOut);

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
