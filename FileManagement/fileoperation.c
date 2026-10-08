#include <stdio.h>
#include <stdlib.h>

void createFile();
void writeFile();
void readFile();
void appendFile();
void copyFile();

int main() {
  int choice;
  while (1) {
    printf("\n--- File Operations ---\n");
    printf("1. Create File\n");
    printf("2. Write File\n");
    printf("3. Read File\n");
    printf("4. Append File\n");
    printf("5. Copy File\n");
    printf("6. Exit\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        createFile();
        break;
      case 2:
        writeFile();
        break;
      case 3:
        readFile();
        break;
      case 4:
        appendFile();
        break;
      case 5:
        copyFile();
        break;
      case 6:
        exit(0);
      default:
        printf("Invalid choice\n");
    }
  }
  return 0;
}

void createFile() {
  FILE *fp;
  char name[50];

  printf("Enter filename: ");
  scanf("%s", name);

  fp = fopen(name, "w");

  if (fp == NULL)
    printf("File creation failed\n");
  else {
    printf("File created successfully\n");
    fclose(fp);
  }
}

void writeFile() {
  FILE *fp;
  char name[50], data[100];

  printf("Enter filename: ");
  scanf("%s", name);

  fp = fopen(name, "w");

  if (fp == NULL) {
    printf("Cannot open file\n");
    return;
  }

  printf("Enter data: ");
  getchar();
  fgets(data, sizeof(data), stdin);

  fprintf(fp, "%s", data);

  fclose(fp);
  printf("Data written successfully\n");
}

void readFile() {
  FILE *fp;
  char name[50], ch;

  printf("Enter filename: ");
  scanf("%s", name);

  fp = fopen(name, "r");

  if (fp == NULL) {
    printf("File not found\n");
    return;
  }

  printf("\nFile Content:\n");

  while ((ch = fgetc(fp)) != EOF)
    printf("%c", ch);

  fclose(fp);
}


void appendFile() {
  FILE *fp;
  char name[50], data[100];

  printf("Enter filename: ");
  scanf("%s", name);

  fp = fopen(name, "a");

  if (fp == NULL) {
    printf("Cannot open file\n");
    return;
  }

  printf("Enter data to append: ");
  getchar();
  fgets(data, sizeof(data), stdin);

  fprintf(fp, "%s", data);

  fclose(fp);

  printf("Data appended successfully\n");
}

void copyFile() {
  FILE *source, *target;
  char src[50], dest[50], ch;

  printf("Enter source file: ");
  scanf("%s", src);

  printf("Enter destination file: ");
  scanf("%s", dest);

  source = fopen(src, "r");
  target = fopen(dest, "w");

  if (source == NULL || target == NULL) {
    printf("Copy failed\n");
    return;
  }

  while ((ch = fgetc(source)) != EOF)
    fputc(ch, target);

  fclose(source);
  fclose(target);

  printf("File copied successfully\n");
}