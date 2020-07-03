//Omni robot control
//Nilay Oza

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"omni_control.h"

double pulsesPerCm = PULSES_PER_ROTATION/(PI*WHEEL_DIAMETER);
double pulsesPerDeg = PULSES_PER_ROTATION/360;

void main()
{
    initializeAll();

    float cursor;
    int i = 0;
    FILE *fp1, *fp2;

    fp1 = fopen("IO/input - Sheet1.csv", "r");
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
    fclose(fp1);

    fp2 = fopen("IO/output.csv","w+");
    if (fp2 == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(fp2,"Angle,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction\n");

    float thetaRad, distFromWall, travelDist;
    float pulseM1, pulseM2, pulseM3, pulseM4, m1Pulse, m2Pulse, m3Pulse, m4Pulse;
    char *thetaDir, *motorDir;
    struct node *sensor1,*sensor2;

    sensor1 = head[SENSOR1];
    sensor2 = head[SENSOR2];

    while(sensor1->next != NULL && sensor2->next != NULL)
    {
        sensor1 = sensor1->next;
        sensor2 = sensor2->next;

        if(sensor1->value > sensor2->value)
        {
            thetaRad = atan((sensor1->value - sensor2->value)/BASE_WIDTH);
            thetaDir = "clockwise";                                                                             //angle is counterclockwise, rotate the robot clockwise
        }
        else if(sensor1->value < sensor2->value)
        {
            thetaRad = atan((sensor2->value - sensor1->value)/BASE_WIDTH);
            thetaDir = "counterclockwise";                                                                      //angle is clockwise, rotate the robot counterclockwise
        }
        else
        {
            thetaRad = atan(0);
            thetaDir = "-";
        }

        //make the robot parallel to the wall
        pulseM1 = pulseM2 = pulseM3 = pulseM4 = (BASE_WIDTH*0.5)*thetaRad*pulsesPerCm;                          // used s=r*theta

        if(sensor1->value > sensor2->value)
        {
            distFromWall = BASE_WIDTH*0.5*(sin(thetaRad)+cos(thetaRad)-1) + sensor2->value*cos(thetaRad);       //b/2*sin(theta) - (b/2 - b/2*cos(theta)) + s2*cos(theta)
        }

        else if(sensor1->value < sensor2->value)
        {
            distFromWall = BASE_WIDTH*0.5*(sin(thetaRad)+cos(thetaRad)-1) + sensor1->value*cos(thetaRad);       //b/2*sin(theta) - (b/2 - b/2*cos(theta)) + s1*cos(theta)
        }
        else
        {
            distFromWall = sensor1->value;
        }
        
        if(distFromWall < TARGET_DISTANCE)
        {
            travelDist = TARGET_DISTANCE - distFromWall;
            motorDir = "counterclockwise";
        }
        else if(distFromWall > TARGET_DISTANCE)
        {
            travelDist = distFromWall - TARGET_DISTANCE;
            motorDir = "clockwise"; 
        }
        else
        {
            travelDist = 0;
            motorDir = "-";
        }

        //translate robot to 30cm distance from the wall
        m1Pulse = m2Pulse = 0;
        m3Pulse = m4Pulse = travelDist*pulsesPerCm;

        fprintf(fp2,"%f,%f,%f,%f,%f,%s,%f,%f,%f,%f,%s\n", thetaRad*180/PI, pulseM1, pulseM2, pulseM3, pulseM4, thetaDir, m1Pulse, m2Pulse, m3Pulse, m4Pulse, motorDir); 
    }
   
    fclose(fp2);
}
