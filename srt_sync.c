/*
 * sync the time lines in a srt file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int ac, char* av[])
{
  FILE* fin;
  FILE* fout;
  char* linebuf = NULL;
  size_t linebuf_size = 0;
  int nread;

  /* line number */
  int line_num;
  char line_num_str[6];

  /* converts to time */
  int hh, mm, ss;
  struct tm when = {0};
  time_t converted_time;

  char* converted_time_str;
  char start_time_str[9];
  char start_mss_str[4];
  char end_time_str[9];
  char end_mss_str[4];
  char new_start_time_str[9];
  char new_end_time_str[9];

  /* temp new timeline string */
  char new_timeline_str[32];

  int shift_ss;
  int shift_num_line;

  /* help */
  if (ac < 5) {
    printf("The .srt file must have an empty line as the last line\n");
    printf("Usage: srt_sync <secs_to_shift> <num_of_lines_shift> <in_file> <out_file>\n");
    return 0;
  }


  /* time shifts in seconds */
  shift_ss = atoi(av[1]);
  /* the number of lines to shift */
  shift_num_line = atoi(av[2]);


  /* open the input file */
  fin = fopen(av[3], "r");
  if (fin == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  /* open the output file*/
  fout = fopen(av[4], "w");
  if (fout == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  do {
    nread = getline(&linebuf, &linebuf_size, fin);
    /* if met EOF */
    if (nread == -1) break;
    /* empty line, skip processing */
    if (strcmp(linebuf, "\n") == 0) {
      fputs(linebuf, fout);
      continue;
    }

    /* the number line */
    line_num = atoi(linebuf);
    line_num += shift_num_line;
    sprintf(line_num_str, "%d\n", line_num);
    fputs(line_num_str, fout);

    /* next read, the time line */
    nread = getline(&linebuf, &linebuf_size, fin);

    /*
     * extract the times
     *
     * ex. 00:02:39,891 --> 00:02:41,592
     *
     *     |-- 8 -|
     *     hh:mm:ss,mss --> hh:mm:ss,mss
     *     ^        ^       ^        ^
     *     0        9       17       26
     */
    strncpy(start_time_str, linebuf, 8);
    start_time_str[8] = '\0';

    strncpy(end_time_str, linebuf + 17, 8);
    end_time_str[8] = '\0';

    strncpy(start_mss_str, linebuf + 9, 3);
    start_mss_str[3] = '\0';

    strncpy(end_mss_str, linebuf + 26, 3);
    end_mss_str[3] = '\0';


    /* process start time */
    sscanf(start_time_str, "%d:%d:%d", &hh, &mm, &ss);
    when.tm_hour = hh;
    when.tm_min = mm;
    when.tm_sec = ss;

    converted_time = mktime(&when) + shift_ss;
    converted_time_str = ctime(&converted_time);

    /*
     * extract the new start time
     *
     * ex. Mon Aug 13 08:23:14 2012
     *
     *     Www Mmm dd hh:mm:ss yyyy
     *     ^          ^
     *     0          11
     */
    strncpy(new_start_time_str, converted_time_str + 11, 8);
    new_start_time_str[8] = '\0';


    /* process end time */
    sscanf(end_time_str, "%d:%d:%d", &hh, &mm, &ss);
    when.tm_hour = hh;
    when.tm_min = mm;
    when.tm_sec = ss;

    converted_time = mktime(&when) + shift_ss;
    converted_time_str = ctime(&converted_time);

    /* extract the new end time */
    strncpy(new_end_time_str, converted_time_str + 11, 8);
    new_end_time_str[8] = '\0';


    /* rebuild the timeline string */
    sprintf(new_timeline_str, "%s,%s --> %s,%s\n",
            new_start_time_str, start_mss_str, new_end_time_str, end_mss_str);

    /* commit the new timeline string to file */
    fputs(new_timeline_str, fout);

    /* next read(s), the content lines */
    do {
      nread = getline(&linebuf, &linebuf_size, fin);
      /* save the line */
      fputs(linebuf, fout);
      /* line break */
      if (strcmp(linebuf, "\n") == 0) break;
    } while (1);

  } while (1);


  if (linebuf) {
    free(linebuf);
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
