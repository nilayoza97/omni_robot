//Omni robot control
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"omni_control.h"

void main()
{
   head[SENSOR1] = (struct node *)malloc(sizeof(struct node));
   head[SENSOR1]->next = NULL;
   head[SENSOR2] = (struct node *)malloc(sizeof(struct node));
   head[SENSOR2]->next = NULL;

   float cursor;
   int i =0, t = 0, b = 0;
   FILE *fp1, *fp2;

   fp1 = fopen("input - Sheet1.csv", "r");
   if (fp1 == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

   while(fscanf(fp1,"%f",&cursor) != EOF)
   {
      if (i%2 == 0 || i == 0)
      {
         static int key = 0;
         add_new_end(cursor, SENSOR1, key);
         key++;
      }
      else
      {
         static int key = 0;
         add_new_end(cursor, SENSOR2, key);
         key++;
      }

      fscanf(fp1,",");
      i++;
   }

   fp2 = fopen("output.csv","w+");
   fprintf(fp2,"Angle,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction\n");
   
   float thetaDeg[100], tanTheta, thetaRad[100], distFromWall[100], travelDist[100];
   float pulseM1[100],pulseM2[100],pulseM3[100],pulseM4[100], m1Pulse[100], m2Pulse[100], m3Pulse[100], m4Pulse[100];
   char *thetaDir, *motorDir;


   struct node *sensor1,*sensor2;
   sensor1 = head[SENSOR1];
   sensor2 = head[SENSOR2];

    while(sensor1->next != NULL && sensor2->next != NULL)
    {
        sensor1 = sensor1->next;
        sensor2 = sensor2->next;

        static int i = 0;
        if(sensor1->value > sensor2->value)
        {
            tanTheta = (sensor1->value - sensor2->value)/BASE_WIDTH;
            thetaDir = "clockwise";              //angle is counterclockwise, rotate the robot clockwise
        }
        if(sensor1->value < sensor2->value)
        {
            tanTheta = (sensor2->value - sensor1->value)/BASE_WIDTH;
            thetaDir = "counterclockwise";       //angle is clockwise, rotate the robot counterclockwise
        }
        if(sensor1->value == sensor2->value)
        {
            tanTheta = 0;
            thetaDir = "-";
        }
        
        thetaRad[i] = (atan(tanTheta));
        thetaDeg[i] = (thetaRad[i]*180)/PI;

        //make the robot parallel to the wall

        pulseM1[i] = (BASE_WIDTH*0.5)*thetaRad[i]*pulsesPerCm;    // used s=r*theta
        pulseM2[i] = (BASE_WIDTH*0.5)*thetaRad[i]*pulsesPerCm;
        pulseM3[i] = (BASE_WIDTH*0.5)*thetaRad[i]*pulsesPerCm;
        pulseM4[i] = (BASE_WIDTH*0.5)*thetaRad[i]*pulsesPerCm;
        
        if(sensor1->value > sensor2->value)
        {
            distFromWall[i] = BASE_WIDTH*0.5*(sin(thetaRad[i])+cos(thetaRad[i])-1) + sensor2->value*cos(thetaRad[i]);  //b/2*sin(theta) - (b/2 - b/2*cos(theta)) + s2*cos(theta)
        }

        if(sensor1->value < sensor2->value)
        {
            distFromWall[i] = BASE_WIDTH*0.5*(sin(thetaRad[i])+cos(thetaRad[i])-1) + sensor1->value*cos(thetaRad[i]);  //b/2*sin(theta) - (b/2 - b/2*cos(theta)) + s1*cos(theta)
        }
        if(sensor1->value == sensor2->value)
        {
            distFromWall[i] = sensor1->value;
        }
        
        if(distFromWall[i] < TARGET_DISTANCE)
        {
            travelDist[i] = TARGET_DISTANCE - distFromWall[i];
            motorDir = "counterclockwise";
        }
        if(distFromWall[i] > TARGET_DISTANCE)
        {
            travelDist[i] = distFromWall[i] - TARGET_DISTANCE;
            motorDir = "clockwise"; 
        }
        if(distFromWall[i] == TARGET_DISTANCE)
        {
            travelDist[i] = 0;
            motorDir = "-";
        }

        m1Pulse[i] = 0;
        m2Pulse[i] = 0;
        m3Pulse[i] = travelDist[i]*pulsesPerCm;
        m4Pulse[i] = travelDist[i]*pulsesPerCm;

        fprintf(fp2,"%f,%f,%f,%f,%f,%s,%f,%f,%f,%f,%s\n", thetaDeg[i], pulseM1[i], pulseM2[i], pulseM3[i], pulseM4[i], thetaDir, m1Pulse[i], m2Pulse[i], m3Pulse[i], m4Pulse[i], motorDir); 
        i++;
    }
   
   fclose(fp1);
   fclose(fp2);

}
