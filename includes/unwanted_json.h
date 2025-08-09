#ifndef __UNWANTED_JSON_H__
#define __UNWANTED_JSON_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

/**
 * A JSON node struct
 * JSON is made up of nodes from the root node down to the leaf nodes
 */
typedef struct unwanted_json_node unwanted_json_node;

/**
 * Tokens used from serializing or deserializing nodes
 * These tokens can be extracted from a JSON string and be parsed into nodes
 * These tokens can be created from nodes to be converted to a JSON string
 */
typedef struct unwanted_json_tokens unwanted_json_tokens;

/**
 * Get the last error message
 * @return error message
 */
char* unwanted_json_error();

/**
 * Prints a JSON node and its child nodes to the console
 * @param node the node to be printed
 */
void unwanted_json_print_nodes(unwanted_json_node* node);

/**
 * Prints the extracted JSON tokens to the console
 * @param tokens the tokens to be printed
 */
void unwanted_json_print_tokens(unwanted_json_tokens* tokens);

/**
 * Frees a JSON node and its child nodes
 * If you have a pointer to one of the child nodes of this node, set it to NULL as it is now invalid
 * @param node the node to be cleaned up
 */
void unwanted_json_cleanup_nodes(unwanted_json_node* node);

/**
 * Frees all JSON tokens
 * @param tokens the tokens to be cleaned up
 */
void unwanted_json_cleanup_tokens(unwanted_json_tokens* tokens);


unwanted_json_tokens* unwanted_json_unparse(unwanted_json_node* node);

unwanted_json_node* unwanted_json_parse(unwanted_json_tokens* tokens);

char* unwanted_json_untokenize(unwanted_json_tokens* tokens);

bool unwanted_json_file_untokenize(unwanted_json_tokens* tokens, FILE* file);

unwanted_json_tokens* unwanted_json_tokenize(char* json_string);

unwanted_json_tokens* unwanted_json_file_tokenize(FILE* file);


unwanted_json_node* unwanted_json_from_string(char* json_string);

unwanted_json_node* unwanted_json_from_file(FILE* file);

char* unwanted_json_to_string(unwanted_json_node* node);

bool unwanted_json_to_file(unwanted_json_node* node, FILE* file);


unwanted_json_node* unwanted_json_create_array();

unwanted_json_node* unwanted_json_create_object();

bool unwanted_json_remove_by_key(unwanted_json_node* node, char* key);

bool unwanted_json_set_string_by_key(unwanted_json_node* node, char* key, char* value);

bool unwanted_json_set_number_by_key(unwanted_json_node* node, char* key, double value);

bool unwanted_json_set_boolean_by_key(unwanted_json_node* node, char* key, bool value);

bool unwanted_json_set_null_by_key(unwanted_json_node* node, char* key);

bool unwanted_json_set_array_by_key(unwanted_json_node* node, char* key, unwanted_json_node* value);

bool unwanted_json_set_object_by_key(unwanted_json_node* node, char* key, unwanted_json_node* value);

bool unwanted_json_remove_by_index(unwanted_json_node* node, size_t index);

bool unwanted_json_set_string_by_index(unwanted_json_node* node, size_t index, char* value);

bool unwanted_json_set_number_by_index(unwanted_json_node* node, size_t index, double value);

bool unwanted_json_set_boolean_by_index(unwanted_json_node* node, size_t index, bool value);

bool unwanted_json_set_null_by_index(unwanted_json_node* node, size_t index);

bool unwanted_json_set_array_by_index(unwanted_json_node* node, size_t index, unwanted_json_node* value);

bool unwanted_json_set_object_by_index(unwanted_json_node* node, size_t index, unwanted_json_node* value);

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
