#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> 
#include <stdio.h> 

int main(void)
{
  FILE *fptr;

  // Open a file in writing mode
  fptr = fopen("filename.txt", "w");

  // Write some text to the file
  fprintf(fptr, "Some text");

  // Close the file
  fclose(fptr);

  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      printf("%s\n", dir->d_name);
    }
    closedir(d);
  }

}
