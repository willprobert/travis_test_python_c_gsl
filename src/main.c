/* main.c

Generate graph according to Watts Strogatz model.

Usage
-----
./watts_strogatz.exe <N> <k> <p_rewire>

Arguments
---------
N : Number of nodes
k : mean degree of nodes
p_rewire : probability of rewiring

Returns
-------
Lines with first entry being node id and subsequent 
columns having ids of nodes to which there is a edge.  

W. Probert, 2020
*/

#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>

// Needs to be a global
gsl_rng * rng;

int check_member_or_self(int , int, int *, int );

int main(int argc, char *argv[])
{
	
	// Parameters
	int N = (int) strtol(argv[1], NULL, 10);; // number of vertices
	int k = (int) strtol(argv[2], NULL, 10);; // edges per vertex (mean degree per node)
	double p_rewire; // probability of rewiring each edge
	sscanf(argv[3],"%lf",&p_rewire);
	
	// Set random seed based upon current time
	struct timespec  tv;
    clock_gettime( CLOCK_REALTIME,&tv);
    long SEED = tv.tv_sec + tv.tv_nsec;
	
	// Set-up random number generation infrastructure
    gsl_rng_env_setup();
    rng = gsl_rng_alloc ( gsl_rng_default);
	gsl_rng_set( rng, SEED );
	
	int i, j, l;
	
	// Allocate memory (needed for large N)
    int** adjacency_matrix = malloc(N * sizeof(int*));
    for(i = 0; i < N; i++)
        adjacency_matrix[i] = malloc(k* sizeof(int));
	
	// Set negative values in adjacency matrix (to make sure it's working)
	for(i = 0; i < N; i++){
		for(j = 0; j < k; j++){
			adjacency_matrix[i][j] = -1;
		}
	}
	
	// Step 1
	// Create ring lattice: form connections with k/2 next nodes (either side; not including self)
	int incr = k/2, neighbour;
	for(i = 0; i < N; i++){
		j = 0; l = 0;
		while(l < k)
		{
			// Make sure we loop to the start of the ring
			neighbour = (i - incr + j + N) % N;
			
			if(neighbour != i){
				adjacency_matrix[i][l] = neighbour;
				l++;
			}
			j++;
		}
	}
	
	// Step 2
	// Create random re-wirings with probability p_rewire
	// for each node, determine number of rewirings using binomial distribution
	int ii, num_rewirings, rewiring_idx[k], new_neighbour;
	
	for(i = 0; i < N; i++){
		
		 num_rewirings = gsl_ran_binomial(rng, p_rewire, k);
		 
		 if(num_rewirings > 0){
			 
			 // Shuffle order of adjacent indices to rewire
			 // (determine which of the adjacent indices need to be rewired)
			 // (does this need to be done for each i?  Can't we copy or point to
			 // and unshuffled array?)
			 for (ii = 0; ii < k; ii++){ rewiring_idx[ii] = ii; }
			 
			 gsl_ran_shuffle(rng, rewiring_idx, k, sizeof(int) );
			 
			 // Determine rewirings
			 for(j = 0; j < num_rewirings; j++){
				 
				new_neighbour = i;
				// Check new_neighbour is not already a connection
				// or is not i.  
				while(check_member_or_self(new_neighbour, i, rewiring_idx, k))
					 new_neighbour = gsl_rng_uniform_int(rng, N);
				
				adjacency_matrix[i][rewiring_idx[j]] = new_neighbour;
				// need to break rewiring_idx[j] AND i also ... 
			 }
		 }
	}
	
	// Print output to console for reading into networkx in Python
	// use networkx.read_adjlist() on console output piped to file.
	for(i = 0; i < N; i++){
		printf("%d ", i);
		for(j = 0; j < k; j++){
				printf("%d ", adjacency_matrix[i][j]);
		}
		printf("\n");
	}
	
	gsl_rng_free ( rng );
	
	return 0;
}


/*****************************************************************************************
*  Name:		check_member_or_self
*  Description: Check if x is 'self' or a member of the 'array' (of length 'length')
******************************************************************************************/

int check_member_or_self(int x, int self, int *array, int length)
{
	if(x == self){
		return 1;
	}
	
    int i;
    for(i = 0; i < length; i++)
    {
        if(array[i] == x)
            return 1;
    }
    return 0;
}
