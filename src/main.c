#include <stdio.h>
#include "unwanted_json.h"

int main(int argc, char** args) {
  FILE* file = fopen("test.json", "r");

  char* name = NULL;
  char* address_state = NULL;
  char* first_pet = NULL;
  bool* car = NULL;
  bool* married = NULL;
  double* age = NULL;
  unwanted_json_node* sub_node = NULL;

  unwanted_json_node* node = NULL;

  unwanted_json_tokens* tokens = unwanted_json_file_tokenize(file);

  printf("\n\n\n");

  if (tokens != NULL) {
    unwanted_json_print_tokens(tokens);

    printf("\n\n\n");

    node = unwanted_json_parse(tokens);

    if (node != NULL) {
      unwanted_json_print_nodes(node);

      printf("\n\n\n");

      name = unwanted_json_get_string_by_key(node, "name");

      if (name != NULL) {
        printf("Name value is: %s\n", name);

        free(name);

        name = NULL;
      } else {
        printf("Error getting value of JSON key %s: %s\n", "name", unwanted_json_error());
      }

      age = unwanted_json_get_number_by_key(node, "age");

      if (age != NULL) {
        printf("Age value is: %d\n", ((int) (*age)));

        free(age);

        age = NULL;
      } else {
        printf("Error getting value of JSON key %s: %s\n", "age", unwanted_json_error());
      }

      sub_node = unwanted_json_get_object_by_key(node, "address");

      if (sub_node != NULL) {

        address_state = unwanted_json_get_string_by_key(sub_node, "state");

        if (address_state != NULL) {
          printf("State value is: %s\n", address_state);

          free(address_state);

          address_state = NULL;
        } else {
          printf("Error getting value of JSON key %s: %s\n", "address.state", unwanted_json_error());
        }
        
        sub_node = NULL;
      } else {
        printf("Error getting value of JSON key %s: %s\n", "address", unwanted_json_error());
      }

      sub_node = unwanted_json_get_array_by_key(node, "pets");

      if (sub_node != NULL) {

        first_pet = unwanted_json_get_string_by_index(sub_node, 0);

        if (first_pet != NULL) {
          printf("Pet value is: %s\n", first_pet);

          free(first_pet);

          first_pet = NULL;
        } else {
          printf("Error getting value of JSON key %s: %s\n", "pets[0]", unwanted_json_error());
        }
        
        sub_node = NULL;
      } else {
        printf("Error getting value of JSON key %s: %s\n", "pets", unwanted_json_error());
      }

      married = unwanted_json_get_boolean_by_key(node, "married");

      if (married != NULL) {
        printf("Married value is: %s\n", (*married) == true ? "true" : "false");

        free(married);

        married = NULL;
      } else {
        printf("Error getting value of JSON key %s: %s\n", "married", unwanted_json_error());
      }

      car = unwanted_json_get_null_by_key(node, "car");

      if (car != NULL) {
        printf("Car value is: %s\n", (*car) == true ? "null" : "not null");

        free(car);

        car = NULL;
      } else {
        printf("Error getting value of JSON key %s: %s\n", "car", unwanted_json_error());
      }
  
      unwanted_json_cleanup_nodes(node);
    } else {
      printf("Error parsing JSON Tokens to Nodes: %s\n", unwanted_json_error());
    }

    unwanted_json_cleanup_tokens(tokens);
  } else {
    printf("Error parsing JSON file: %s\n", unwanted_json_error());
  }

  fclose(file);

  return 0;
}
