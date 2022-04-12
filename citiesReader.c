#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ListOfCities* citiesReader(int popMin){

  ListOfCities* cities = malloc(sizeof(ListOfCities));

//--------------------------------------------------------------------
//--- READING cities with population greater than or equal to 'popMin'
//--------------------------------------------------------------------

  printf("== Reading cities with population >= %i from 'citiesList.csv' ==\n", popMin);

  FILE* inputFile = NULL;
  inputFile = fopen("citiesList.csv", "r");
  if(inputFile != NULL){

    char line[512];
    const char s[2] = ",";
    char *token;
    
    // Count the number of cities with population greater than or equal to 'popMin'
    cities->number = 0;
    while(fgets(line, 512, inputFile) != NULL){
      token = strtok(line, s);
      for(int i=0; i<14; i++){
        token = strtok(NULL, s);
      }
      int myPop = atoi(token);
      if(myPop >= popMin) cities->number++;
    }
    fseek(inputFile, 0, SEEK_SET);
    
    // Allocate arrays
    cities->name = malloc(cities->number*sizeof(char*));
    cities->pop  = malloc(cities->number*sizeof(int));
    cities->lon  = malloc(cities->number*sizeof(float));
    cities->lat  = malloc(cities->number*sizeof(float));
    cities->dep  = malloc(cities->number*sizeof(int));
    
    // Reading/Saving data
    int index=0;
    char *myName = malloc(32*sizeof(char));
    while(fgets(line, 512, inputFile) != NULL){
      
      token = strtok(line, s);
      for(int i=0; i<1; i++) token = strtok(NULL, s);
      int myDep = atoi(token);
      for(int i=0; i<2;  i++) token = strtok(NULL, s);
      strncpy(myName, token, 32);
      for(int i=0; i<11; i++) token = strtok(NULL, s);
      int myPop = atoi(token);
      for(int i=0; i<5;  i++) token = strtok(NULL, s);
      float myLon = atof(token);
      for(int i=0; i<1;  i++) token = strtok(NULL, s);
      float myLat = atof(token);
      
      if(myPop >= popMin){
        cities->name[index] = (char*) malloc(32*sizeof(char));
        strncpy(cities->name[index], myName, 32);
        cities->pop[index] = myPop;
        cities->lon[index] = myLon;
        cities->lat[index] = myLat;
        cities->dep[index] = myDep;
        index++;
      }
    }
    free(myName);
    fclose(inputFile);
  }
  
//--------------------------------------------------------------------
//--- WRITING cities with population greater than or equal to 'popMin'
//--------------------------------------------------------------------

  printf("== Writing cities with population >= %i in 'resuCities.dat' ==\n", popMin);

  FILE* outputFile = NULL;
  outputFile = fopen("resuCities.dat", "w");
  if(outputFile != NULL){
    for(int i=0; i<cities->number; i++){
      fprintf(outputFile, "%i %f %f\n", cities->pop[i], cities->lon[i], cities->lat[i]);
    }
    fclose(outputFile);
  }

  return cities;
}
