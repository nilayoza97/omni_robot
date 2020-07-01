//Omni robot control
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define PI 3.14159
#define BASE_WIDTH 30.0
#define WHEEL_DIAMETER 5
#define PULSES_PER_ROTATION 2000
#define NUM_OF_INPUTS 20
#define TARGET_DISTANCE 30

double pulsesPerCm = PULSES_PER_ROTATION/(PI*WHEEL_DIAMETER);
double pulsesPerDeg = PULSES_PER_ROTATION/360;

struct node
{
    float value;
    int key;
    struct node *next;
};
struct node *(head[2]);

void add_new_end(float value, int i, int key)
{
    struct node *ptr, *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    ptr = head[i];
    temp->value = value;
    temp->key = key;
    temp->next = NULL;
    while(ptr->next != NULL)
    {
        ptr = ptr->next;
    }
    ptr->next = temp;
}

void add_new_start(float value, int i, int key)
{  
    struct node *temp;
    temp = (struct node *)malloc(sizeof(struct node));
    temp->key = head[i]->key;
    temp->value = head[i]->value;
    head[i]->value = value;
    head[i]->key = key;
    temp->next = head[i]->next;
    head[i]->next = temp;
}

void printList(int i)
{
    struct node *temp;
    temp = head[i];

    while(temp->next != NULL)
    {
        temp = temp->next;
        printf("sensor item %d : %f \n",temp->key, temp->value);
    }
}

void main()
{
   head[0] = (struct node *)malloc(sizeof(struct node));
   head[0]->next = NULL;
   head[1] = (struct node *)malloc(sizeof(struct node));
   head[1]->next = NULL;

   float cursor;
   int i =0, t = 0, b = 0;
   FILE *fp1, *fp2;
   struct node *point;
   //printf("headptr : %f, %p",head->value, head->next);
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
         add_new_end(cursor, 0, key);
         key++;
      }
      else
      {
         static int key = 0;
         add_new_end(cursor, 1, key);
         key++;
      }

      fscanf(fp1,",");
      i++;
   }

   fp2 = fopen("output.csv","w+");
   fprintf(fp2,"Angle,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction,M1 ticks,M2 ticks,M3 ticks,M4 ticks,Direction\n");
   
   float angle[100], calc, thetaRad[100], distFromWall[100], travelDist[100];
   float pulseM1[100],pulseM2[100],pulseM3[100],pulseM4[100], m1Pulse[100], m2Pulse[100], m3Pulse[100], m4Pulse[100];
   char *thetaDir[100] , *motDir[100];


   struct node *sensor1,*sensor2;
   sensor1 = head[0];
   sensor2 = head[1];

    while(sensor1->next != NULL && sensor2->next != NULL)
    {
        sensor1 = sensor1->next;
        sensor2 = sensor2->next;

        static int i = 0;
        if(sensor1->value > sensor2->value)
        {
            calc = (sensor1->value - sensor2->value)/BASE_WIDTH;
            *thetaDir = "clockwise";              //angle is counterclockwise, rotate the robot clockwise
        }
        if(sensor1->value < sensor2->value)
        {
            calc = (sensor2->value - sensor1->value)/BASE_WIDTH;
            *thetaDir = "counterclockwise";       //angle is clockwise, rotate the robot counterclockwise
        }
        if(sensor1->value == sensor2->value)
        {
            calc = 0;
            *thetaDir = "-";
        }
        
        thetaRad[i] = (atan(calc));
        angle[i] = (thetaRad[i]*180)/PI;

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
            *motDir = "counterclockwise";
        }
        if(distFromWall[i] > TARGET_DISTANCE)
        {
            travelDist[i] = distFromWall[i] - TARGET_DISTANCE;
            *motDir = "clockwise"; 
        }
        if(distFromWall[i] == TARGET_DISTANCE)
        {
            travelDist[i] = 0;
            *motDir = "-";
        }

        m1Pulse[i] = 0;
        m2Pulse[i] = 0;
        m3Pulse[i] = travelDist[i]*pulsesPerCm;
        m4Pulse[i] = travelDist[i]*pulsesPerCm;

        fprintf(fp2,"%f,%f,%f,%f,%f,%s,%f,%f,%f,%f,%s\n", angle[i], pulseM1[i], pulseM2[i], pulseM3[i], pulseM4[i], *thetaDir, m1Pulse[i], m2Pulse[i], m3Pulse[i], m4Pulse[i], *motDir); 
        i++;
    }
   
   fclose(fp1);
   fclose(fp2);

}
