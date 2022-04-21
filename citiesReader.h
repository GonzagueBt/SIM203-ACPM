#ifndef CITIES_READER
#define CITIES_READER

typedef struct ListOfCities ListOfCities;

struct ListOfCities
{
  int    number;
  char** name;
  int*   pop;
  float* lon;
  float* lat;
  int*   dep;
};

ListOfCities* citiesReader(int popMin);
ListOfCities* citiesReader_maxbyDep(int popMin);
ListOfCities* citiesReader_myDep(int popMin, int dep);

#endif
