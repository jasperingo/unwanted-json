#include "unwanted_json.h"

#define FILE_LINE_BUFFER 5

struct unwanted_json_node {
  size_t level;
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

void unwanted_json_cleanup_node(unwanted_json_node* node) {
  size_t i;

  if (node != NULL) {
    switch (node->type) {
      case Node_String:
        free(node->string_value);
      break;

      case Node_Array:
       for (i = 0; i < node->array_value_size; i++) {
          unwanted_json_cleanup_node(&node->array_value[i]);
        }

        free(node->array_value);
      break;
      
      case Node_Object:
        for (i = 0; i < node->object_value_size; i++) {
          unwanted_json_cleanup_node(&node->object_value[i]);
        }

        free(node->object_value_key);
        free(node->object_value);
      break;
      
      default:
        break;
    }
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
        printf("%d) Object Node key: %s\n", node->object_value[i].level, node->object_value[i].object_value_key);
        unwanted_json_print_node(&node->object_value[i]);
      }
    break;

    case Node_Array:
      printf("%d) Array Node with length: %d\n", node->level, node->array_value_size);

      for (i = 0; i < node->array_value_size; i++) {
        printf("%d) Array Node Index: %d\n", node->array_value[i].level, i);
        unwanted_json_print_node(&node->array_value[i]);
      }
    break;
  }
}

void unwanted_json_print_nodes(unwanted_json_node* node) {
  if (node != NULL && node->level == 0) {
    printf("unwanted_json Nodes\n");

    unwanted_json_print_node(node);
  } else {
    printf("unwanted_json Node not root Node\n");
  }
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
        node->object_value = realloc(node->object_value, (node->object_value_size + 2) * sizeof(*node->object_value));

        if (node->object_value == NULL) {
          unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_node object_value";

          return false;
        }
      }

      node->object_value[node->object_value_size].object_value_key = strdup(key);

      if (node->object_value[node->object_value_size].object_value_key == NULL) {
        unwanted_json_error_message = "Failed to set unwanted_json_node object_value_key";

        return false;
      }

      if (unwanted_json_parse_value(&node->object_value[node->object_value_size], tokens, tokens_index, node_level) == false) {
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
      node->array_value = realloc(node->array_value, (node->array_value_size + 2) * sizeof(*node->array_value));

      if (node->array_value == NULL) {
        unwanted_json_error_message = "Failed to re-allocate memory for unwanted_json_node array_value";

        return false;
      }
    }

    if (unwanted_json_parse_value(&node->array_value[node->array_value_size], tokens, tokens_index, node_level) == false) {
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
