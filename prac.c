#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define pi 3.14159
#define baseWidth 30.0
#define wheelDia 5
#define pulsesPerRotation 2000

double pulsesPerCm = pulsesPerRotation/(pi*wheelDia);
double pulsesPerDeg = pulsesPerRotation/360;

void main()
{
   double data[40] = {30, 10, 20, 5, 18, 24};
   double angle[40], calc,thetaRad[40];
   int a = 6,i,j = 1,k = 0;
   for (i = 0; i<a; i++)
   {  
      
      //angle of the robot with the wall
      calc = (data[i] - data[j])/baseWidth;
      thetaRad[k] = (atan(calc));
      angle[k] = (thetaRad[k]*180)/pi;
      printf("angle[%i] with wall in degrees : %f\n",k+1,angle[k]);
     
      

      //make the robot parallel to the wall
      double pulseM1[20],pulseM2[20],pulseM3[20],pulseM4[20];

      pulseM1[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm;    // used s=r*theta
      pulseM2[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm;
      pulseM3[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm;
      pulseM4[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm;

      //bring it 30cm away from the wall
      

      j++;
      j++;
      i++;
      k = k+1;
   }
}
