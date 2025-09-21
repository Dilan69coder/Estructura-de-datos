#include <stdio.h>
int main(){
    float a,b,suma,resta,multiplicacion,division;
    int operacion, contador=0;
    printf("Ingrese el primer numero:  ");
    scanf("%f",&a);
     printf("Ingrese el segundo numero:  ");
    scanf("%f",&b);

    suma = a+b;
    resta = a-b;
    multiplicacion = a*b;
    division = a/b;

    printf("Ingrese la operacion a realizar (1=suma, 2=resta, 3=multiplicacion, 4=division): ");
    scanf("%i", &operacion);
    if (operacion>=1 && operacion<=4 ){
        while (contador<=3){
            switch (operacion){
                case 1:
                    printf("El resultado de la suma es: %f", suma);
                    break;
                case 2:
                    printf("El resultado de la resta es: %f", resta);
                    break;
                case 3:
                    printf("El resultado de la multiplicacion es: %f", multiplicacion);
                    break;
                case 4:
                    printf("El resultado de la division es: %f", division);
                    break;
    }
    contador++;

} 
}
}