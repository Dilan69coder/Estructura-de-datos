#include <stdio.h>
int factorial(int n);
int contador = 5;
int main(){
int x, n;
printf("INgrese el factorial a calcular ");
scanf("%d", &n);
x = factorial(n);
printf ("El factorial es %d", x);
}
int factorial(int n){
    while (contador >= 1){
        if (n==0){
            return 1;
        }
    return n*factorial(n-1);
        contador--;
}
}