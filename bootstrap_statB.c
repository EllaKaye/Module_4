/*
 Function for bootstrap giving an option for the statistic to calulate - working
 //THE FUNCTIONS FOR THE STATISTICS ARE VOID
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




//here define different statistics e.g. mean

void mean_vec( double a[], int b, double *result){
    double m = 0;
    for (int i = 0; i < b; i++){
        m += a[i];
    }
    m = m / b;
    
    *result = m;
}

void sd_vec( double a[], int b, double *result){
    double sd = 0;
    double m = 0;
    mean_vec(a,b,&m);
    for (int i =0; i <b; i++){
        sd += (a[i] - m) * (a[i] - m);
    }
    sd = sqrt( sd / (b-1) );
    
    *result = sd;
}




void bootstrap(double x[], double *result, int B, int n, int stat_num) {
    //printf("\nI am now here.\n\n");
    
    //create a pointer to save the function for the statistic
    void (*stat_fun) (double a[], int b, double *result);
    
    //this will store the value of the statistic of interest
    //double stat = 0 ;
    
    // the following is for random number generation
    static gsl_rng *restrict r = NULL;
    
    if(r == NULL) { // First call to this function, setup RNG
        gsl_rng_env_setup();
        r = gsl_rng_alloc(gsl_rng_mt19937);
        gsl_rng_set(r, time(NULL));
    }
    
    // T_boot stores the values of the statistic of interest for every iteration
    double T_boot[ B ];
    
    // a is the pointer that stores the indices
    int a[ n ];
    
    // check which statistic i need to calculate
    switch(stat_num){
        case 1: stat_fun = mean_vec;
            break;
            
        case 2: stat_fun = sd_vec;
            break;
        default :
            /* Optional */
            stat_fun = mean_vec;
            
    }
    
    //create B bootstrap samples and compute B statistics
    for (int i = 0; i < B; i++) {
        
        //printf("\n i = %d\n", i);
        
        //resample the given data vector a
        samp_k_from_n(n, n, a, r);
        //printf("B\n\n");
        
        // create a pointer for the new data vector
        double x_star[ n ];
        
        //fill in x_sta with the resampled datapoints
        for (int j = 0; j < n; j++) {
            //printf("a[%d]=%d\n", j, a[j]);
            x_star[j] = x[a[j]];
        }
        
        //printf("B\n\n");
        
        //compute the statistic of interest
        
        stat_fun(x_star, n, &T_boot[i]);
        
        
        printf("\n\n T_[%i] = %f\n\n", i, T_boot[i]);
    }
    
    
    printf("\n\n I've finished the for loop\n\n");
    //double temp = sqrt(gsl_stats_variance(T_boot, 1, B));
    //printf("\n\ntemp is %f\n\n", temp);
    
    sd_vec(T_boot, B, result);
    //*result = sqrt(gsl_stats_variance(T_boot, 1, B));
    printf("\n\nresult is %f\n\n", *result);
    //printf("\n\nresult is %f\n\n", sqrt(gsl_stats_variance(T_boot, 1, B)));	
    
}



///////////////////////////////////////////////
int main(void) {
    //printf("\nI am here.\n\n");
    double data[6] = {9.0, 2.0, 4.0, 8.0, 1.0, 10.0};
    
    double result = 0;
    
    bootstrap(data, &result, 10, 6, 1);
    
    printf(" se is %f.\n", result);
    return 0;
}


/*
 
 double mat[10*5];
 mat[ j + i*n] 
 //5 is the number of columns
 mat[2 + 10*3]
 */