#ifndef __UNWANTED_JSON_H__
#define __UNWANTED_JSON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

typedef enum {
  Token_String,
  Token_Number,
  Token_True,
  Token_False,
  Token_Null,
  Token_Comma,
  Token_Colon,
  Token_BraceOpen,
  Token_BraceClose,
  Token_BracketOpen,
  Token_BracketClose,
} unwanted_json_token_type;

typedef enum {
  Node_String,
  Node_Number,
  Node_Boolean,
  Node_Null,
  Node_Object,
  Node_Array,
} unwanted_json_node_type;

typedef struct unwanted_json_node unwanted_json_node;

typedef struct unwanted_json_token unwanted_json_token;

typedef struct unwanted_json_tokens unwanted_json_tokens;


char* unwanted_json_error();

void unwanted_json_cleanup_tokens(unwanted_json_tokens* tokens);

void unwanted_json_print_tokens(unwanted_json_tokens* tokens);

void unwanted_json_print_nodes(unwanted_json_node* node);

void unwanted_json_cleanup_nodes(unwanted_json_node* node);

unwanted_json_tokens* unwanted_json_unparse(unwanted_json_node* node);

unwanted_json_node* unwanted_json_parse(unwanted_json_tokens* tokens);

char* unwanted_json_untokenize(unwanted_json_tokens* tokens);

bool unwanted_json_file_untokenize(unwanted_json_tokens* tokens, FILE* file);

unwanted_json_tokens* unwanted_json_tokenize(char* json_string);

unwanted_json_tokens* unwanted_json_file_tokenize(FILE* file);

char* unwanted_json_get_string_by_key(unwanted_json_node* node, char* key);

double* unwanted_json_get_number_by_key(unwanted_json_node* node, char* key);

bool* unwanted_json_get_boolean_by_key(unwanted_json_node* node, char* key);

bool* unwanted_json_get_null_by_key(unwanted_json_node* node, char* key);

unwanted_json_node* unwanted_json_get_array_by_key(unwanted_json_node* node, char* key);

unwanted_json_node* unwanted_json_get_object_by_key(unwanted_json_node* node, char* key);

char* unwanted_json_get_string_by_index(unwanted_json_node* node, size_t index);

double* unwanted_json_get_number_by_index(unwanted_json_node* node, size_t index);

bool* unwanted_json_get_boolean_by_index(unwanted_json_node* node, size_t index);

bool* unwanted_json_get_null_by_index(unwanted_json_node* node, size_t index);

unwanted_json_node* unwanted_json_get_array_by_index(unwanted_json_node* node, size_t index);

unwanted_json_node* unwanted_json_get_object_by_index(unwanted_json_node* node, size_t index);

#endif
