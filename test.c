
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
    char ch;
    char* d,a;
    FILE *fp;
    fp = fopen("input - Sheet1.csv", "r");
    if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
 while((ch = fgetc(fp)) != EOF){
      printf("%c", ch);
 }
    
    fclose(fp);
   return 0;

}