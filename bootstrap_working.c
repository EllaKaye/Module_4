/*
Function for bootstrap
*/

#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include <math.h>
#include <time.h>

void samp_k_from_n (int k, int n, int * a, gsl_rng *restrict r)
{
  //printf("\nI got further.\n\n");
  int b[n];
  
  
  /* sample k objects from an array of size n with replacement */
	//printf("\n n = %d\n", n);		
  

	//printf("\n\nZ\n\n");
	
  for (int i = 0; i < n; i++) 
    {
      b[i] = i;
    }
    
  //printf("\n\nA\n\n");
  gsl_ran_sample (r, a, k, b, n, sizeof(int));
  //printf("\n\nA2\n\n");
}

void samp_k_from_n_no_replace (int k, int n, int * a, gsl_rng *restrict r)
{
  //printf("\nI got further.\n\n");
  int b[n];
    
  /* sample k objects from an array of size n with replacement */
	//printf("\n n = %d\n", n);		
  
	//printf("\n\nZ\n\n");
	
  for (int i = 0; i < n; i++) 
    {
      b[i] = i;
    }
    
  //printf("\n\nA\n\n");
  gsl_ran_choose (r, a, k, b, n, sizeof(int));
  //printf("\n\nA2\n\n");
}


void bootstrap(double x[], double *result, int B, int n) 
{
  //printf("\nI am now here.\n\n");

  static gsl_rng *restrict r = NULL;
  
  if(r == NULL) { // First call to this function, setup RNG
    gsl_rng_env_setup();
    r = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(r, time(NULL));
  }

	
	double T_boot[B];

	int a[n];
	
	for (int i = 0; i < B; i++) {

		//printf("\n i = %d\n", i);		
		samp_k_from_n(n, n, a, r);
		//printf("B\n\n");
		
		double x_star[n];
		
		for (int j = 0; j < n; j++)
			{
				//printf("a[%d]=%d\n", j, a[j]);
				x_star[j] = x[a[j]];
			}
			//printf("B\n\n");

		T_boot[i] = gsl_stats_mean(x_star, 1, n);
		//printf("\n\n T_[%i] = %f\n\n", i, T_boot[i]);
	}
	
	
	//printf("\n\n I've finished the for loop\n\n");
	//double temp = sqrt(gsl_stats_variance(T_boot, 1, B));
	//printf("\n\ntemp is %f\n\n", temp);	
	
	*result = sqrt(gsl_stats_variance(T_boot, 1, B));
	//printf("\n\nresult is %f\n\n", *result);	
	//printf("\n\nresult is %f\n\n", sqrt(gsl_stats_variance(T_boot, 1, B)));	

}

void BLB_serial(double x[], double *result, float gamma, int s, int r, int n)
// n is length of the data 
{
	int b = floor(pow(n, gamma));
	
	for (int i = 0; i < s; i++)
	{
		// select subsample of the data
		
		// run bootstrap on that subsample
		
		// store the result
	}
	
	// take average of results from the s subsamples	
}

int main(void) {
	//printf("\nI am here.\n\n");
	double data[6] = {9.0, 2.0, 4.0, 8.0, 1.0, 10.0};
	
	double result;
	
	bootstrap(data, &result, 10, 6);
	
	printf("se is %f.\n", result);
	return 0;
}