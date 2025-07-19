#include <stdio.h>
#include "unwanted_json.h"

int main(int argc, char** args) {
  FILE* file = fopen("test.json", "r");

  unwanted_json_node* node = NULL;

  unwanted_json_tokens* tokens = unwanted_json_file_tokenize(file);

  if (tokens != NULL) {
    unwanted_json_print_tokens(tokens);

    node = unwanted_json_parse(tokens);

    if (node != NULL) {
      unwanted_json_print_nodes(node);
  
      unwanted_json_cleanup_nodes(node);
    } else {
      printf("Error parsing JSON Tokens to Nodes: %s", unwanted_json_error());
    }

    unwanted_json_cleanup_tokens(tokens);
  } else {
    printf("Error parsing JSON file: %s", unwanted_json_error());
  }

  fclose(file);

  return 0;
}
