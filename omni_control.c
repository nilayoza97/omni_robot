//Omni robot control
#include<stdio.h>
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
   float sensor1[100], sensor2[100], d;
   int i =0, t = 0, b = 0;
   FILE *fp, *pf;
   
   fp = fopen("input - Sheet1.csv", "r");
   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

   while(fscanf(fp,"%f",&d) != EOF)
   {
      if (i%2 == 0 || i == 0)
      {
         sensor1[t] = d;
         t++;
      }
      else
      {
         sensor2[b] = d;
         b++;
      }

      fscanf(fp,",");
      i++;
   }
   
   pf = fopen("output.csv","w+");
   fprintf(pf,"Angle,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction\n");
   
   float angle[100], calc, thetaRad[100], distFromWall[100], travelDist[100];
   float pulseM1[100],pulseM2[100],pulseM3[100],pulseM4[100], m1Pulse[100], m2Pulse[100], m3Pulse[100], m4Pulse[100];
   char *thetaDir[100] , *motDir[100];

   for (i = 0; i<numOfInputs; i++)
   {  
      
      //angle of the robot with the wall
      if(sensor1[i] > sensor2[i]) 
      {
         calc = (sensor1[i] - sensor2[i])/baseWidth;
         *thetaDir = "clockwise";              //angle is counterclockwise, rotate the robot clockwise
      }
      if(sensor1[i] < sensor2[i]) 
      {
         calc = (sensor2[i] - sensor1[i])/baseWidth;
         *thetaDir = "counterclockwise";       //angle is clockwise, rotate the robot counterclockwise
      }
      if(sensor1[i] == sensor2[i])
      {
         calc = 0;
         *thetaDir = "-";                 
      }
            
      thetaRad[i] = (atan(calc));
      angle[i] = (thetaRad[i]*180)/pi;      

      //make the robot parallel to the wall

      pulseM1[i] = (baseWidth*0.5)*thetaRad[i]*pulsesPerCm;    // used s=r*theta
      pulseM2[i] = (baseWidth*0.5)*thetaRad[i]*pulsesPerCm;
      pulseM3[i] = (baseWidth*0.5)*thetaRad[i]*pulsesPerCm;
      pulseM4[i] = (baseWidth*0.5)*thetaRad[i]*pulsesPerCm; 

      //bring it 30cm away from the wall

      if(sensor1[i] > sensor2[i])
      {
         distFromWall[i] = baseWidth*0.5*(sin(thetaRad[i])+cos(thetaRad[i])-1) + sensor2[i]*cos(thetaRad[i]);
      }
      if(sensor1[i] < sensor2[i])
      {
         distFromWall[i] = baseWidth*0.5*(sin(thetaRad[i])+cos(thetaRad[i])-1) + sensor1[i]*cos(thetaRad[i]);
      }
      if(sensor1[i] == sensor2[i])
      {
         distFromWall[i] = sensor1[i];
      } 


      if(distFromWall[i] < targetDist)
      {
         travelDist[i] = targetDist - distFromWall[i];
         *motDir = "counterclockwise";
      }
      if(distFromWall[i] > targetDist)
      {
         travelDist[i] = distFromWall[i] - targetDist;
         *motDir = "clockwise"; 
      }
      if(distFromWall[i] == targetDist)
      {
         travelDist[i] = 0;
         *motDir = "-";
      }
      
      m1Pulse[i] = 0;
      m2Pulse[i] = 0;
      m3Pulse[i] = travelDist[i]*pulsesPerCm;
      m4Pulse[i] = travelDist[i]*pulsesPerCm;

      fprintf(pf,"%f,%f,%f,%f,%f,%s,%f,%f,%f,%f,%s\n", angle[i], pulseM1[i], pulseM2[i], pulseM3[i], pulseM4[i], *thetaDir, m1Pulse[i], m2Pulse[i], m3Pulse[i], m4Pulse[i], *motDir);
   }

   fclose(fp);
   fclose(pf);
}
