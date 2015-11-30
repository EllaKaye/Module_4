/*
Function for bootstrap

cd Box\ Sync/My\ Eduction/OxWaSP/Modules/Module\ 4/C\ Code/
compile with gcc -I/usr/local/include -L/usr/local/lib -o BLB_serial BLB_serial.c -std=c11 -lgsl
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


void bootstrap(double x[], double *result, int B, int n, gsl_rng *restrict r) 
{
  //printf("\nI am now here.\n\n");
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

// takes a subsample, x, of size b, and bootstraps things of size n, B iterations
// (B is R in the context of BLB (not to be confused with r for rng))
void bootstrap_b(double x[], double *result, int b, int B, int n, gsl_rng *restrict r) 
{
  //printf("\nI am now here.\n\n");
	double T_boot[B]; // to store result of doing statistic (mean) on each replicate

	//int a[n];
	
	for (int i = 0; i < B; i++) {

		//printf("\n i = %d\n", i);		
		//samp_k_from_n(n, n, a, r);
		//printf("B\n\n");
		
		double x_star[n];
		gsl_ran_sample(r, x_star, n, x, b, sizeof(double));
		
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

void bootstrap_b_multi(double subsamp[], double *result, int b, int B, int n, gsl_rng *restrict r) 
{
  //printf("\nI am now here.\n\n");
	double theta_star[B]; // to store result of doing statistic (mean) on each replicate

	for (int i = 0; i < B; i++) {
	
		// indices for resample
		unsigned int w[b]; // where the weights (# of each index) will be stored
		double p[b]; //probs for multinomial resampling, then set equal
		for (int j = 0; j < b; j++) {
  			p[j] = 1;
  		}
		gsl_ran_multinomial(r, b, n, p, w);
		
		double v[b];
		for (int j = 0; j < b; j++) {
			v[j] = (double) w[j];
			
			//printf("W[%i] = %i\n", j, w[j]);
			//printf("V[%i] = %f\n", j, v[j]);
			//printf("subsamp[%i] = %f\n", j, subsamp[j]);
		}
		//printf("\n);

		// calculate theta_star
		theta_star[i] = gsl_stats_wmean(v, 1, subsamp, 1, b);
		printf("%f\n", theta_star[i]);
		//printf("\n\n T_[%i] = %f\n\n", i, T_boot[i]);
	}
	
	
	//printf("\n\n I've finished the for loop\n\n");
	//double temp = sqrt(gsl_stats_variance(T_boot, 1, B));
	//printf("\n\ntemp is %f\n\n", temp);	
	
	*result = sqrt(gsl_stats_variance(theta_star, 1, B));
	//printf("\n\nresult is %f\n\n", *result);	
	//printf("\n\nresult is %f\n\n", sqrt(gsl_stats_variance(T_boot, 1, B)));	

}



void BLB_serial(double x[], double *result, float gamma, int s, int R, int n)
// n is length of the data 
{
  static gsl_rng *restrict r = NULL;
  
  if(r == NULL) { // First call to this function, setup RNG
    gsl_rng_env_setup();
    r = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(r, time(NULL));
  }

	int b;
	b = (int) floor(pow(n, gamma));
	
	double xis[s];
	
	for (int i = 0; i < s; i++)
	{
		// select subsample of size b of the data
		double subsamp[b];
		gsl_ran_choose(r, subsamp, b, x, n, sizeof(double));
				
		// run bootstrap on that subsample (resamples n for each replicate)
		bootstrap_b(subsamp, &xis[i], b, R, n, r);
	}
	
	// take average of results from the s subsamples
	*result = gsl_stats_mean(xis, 1, s);	
}

void BLB_serial_multi(double x[], double *result, float gamma, int s, int R, int n)
// n is length of the data 
{
  static gsl_rng *restrict r = NULL;
  
  if(r == NULL) { // First call to this function, setup RNG
    gsl_rng_env_setup();
    r = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(r, time(NULL));
  }

	int b;
	b = (int) floor(pow(n, gamma));
	
	double xis[s];
	
	for (int i = 0; i < s; i++)
	{
		// select subsample of size b of the data
		double subsamp[b];
		gsl_ran_choose(r, subsamp, b, x, n, sizeof(double));
				
		// run bootstrap on that subsample (uses weights)
		bootstrap_b_multi(subsamp, &xis[i], b, R, n, r);
	}
	
	// take average of results from the s subsamples
	*result = gsl_stats_mean(xis, 1, s);	
}


int main(void) {
	//printf("\nI am here.\n\n");
	double data[6] = {3.0, 4.0, 5.0, 7.0, 2.0, 8.0};
	
	double result;
	
	//bootstrap(data, &result, 10, 6);
	
	BLB_serial_multi(data, &result, 0.9, 15, 100, 6);
	
	printf("se is %f.\n", result);
	return 0;
}