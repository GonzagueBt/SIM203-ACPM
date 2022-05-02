
float distance(float v1lon,float v1lat, float v2lon, float v2lat);
int* Prim(ListOfCities* cities);
void citiesWriter(const char* write, int* voisin, int number, int index);
void citiesWriterDep(const char* write, int* voisin, int number, int index);
float network_size(ListOfCities* cities, int* voisin, int n);
ListOfCities* maxbyDep(ListOfCities* cities);
