/* This program is used to go through a given the plan of city roads and use
   it to determine the best path to choose for several uber-like taxi service to
   reach another grid. The plan of city roads is a data set which includes the
   name of street intersections and the time (in seconds) it takes to travel to
   the intersection to it's east, north, west and south respectively. 
   The path planning is achieved using Dijkstra's algorithm.
   The shortest time from any available taxi to any intersection of city roads
   is drawn onto a diagram in stage 3.
   
   Written by Hanze Hu, last edited on 16th October, 2017.
*/

/* Algorithms are fun!! */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/* define values for exiting the program and true or false statements used */
#define FALSE -1
#define TRUE !FALSE

#define FIRST 0
#define RESET 0

/* the number of rows between each number lines in the graph */
#define ROW_IN_GRAPH 3

/* any distance of 999 is considered to be blocked or unvisited */
#define BLOCKED 999

/* the name structure of intersections */
typedef struct grid_name{
	int col;
	char row;
} name_t;

/* stores all the data for each intersection */
typedef struct path_time{
	name_t grid;
	int E;  	//East
	int N;		//North
	int W;		//West
	int S;		//South
	int low_dist;	//shortest distance from a vehicle
	name_t from;	//the grid where the shortest distance is
	int steps;		//number of steps it takes from a vehicle
	int visited;	//marks whether a grid has been visited and checked
	char dir;  	/*which direction is the shortest path from, l-left, r-right,
	u-up, d-down or c-central (where a vehicle lies) */
} path_t;

/* stores most relevent overall data */
typedef struct overall_data{
	int r; 	//number of rows
	int c;	//number of columns
	int total_time;
	int imposs_paths;
} all_t;

/* initialise all functions */
void check_paths(path_t*, all_t*);
name_t* create_car(char, int);
int loc_on_map(name_t*, int);
void check_grid(name_t*, path_t**,int, int);


int main(int argc, char* argv[]){
	int i, j, k, inter, col, no_cars, step, id, count = 0, shortest=BLOCKED;
	/* used to confirm that all intersections have been visited */
	int all_visited = FALSE;
	char row;

	all_t* values = malloc(sizeof(all_t));
	values -> total_time = RESET;
	values -> imposs_paths = RESET;

	/* Stage 1 */
	scanf("%d%d",&values->c,&values->r);	//overall rows and columns
	inter=values->r*values->c;   			// total number of intersections

	/* storing all grid information into system using malloc and struct types */
	path_t **map = malloc(inter*sizeof(path_t*));
	for(i=0; i< inter; i++)
		map[i] = malloc(sizeof(path_t));
	for(i=0; i< inter; i++){
		scanf("%d%c%d%d%d%d",&map[i] -> grid.col, &map[i] -> grid.row,
			&map[i ]-> E, &map[i] -> N, &map[i] -> W, &map[i] -> S);
		check_paths(map[i], values);
		map[i] -> low_dist = BLOCKED;
		map[i] -> steps = RESET;
		map[i] -> visited = FALSE;
		map[i] -> dir = 'c';
	}
	
	/* storing the location of all available vehicles */
	name_t* uber[inter];
	i = 0;
	while(scanf("%d%c", &col, &row)>0){
		uber[i]=create_car(row, col);
		i++;
	}
	no_cars=i;
	name_t **cars = malloc(no_cars*sizeof(name_t*));
	for(i=0; i< no_cars; i++){
		cars[i]=uber[i];
	}
	
	/* printing the output for all of stage 1 */
	printf("S1: grid is %d x %d, and has %d intersections\n",values -> c,
		values -> r,inter);
	printf("S1: of %d possibilities, %d of them cannot be used\n", inter*4,
		values -> imposs_paths);	// 4 directions for each intersection
	printf("S1: total cost of remaining possibilities is %d seconds\n", 
		values -> total_time);
	printf("S1: %d grid locations supplied, first one is %d%c, last one is",
		no_cars, cars[0]->col, cars[0]->row);
	printf("%d%c\n", cars[no_cars-1]->col, cars[no_cars-1]->row);
	
	
	/* Stage 2 */
	/* uses check_grid to change the lowest_distance from any car */
	printf("\n");
	name_t* next_loc = malloc(sizeof(name_t));
	name_t* check_next=malloc(sizeof(name_t));
	shortest=BLOCKED;
	map[loc_on_map(cars[FIRST],values->c)] -> low_dist = RESET;
	check_next->row=cars[FIRST]->row;
	check_next->col=cars[FIRST]->col;
	check_grid(check_next,map,values->c,values->r);
	
	/* check_grid is ran until all the intersections have been visited */
	while(all_visited==FALSE){
		all_visited=TRUE;
		for(j=0;j<inter;j++){
			if(map[j]->visited==FALSE&&map[j]->low_dist<shortest){
				check_next->col=map[j]->grid.col;
				check_next->row=map[j]->grid.row;
				shortest=map[j]->low_dist;
				all_visited=FALSE;
			}
		}
		shortest=BLOCKED;
		check_grid(check_next, map,values->c,values->r);
	}
	
	/* returns the required output for stage 2 */
	for(i=1;i<no_cars;i++){
		printf("S2: start at grid %d%c, cost of 0\n",cars[0]->col,cars[0]->row);
		step=map[loc_on_map(cars[i],values->c)]->steps;
		while(step>0){
			id=loc_on_map(cars[i],values->c);
			step--;
			for(j=0;j<step;j++){
				next_loc->row=map[id]->from.row;
				next_loc->col=map[id]->from.col;
				id=loc_on_map(next_loc, values->c);
			}
			printf("S2:       then to %d%c, cost of %d\n",map[id]->grid.col,
					map[id]->grid.row,map[id]->low_dist);
		}
	}
	
	
	/* Stage 3 */
	/* run the loop again for all other vehicles to find the shortest time to 
	   each intersections */
	for(k=1; k< no_cars; k++){
		for(i = 0; i < inter; i++)
			map[i]->visited=FALSE;
		map[loc_on_map(cars[k],values->c)] -> low_dist = RESET;
		map[loc_on_map(cars[k],values->c)] -> dir = 'c';
		check_next->row=cars[k]->row;
		check_next->col=cars[k]->col;
		all_visited=FALSE;
		shortest=BLOCKED;
		check_grid(check_next, map, values->c, values->r);
		while(all_visited==FALSE){
			all_visited=TRUE;
			for(j=0;j<inter;j++){
				if(map[j]->visited==FALSE&&map[j]->low_dist<shortest){
					check_next->col=map[j]->grid.col;
					check_next->row=map[j]->grid.row;
					shortest=map[j]->low_dist;
					all_visited=FALSE;
				}
			}
		shortest=BLOCKED;
		check_grid(check_next, map,values->c,values->r);
		}
	}
	
	/* the graph is formatted and outputted */
	char first_row[values->c];
	char second_row[values->c];
	char row_op[values->c];
	printf("\nS3:");
	for(i=0;i<values->c;i++){
		printf("        %d",i);
	}
	printf("\nS3:   +----+");
	for(i=0;i<values->c-1;i++){
		printf("--------+");
	}
	for(i=0;i<values->r-1;i++){
		memset(first_row, RESET, sizeof(first_row));
		memset(second_row, RESET, sizeof(second_row));
		memset(row_op, RESET, sizeof(row_op));
		for(j=0;j<values->c;j++){
			count=i*values->c+j;
			first_row[j]=map[count]->dir;
			second_row[j]=map[count+values->c]->dir;
			row_op[j]='k';
		}
		for(k=0; k<ROW_IN_GRAPH; k++){
			printf("\nS3:");
			/* for all lines except the last */
			for(j=0;j<values->c;j++){
				row='a'+i;
				/* for every number row */
				if(k==0){
					count=i*values->c+j;
					if(j==0)
						printf(" %c | ",row);
					/* check if < or > is needed, if not, fill it with space */
					if(j<values->c){
						printf("%4d",map[count]->low_dist);
						if(first_row[j]=='r')
							printf(" <<<<");
						else if(first_row[j+1]=='l')
							printf(" >>>>");
						else
							printf("     ");
					/* check if ^ or v is needed in the following two rows */
						if(first_row[j]=='d')
							row_op[j]='u';
						else if(second_row[j]=='u')
							row_op[j]='d';
					}
				}
				/* for the two lines between number rows, only print ^ and v or
				   blank space based on the previouse row */
				else{
					if(j==0)
						printf("   |");
					if(row_op[j]=='u')
						printf("    ^");
					else if(row_op[j]=='d')
						printf("    v");
					else
						printf("     ");
					if(j<values->c-1)
						printf("    ");
				}
			}
		}
	}
	/* prints the final line of stage 3 */
	printf("\nS3:");
	for(j=0;j<values->c;j++){
		row='a'+values->r-1;
		count=(values->r-1)*values->c+j;
		if(j==0)
			printf(" %c | ",row);
		if(second_row[j]=='r')
			printf("%4d <<<<",map[count]->low_dist);
		else if(second_row[j+1]=='l')
			printf("%4d >>>>",map[count]->low_dist);
		else if(second_row[j]=='d')
			printf("%4d     ",map[count]->low_dist);
		else
			printf("%4d     ",map[count]->low_dist);
	}
	printf("\n"); //fitted for formatting
	
	/* This section frees all the stored malloc data */
	for(i = 0; i < inter; i++){
		free(map[i]);
		map[i] = NULL;
	}
	free(map);
	map = NULL;
	for(i = 0; i < no_cars; i++){
		free(cars[i]);
		cars[i] = NULL;
	}
	free(cars);
	cars = NULL;
	free(values);
	values = NULL;
	free(check_next);
	check_next = NULL;
	
	return 0;
}


/* this function is used to count the number of blocked pathes by checking the
   distance to all sides of a given intersection */
void check_paths(path_t *p, all_t *values){
	if(p -> E == BLOCKED)
		values -> imposs_paths++;
	else
		values -> total_time = values -> total_time + p -> E;
	if(p -> N == BLOCKED)
		values -> imposs_paths++;
	else
		values -> total_time = values -> total_time + p -> N;
	if(p -> W == BLOCKED)
		values -> imposs_paths++;
	else
		values -> total_time = values -> total_time + p -> W;
	if(p -> S == BLOCKED)
		values -> imposs_paths++;
	else
		values -> total_time = values -> total_time + p -> S;
}

/* dedicating dynamic memory for storing the location of cars */
name_t* create_car(char row, int col){
	name_t* new_car=malloc(sizeof(name_t));
	new_car->row =row;
	new_car->col=col;
	return new_car;
}

/* determining the map id of any location on the map */
int loc_on_map(name_t* car_loc, int columns){
	int row2, col2;
	row2 = car_loc->row-'a';
	col2 = car_loc->col;
	return (row2)*columns+(col2);
}

/* checks each grid and mark it as visited, using Dijkstra's algorithm */
void check_grid(name_t* origin, path_t **map, int column,int row){
	int id, id_col, id_left, id_right, id_up, id_down, change_it=FALSE;
	char id_row;
	id = loc_on_map(origin, column);
	map[id]->visited=TRUE;
	id_col=origin->col;
	id_row=origin->row;
	/* this section determines the map id for intersections in all directions
	   and returns FALSE if the road is blocked or if it has been visited*/
	if(id_col==0)
		id_left=FALSE;
	else
		id_left=id-1;
	if(id_col==column-1)
		id_right=FALSE;
	else
		id_right=id+1;
	if(id_row=='a')
		id_up=FALSE;
	else
		id_up=id-column;
	if(id_row=='a'+row-1)
		id_down=FALSE;
	else
		id_down=id+column;

    /* this section determines whether or not to replace the shortest distance
       based on whether or not there is a possible shorter path and is repeated
       four times for each direction */
    /* checking intersection to the west(left) */
	if(id_left != FALSE && map[id_left] -> low_dist > 
		map[id] -> low_dist + map[id] -> W){
		change_it = TRUE;
		}
	else if(id_left != FALSE && map[id_left] -> low_dist ==
		map[id] -> low_dist + map[id] -> W){
		if(map[id_left] -> from.col > map[id] -> grid.col){
			change_it = TRUE;
		}
		else if(map[id_left] -> from.col == map[id] -> grid.col){
			if(map[id_left] -> from.row > map[id] -> grid.row)
				change_it = TRUE;
		}
	}
	if(change_it == TRUE){
		map[id_left] -> low_dist = map[id] -> low_dist + map[id] -> W;
		map[id_left] -> from.col = origin -> col;
		map[id_left] -> from.row = origin -> row;
		map[id_left] -> steps = map[id] -> steps + 1;
		map[id_left] -> dir = 'r';
		change_it = FALSE;
	}
	/* checking intersection to the east(right) */
	if(id_right != FALSE && map[id_right] -> low_dist > 
		map[id] -> low_dist + map[id] -> E){
		change_it = TRUE;
		}
	else if(id_right != FALSE && map[id_right] -> low_dist ==
		map[id] -> low_dist + map[id] -> E){
		if(map[id_right] -> from.col > map[id] -> grid.col){
			change_it = TRUE;
		}
		else if(map[id_right] -> from.col == map[id] -> grid.col){
			if(map[id_right] -> from.row > map[id] -> grid.row)
				change_it = TRUE;
		}
	}
	if(change_it == TRUE){
		map[id_right] -> low_dist = map[id] -> low_dist + map[id] -> E;
		map[id_right] -> from.col = origin -> col;
		map[id_right] -> from.row = origin -> row;
		map[id_right] -> steps = map[id] -> steps + 1;
		map[id_right] -> dir = 'l';
		change_it = FALSE;
	}
	/* checking intersection to the north(up) */
	if(id_up != FALSE && map[id_up] -> low_dist > 
		map[id] -> low_dist + map[id] -> N){
		change_it = TRUE;
		}
	else if(id_up != FALSE && map[id_up] -> low_dist ==
		map[id] -> low_dist + map[id] -> N){
		if(map[id_up] -> from.col > map[id] -> grid.col){
			change_it = TRUE;
		}
		else if(map[id_up] -> from.col == map[id] -> grid.col){
			if(map[id_up] -> from.row > map[id] -> grid.row)
				change_it = TRUE;
		}
	}
	if(change_it == TRUE){
		map[id_up] -> low_dist = map[id] -> low_dist + map[id] -> N;
		map[id_up] -> from.col = origin -> col;
		map[id_up] -> from.row = origin -> row;
		map[id_up] -> steps = map[id] -> steps + 1;
		map[id_up] -> dir = 'd';
		change_it = FALSE;
	}
	/* checking intersection to the south(down) */
	if(id_down != FALSE && map[id_down] -> low_dist > 
		map[id] -> low_dist + map[id] -> S){
		change_it = TRUE;
		}
	else if(id_down != FALSE && map[id_down] -> low_dist ==
		map[id] -> low_dist + map[id] -> S){
		if(map[id_down] -> from.col > map[id] -> grid.col){
			change_it = TRUE;
		}
		else if(map[id_down] -> from.col == map[id] -> grid.col){
			if(map[id_down] -> from.row > map[id] -> grid.row)
				change_it = TRUE;
		}
	}
	if(change_it == TRUE){
		map[id_down] -> low_dist = map[id] -> low_dist + map[id] -> S;
		map[id_down] -> from.col = origin -> col;
		map[id_down] -> from.row = origin -> row;
		map[id_down] -> steps = map[id] -> steps + 1;
		map[id_down] -> dir = 'u';
		change_it = FALSE;
	}
}

/* Algorithms are fun!! */