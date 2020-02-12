# ShortestPathAlgorithm
This program is used to go through a given the plan of city roads and use it to determine the best path to choose for several uber-like taxi service to reach another grid. 

# Input
The plan of city roads is a data set which includes the name of street intersections and the time (in seconds) it takes to travel to the intersection to it's east, north, west and south respectively. 
Grid locations of where the taxis are, so that the the taxi that could reach certain grid in the shortest amount of time could be dispatched.
The text files should be inputed to the executable file in the command prompt.

# Output
The output should be in 3 stages:
1. Reading input: size of city grid, total number of possibilities, grid locations supplied.
2. Shortest path to any grid on the map from the closest taxi (grid location provided).
3. A rendered map version of output 2, showing how long it will take to get to each location (values shown are values of each step and not total value)

# Notes
The path planning is achieved using Dijkstra's algorithm.
The shortest time from any available taxi to any intersection of city roads is drawn onto a diagram in stage 3.
