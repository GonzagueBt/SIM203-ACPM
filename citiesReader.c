#include "citiesReader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
whichMode : Indication pour connaitre le mode de lecture du fichier, et d'écriture dans ResuCities.dat (ou resuCitiesDep.dat)
  3 possibilités :
  - 0 : mode classique, on lit toutes les villes dont la population est supérieur à popMin
  - 1 : mode plus grade ville par département 
  - 2 : mode par département, on lit uniquement les villes apprtenant au département 'dep' dont la population est supérieur à popMin 
*/
ListOfCities* citiesReader(int popMin,int whichMode, int dep){

  ListOfCities* cities = malloc(sizeof(ListOfCities));
  int* popATM = malloc(96*sizeof(int));     //taille actuelle de la ville choisie pour chaque département (0 si aucune ville)
  int* tab_index = malloc(96*sizeof(int));     //contient l'index actuel dans cities de chaque département, pour pouvoir changer lorsque qu'une ville plus grande est trouvée
  for(int i=0; i<96; i++){
      popATM[i]=0;        //aucune ville au début
      tab_index[i]=0;
  }

//--------------------------------------------------------------------
//--- READING cities with population greater than or equal to 'popMin'
//--------------------------------------------------------------------

  //printf("== Reading cities with population >= %i from 'citiesList.csv' ==\n", popMin);

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
      for(int i=0; i<1; i++) token = strtok(NULL, s);
      int myDep = atoi(token);                     //on récupère le département
      for(int i=0; i<13; i++){
        token = strtok(NULL, s);
      }
      int myPop = atoi(token);
      if(whichMode==0 && myPop >= popMin) cities->number++;
      else if(whichMode==1 && myPop >= popMin && popATM[myDep]==0){
        cities->number++;
        popATM[myDep]=1;   
      }
      else if(whichMode==2 && myPop >= popMin && myDep==dep) cities->number++;
    }
    fseek(inputFile, 0, SEEK_SET);
    // Allocate arrays
    cities->name = malloc(cities->number*sizeof(char*));
    cities->pop  = malloc(cities->number*sizeof(int));
    cities->lon  = malloc(cities->number*sizeof(float));
    cities->lat  = malloc(cities->number*sizeof(float));
    cities->dep  = malloc(cities->number*sizeof(int));
    
    for(int i=0; i<96; i++){
      popATM[i]=0;        //aucune ville au début
    }

    // Reading/Saving data
    int index=0;
    int k;
    char *myName = malloc(32*sizeof(char));
    while(fgets(line, 512, inputFile) != NULL){
      
      token = strtok(line, s);
      for(int i=0; i<1; i++) token = strtok(NULL, s);
      int myDep = atoi(token);
      for(int i=0; i<4;  i++) token = strtok(NULL, s);
      strncpy(myName, token, 32);
      for(int i=0; i<9; i++) token = strtok(NULL, s);
      int myPop = atoi(token);
      for(int i=0; i<5;  i++) token = strtok(NULL, s);
      float myLon = atof(token);
      for(int i=0; i<1;  i++) token = strtok(NULL, s);
      float myLat = atof(token);
      
      if(myPop >= popMin){
        if(whichMode==0 || (whichMode==2 && myDep==dep)){
          cities->name[index] = (char*) malloc(32*sizeof(char));
          strncpy(cities->name[index], myName, 32);
          cities->pop[index] = myPop;
          cities->lon[index] = myLon;
          cities->lat[index] = myLat;
          cities->dep[index] = myDep;
          index++;
        }
        else if(whichMode==1 && myPop >= popATM[myDep]){
          if(popATM[myDep]==0){                     // ce département n'a pas encore de ville dans cities
            cities->name[index] = (char*) malloc(32*sizeof(char));
            strncpy(cities->name[index], myName, 32);
            cities->pop[index] = myPop;
            cities->lon[index] = myLon;
            cities->lat[index] = myLat;
            cities->dep[index] = myDep;
            tab_index[myDep] = index;
            index++;
          }
          else{
              k=tab_index[myDep];
              cities->name[k] = (char*) malloc(32*sizeof(char));
              strncpy(cities->name[k], myName, 32);
              cities->pop[k] = myPop;
              cities->lon[k] = myLon;
              cities->lat[k] = myLat;
              cities->dep[k] = myDep;
          }
          popATM[myDep] = myPop;
        }
      }
    }
    free(myName);
    fclose(inputFile);
  }
  
//--------------------------------------------------------------------
//--- WRITING cities with population greater than or equal to 'popMin'
//--------------------------------------------------------------------

  //printf("== Writing cities with population >= %i in 'resuCities.dat' ==\n", popMin);

  FILE* outputFile = NULL;
  char* namefile = "resuCities.dat";
  char* write = "w";
  if(whichMode==2){
    namefile = "resuCitiesDep.dat";
    write =  "a";
  }
  outputFile = fopen(namefile, write);
  if(outputFile != NULL){
    for(int i=0; i<cities->number; i++){
      fprintf(outputFile, "%i %f %f %i\n", cities->pop[i], cities->lon[i], cities->lat[i], cities->dep[i]);
    }
    fclose(outputFile);
  }

  return cities;
}
