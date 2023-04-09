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
	char line[512];
	while (fgets(line, sizeof(line), fp)) {
		// Ignore comments
		if (line[0] == '#') {
			continue;
		}

		// Parse key-value pairs
		char* pos = strchr(line, '=');
		if (!pos) {
			continue;
		}

		// Allocate space for the new pair and copy the key
		conf_pair* pair = (conf_pair*)malloc(sizeof(conf_pair));
		if (!pair) {
			fclose(fp);
			conf_free(data);
			perror("Failed to allocate memory");
			return NULL;
		}
		strncpy(pair->key, line, pos - line);
		pair->key[pos - line] = '\0';

		// Determine the type of the value
		char*  end;
		double dval = strtod(pos + 1, &end);
		if (*end == '\n' || *end == '\0') {
			if ((long long)dval == dval) {
				// The value is an integer or a long
				if (dval > INT_MAX || dval < INT_MIN) {
					pair->type		 = CONF_LONG;
					pair->value.lval = (long long)dval;
				} else {
					pair->type		 = CONF_INT;
					pair->value.ival = (int)dval;
				}
			} else {
				// The value is a float or a double
				pair->type		 = CONF_DOUBLE;
				pair->value.dval = dval;
			}
		} else {
			// The value is a string, remove starting and trailing
			pair->type = CONF_STRING;
			size_t len = strlen(pos + 1) > MAX_VALUE_LENGTH ? MAX_VALUE_LENGTH
															: strlen(pos + 1);
			pair->value.str = (char*)malloc(len);
			if (!pair->value.str) {
				fclose(fp);
				conf_free(data);
				perror("Failed to allocate memory");
				return NULL;
			}
			strncpy(pair->value.str, pos + 1, len);
			pair->value.str[len - 1] = '\0';
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
	if (data) {
		if (data->pairs) {
			for (int i = 0; i < data->count; i++) {
				if (data->pairs[i].type == CONF_STRING) {
					free(data->pairs[i].value.str);
				}
			}
			free(data->pairs);
		}
		free(data);
	}
}

const conf_pair* conf_get_pair(const conf_data* data, const char* key)
{
	if (data && data->pairs && key) {
		for (int i = 0; i < data->count; i++) {
			if (strcmp(data->pairs[i].key, key) == 0) {
				return &data->pairs[i];
			}
		}
	}
	return NULL;
}

int conf_get_int(const conf_data* data, const char* key, int default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	if (pair && pair->type == CONF_INT) {
		return pair->value.ival;
	}
	return default_value;
}

long conf_get_long(const conf_data* data, const char* key, long default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	if (pair && pair->type == CONF_LONG) {
		return pair->value.lval;
	}
	return default_value;
}

float conf_get_float(const conf_data* data, const char* key,
					 float default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	if (pair && (pair->type == CONF_FLOAT)) {
		return pair->value.fval;
	}
	if (pair && (pair->type == CONF_DOUBLE)) {
		return (float)pair->value.dval;
	}
	return default_value;
}

double conf_get_double(const conf_data* data, const char* key,
					   double default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	if (pair && pair->type == CONF_DOUBLE) {
		return pair->value.dval;
	}
	return default_value;
}

const char* conf_get_string(const conf_data* data, const char* key,
							const char* default_value)
{
	const conf_pair* pair = conf_get_pair(data, key);
	if (pair && pair->type == CONF_STRING) {
		return pair->value.str;
	}
	return default_value;
}
