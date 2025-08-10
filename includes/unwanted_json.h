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
 * A node should be cleaned up when no longer used
 * Cleaning up a parent node will also clean up all it's child nodes
 */
typedef struct unwanted_json_node unwanted_json_node;

/**
 * Tokens used from serializing or deserializing nodes
 * These tokens can be extracted from a JSON string and be parsed into nodes
 * These tokens can be created from nodes to be converted to a JSON string
 * Tokens should be cleaned up when no longer used
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

/**
 * Converts JSON nodes to JSON tokens
 * @param node the node to be converted
 * @return tokens or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_tokens* unwanted_json_unparse(unwanted_json_node* node);

/**
 * Converts JSON tokens to JSON node
 * @param tokens the tokens to be converted
 * @return node or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_parse(unwanted_json_tokens* tokens);

/**
 * Converts JSON tokens to JSON string
 * This string should be freed when done with
 * @param tokens the tokens to be converted
 * @return string or NULL if error - call unwanted_json_error to get error message
 */
char* unwanted_json_untokenize(unwanted_json_tokens* tokens);

/**
 * Converts JSON tokens to JSON string and write it to a file
 * The file should be opened for writing before method call and closed after method call
 * @param tokens the tokens to be converted
 * @param file FILE to write the JSON string to
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_file_untokenize(unwanted_json_tokens* tokens, FILE* file);

/**
 * Converts a JSON string into JSON tokens
 * @param json_string the node to be converted
 * @return tokens or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_tokens* unwanted_json_tokenize(char* json_string);

/**
 * Reads the content of a JSON file and converts it to JSON tokens
 * The file should be opened for reading before method call and closed after method call
 * @param file the FILE to read the JSON string from
 * @return tokens or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_tokens* unwanted_json_file_tokenize(FILE* file);

/**
 * Converts a JSON string to a JSON node
 * This method combines the tokenization and the parsing process
 * @param json_string the string to be converted
 * @return node or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_from_string(char* json_string);

/**
 * Reads the content of a JSON file and converts it to a JSON node
 * The file should be opened for reading before method call and closed after method call
 * This method combines the tokenization and the parsing process
 * @param file the FILE to read the JSON string from
 * @return node or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_from_file(FILE* file);

/**
 * Converts a JSON node to a JSON string
 * This method combines the reverse parsing and the reverse tokenization process
 * @param node the node to be converted
 * @return string or NULL if error - call unwanted_json_error to get error message
 */
char* unwanted_json_to_string(unwanted_json_node* node);

/**
 * Converts JSON node to JSON string and write it to a file
 * The file should be opened for writing before method call and closed after method call
 * This method combines the reverse parsing and the reverse tokenization process
 * @param node the node to be converted
 * @param file FILE to write the JSON string to
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_to_file(unwanted_json_node* node, FILE* file);

/**
 * Creates a JSON node of type Array
 * @return node or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_create_array();

/**
 * Creates a JSON node of type Object
 * @return node or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_create_object();

/**
 * Removes a child JSON node from it's parent node of type Object
 * The child node is freed and any existing pointer to it shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_remove_by_key(unwanted_json_node* node, char* key);

/**
 * Sets the string value of a child JSON node on it's parent node of type Object
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @param value to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_string_by_key(unwanted_json_node* node, char* key, char* value);

/**
 * Sets the number value of a child JSON node on it's parent node of type Object
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @param value to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_number_by_key(unwanted_json_node* node, char* key, double value);

/**
 * Sets the boolean value of a child JSON node on it's parent node of type Object
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @param value to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_boolean_by_key(unwanted_json_node* node, char* key, bool value);

/**
 * Sets the null value of a child JSON node on it's parent node of type Object
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_null_by_key(unwanted_json_node* node, char* key);

/**
 * Sets the array value of a child JSON node on it's parent node of type Object
 * The existing child node is freed and any existing pointer to it shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @param value JSON node of type Array to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_array_by_key(unwanted_json_node* node, char* key, unwanted_json_node* value);

/**
 * Sets the object value of a child JSON node on it's parent node of type Object
 * The existing child node is freed and any existing pointer to it shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @param value JSON node of type Object to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_object_by_key(unwanted_json_node* node, char* key, unwanted_json_node* value);

/**
 * Removes a child JSON node from it's parent node of type Array
 * The child node is freed and any existing pointer to it shouldn't be used
 * @param node the parent node
 * @param key string key of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_remove_by_index(unwanted_json_node* node, size_t index);

/**
 * Sets the string value of a child JSON node on it's parent node of type Array
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key array index of the child node
 * @param value to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_string_by_index(unwanted_json_node* node, size_t index, char* value);

/**
 * Sets the number value of a child JSON node on it's parent node of type Array
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key array index of the child node
 * @param value to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_number_by_index(unwanted_json_node* node, size_t index, double value);

/**
 * Sets the boolean value of a child JSON node on it's parent node of type Array
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key array index of the child node
 * @param value to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_boolean_by_index(unwanted_json_node* node, size_t index, bool value);

/**
 * Sets the null value of a child JSON node on it's parent node of type Array
 * The existing values of the child node is freed and any existing pointer to them shouldn't be used
 * @param node the parent node
 * @param key array index of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_null_by_index(unwanted_json_node* node, size_t index);

/**
 * Sets the array value of a child JSON node on it's parent node of type Array
 * The existing child node is freed and any existing pointer to it shouldn't be used
 * @param node the parent node
 * @param key array index of the child node
 * @param value JSON node of type Array to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_array_by_index(unwanted_json_node* node, size_t index, unwanted_json_node* value);

/**
 * Sets the object value of a child JSON node on it's parent node of type Array
 * The existing child node is freed and any existing pointer to it shouldn't be used
 * @param node the parent node
 * @param key array index of the child node
 * @param value JSON node of type Object to set as value of the child node
 * @return true or false if error - call unwanted_json_error to get error message
 */
bool unwanted_json_set_object_by_index(unwanted_json_node* node, size_t index, unwanted_json_node* value);

/**
 * Gets the string value of a child JSON node from it's parent node of type Object
 * @param node the parent node
 * @param key string key of the child node
 * @return string or NULL if error - call unwanted_json_error to get error message
 */
char* unwanted_json_get_string_by_key(unwanted_json_node* node, char* key);

/**
 * Gets the number value of a child JSON node from it's parent node of type Object
 * @param node the parent node
 * @param key string key of the child node
 * @return pointer to double or NULL if error - call unwanted_json_error to get error message
 */
double* unwanted_json_get_number_by_key(unwanted_json_node* node, char* key);

/**
 * Gets the boolean value of a child JSON node from it's parent node of type Object
 * @param node the parent node
 * @param key string key of the child node
 * @return pointer to bool or NULL if error - call unwanted_json_error to get error message
 */
bool* unwanted_json_get_boolean_by_key(unwanted_json_node* node, char* key);

/**
 * Gets the null value of a child JSON node from it's parent node of type Object
 * @param node the parent node
 * @param key string key of the child node
 * @return pointer to bool - true if value is null else false, or NULL if error - call unwanted_json_error to get error message
 */
bool* unwanted_json_get_null_by_key(unwanted_json_node* node, char* key);

/**
 * Gets the array value of a child JSON node from it's parent node of type Object
 * @param node the parent node
 * @param key string key of the child node
 * @return JSON node of type Array or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_get_array_by_key(unwanted_json_node* node, char* key);

/**
 * Gets the object value of a child JSON node from it's parent node of type Object
 * @param node the parent node
 * @param key string key of the child node
 * @return JSON node of type Object or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_get_object_by_key(unwanted_json_node* node, char* key);

/**
 * Gets the string value of a child JSON node from it's parent node of type Array
 * @param node the parent node
 * @param index array index of the child node
 * @return string or NULL if error - call unwanted_json_error to get error message
 */
char* unwanted_json_get_string_by_index(unwanted_json_node* node, size_t index);

/**
 * Gets the number value of a child JSON node from it's parent node of type Array
 * @param node the parent node
 * @param index array index of the child node
 * @return pointer to double or NULL if error - call unwanted_json_error to get error message
 */
double* unwanted_json_get_number_by_index(unwanted_json_node* node, size_t index);

/**
 * Gets the boolean value of a child JSON node from it's parent node of type Array
 * @param node the parent node
 * @param index array index of the child node
 * @return pointer to bool or NULL if error - call unwanted_json_error to get error message
 */
bool* unwanted_json_get_boolean_by_index(unwanted_json_node* node, size_t index);

/**
 * Gets the null value of a child JSON node from it's parent node of type Array
 * @param node the parent node
 * @param index array index of the child node
 * @return pointer to bool - true if value is null else false, or NULL if error - call unwanted_json_error to get error message
 */
bool* unwanted_json_get_null_by_index(unwanted_json_node* node, size_t index);

/**
 * Gets the array value of a child JSON node from it's parent node of type Array
 * @param node the parent node
 * @param index array index of the child node
 * @return JSON node of type Array or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_get_array_by_index(unwanted_json_node* node, size_t index);

/**
 * Gets the object value of a child JSON node from it's parent node of type Array
 * @param node the parent node
 * @param index array index of the child node
 * @return JSON node of type Object or NULL if error - call unwanted_json_error to get error message
 */
unwanted_json_node* unwanted_json_get_object_by_index(unwanted_json_node* node, size_t index);

#endif
