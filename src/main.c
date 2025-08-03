#include <stdio.h>
#include "unwanted_json.h"

int main(int argc, char** args) {
  FILE* file;

  FILE* out_file;

  char* json_string = NULL;

  unwanted_json_node* node = NULL;

  unwanted_json_tokens* tokens = NULL;

  unwanted_json_tokens* reverse_tokens = NULL;


  file = fopen("test.json", "r");

  if (file == NULL) {
    printf("Failed to open input JSON file");

    return 1;
  }

  // tokens = unwanted_json_file_tokenize(file);

  // printf("\n\n\n");

  // if (tokens != NULL) {
    // unwanted_json_print_tokens(tokens);

    printf("\n\n\n");

    // node = unwanted_json_parse(tokens);

    node = unwanted_json_from_file(file);

    if (node != NULL) {
      unwanted_json_print_nodes(node);

      
      printf("\n\n\n REversing...\n\n");

      // reverse_tokens = unwanted_json_unparse(node);

      // printf("\n\n\n");

      // if (reverse_tokens != NULL) {
        // unwanted_json_print_tokens(reverse_tokens);

        // json_string = unwanted_json_untokenize(reverse_tokens);

        json_string = unwanted_json_to_string(node);

        if (json_string != NULL) {
          printf("JSON string is: %s\n", json_string);

          free(json_string);

          json_string = NULL;
        } else {
          printf("Error parsing JSON Tokens to String: %s\n", unwanted_json_error());
        }
        
        
        // out_file = fopen("output.json", "w");
        
        // if (out_file != NULL) {
        //   // if (unwanted_json_file_untokenize(reverse_tokens, out_file)) {

        //   if (unwanted_json_to_file(node, out_file)) {
        //     printf("JSON Tokens parsed to string and written to File\n");
        //   } else {
        //     printf("Error parsing JSON Tokens to File: %s\n", unwanted_json_error());
        //   }

        //   fclose(out_file);
        // } else {
        //   printf("Failed to open JSON output file\n");
        // }

        // unwanted_json_cleanup_tokens(reverse_tokens);
      // } else {
      //   printf("Error parsing JSON Nodes to Tokens: %s\n", unwanted_json_error());
      // }
  
      unwanted_json_cleanup_nodes(node);
    } else {
      printf("Error parsing JSON Tokens to Nodes: %s\n", unwanted_json_error());
    }

    // unwanted_json_cleanup_tokens(tokens);
  // } else {
  //   printf("Error parsing JSON file: %s\n", unwanted_json_error());
  // }

  fclose(file);

  return 0;
}
