//Omni robot control
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define pi 3.14159
#define baseWidth 30.0
#define wheelDia 5
#define pulsesPerRotation 2000
#define numOfInputs 20
#define targetDist 30

double pulsesPerCm = pulsesPerRotation/(pi*wheelDia);
double pulsesPerDeg = pulsesPerRotation/360;

void main()
{
   float tmp[100], s1[100], s2[100], d;
    int h=0, t = 0, b = 0, l = 0;
    FILE *fp, *pf;
    fp = fopen("input - Sheet1.csv", "r");
    if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
   while(fscanf(fp,"%f",&d) != EOF)
   {
      tmp[h] = d;
      if (h%2 == 0 || h == 0)
      {
         s1[t] = tmp[h];
         t++;
      }
      else
      {
         s2[b] = tmp[h];
         b++;
      }
      fscanf(fp,",");
      h++;
   }
   
   pf = fopen("output.csv","w+");
   fprintf(pf,"Angle,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction\n");
   float angle[100], calc, thetaRad[100], distFromWall[100], travelDist[100];
   float pulseM1[100],pulseM2[100],pulseM3[100],pulseM4[100], m1Pulse[100], m2Pulse[100], m3Pulse[100], m4Pulse[100];

   int a = numOfInputs,i,k = 0;
   char *thetaDir[100] , *motDir[100];
   for (i = 0; i<a; i++)
   {  
      
      //angle of the robot with the wall
      if(s1[i] > s2[i]) 
      {
         calc = (s1[i] - s2[i])/baseWidth;
         *thetaDir = "clockwise";              //angle is counterclockwise, rotate the robot clockwise
      }
      if(s1[i] < s2[i]) 
      {
         calc = (s2[i] - s1[i])/baseWidth;
         *thetaDir = "counterclockwise";       //angle is clockwise, rotate the robot counterclockwise
      }
      if(s1[i] == s2[i])
      {
         calc = 0;
         *thetaDir = "-";                 
      }
            
      thetaRad[k] = (atan(calc));
      angle[k] = (thetaRad[k]*180)/pi;      

      //make the robot parallel to the wall

      pulseM1[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm;    // used s=r*theta
      pulseM2[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm;
      pulseM3[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm;
      pulseM4[k] = (baseWidth*0.5)*thetaRad[k]*pulsesPerCm; 

      //bring it 30cm away from the wall
      if(s1[i] - s2[i] > 0)
      {
         distFromWall[k] = (((s1[i]*cos(angle[k]))- (s2[i]*cos(angle[k])))/2) + (s2[i]*cos(angle[k]));
      }
      if(s1[i] - s2[i] < 0)
      {
         distFromWall[k] = (((s2[i]*cos(angle[k])) - (s1[i]*cos(angle[k])))/2) + (s1[i]*cos(angle[k]));
      }
      if(distFromWall[k] < targetDist)
      {
         travelDist[k] = targetDist - distFromWall[k];
         *motDir = "counterclockwise";
      }
      if(distFromWall[k] > targetDist)
      {
         travelDist[k] = distFromWall[k] - targetDist;
         *motDir = "clockwise"; 
      }
      if(distFromWall[k] == targetDist)
      {
         *motDir = "-";
      }
      
      m1Pulse[k] = 0;
      m2Pulse[k] = 0;
      m3Pulse[k] = travelDist[k]*pulsesPerCm;
      m4Pulse[k] = travelDist[k]*pulsesPerCm;

      fprintf(pf,"%f,%f,%f,%f,%f,%s,%f,%f,%f,%f,%s\n", angle[k], pulseM1[k], pulseM2[k], pulseM3[k], pulseM4[k], *thetaDir, m1Pulse[k], m2Pulse[k], m3Pulse[k], m4Pulse[k], *motDir);

      k++;
   }

   fclose(fp);
   fclose(pf);
}
