#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int addResult(double seconds){
  FILE *fpo = fopen("results.txt", "r");
    if(fpo == NULL){
      printf("Error opening old result file\n");
      return -1;
    }
    FILE *fpw = fopen("results1.txt", "w");
    if(fpw == NULL){
      printf("Error opening new result file\n");
      return -1;
    }
    char playerName[128];
    char newPlayerName[128];
    double playerTime;
    int notWritten = 1;
    printf("Enter your name to be added to the top ten player list\n");
    scanf("%s", newPlayerName);
    int count = 0;
    // Skip the first line
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), fpo) == NULL) {
        // File is empty
        fclose(fpo);
        return 0;
    }
    fprintf(fpw, "%s", buffer);
    int index;
    while(fscanf(fpo, "%d %s  %lf", &index, playerName, &playerTime) == 3){
      count++;
      if(playerTime > seconds && notWritten == 1){
        printf("You are at place n. %d!\n", count);
        fprintf(fpw,"%d %s  %.2f\n", count, newPlayerName, seconds);
        count++;
        // fprintf(fpw,"%s  %.2f\n", playerName, playerTime);
        notWritten = 0;
      }
      fprintf(fpw,"%d %s  %.2f\n", count, playerName, playerTime);
    }
    fclose(fpw);
    fclose(fpo);
    system("rm results.txt");
    system("mv results1.txt results.txt");
    system("echo ''");
    system("echo ''");
    system("cat results.txt");
    sleep(3);
  return 0;
}
