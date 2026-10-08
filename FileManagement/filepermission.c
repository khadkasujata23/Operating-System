#include <stdio.h>
#include <sys/stat.h>

void displayPermission(char []);
void modifyPermission(char []);

int main() {
  char filename[50];

  printf("Enter filename: ");
  scanf("%s", filename);

  displayPermission(filename);
  modifyPermission(filename);
  displayPermission(filename);

  return 0;
}


void displayPermission(char filename[]) {
  struct stat file;
  stat(filename, &file);
  printf("\nFile Permissions:\n");
  printf("Owner Read: %s\n", (file.st_mode & S_IRUSR) ? "Yes":"No");
  printf("Owner Write: %s\n", (file.st_mode & S_IWUSR) ? "Yes":"No");
  printf("Owner Execute: %s\n", (file.st_mode & S_IXUSR) ? "Yes":"No");
}


void modifyPermission(char filename[]) {
  int mode;
  printf("\nEnter permission mode (example 755): ");
  scanf("%o", &mode);
  chmod(filename, mode);
  printf("Permission changed successfully\n");
}