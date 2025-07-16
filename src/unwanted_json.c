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

void unwanted_json_tokens_cleanup(unwanted_json_tokens* tokens) {
  size_t i;

  if (tokens != NULL) {
    for (i = 0; i < tokens->size; i++) {
      free(tokens->values[i].value);
    }

    free(tokens->values);

    free(tokens);
  }
}

void unwanted_json_print_tokens(unwanted_json_tokens* tokens) {
  size_t i;

  printf("%d tokens\n", tokens->size);

  for (i = 0; i < tokens->size; i++) {
    printf("%d) Type(%d) - %s\n", i, tokens->values[i].type, tokens->values[i].value);
  }
}

unwanted_json_tokens* unwanted_json_tokenize(char* json_string) {

  size_t json_string_size = 0;

  size_t json_string_index = 0;

  unwanted_json_tokens* tokens = NULL;

  char* string_token_value = NULL;

  size_t string_token_value_size = 0;


  json_string_size = strlen(json_string);

  tokens = malloc(sizeof(*tokens));

  if (tokens == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_tokens";

    return NULL;
  }

  tokens->size = 0;
  tokens->values = malloc(sizeof(*tokens->values));

  if (tokens->values == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_tokens values";

    free(tokens);

    return NULL;
  }


  while (json_string_index < json_string_size) {
    char json_char = json_string[json_string_index];

    if (json_char == '{' || json_char == '}' || json_char == '[' || json_char == ']' || json_char == ':' || json_char == ',') {
      if (tokens->size > 0) {
        tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

        if (tokens->values == NULL) {
          unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";
  
          free(tokens);

          return NULL;
        }
      }
      
      switch (json_char) {
        case '{':
          tokens->values[tokens->size].type = Token_BraceOpen;
        break;

        case '}':
          tokens->values[tokens->size].type = Token_BraceClose;
        break;

        case '[':
          tokens->values[tokens->size].type = Token_BracketOpen;
        break;

        case ']':
          tokens->values[tokens->size].type = Token_BracketClose;
        break;

        case ':':
          tokens->values[tokens->size].type = Token_Colon;
        break;
        
        case ',':
          tokens->values[tokens->size].type = Token_Comma;
        break;
      }

      tokens->values[tokens->size].value = malloc(2 * sizeof(*tokens->values[tokens->size].value));
      
      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to allocate memory for one of unwanted_json_tokens values value";
        
        unwanted_json_tokens_cleanup(tokens);
        
        return NULL;
      }
      
      tokens->values[tokens->size].value[0] = json_char;
      tokens->values[tokens->size].value[1] = '\0';

      tokens->size++;

      json_string_index++;

    } else if (json_char == '"') {

      string_token_value_size = 0;

      string_token_value = malloc(2 * sizeof(*string_token_value));

      if (string_token_value == NULL) {
        unwanted_json_error_message = "Failed to allocate memory for one of unwanted_json_tokens values string value";

        unwanted_json_tokens_cleanup(tokens);
        
        return NULL;
      }

      json_char = json_string[++json_string_index];

      while (json_char != '"') {
        if (string_token_value_size > 0) {
          string_token_value = realloc(string_token_value, (string_token_value_size + 2) * sizeof(*string_token_value));

          if (string_token_value == NULL) {
            unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values string value";
    
            unwanted_json_tokens_cleanup(tokens);

            return NULL;
          }
        }

        string_token_value[string_token_value_size] = json_char;
        string_token_value[string_token_value_size + 1] = '\0';

        string_token_value_size++;

        json_char = json_string[++json_string_index];
      }


      if (tokens->size > 0) {
        tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

        if (tokens->values == NULL) {
          unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";
  
          free(tokens);

          return NULL;
        }
      }


      tokens->values[tokens->size].type = Token_String;

      tokens->values[tokens->size].value = string_token_value;

      tokens->size++;

      json_string_index++;

    } else if (isalpha(json_char) > 0) {

      string_token_value_size = 0;

      string_token_value = malloc(2 * sizeof(*string_token_value));

      if (string_token_value == NULL) {
        unwanted_json_error_message = "Failed to allocate memory for one of unwanted_json_tokens values string value";

        unwanted_json_tokens_cleanup(tokens);
        
        return NULL;
      }

      while (isalpha(json_char) > 0) {
        if (string_token_value_size > 0) {
          string_token_value = realloc(string_token_value, (string_token_value_size + 2) * sizeof(*string_token_value));

          if (string_token_value == NULL) {
            unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values string value";
    
            unwanted_json_tokens_cleanup(tokens);

            return NULL;
          }
        }

        string_token_value[string_token_value_size] = json_char;
        string_token_value[string_token_value_size + 1] = '\0';

        string_token_value_size++;

        json_char = json_string[++json_string_index];
      }


      if (tokens->size > 0) {
        tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

        if (tokens->values == NULL) {
          unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";
  
          free(tokens);

          return NULL;
        }
      }


      if (strcmp(string_token_value, "true") == 0) {
        tokens->values[tokens->size].type = Token_True;

        tokens->values[tokens->size].value = string_token_value;

        tokens->size++;
      } else if (strcmp(string_token_value, "false") == 0) {
        tokens->values[tokens->size].type = Token_False;

        tokens->values[tokens->size].value = string_token_value;

        tokens->size++;
      } else if (strcmp(string_token_value, "null") == 0) {
        tokens->values[tokens->size].type = Token_Null;

        tokens->values[tokens->size].value = string_token_value;

        tokens->size++;
      } else {
        unwanted_json_error_message = "Invalid token provided";

        unwanted_json_tokens_cleanup(tokens);

        free(string_token_value);

        return NULL;
      }

    } else {
      json_string_index++;
    }
  }

  return tokens;
}


unwanted_json_tokens* unwanted_json_file_tokenize(FILE* file) {
  char line[FILE_LINE_BUFFER];

  size_t line_size = 0;

  char* file_lines = NULL;

  size_t file_lines_size = 0;

  unwanted_json_tokens* tokens = NULL;


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
  printf("File char count: %d\n\n\n", file_lines_size);

  
  tokens = unwanted_json_tokenize(file_lines);

  free(file_lines);

  return tokens;
}
