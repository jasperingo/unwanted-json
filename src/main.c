#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "unwanted_json.h"

#define HELP_OPTION "--help"
#define VERBOSE_OPTION "--verbose"
#define INPUT_OPTION "--read"
#define OUTPUT_OPTION "--write"

int main(int argc, char** args) {
  size_t i = 0;

  bool needs_help = false;

  bool verbose = false;

  char* input_file_path = NULL;

  char* output_file_path = NULL;

  FILE* file = NULL;

  char* json_string = NULL;

  unwanted_json_node* node = NULL;

  unwanted_json_tokens* tokens = NULL;

  i = 1;

  while (i < argc) {
    if (strcmp(args[i], HELP_OPTION) == 0) {
      needs_help = true;
      break;
    } else if (strcmp(args[i], VERBOSE_OPTION) == 0) {
      verbose = true;
    } else if (strcmp(args[i], INPUT_OPTION) == 0) {
      if ((i + 1) < argc) {
        input_file_path = args[++i];
      }
    } else if (strcmp(args[i], OUTPUT_OPTION) == 0) {
      if ((i + 1) < argc) {
        output_file_path = args[++i];
      }
    } else {
      printf("Invalid option: %s\n", args[i]);
      printf("Use %s option to see usage information\n", HELP_OPTION);
      return EXIT_FAILURE;
    }

    i++;
  }

  if (needs_help) {
    printf("Usage: UnwantedJson.exe [options]\n");
    printf("Options:\n");
    printf("%s         Display this information\n", HELP_OPTION);
    printf("%s         Display JSON tokens and nodes\n", VERBOSE_OPTION);
    printf("%s         JSON file to read data from (required)\n", INPUT_OPTION);
    printf("%s         JSON file to write data to. Will write output to console if not provided\n", OUTPUT_OPTION);

    return EXIT_SUCCESS;
  }

  if (input_file_path == NULL) {
    printf("Required option %s not provided\n", INPUT_OPTION);

    return EXIT_FAILURE;
  }

  file = fopen(input_file_path, "r");

  if (file == NULL) {
    printf("Failed to open input JSON file");

    return EXIT_FAILURE;
  }

  tokens = unwanted_json_file_tokenize(file);
  
  fclose(file);

  if (tokens != NULL) {

    if (verbose) {
      printf("\n\n\n");
      unwanted_json_print_tokens(tokens);
    }

    node = unwanted_json_parse(tokens);
  
    if (node != NULL) {
      
      if (verbose) {
        printf("\n\n\n");
        unwanted_json_print_nodes(node);
      }

      if (output_file_path != NULL) {
        
        file = fopen(output_file_path, "w");
        
        if (file != NULL) {

          if (unwanted_json_to_file(node, file)) {
            printf("\n\n\nJSON Tokens parsed to string and written to File\n");
          } else {
            printf("Error parsing JSON Tokens to File: %s\n", unwanted_json_error());
          }

          fclose(file);
        } else {
          printf("Failed to open JSON output file\n");
        }

      } else {
        json_string = unwanted_json_to_string(node);

        if (json_string != NULL) {
          printf("\n\n\nJSON string is: %s\n", json_string);

          free(json_string);

          json_string = NULL;
        } else {
          printf("Error parsing JSON Tokens to String: %s\n", unwanted_json_error());
        }
      }
  
      unwanted_json_cleanup_nodes(node);
    } else {
      printf("Error parsing JSON Tokens to Nodes: %s\n", unwanted_json_error());
    }

    unwanted_json_cleanup_tokens(tokens);
  } else {
    printf("Error parsing JSON file: %s\n", unwanted_json_error());
  }

  return EXIT_SUCCESS;
}


void test_parse_file(void) {
  FILE* file;

  unwanted_json_node* node = NULL;

  unwanted_json_tokens* tokens = NULL;

  file = fopen("test.json", "r");

  if (file != NULL) {

    tokens = unwanted_json_file_tokenize(file);

    printf("\n\n\n");

    if (tokens != NULL) {
      unwanted_json_print_tokens(tokens);

      printf("\n\n\n");

      node = unwanted_json_parse(tokens);

      if (node != NULL) {
        unwanted_json_print_nodes(node);
    
        unwanted_json_cleanup_nodes(node);
      } else {
        printf("Error parsing JSON Tokens to Nodes: %s\n", unwanted_json_error());
      }

      unwanted_json_cleanup_tokens(tokens);
    } else {
      printf("Error parsing JSON file: %s\n", unwanted_json_error());
    }

    fclose(file);

  } else {
    printf("Failed to open input JSON file");
  }
}

void test_stringy(void) {
  FILE* file;

  char* json_string = NULL;

  unwanted_json_node* node = NULL;

  unwanted_json_node* sub_node = NULL;

  unwanted_json_node* sub_sub_node = NULL;

  unwanted_json_tokens* tokens = NULL;

  unwanted_json_tokens* reverse_tokens = NULL;
  
  node = unwanted_json_create_object();

  if (node != NULL) {

    unwanted_json_set_string_by_key(node, "name", "Jasper");
    unwanted_json_set_string_by_key(node, "phone", "93938829");
    unwanted_json_set_string_by_key(node, "cat_name", "Benny");
    unwanted_json_set_number_by_key(node, "age", 20);
    unwanted_json_set_boolean_by_key(node, "has_pet", true);
    unwanted_json_set_null_by_key(node, "phone");
    
    sub_node = unwanted_json_create_object();
    unwanted_json_set_string_by_key(sub_node, "first", "Yes");
    unwanted_json_set_string_by_key(sub_node, "second", "No");
    
    unwanted_json_set_object_by_key(node, "positions", sub_node);
    
    
    unwanted_json_set_string_by_key(sub_node, "third", "Nein");
    unwanted_json_set_boolean_by_key(sub_node, "fourth", false);
    
    sub_node = unwanted_json_create_array();
    unwanted_json_set_string_by_index(sub_node, 0, "Yam");
    unwanted_json_set_string_by_index(sub_node, 1, "Rice");
    
    unwanted_json_set_array_by_key(node, "foods", sub_node);
    
    
    unwanted_json_set_string_by_index(sub_node, 2, "Beans");
    unwanted_json_set_number_by_index(sub_node, 3, 100);
    
    sub_sub_node = unwanted_json_create_array();
    unwanted_json_set_string_by_index(sub_sub_node, 0, "Kill");
    unwanted_json_set_string_by_index(sub_sub_node, 1, "Dry");
    unwanted_json_set_null_by_index(sub_sub_node, 2);
    
    unwanted_json_print_nodes(sub_node);
    
    unwanted_json_set_array_by_index(sub_node, 4, sub_sub_node);
    
    unwanted_json_print_nodes(sub_node);

    unwanted_json_remove_by_key(node, "cat_name");

    unwanted_json_print_nodes(sub_node);
    
    if (unwanted_json_remove_by_key(unwanted_json_get_object_by_key(node, "positions"), "fourth") == false) {
      printf("Error removiing JSON array value: %s\n", unwanted_json_error());
    }
    
    if (unwanted_json_remove_by_index(sub_node, 1) == false) {
      printf("Error removiing JSON array value: %s\n", unwanted_json_error());
    }


    if (unwanted_json_remove_by_index(sub_sub_node, 1) == false) {
      printf("Error removiing JSON array value 2: %s\n", unwanted_json_error());
    }
    
    unwanted_json_print_nodes(node);

    json_string = unwanted_json_to_string(node);

    if (json_string != NULL) {
      printf("\n\nJSON string is: %s\n", json_string);

      free(json_string);

      json_string = NULL;
    } else {
      printf("Error parsing JSON Tokens to String: %s\n", unwanted_json_error());
    }
      
      
    file = fopen("output.json", "w");
      
    if (file != NULL) {
      if (unwanted_json_to_file(node, file)) {
        printf("JSON Tokens parsed to string and written to File\n");
      } else {
        printf("Error parsing JSON Tokens to File: %s\n", unwanted_json_error());
      }

      fclose(file);
    } else {
      printf("Failed to open JSON output file\n");
    }

    unwanted_json_cleanup_nodes(node);
  } else {
    printf("Error parsing JSON Tokens to Nodes: %s\n", unwanted_json_error());
  }
}
