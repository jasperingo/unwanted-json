#include "unwanted_json.h"

struct unwanted_json_node {
  size_t level;
  unwanted_json_node_type type;
  char* string_value;
  bool boolean_value;
  double number_value;
  size_t array_value_size;
  unwanted_json_node** array_value;
  char* object_value_key;
  size_t object_value_size;
  unwanted_json_node** object_value;
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

void unwanted_json_cleanup_node(unwanted_json_node* node);

bool unwanted_json_parse_value(unwanted_json_node* node, unwanted_json_tokens* tokens, size_t* tokens_index, size_t node_level);

char* unwanted_json_error() {
  return unwanted_json_error_message;
}

void unwanted_json_cleanup_tokens(unwanted_json_tokens* tokens) {
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

char* unwanted_json_untokenize(unwanted_json_tokens* tokens) {
  size_t i = 0;

  size_t token_size = 0;

  size_t json_string_size = 0;

  char* json_string = NULL;

  if (tokens == NULL || tokens->size == 0) {
    unwanted_json_error_message = "Provided unwanted_json_tokens is invalid";

    return NULL;
  }

  json_string = malloc(sizeof(*json_string));

  if (json_string == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for JSON string";

    return NULL;
  }

  json_string[0] = '\0';

  for (i = 0; i < tokens->size; i++) {
    token_size = strlen(tokens->values[i].value);

    json_string = realloc(json_string, (json_string_size + token_size + (tokens->values[i].type == Token_String ? 2 : 0) + 1) * sizeof(*json_string));

    if (json_string == NULL) {
      unwanted_json_error_message = "Failed to re-allocate memory for JSON string";

      free(json_string);

      return NULL;
    }

    if (tokens->values[i].type == Token_String) {
      json_string[json_string_size] = '"';
      json_string[json_string_size + 1] = '\0';
    }
    
    strcat(json_string, tokens->values[i].value);

    if (tokens->values[i].type == Token_String) {
      json_string[json_string_size + token_size + 1] = '"';
      json_string[json_string_size + token_size + 2] = '\0';
    }

    json_string_size = strlen(json_string);
  }
  
  return json_string;
}

bool unwanted_json_file_untokenize(unwanted_json_tokens* tokens, FILE* file) {
  char* json_string = NULL;

  json_string = unwanted_json_untokenize(tokens);

  if (json_string == NULL) {
    return false;
  }

  if (fputs(json_string, file) == EOF) {
    unwanted_json_error_message = "Failed to write JSON string to File";

    free(json_string);

    return false;
  }

  free(json_string);

  return true;
}

unwanted_json_tokens* unwanted_json_tokenize(char* json_string) {
  size_t json_string_size = 0;

  size_t json_string_index = 0;
  
  unwanted_json_tokens* tokens = NULL;

  size_t number_minus_token_count = 0;

  size_t number_plus_token_count = 0;
  
  size_t number_exponent_token_count = 0;

  size_t number_period_token_count = 0;

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

    if (
      json_char == '{' || 
      json_char == '}' || 
      json_char == '[' || 
      json_char == ']' || 
      json_char == ':' || 
      json_char == ','
    ) {
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
        
        unwanted_json_cleanup_tokens(tokens);
        
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

        unwanted_json_cleanup_tokens(tokens);
        
        return NULL;
      }

      json_char = json_string[++json_string_index];

      while (json_char != '"') {
        if (string_token_value_size > 0) {
          string_token_value = realloc(string_token_value, (string_token_value_size + 2) * sizeof(*string_token_value));

          if (string_token_value == NULL) {
            unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values string value";
    
            unwanted_json_cleanup_tokens(tokens);

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

        unwanted_json_cleanup_tokens(tokens);
        
        return NULL;
      }

      while (isalpha(json_char) > 0) {
        if (string_token_value_size > 0) {
          string_token_value = realloc(string_token_value, (string_token_value_size + 2) * sizeof(*string_token_value));

          if (string_token_value == NULL) {
            unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values string value";
    
            unwanted_json_cleanup_tokens(tokens);

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
        unwanted_json_error_message = "Invalid JSON token provided";

        unwanted_json_cleanup_tokens(tokens);

        free(string_token_value);

        return NULL;
      }

    } else if (json_char == '-' || isdigit(json_char) > 0) {

      number_minus_token_count = 0;

      number_plus_token_count = 0;

      number_exponent_token_count = 0;

      number_period_token_count = 0;

      string_token_value_size = 0;

      string_token_value = malloc(2 * sizeof(*string_token_value));

      if (string_token_value == NULL) {
        unwanted_json_error_message = "Failed to allocate memory for one of unwanted_json_tokens values string value";

        unwanted_json_cleanup_tokens(tokens);
        
        return NULL;
      }

      while (json_char == '-' || json_char == '+' || json_char == 'e' || json_char == '.' || isdigit(json_char) > 0) {
        switch (json_char) {
          case '-':
            if (number_minus_token_count >= 2) {
              unwanted_json_error_message = "Invalid JSON number token provided";

              unwanted_json_cleanup_tokens(tokens);

              free(string_token_value);

              return NULL;
            } else {
              number_minus_token_count++;
            }
          break;
          
          case '+':
            if (number_plus_token_count >= 1) {
              unwanted_json_error_message = "Invalid JSON number token provided";

              unwanted_json_cleanup_tokens(tokens);

              free(string_token_value);

              return NULL;
            } else {
              number_plus_token_count++;
            }
          break;

          case 'e':
            if (number_exponent_token_count >= 1) {
              unwanted_json_error_message = "Invalid JSON number token provided";

              unwanted_json_cleanup_tokens(tokens);

              free(string_token_value);

              return NULL;
            } else {
              number_exponent_token_count++;
            }
          break;

          case '.':
            if (number_period_token_count >= 1) {
              unwanted_json_error_message = "Invalid JSON number token provided";

              unwanted_json_cleanup_tokens(tokens);

              free(string_token_value);

              return NULL;
            } else {
              number_period_token_count++;
            }
          break;
        }

        if (string_token_value_size > 0) {
          string_token_value = realloc(string_token_value, (string_token_value_size + 2) * sizeof(*string_token_value));

          if (string_token_value == NULL) {
            unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values string value";
    
            unwanted_json_cleanup_tokens(tokens);

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


      tokens->values[tokens->size].type = Token_Number;

      tokens->values[tokens->size].value = string_token_value;

      tokens->size++;

    } else if (isspace(json_char) > 0) {
      json_string_index++;
    } else {
      unwanted_json_error_message = "Invalid JSON token provided";

      unwanted_json_cleanup_tokens(tokens);

      return NULL;
    }
  }

  return tokens;
}

unwanted_json_tokens* unwanted_json_file_tokenize(FILE* file) {
  char line[128];

  size_t line_size = 0;

  char* file_lines = NULL;

  size_t file_lines_size = 0;

  unwanted_json_tokens* tokens = NULL;


  file_lines = malloc((sizeof(line) + 2) * sizeof(*file_lines));

  if (file_lines == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for file contents";

    return NULL;
  }

  file_lines[0] = '\0';

  while (fgets(line, sizeof(line), file) != NULL) {
    
    // printf("File line: %s\n", line);

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

  // printf("File content: %s\n", file_lines);
  // printf("File char count: %d", file_lines_size);

  
  tokens = unwanted_json_tokenize(file_lines);

  free(file_lines);

  return tokens;
}

void unwanted_json_cleanup_node_value(unwanted_json_node* node) {
  size_t i;

  if (node != NULL) {
    switch (node->type) {
      case Node_String:
        free(node->string_value);
      break;

      case Node_Array:
       for (i = 0; i < node->array_value_size; i++) {
          unwanted_json_cleanup_node(node->array_value[i]);
        }

        free(node->array_value);
      break;
      
      case Node_Object:
        for (i = 0; i < node->object_value_size; i++) {
          unwanted_json_cleanup_node(node->object_value[i]);
        }

        free(node->object_value_key);
        free(node->object_value);
      break;
      
      default:
        break;
    }
  }
}

void unwanted_json_cleanup_node(unwanted_json_node* node) {
  size_t i;

  if (node != NULL) {
    unwanted_json_cleanup_node_value(node);

    free(node);
  }
}

void unwanted_json_cleanup_nodes(unwanted_json_node* node) {
  if (node != NULL && node->level == 0) {
    unwanted_json_cleanup_node(node);

    free(node);
  }
}

void unwanted_json_print_node(unwanted_json_node* node) {
  size_t i;

  switch (node->type) {
    case Node_String:
      printf("%d) String Node: %s\n", node->level, node->string_value);
    break;
    
    case Node_Boolean:
      printf("%d) Boolean Node: %d\n", node->level, node->boolean_value);
    break;

    case Node_Number:
      printf("%d) Number Node: %f\n", node->level, node->number_value);
    break;

    case Node_Null:
      printf("%d) Null Node\n", node->level);
    break;
    
    case Node_Object:
      printf("%d) Object Node with number of keys: %d\n", node->level, node->object_value_size);

      for (i = 0; i < node->object_value_size; i++) {
        printf("%d) Object Node key: %s\n", node->object_value[i]->level, node->object_value[i]->object_value_key);
        unwanted_json_print_node(node->object_value[i]);
      }
    break;

    case Node_Array:
      printf("%d) Array Node with length: %d\n", node->level, node->array_value_size);

      for (i = 0; i < node->array_value_size; i++) {
        printf("%d) Array Node Index: %d\n", node->array_value[i]->level, i);
        unwanted_json_print_node(node->array_value[i]);
      }
    break;
  }
}

void unwanted_json_print_nodes(unwanted_json_node* node) {
  if (node != NULL) {
    printf("unwanted_json Nodes\n");

    unwanted_json_print_node(node);
  } else {
    printf("unwanted_json Node is invalid\n");
  }
}

bool unwanted_json_unparse_value(unwanted_json_node* node, unwanted_json_tokens* tokens) {
  size_t i;

  switch (node->type) {
    case Node_String:
      tokens->values[tokens->size].type = Token_String;

      tokens->values[tokens->size].value = strdup(node->string_value);

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }

      tokens->size++;

      return true;
    
    case Node_Boolean:
      tokens->values[tokens->size].type = node->boolean_value ? Token_True : Token_False;

      tokens->values[tokens->size].value = strdup(node->boolean_value ? "true" : "false");

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }

      tokens->size++;

      return true;

    case Node_Null:
      tokens->values[tokens->size].type = Token_Null;

      tokens->values[tokens->size].value = strdup("null");

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }
      
      tokens->size++;

      return true;

    case Node_Number:
      tokens->values[tokens->size].type = Token_Number;

      tokens->values[tokens->size].value = malloc(512); // random buffer size to serve for now

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }

      if (fmod(node->number_value, 1) != 0) {
        sprintf(tokens->values[tokens->size].value, "%f", node->number_value);
      } else {
        sprintf(tokens->values[tokens->size].value, "%d", (int) node->number_value);
      }

      tokens->size++;
      
      return true;

    case Node_Object:
      tokens->values[tokens->size].type = Token_BraceOpen;

      tokens->values[tokens->size].value = strdup("{");

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }
      
      tokens->size++;

      for (i = 0; i < node->object_value_size; i++) {
        tokens->values = realloc(tokens->values, (tokens->size + 3) * sizeof(*tokens->values));

        if (tokens->values == NULL) {
          unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";

          return false;
        }


        tokens->values[tokens->size].type = Token_String;

        tokens->values[tokens->size].value = strdup(node->object_value[i]->object_value_key);

        if (tokens->values[tokens->size].value == NULL) {
          unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

          return false;
        }

        tokens->size++;

        
        tokens->values[tokens->size].type = Token_Colon;
        
        tokens->values[tokens->size].value = strdup(":");

        if (tokens->values[tokens->size].value == NULL) {
          unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

          return false;
        }

        tokens->size++;


        if (unwanted_json_unparse_value(node->object_value[i], tokens) == false) {
          return false;
        }


        if (i < (node->object_value_size - 1)) {
          tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

          if (tokens->values == NULL) {
            unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";

            return false;
          }

          tokens->values[tokens->size].type = Token_Comma;

          tokens->values[tokens->size].value = strdup(",");

          if (tokens->values[tokens->size].value == NULL) {
            unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

            return false;
          }
          
          tokens->size++;
        }
      }

      tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

      if (tokens->values == NULL) {
        unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";

        return false;
      }

      tokens->values[tokens->size].type = Token_BraceClose;

      tokens->values[tokens->size].value = strdup("}");

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }
      
      tokens->size++;

      return true;

    case Node_Array:
      tokens->values[tokens->size].type = Token_BracketOpen;

      tokens->values[tokens->size].value = strdup("[");

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }
      
      tokens->size++;


      for (i = 0; i < node->array_value_size; i++) {
        tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

        if (tokens->values == NULL) {
          unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";

          return false;
        }


        if (unwanted_json_unparse_value(node->array_value[i], tokens) == false) {
          return false;
        }


        if (i < (node->array_value_size - 1)) {
          tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

          if (tokens->values == NULL) {
            unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";

            return false;
          }

          tokens->values[tokens->size].type = Token_Comma;

          tokens->values[tokens->size].value = strdup(",");

          if (tokens->values[tokens->size].value == NULL) {
            unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

            return false;
          }
          
          tokens->size++;
        }
      }

      tokens->values = realloc(tokens->values, (tokens->size + 1) * sizeof(*tokens->values));

      if (tokens->values == NULL) {
        unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_tokens values";

        return false;
      }

      tokens->values[tokens->size].type = Token_BracketClose;

      tokens->values[tokens->size].value = strdup("]");

      if (tokens->values[tokens->size].value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_tokens value";

        return false;
      }
      
      tokens->size++;

      return true;

    default:
      unwanted_json_error_message = "Invalid JSON node passed to unparser";

      return false;
  }
}

unwanted_json_tokens* unwanted_json_unparse(unwanted_json_node* node) {
  size_t i;

  unwanted_json_tokens* tokens = NULL;

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

  if (unwanted_json_unparse_value(node, tokens) == false) {
    unwanted_json_cleanup_tokens(tokens);

    return NULL;
  }

  return tokens;
}

bool unwanted_json_parse_object(unwanted_json_node* node, unwanted_json_tokens* tokens, size_t* tokens_index, size_t node_level) {
  char* key = NULL;

  unwanted_json_token token = tokens->values[++(*tokens_index)];

  node->object_value_size = 0;
  node->object_value = malloc(sizeof(*node->object_value));

  if (node->object_value == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_node object_value";

    return false;
  }

  while (token.type != Token_BraceClose) {
    if (token.type == Token_String) {
      key = token.value;

      token = tokens->values[++(*tokens_index)];

      if (token.type != Token_Colon) {
        unwanted_json_error_message = "Failed to parse Colon token after unwanted_json_node object_value_key";

        return false;
      }

      token = tokens->values[++(*tokens_index)];

      if (node->object_value_size > 0) {
        node->object_value = realloc(node->object_value, (node->object_value_size + 1) * sizeof(*node->object_value));

        if (node->object_value == NULL) {
          unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_node object_value";

          return false;
        }
      }

      node->object_value[node->object_value_size] = unwanted_json_create_object();

      if (node->object_value[node->object_value_size] == NULL) {
        return false;
      }

      node->object_value[node->object_value_size]->object_value_key = strdup(key);

      if (node->object_value[node->object_value_size]->object_value_key == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_node object_value_key";

        return false;
      }

      if (unwanted_json_parse_value(node->object_value[node->object_value_size], tokens, tokens_index, node_level) == false) {
        return false;
      }

      node->object_value_size++;
    } else {
      unwanted_json_error_message = "Failed to parse token for unwanted_json_node object_value_key";

      return false;
    }

    token = tokens->values[++(*tokens_index)];

    if (token.type == Token_Comma) {
      token = tokens->values[++(*tokens_index)];
    }
  }

  return true;
}

bool unwanted_json_parse_array(unwanted_json_node* node, unwanted_json_tokens* tokens, size_t* tokens_index, size_t node_level) {
  unwanted_json_token token = tokens->values[++(*tokens_index)];

  node->array_value_size = 0;
  node->array_value = malloc(sizeof(*node->array_value));

  if (node->array_value == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_node array_value";

    return false;
  }

  while (token.type != Token_BracketClose) {
    if (node->array_value_size > 0) {
      node->array_value = realloc(node->array_value, (node->array_value_size + 1) * sizeof(*node->array_value));

      if (node->array_value == NULL) {
        unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_node array_value";

        return false;
      }
    }

    node->array_value[node->array_value_size] = unwanted_json_create_array();

    if (node->array_value[node->array_value_size] == NULL) {
      return false;
    }

    if (unwanted_json_parse_value(node->array_value[node->array_value_size], tokens, tokens_index, node_level) == false) {
      return false;
    }

    node->array_value_size++;

    token = tokens->values[++(*tokens_index)];

    if (token.type == Token_Comma) {
      token = tokens->values[++(*tokens_index)];
    }
  }

  return true;
}

bool unwanted_json_parse_value(unwanted_json_node* node, unwanted_json_tokens* tokens, size_t* tokens_index, size_t node_level) {
  unwanted_json_token token = tokens->values[*tokens_index];
  
  node->level = node_level;

  switch (token.type) {
    case Token_String:
      node->type = Node_String;
      node->string_value = strdup(token.value);

      if (node->string_value == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_node string_value";

        return false;
      } else {
        return true;
      }
    case Token_Number:
      node->type = Node_Number;
      node->number_value = atof(token.value);
      return true;
    case Token_True:
      node->type = Node_Boolean;
      node->boolean_value = true;
      return true;
    case Token_False:
      node->type = Node_Boolean;
      node->boolean_value = false;
      return true;
    case Token_Null:
      node->type = Node_Null;
      return true;
    case Token_BraceOpen:
      node->type = Node_Object;
      return unwanted_json_parse_object(node, tokens, tokens_index, node_level + 1);
    case Token_BracketOpen:
      node->type = Node_Array;
      return unwanted_json_parse_array(node, tokens, tokens_index, node_level + 1);
    default:
      unwanted_json_error_message = "Invalid JSON tokens passed to parser";

      return false;
  }
}

unwanted_json_node* unwanted_json_parse(unwanted_json_tokens* tokens) {
  size_t tokens_index = 0;

  unwanted_json_node* node = NULL;

  if (tokens == NULL || tokens->size == 0) {
    unwanted_json_error_message = "Invalid JSON tokens passed to parser";

    return NULL;
  }

  node = malloc(sizeof(*node));

  if (node == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_node";

    return NULL;
  }

  if (unwanted_json_parse_value(node, tokens, &tokens_index, 0) == false) {
    unwanted_json_cleanup_node(node);

    return NULL;
  }

  return node;
}



unwanted_json_node* unwanted_json_from_string(char* json_string) {
  unwanted_json_node* node = NULL;
  
  unwanted_json_tokens* tokens = NULL;

  tokens = unwanted_json_tokenize(json_string);

  if (tokens == NULL) {
    return NULL;
  }

  node = unwanted_json_parse(tokens);

  unwanted_json_cleanup_tokens(tokens);

  return node;
}

unwanted_json_node* unwanted_json_from_file(FILE* file) {
  unwanted_json_node* node = NULL;

  unwanted_json_tokens* tokens = NULL;

  tokens = unwanted_json_file_tokenize(file);

  if (tokens == NULL) {
    return NULL;
  }

  node = unwanted_json_parse(tokens);

  unwanted_json_cleanup_tokens(tokens);

  return node;
}

char* unwanted_json_to_string(unwanted_json_node* node) {
  char* json_string = NULL;

  unwanted_json_tokens* tokens = NULL;

  tokens = unwanted_json_unparse(node);

  if (tokens == NULL) {
    return NULL;
  }

  json_string = unwanted_json_untokenize(tokens);
  
  unwanted_json_cleanup_tokens(tokens);

  return json_string;
}

bool unwanted_json_to_file(unwanted_json_node* node, FILE* file) {
  bool result;

  unwanted_json_tokens* tokens = NULL;

  tokens = unwanted_json_unparse(node);

  if (tokens == NULL) {
    return NULL;
  }

  result = unwanted_json_file_untokenize(tokens, file);
  
  unwanted_json_cleanup_tokens(tokens);

  return result;
}



unwanted_json_node* unwanted_json_create_node(unwanted_json_node_type type) {
  unwanted_json_node* node = NULL;

  node = malloc(sizeof(*node));

  if (node == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_node";

    return NULL;
  }

  node->level = 0;
  node->type = type;

  return node;
}

unwanted_json_node* unwanted_json_create_array() {
  unwanted_json_node* node = NULL;

  node = unwanted_json_create_node(Node_Array);

  if (node == NULL) {
    return NULL;
  }

  node->array_value_size = 0;
  node->array_value = malloc(sizeof(*node->array_value));

  if (node->array_value == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_node values";

    free(node);

    return NULL;
  }

  return node;
}

unwanted_json_node* unwanted_json_create_object() {
  unwanted_json_node* node = NULL;

  node = unwanted_json_create_node(Node_Object);

  if (node == NULL) {
    return NULL;
  }

  node->object_value_size = 0;
  node->object_value = malloc(sizeof(*node->object_value));

  if (node->object_value == NULL) {
    unwanted_json_error_message = "Failed to allocate memory for unwanted_json_node values";

    free(node);

    return NULL;
  }

  return node;
}

unwanted_json_node** unwanted_json_set_node_by_key(unwanted_json_node* node, char* key, unwanted_json_node_type type, bool has_node) {
  size_t i;

  if (node == NULL || node->type != Node_Object) {
    unwanted_json_error_message = "Provided unwanted_json_node is not an object";

    return NULL;
  }

  for (i = 0; i < node->object_value_size; i++) {
    if (strcmp(node->object_value[i]->object_value_key, key) == 0) {
      if (has_node) {
        unwanted_json_cleanup_node(node->object_value[i]);
      } else {
        if (
          node->object_value[i]->type == Node_Array || 
          node->object_value[i]->type == Node_Object || 
          node->object_value[i]->type == Node_String
        ) {
          unwanted_json_cleanup_node_value(node->object_value[i]);
        }

        node->object_value[i]->type = type;
      }

      return &node->object_value[i];
    }
  }

  node->object_value = realloc(node->object_value, (node->object_value_size + 1) * sizeof(*node->object_value));

  if (node->object_value == NULL) {
    unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_node object_value";

    return NULL;
  }

  if (!has_node) {
    node->object_value[node->object_value_size] = unwanted_json_create_node(type);

    if (node->object_value[node->object_value_size] == NULL) {
      return NULL;
    }

    node->object_value[node->object_value_size]->object_value_key = strdup(key);
    
    if (node->object_value[node->object_value_size]->object_value_key == NULL) {
      unwanted_json_error_message = "Failed to set unwanted_json_node object_value_key";

      return NULL;
    }

    node->object_value[node->object_value_size]->level = node->level + 1;
  }

  node->object_value_size++;

  return &node->object_value[node->object_value_size - 1];
}

bool unwanted_json_set_string_by_key(unwanted_json_node* node, char* key, char* value) {
  char* data = NULL;

  unwanted_json_node** result_node = NULL;

  data = strdup(value);

  if (data == NULL) { 
    unwanted_json_error_message = "Failed to set value of provided string on unwanted_json_node";

    return false;
  }

  result_node = unwanted_json_set_node_by_key(node, key, Node_String, false);

  if (result_node == NULL) {
    return false;
  }

  (*result_node)->string_value = data;

  return true;
}

bool unwanted_json_set_number_by_key(unwanted_json_node* node, char* key, double value) {
  unwanted_json_node** result_node = NULL;

  result_node = unwanted_json_set_node_by_key(node, key, Node_Number, false);

  if (result_node == NULL) {
    return false;
  }

  (*result_node)->number_value = value;

  return true;
}

bool unwanted_json_set_boolean_by_key(unwanted_json_node* node, char* key, bool value) {
  unwanted_json_node** result_node = NULL;

  result_node = unwanted_json_set_node_by_key(node, key, Node_Boolean, false);

  if (result_node == NULL) {
    return false;
  }

  (*result_node)->boolean_value = value;

  return true;
}

bool unwanted_json_set_null_by_key(unwanted_json_node* node, char* key) {
  unwanted_json_node** result_node = NULL;

  result_node = unwanted_json_set_node_by_key(node, key, Node_Null, false);

  if (result_node == NULL) {
    return false;
  }

  return true;
}

void unwanted_json_set_nodes_level(unwanted_json_node* node, size_t node_level) {
  size_t i;

  node->level = node_level;

  switch (node->type) {
    case Node_Object:
      for (i = 0; i < node->object_value_size; i++) {
        unwanted_json_set_nodes_level(node->object_value[i], node_level + 1);
      }
    break;

    case Node_Array:
      for (i = 0; i < node->array_value_size; i++) {
        unwanted_json_set_nodes_level(node->array_value[i], node_level + 1);
      }
    break;
  }
}

bool unwanted_json_set_array_by_key(unwanted_json_node* node, char* key, unwanted_json_node* value) {
  char* data_key = NULL;

  unwanted_json_node** result_node = NULL;

  if (value->type != Node_Array) {
    unwanted_json_error_message = "Provided unwanted_json_node as value is not an array";

    return NULL;
  }

  data_key = strdup(key);

  if (data_key == NULL) {
    unwanted_json_error_message = "Failed to set unwanted_json_node object_value_key";

    return false;
  }

  result_node = unwanted_json_set_node_by_key(node, key, value->type, true);

  if (result_node == NULL) {
    return false;
  }

  *result_node = value;

  (*result_node)->object_value_key = data_key;

  unwanted_json_set_nodes_level(*result_node, node->level + 1);

  return true;
}

bool unwanted_json_set_object_by_key(unwanted_json_node* node, char* key, unwanted_json_node* value) {
  char* data_key = NULL;

  unwanted_json_node** result_node = NULL;

  if (value->type != Node_Object) {
    unwanted_json_error_message = "Provided unwanted_json_node as value is not an object";

    return NULL;
  }

  data_key = strdup(key);

  if (data_key == NULL) {
    unwanted_json_error_message = "Failed to set unwanted_json_node object_value_key";

    return false;
  }

  result_node = unwanted_json_set_node_by_key(node, key, value->type, true);

  if (result_node == NULL) {
    return false;
  }

  *result_node = value;

  (*result_node)->object_value_key = data_key;

  unwanted_json_set_nodes_level(*result_node, node->level + 1);

  return true;
}


unwanted_json_node** unwanted_json_set_node_by_index(unwanted_json_node* node, size_t index, unwanted_json_node_type type, bool has_node) {
  if (node == NULL || node->type != Node_Array) {
    unwanted_json_error_message = "Provided unwanted_json_node is not an array";

    return NULL;
  }

  if (index < 0 || index > node->array_value_size) {
    unwanted_json_error_message = "Invalid index provided for setting unwanted_json_node array value";

    return NULL;
  }
  
  if (index < node->array_value_size) {
    if (has_node) {
      unwanted_json_cleanup_node(node->object_value[index]);
    } else {
      if (
        node->array_value[index]->type == Node_Array || 
        node->array_value[index]->type == Node_Object || 
        node->array_value[index]->type == Node_String
      ) {
        unwanted_json_cleanup_node_value(node->object_value[index]);
      }

      node->object_value[index]->type = type;
    }

    return &node->array_value[index];
  }

  node->array_value = realloc(node->array_value, (node->array_value_size + 1) * sizeof(*node->array_value));

  if (node->array_value == NULL) {
    unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_node array_value";

    return NULL;
  }

  if (!has_node) {
    node->array_value[node->array_value_size] = unwanted_json_create_node(type);

    if (node->array_value[node->array_value_size] == NULL) {
      return NULL;
    }

    node->array_value[node->array_value_size]->level = node->level + 1;
  }

  node->array_value_size++;

  return &node->array_value[node->array_value_size - 1];
}

bool unwanted_json_set_string_by_index(unwanted_json_node* node, size_t index, char* value) {
  char* data = NULL;

  unwanted_json_node** result_node = NULL;

  data = strdup(value);

  if (data == NULL) { 
    unwanted_json_error_message = "Failed to set value of provided string on unwanted_json_node";

    return false;
  }

  result_node = unwanted_json_set_node_by_index(node, index, Node_String, false);

  if (result_node == NULL) {
    return false;
  }

  (*result_node)->string_value = data;

  return true;
}

bool unwanted_json_set_number_by_index(unwanted_json_node* node, size_t index, double value) {
  unwanted_json_node** result_node = NULL;

  result_node = unwanted_json_set_node_by_index(node, index, Node_Number, false);

  if (result_node == NULL) {
    return false;
  }

  (*result_node)->number_value = value;

  return true;
}

bool unwanted_json_set_boolean_by_index(unwanted_json_node* node, size_t index, bool value) {
  unwanted_json_node** result_node = NULL;

  result_node = unwanted_json_set_node_by_index(node, index, Node_Boolean, false);

  if (result_node == NULL) {
    return false;
  }

  (*result_node)->boolean_value = value;

  return true;
}

bool unwanted_json_set_null_by_index(unwanted_json_node* node, size_t index) {
  unwanted_json_node** result_node = NULL;

  result_node = unwanted_json_set_node_by_index(node, index, Node_Null, false);

  if (result_node == NULL) {
    return false;
  }

  return true;
}

bool unwanted_json_set_array_by_index(unwanted_json_node* node, size_t index, unwanted_json_node* value) {
  unwanted_json_node** result_node = NULL;

  if (value->type != Node_Array) {
    unwanted_json_error_message = "Provided unwanted_json_node as value is not an array";

    return NULL;
  }

  result_node = unwanted_json_set_node_by_index(node, index, value->type, true);

  if (result_node == NULL) {
    return false;
  }

  *result_node = value;

  unwanted_json_set_nodes_level(*result_node, node->level + 1);

  return true;
}

bool unwanted_json_set_object_by_index(unwanted_json_node* node, size_t index, unwanted_json_node* value) {
  unwanted_json_node** result_node = NULL;

  if (value->type != Node_Object) {
    unwanted_json_error_message = "Provided unwanted_json_node as value is not an object";

    return NULL;
  }

  result_node = unwanted_json_set_node_by_index(node, index, value->type, true);

  if (result_node == NULL) {
    return false;
  }

  *result_node = value;

  unwanted_json_set_nodes_level(*result_node, node->level + 1);

  return true;
}



bool unwanted_json_remove_value(unwanted_json_node*** node, size_t size, size_t index) {
  memset((*node) + index, 0, sizeof(**node));

	memmove((*node) + index, (*node) + index + 1, (size - index - 1) * sizeof(**node));

  *node = realloc(*node, (size - 1) * sizeof(**node));

  if (*node == NULL) {
    unwanted_json_error_message = "Provided re-allocate memory to remove unwanted_json_node value";

    return false;
  }

  return true;
}

bool unwanted_json_remove_by_key(unwanted_json_node* node, char* key) {
  size_t i;

  bool result;

  if (node == NULL || node->type != Node_Object) {
    unwanted_json_error_message = "Provided unwanted_json_node is not an object";

    return false;
  }

  for (i = 0; i < node->object_value_size; i++) {
    if (strcmp(node->object_value[i]->object_value_key, key) == 0) {
      if (
        node->object_value[i]->type == Node_Array || 
        node->object_value[i]->type == Node_Object || 
        node->object_value[i]->type == Node_String
      ) {
        unwanted_json_cleanup_node(node->object_value[i]);
      }

      result = unwanted_json_remove_value(&node->object_value, node->object_value_size, i);

      node->object_value_size--;

      return result;
    }
  }

  unwanted_json_error_message = "Invalid key provided for removing unwanted_json_node object value";

  return false;
}

bool unwanted_json_remove_by_index(unwanted_json_node* node, size_t index) {
  bool result;

  if (node == NULL || node->type != Node_Array) {
    unwanted_json_error_message = "Provided unwanted_json_node is not an array";

    return false;
  }

  if (index < 0 || index >= node->array_value_size) {
    unwanted_json_error_message = "Invalid index provided for removing unwanted_json_node array value";

    return false;
  }
  
  if (
    node->array_value[index]->type == Node_Array || 
    node->array_value[index]->type == Node_Object || 
    node->array_value[index]->type == Node_String
  ) {
    unwanted_json_cleanup_node(node->array_value[index]);
  }

  result = unwanted_json_remove_value(&node->array_value, node->array_value_size, index);

  node->array_value_size--;

  return result;
}



unwanted_json_node* unwanted_json_get_node_by_key(unwanted_json_node* node, char* key) {
  size_t i;

  if (node == NULL || node->type != Node_Object) {
    unwanted_json_error_message = "Provided unwanted_json_node is not an object";

    return NULL;
  }

  for (i = 0; i < node->object_value_size; i++) {
    if (strcmp(node->object_value[i]->object_value_key, key) == 0) {
      return node->object_value[i];
    }
  }

  unwanted_json_error_message = "Node with provided key not in provided unwanted_json_node";

  return NULL;
}

char* unwanted_json_get_string_by_key(unwanted_json_node* node, char* key) {
  char* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_key(node, key);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_String) {
    unwanted_json_error_message = "Node with provided key is not an unwanted_json_node string";

    return NULL;
  }

  result = strdup(result_node->string_value);

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided string unwanted_json_node";
  }

  return result;
}

double* unwanted_json_get_number_by_key(unwanted_json_node* node, char* key) {
  double* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_key(node, key);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Number) {
    unwanted_json_error_message = "Node with provided key is not an unwanted_json_node number";

    return NULL;
  }

  result = malloc(sizeof(*result));

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided number unwanted_json_node";
  } else {
    (*result) = result_node->number_value;
  }

  return result;
}

bool* unwanted_json_get_boolean_by_key(unwanted_json_node* node, char* key) {
  bool* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_key(node, key);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Boolean) {
    unwanted_json_error_message = "Node with provided key is not an unwanted_json_node boolean";

    return NULL;
  }

  result = malloc(sizeof(*result));

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided boolean unwanted_json_node";
  } else {
    (*result) = result_node->boolean_value;
  }

  return result;
}

bool* unwanted_json_get_null_by_key(unwanted_json_node* node, char* key) {
  bool* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_key(node, key);

  if (result_node == NULL) {
    return NULL;
  }

  result = malloc(sizeof(*result));

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided null unwanted_json_node";
  } else {
    (*result) = result_node->type != Node_Null ? false : true;
  }

  return result;
}

unwanted_json_node* unwanted_json_get_array_by_key(unwanted_json_node* node, char* key) {
  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_key(node, key);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Array) {
    unwanted_json_error_message = "Node with provided key is not an unwanted_json_node array";

    return NULL;
  }

  return result_node;
}

unwanted_json_node* unwanted_json_get_object_by_key(unwanted_json_node* node, char* key) {
  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_key(node, key);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Object) {
    unwanted_json_error_message = "Node with provided key is not an unwanted_json_node object";

    return NULL;
  }

  return result_node;
}

unwanted_json_node* unwanted_json_get_node_by_index(unwanted_json_node* node, size_t index) {
  if (node == NULL || node->type != Node_Array) {
    unwanted_json_error_message = "Provided unwanted_json_node is not an array";

    return NULL;
  }

  if (index < 0 || index >= node->array_value_size) {
    unwanted_json_error_message = "Node with provided index not in provided unwanted_json_node";

    return NULL;
  }

  return node->array_value[index];
}

char* unwanted_json_get_string_by_index(unwanted_json_node* node, size_t index) {
  char* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_index(node, index);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_String) {
    unwanted_json_error_message = "Node with provided index is not an unwanted_json_node string";

    return NULL;
  }

  result = strdup(result_node->string_value);

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided string unwanted_json_node";
  }

  return result;
}

double* unwanted_json_get_number_by_index(unwanted_json_node* node, size_t index) {
  double* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_index(node, index);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Number) {
    unwanted_json_error_message = "Node with provided index is not an unwanted_json_node number";

    return NULL;
  }

  result = malloc(sizeof(*result));

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided number unwanted_json_node";
  } else {
    (*result) = result_node->number_value;
  }

  return result;
}

bool* unwanted_json_get_boolean_by_index(unwanted_json_node* node, size_t index) {
  bool* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_index(node, index);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Boolean) {
    unwanted_json_error_message = "Node with provided index is not an unwanted_json_node boolean";

    return NULL;
  }

  result = malloc(sizeof(*result));

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided boolean unwanted_json_node";
  } else {
    (*result) = result_node->boolean_value;
  }

  return result;
}

bool* unwanted_json_get_null_by_index(unwanted_json_node* node, size_t index) {
  bool* result = NULL;

  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_index(node, index);

  if (result_node == NULL) {
    return NULL;
  }

  result = malloc(sizeof(*result));

  if (result == NULL) {
    unwanted_json_error_message = "Failed to extract value of provided null unwanted_json_node";
  } else {
    (*result) = result_node->type != Node_Null ? false : true;
  }

  return result;
}

unwanted_json_node* unwanted_json_get_array_by_index(unwanted_json_node* node, size_t index) {
  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_index(node, index);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Array) {
    unwanted_json_error_message = "Node with provided index is not an unwanted_json_node array";

    return NULL;
  }

  return result_node;
}

unwanted_json_node* unwanted_json_get_object_by_index(unwanted_json_node* node, size_t index) {
  unwanted_json_node* result_node = NULL;

  result_node = unwanted_json_get_node_by_index(node, index);

  if (result_node == NULL) {
    return NULL;
  }

  if (result_node->type != Node_Object) {
    unwanted_json_error_message = "Node with provided index is not an unwanted_json_node object";

    return NULL;
  }

  return result_node;
}
