#include <stdio.h
#include <stdio.h>
#include <math.h>

// this is midly wrong as the float representation of PI is too big for use to save it, 
// which means we need to use the math M_PI as that contains a more precise representation of pi
const float PI = 3.1415;
// this is more precise
// const float PI = M_PI;


int main(int argc,char **argv) {
    printf("Specify the radius of the sphere: ");
    int radius = 0;
    scanf("%d", &radius);
    float volume = (4.0/3.0) * PI * pow(radius, 3);
    printf("the volueme of the sphere is %f \n", volume);
}


