#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

int GetData(FILE *fi, int *m, vector<double> &X);
double GetForce(int n, vector<double> X, int m, vector<double> Y);

void print(vector<double> &v, int size) {
    for(int i = 0; i < size; i++) {
        printf("%lf ", v[i]);
    }
    printf("\n");
}

int main(void)
{
    int n, m; //numbers of members
    FILE *fi; //data
    vector<double> X, Y;
    fi = fopen("input.txt", "r");
    if(!fi) 
    {
        printf("There is not file\n");
        return -1;
    }
    if(GetData(fi, &n, X) != 0)
    {
        printf("Left force is null\n");
        return -1;
    }
    if(GetData(fi, &m, Y) != 0)
    {
        printf("Right force is null\n");
        return -1;
    }
    if((n == 0) && (m == 0))
    {
        fprintf(stderr, "Wrong data\n");
        return -1;
    }
    printf("n = %d, m = %d\n", n, m);
    print(X, n);
    print(Y, m);
    fclose(fi);
    
    double F1 = GetForce(n, X, m, Y);
    
    printf("the first comand :  %lf\n", F1);
    
    double F2 = GetForce(m, Y, n, X);    
    
    printf("the second comand :  %lf\n", F2);
    return 0;
}

int GetData(FILE *fi, int *m, vector<double> &X)
{
    int n;
    double k;
    if(!fscanf(fi, "%d", &n) || (n < 0))
    {
        fprintf(stderr, "Wrong data\n");
        return -1;
    }
    *m = n;
    for(int i = 0; i < n; i++)
    {
        if(!fscanf(fi, "%lf", &k))
            return -1;
        if(k <= 0)
            return -1;
        X.push_back(k);
    }
    return 0;
}

double GetForce(int n, vector<double> X, int m, vector<double> Y) // calculate for the first comand
{
    double E = 0;
    if(m == 0)
    {
        for(int i = 0; i < n; i++)
            E += X[i];
    }
    else if(n == 0)
        E = 0;
    else
    {
        vector<double> X1, Y1;
        X1 = X; Y1 = Y;
        X1.pop_back();
        Y1.pop_back();
        E = X[n-1]/(X[n-1] + Y[m-1]) * GetForce(n, X, m-1, Y1) + Y[m-1]/(X[n-1] + Y[m-1]) * GetForce(n-1, X1, m, Y);
    }
    return E;
}


