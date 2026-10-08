#include <stdio.h>
#include <dirent.h>

void listDirectory();

int main() {
  listDirectory();
  return 0;
}

void listDirectory() {
  DIR *dir;
  struct dirent *entry;

  char path[100];

  printf("Enter directory path: ");
  scanf("%s", path);

  dir = opendir(path);

  if (dir == NULL) {
    printf("Unable to open directory\n");
    return;
  }

  printf("\nFiles in directory:\n");

  while ((entry = readdir(dir)) != NULL)
    printf("%s\n", entry->d_name);
  closedir(dir);
}