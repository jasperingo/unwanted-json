#ifndef __UNWANTED_JSON_H__
#define __UNWANTED_JSON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

unwanted_json_tokens* unwanted_json_file_tokenize(FILE* file);

#endif
