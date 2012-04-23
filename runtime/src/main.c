#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "classfile_structs.h"
#include "classfile_consts.h"

int main(int argc, const char* argv[])
{
  FILE *fp = fopen("test/bin/bonkers/Start.class", "rb");
  if (fp == NULL)
  {
    printf("Error opening file.\n");
    return;
  }
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *data = malloc(size);
  size_t read = fread(data, 1, size, fp);
  if (read != size)
  {
    printf("Didn't read all of file. ");
  }
  printf("Read %li bytes from .class file.\n\n", size);
  fclose(fp);
  
  class_file class;
  decode_class_header(data, size, &class);
  print_class_header(&class);
}
