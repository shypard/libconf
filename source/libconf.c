/**
 * @file libconf.c
 * @brief Implementation for the libconf library for reading configuration files
 * in Key=Value format.
 *
 * This library provides functionality to read configuration files in Key=Value
 * format, such as Unix-style configuration files. Comments starting with '#'
 * are ignored. The library supports different data types such as integer, long,
 * float, double, string, and char.
 */

#include "libconf.h"

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

conf_data* conf_load(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		perror("Failed to open file");
		return NULL;
	}

	// Allocate space for the conf_data struct and initialize it
	conf_data* data = (conf_data*)malloc(sizeof(conf_data));
	if (!data) {
		fclose(fp);
		perror("Failed to allocate memory");
		return NULL;
	}
	data->count = 0;
	data->pairs = NULL;

	// Read the file line by line
	char   line[512];
	char*  key	   = line;
	size_t key_len = 0;
	while (fgets(line, sizeof(line), fp)) {
		// Ignore comments
		if (line[0] == '#') {
			continue;
		}

		// Parse key-value pairs and set the length of the key
		char* pos = strchr(line, '=');
		if (!pos) {
			continue;
		}
		key_len = pos - line;

		// Allocate space for the new pair and copy the key
		conf_pair* pair = (conf_pair*)malloc(sizeof(conf_pair));
		if (!pair) {
			fclose(fp);
			conf_free(data);
			perror("Failed to allocate memory");
			return NULL;
		}

		// Remove leading and trailing spaces from the key
		while (isspace(*key)) {
			key++;
			key_len--;
		}
		while (isspace(key[key_len - 1])) {
			key_len--;
		}

		// Copy the key to the pair
		strncpy(pair->key, key, key_len);
		pair->key[key_len] = '\0';

		// Determine the type of the value
		char*  end;
		double dval = strtod(pos + 1, &end);
		if (*end == '\n' || *end == '\0') {
			// The value is an integer or a long
			if ((long long)dval == dval) {
				pair->type		 = CONF_LONG;
				pair->value.lval = (long long)dval;
			} else {
				// The value is a float or a double
				pair->type		 = CONF_DOUBLE;
				pair->value.dval = dval;
			}
		} else {
			// The value is a string, remove starting and trailing
			pair->type = CONF_STRING;
			size_t len =
				strlen(pos + 1) > MAX_VAL_LEN ? MAX_VAL_LEN : strlen(pos + 1);

			pair->value.str = (char*)malloc(len);
			if (!pair->value.str) {
				fclose(fp);
				conf_free(data);
				perror("Failed to allocate memory");
				return NULL;
			}

			// remove leading spaces and '='
			while (isspace(*pos) || *pos == '=') {
				pos++;
				len--;
			}

			// remove trailing spaces
			while (isspace(pos[len - 1])) {
				len--;
			}

			strncpy(pair->value.str, pos, len);
			pair->value.str[len] = '\0';
		}

		// Add the new pair to the array
		conf_pair* new_pairs = (conf_pair*)realloc(
			data->pairs, sizeof(conf_pair) * (data->count + 1));
		if (!new_pairs) {
			fclose(fp);
			conf_free(data);
			return NULL;
		}
		data->pairs				 = new_pairs;
		data->pairs[data->count] = *pair;
		data->count++;
		free(pair);
	}

	fclose(fp);
	return data;
}

void conf_free(conf_data* data)
{
	if (!data) return;

	/* Iterate through all config pairs and free them*/
	for (int i = 0; i < data->count; i++) {
		if (data->pairs[i].type == CONF_STRING) {
			free(data->pairs[i].value.str);
		}
	}

	/* Free the array of pairs and the conf_data struct */
	free(data->pairs);
	free(data);
}

const conf_pair* conf_get_pair(const conf_data* data, const char* key)
{
	if (!data || !key || !data->pairs) return NULL;

	/* Iterate through all pairs and return the pair w/ correct key */
	for (int i = 0; i < data->count; i++) {
		if (strcmp(data->pairs[i].key, key) == 0) {
			return &data->pairs[i];
		}
	}

	/* No pair with the given key was found */
	return NULL;
}

int conf_get_int(const conf_data* data, const char* key, int default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);

	if (!pair) return default_value;

	/* Check the correct value type */
	switch (pair->type) {
	case CONF_INT:
		return pair->value.ival;
	case CONF_LONG:
		return (int)pair->value.lval;
	default:
		return default_value;
	}
}

long conf_get_long(const conf_data* data, const char* key, long default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	return (pair && pair->type == CONF_LONG) ? pair->value.lval : default_value;
}

float conf_get_float(const conf_data* data, const char* key,
					 float default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);

	if (!pair) return default_value;

	/* Check the correct value type */
	switch (pair->type) {
	case CONF_FLOAT:
		return (float)pair->value.fval;
	case CONF_DOUBLE:
		return (float)pair->value.dval;
	default:
		return default_value;
	}
}

double conf_get_double(const conf_data* data, const char* key,
					   double default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	return (pair && pair->type == CONF_DOUBLE) ? pair->value.dval
											   : default_value;
}

const char* conf_get_string(const conf_data* data, const char* key,
							const char* default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	return (pair && pair->type == CONF_STRING) ? pair->value.str
											   : default_value;
}
