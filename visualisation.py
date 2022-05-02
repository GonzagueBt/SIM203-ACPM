# Execution:
#   python visualisation.py

import matplotlib
matplotlib.use('Agg')
from matplotlib import pyplot
from numpy import loadtxt
import numpy as np

# draw the cities
cities = loadtxt("resuCities.dat", dtype=float, delimiter=" ")
if np.size(cities)>0 :
  pyplot.scatter(cities[:,1], cities[:,2], s=cities[:,0]/1000, c=cities[:,0], alpha=0.5);
  # draw the graph
  graph = loadtxt("resuGraph.dat", dtype=int)
  print("Reseau plus grande ville de chaque departement : "+str(graph.shape[0]+1)+ " villes")
  for x in range(graph.shape[0]):
    edge = [graph[x,0], graph[x,1]]
    pyplot.plot(cities[edge,1], cities[edge,2], 'r')

citiesDep = loadtxt("resuCitiesDep.dat", dtype=float, delimiter=" ")
if np.size(citiesDep)>0 :
  pyplot.scatter(citiesDep[:,1], citiesDep[:,2], s=citiesDep[:,0]/1000, c=citiesDep[:,0], alpha=0.5);
  #draw department
  graph = loadtxt("resuGraphDep.dat", dtype=int)
  print("Reseaux departements : "+str(graph.shape[0]+1)+" villes")
  for x in range(graph.shape[0]):
    edge = [graph[x,0], graph[x,1]]
    pyplot.plot(citiesDep[edge,1], citiesDep[edge,2], 'b')

pyplot.xlabel('Longitude', size=16)
pyplot.ylabel('Latitude', size=16)

# save in a PNG file
pyplot.savefig('result.png')

# show the result
pyplot.show()
