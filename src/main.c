#include <stdio.h>
#include "unwanted_json.h"

void print_items(int* items, size_t size) {
  size_t i;

	printf("\nPrinting items\n");
  
  for (i = 0; i < size; i++) {
    printf("%d,", items[i]);
  }

  printf("\nPrinted items\n");
}

void remove_items(int** items, size_t size, size_t index) {
  if (index >= size) {
    return;
  }

  memset((*items) + index, 0, sizeof(**items));

	memmove((*items) + index, (*items) + index + 1, (size - index - 1) * sizeof(**items));

  *items = realloc(*items, (size - 1) * sizeof(**items));
}

void test_items_change() {
   size_t i;

  size_t items_size = 10;

  int* items = malloc(items_size * sizeof(*items));

  for (i = 0; i < items_size; i++) {
    items[i] = i;
  }

  print_items(items, items_size);
  
  remove_items(&items, items_size, 5);

  items_size--;
  
  print_items(items, items_size);

  remove_items(&items, items_size, 0);

  items_size--;
  
  print_items(items, items_size);

  remove_items(&items, items_size, 8);

  items_size--;
  
  print_items(items, items_size);

  free(items);
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

int main(int argc, char** args) {
  // test_parse_file();

  test_stringy();
  // FILE* file;

  // FILE* out_file;

  // char* json_string = NULL;

  // unwanted_json_node* node = NULL;

  // unwanted_json_node* sub_node = NULL;

  // unwanted_json_node* sub_sub_node = NULL;

  // unwanted_json_tokens* tokens = NULL;

  // unwanted_json_tokens* reverse_tokens = NULL;

  
  // node = unwanted_json_create_object();

  // file = fopen("test.json", "r");

  // if (file == NULL) {
  //   printf("Failed to open input JSON file");

  //   return 1;
  // }

  // tokens = unwanted_json_file_tokenize(file);

  // printf("\n\n\n");

  // if (tokens != NULL) {
    // unwanted_json_print_tokens(tokens);

    // printf("\n\n\n");

    // node = unwanted_json_parse(tokens);

    // node = unwanted_json_from_file(file);

    // if (node != NULL) {
    //   unwanted_json_print_nodes(node);

      
    //   printf("\n\n\n REversing...\n\n");

      // reverse_tokens = unwanted_json_unparse(node);

      // printf("\n\n\n");

      // unwanted_json_set_string_by_key(node, "name", "Jasper");
      // unwanted_json_set_string_by_key(node, "phone", "93938829");
      // unwanted_json_set_string_by_key(node, "cat_name", "Benny");
      // unwanted_json_set_number_by_key(node, "age", 20);
      // unwanted_json_set_boolean_by_key(node, "has_pet", true);
      // unwanted_json_set_null_by_key(node, "phone");
      
      // sub_node = unwanted_json_create_object();
      // unwanted_json_set_string_by_key(sub_node, "first", "Yes");
      // unwanted_json_set_string_by_key(sub_node, "second", "No");
      
      // unwanted_json_set_object_by_key(node, "positions", &sub_node);
      
      
      // unwanted_json_set_string_by_key(sub_node, "third", "Nein");
      // unwanted_json_set_boolean_by_key(sub_node, "fourth", false);
      
      // sub_node = unwanted_json_create_array();
      // unwanted_json_set_string_by_index(sub_node, 0, "Yam");
      // unwanted_json_set_string_by_index(sub_node, 1, "Rice");
      
      // unwanted_json_set_array_by_key(node, "foods", &sub_node);
      
      
      // unwanted_json_set_string_by_index(sub_node, 2, "Beans");
      // unwanted_json_set_number_by_index(sub_node, 3, 100);
      
      // sub_sub_node = unwanted_json_create_array();
      // unwanted_json_set_string_by_index(sub_sub_node, 0, "Kill");
      // unwanted_json_set_string_by_index(sub_sub_node, 1, "Dry");
      // unwanted_json_set_null_by_index(sub_sub_node, 2);
      
      // unwanted_json_print_nodes(sub_node);
      
      // unwanted_json_set_array_by_index(sub_node, 4, &sub_sub_node);
      
      // unwanted_json_print_nodes(sub_node);

      // unwanted_json_remove_by_key(node, "cat_name");

      // unwanted_json_print_nodes(sub_node);
      
      // if (unwanted_json_remove_by_index(sub_node, 1) == false) {
      //   printf("Error removiing JSON array value: %s\n", unwanted_json_error());
      // }

      // if (unwanted_json_remove_by_index(sub_sub_node, 1) == false) {
      //   printf("Error removiing JSON array value 2: %s\n", unwanted_json_error());
      // }
      
      // unwanted_json_print_nodes(node);

      // if (reverse_tokens != NULL) {
        // unwanted_json_print_tokens(reverse_tokens);

        // json_string = unwanted_json_untokenize(reverse_tokens);

        // json_string = unwanted_json_to_string(node);

        // if (json_string != NULL) {
        //   printf("\n\nJSON string is: %s\n", json_string);

        //   free(json_string);

        //   json_string = NULL;
        // } else {
        //   printf("Error parsing JSON Tokens to String: %s\n", unwanted_json_error());
        // }
        
        
        // unwanted_json_cleanup_nodes(sub_node);
        
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
  
    //   unwanted_json_cleanup_nodes(node);
    // } else {
    //   printf("Error parsing JSON Tokens to Nodes: %s\n", unwanted_json_error());
    // }

    // unwanted_json_cleanup_tokens(tokens);
  // } else {
  //   printf("Error parsing JSON file: %s\n", unwanted_json_error());
  // }

  // fclose(file);

  return 0;
}
