#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <time.h>

long int MAX_MEMORY = 1024*1024*1024;
//int MAX_ENTRIES_IN_CELL_ARRAY = sqrt(2*MAX_ENTRIES_IN_DISTANCE_ARRAY) / 12;

double MAX_DISTANCE;
double NBR_DISTANCE_POINTS;



short unsigned int * distances;

short int ** cells;
short int * cells_entries;

int nbr_threads;
unsigned long int nbr_cells = 0;

//cell file
FILE *fp;

//block size for calculations

//FUNCTIONS
//generate_random_cells(int nbr_of_cells);


int main(int argc, char * argv[]){

  //Parse command line arguments
  memmove(argv[1], argv[1]+2, strlen(argv[1])); //Removing -t
  sscanf(argv[1], "%d", &nbr_threads);

  MAX_DISTANCE = sqrt(3*20*20); //Max distance in coordinate system bounded by -10 to 10
  NBR_DISTANCE_POINTS = MAX_DISTANCE/0.01; //2 decimal precision
  //create array of distances
  distances = (short unsigned int*) malloc(sizeof(short unsigned int)*NBR_DISTANCE_POINTS);
  for(int ix=0; ix<NBR_DISTANCE_POINTS; ix++){
    distances[ix] = (short unsigned int) 0; //index related to distance: ix*10
  }

  /*
  //Generate file with cells
  fp = fopen("cells", "w");
  int number_of_cells = 10000;
  srand(time(NULL));
  for(int ix=0; ix<number_of_cells; ix++){
    double r1 = (double) rand()/RAND_MAX*20.0-10.0;
    double r2 = (double) rand()/RAND_MAX*20.0-10.0;
    double r3 = (double) rand()/RAND_MAX*20.0-10.0;
    fprintf(fp, "%+07.03lf %+07.03lf %+07.03lf\n", r1, r2, r3);
  }
  fclose(fp);
  */

  fp = fopen("cells", "r");
  //count lines
  fseek(fp, 0, SEEK_END);
  int size_of_file = ftell(fp);
  int bytes_per_line = 24;
  double nbr_cells = size_of_file / bytes_per_line;  

  //parse cells, TODO later: blocks
  cells_entries = (short int *) malloc(sizeof(short int)*3*nbr_cells);
  cells = (short int **) malloc(sizeof(short int *)*nbr_cells);
  for(int ix=0, jx=0; ix<nbr_cells; ix++, jx+=3){
    cells[ix] = cells_entries + jx;
  }

  fseek(fp,0,SEEK_SET);
  for(int ix=0; ix<nbr_cells; ix++){
    double x;
    double y;
    double z;
    fscanf(fp, "%lf %lf %lf", &x, &y, &z);
    x = x*1000;
    y = y*1000;
    z = z*1000;
    cells[ix][0] = (short int) x;
    cells[ix][1] = (short int) y;
    cells[ix][2] = (short int) z;
    }
    
  fclose(fp);

  
  //Calculate distances and add to distance vector
  for(int ix=0; ix<nbr_cells; ix++){
    int ax = cells[ix][0];
    int ay = cells[ix][1];
    int az = cells[ix][2];
    for(int jx=ix+1; jx<nbr_cells; jx++){
      int dist;
      int dx = ax-cells[jx][0];
      int dy = ay-cells[jx][1];
      int dz = az-cells[jx][2];
      dist = dx*dx + dy*dy + dz*dz;
      dist = (int) sqrt(dist)*0.1 + 0.5;
      distances[dist]++;
    }
  }
  
  for(size_t ix=0; ix<NBR_DISTANCE_POINTS; ix++){
    //printf("%d %d\n", ix, distances[ix]);
    if(distances[ix] != 0){
      double dist = (double) ix/100;
      //printf("%05.02lf %d\n", dist, distances[ix]);
    }
  }

  //free(distances);
  //free(cells_entries);
  
}
