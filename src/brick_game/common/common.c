#include "common.h"

void read_highscore(int *high_score) {
  FILE *fp = fopen("hiscore.txt", "r+");
  if (!fp) {
    *high_score = 0;
    fp = fopen("hiscore.txt", "a+");
    if (fp) {
      fprintf(fp, "0\n");
    } 
  } else {
    if (fscanf(fp, "%d", high_score) != 1) {
      *high_score = 0;
    }
  }
  if (fp)
    fclose(fp);
}

void write_highscore(int high_score) {
  FILE *fp = fopen("hiscore.txt", "w");
  if (fp) {
    fprintf(fp, "%d\n", high_score);
    fclose(fp);
  }
}
