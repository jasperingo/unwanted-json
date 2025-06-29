#include <stdio.h>
#include "unwanted_json.h"

int main(int argc, char** args) {
  FILE* file = fopen("test.json", "r");

  unwanted_json_file_tokenize(file);

  fclose(file);

  return 0;
}
