#include "unwanted_json.h"

#define FILE_LINE_BUFFER 5

struct unwanted_json_node {
  unwanted_json_node_type type;
  char* string_value;
  bool boolean_value;
  double number_value;
  size_t array_value_size;
  unwanted_json_node* array_value;
  char* object_value_key;
  size_t object_value_size;
  unwanted_json_node* object_value;
};

struct unwanted_json_token {
  unwanted_json_token_type type;
  char* value;
};

struct unwanted_json_tokens {
  size_t size;
  unwanted_json_token* values;
};

char* unwanted_json_error_message = NULL;

char* unwanted_json_error() {
  return unwanted_json_error_message;
}

unwanted_json_tokens* unwanted_json_file_tokenize(FILE* file) {
  char line[FILE_LINE_BUFFER];

  size_t line_size = 0;

  char* file_lines = NULL;

  size_t file_lines_size = 0;

  unwanted_json_tokens* token = NULL;


  file_lines = malloc((FILE_LINE_BUFFER + 2) * sizeof(*file_lines));

  if (file_lines == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for file contents";

    return NULL;
  }

  file_lines[0] = '\0';

  while (fgets(line, sizeof(line), file) != NULL) {
    
    printf("File line: %s\n", line);

    line_size = strlen(line);

    if (file_lines_size > 0) {
      file_lines = realloc(file_lines, (file_lines_size + line_size + 1) * sizeof(*file_lines));

      if (file_lines == NULL) {
        unwanted_json_error_message = "Failed to re-allocate memory for file contents";

        return NULL;
      }
    }

    strcat(file_lines, line);
    
    file_lines_size += line_size;
  }

  printf("File content: %s\n", file_lines);
  printf("File char count: %d\n", file_lines_size);

  free(file_lines);

  // token = malloc(sizeof(*token));

  // if (token == NULL) {
  //   unwanted_json_error_message = "Failed to allocate memory for unwanted_json_tokens";

  //   return NULL;
  // }

  // token->size = 0;
  // token->values = malloc(sizeof(*token->values));

  // if (token->values == NULL) {
  //   unwanted_json_error_message = "Failed to allocate memory for unwanted_json_tokens items";

  //   free(token);

  //   return NULL;
  // }



  return token;
}
