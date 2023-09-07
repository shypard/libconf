/**
 * @file libconf.h
 * @brief Library for reading configuration files in Key=Value format.
 *
 * This library provides functionality to read configuration files in Key=Value
 * format, such as Unix-style configuration files. Comments starting with '#'
 * are ignored. The library supports different data types such as integer, long,
 * float, double, string, and char.
 */

#ifndef LIBCONF_H
#define LIBCONF_H

#include <stdio.h>

/** Set the maximum length of key and string values */
#define MAX_KEY_LEN 128
#define MAX_VAL_LEN 256

/**
 * @brief Enumeration of supported data types.
 */
typedef enum {
	CONF_INT,	 /**< Integer type */
	CONF_LONG,	 /**< Long type */
	CONF_FLOAT,	 /**< Float type */
	CONF_DOUBLE, /**< Double type */
	CONF_STRING, /**< String type */
	CONF_CHAR	 /**< Char type */
} conf_type;

/**
 * @brief Union for storing different data types.
 */
typedef union {
	int	   ival; /**< Integer value */
	long   lval; /**< Long value */
	float  fval; /**< Float value */
	double dval; /**< Double value */
	char*  str;	 /**< String value */
	char   cval; /**< Char value */
} conf_value;

/**
 * @brief Struct for storing a key-value pair.
 */
typedef struct {
	char	   key[MAX_KEY_LEN]; /**< Key string */
	conf_type  type;			 /**< Data type */
	conf_value value;			 /**< Value */
} conf_pair;

/**
 * @brief Struct for storing configuration data.
 */
typedef struct {
	conf_pair* pairs; /**< Array of key-value pairs */
	int		   count; /**< Number of key-value pairs */
} conf_data;

/**
 * @brief Reads a configuration file and returns a pointer to the conf_data
 * struct.
 *
 * @param[in] filename Name of the configuration file.
 *
 * @return Pointer to the conf_data struct on success, NULL on failure.
 *
 * The conf_data struct should be freed using the conf_free() function when it
 * is no longer needed.
 */
extern conf_data* conf_load(const char* filename);

/**
 * @brief Frees the memory allocated by a conf_data struct.
 *
 * @param[in] data Pointer to the conf_data struct to free.
 *
 * This function should be called when the conf_data struct is no longer needed
 * to free the memory allocated by the struct and its members.
 */
extern void conf_free(conf_data* data);

/**
 * @brief Gets a pointer to a conf_pair struct for a given key.
 *
 * @param[in] data Pointer to the conf_data struct.
 * @param[in] key  Key string.
 *
 * @return Pointer to the conf_pair struct on success, NULL on failure.
 *
 * The conf_pair struct contains a value and a type. The type can be used to
 * determine which member of the conf_value union to use.
 */
extern const conf_pair* conf_get_pair(const conf_data* data, const char* key);

/**
 * @brief Gets the integer value associated with a given key.
 *
 * @param[in] data           Pointer to the conf_data struct.
 * @param[in] key            Key string.
 * @param[in] default_value  Default value to return if the key is not found or
 * the value is not an integer.
 *
 * @return Integer value associated with the key, or the default value if the
 * key is not found or the value is not an integer.
 */
extern int conf_get_int(const conf_data* data, const char* key,
						int default_value);

/**
 * @brief Gets the long value associated with a given key.
 *
 * @param[in] data           Pointer to the conf_data struct.
 * @param[in] key            Key string.
 * @param[in] default_value  Default value to return if the key is not found or
 * the value is not a long.
 *
 * @return Long value associated with the key, or the default value if the key
 * is not found or the value is not a long.
 */
extern long conf_get_long(const conf_data* data, const char* key,
						  long default_value);

/**
 * @brief Gets the float value associated with a given key.
 *
 * @param[in] data           Pointer to the conf_data struct.
 * @param[in] key            Key string.
 * @param[in] default_value  Default value to return if the key is not found or
 * the value is not a float.
 *
 * @return Float value associated with the key, or the default value if the key
 * is not found or the value is not a float.
 */
extern float conf_get_float(const conf_data* data, const char* key,
							float default_value);

/**
 * @brief Gets the double value associated with a given key.
 *
 * @param[in] data           Pointer to the conf_data struct.
 * @param[in] key            Key string.
 * @param[in] default_value  Default value to return if the key is not found or
 * the value is not a double.
 *
 * @return Double value associated with the key, or the default value if the key
 * is not found or the value is not a double.
 */
extern double conf_get_double(const conf_data* data, const char* key,
							  double default_value);

/**
 * @brief Gets the string value associated with a given key.
 *
 * @param[in] data           Pointer to the conf_data struct.
 * @param[in] key            Key string.
 * @param[in] default_value  Default value to return if the key is not found or
 * the value is not a string.
 *
 * @return String value associated with the key, or the default value if the key
 * is not found or the value is not a string.
 *
 * The returned string should be freed using the standard library function
 * free() when it is no longer needed.
 */
extern const char* conf_get_string(const conf_data* data, const char* key,
								   const char* default_value);

/**
 * @brief Gets the character value associated with a given key.
 *
 * @param[in] data           Pointer to the conf_data struct.
 * @param[in] key            Key string.
 * @param[in] default_value  Default value to return if the key is not found or
 * the value is not a character.
 *
 * @return Character value associated with the key, or the default value if the
 * key is not found or the value is not a character.
 */
extern char conf_get_char(const conf_data* data, const char* key,
						  char default_value);

#endif /* LIBCONF_H */
