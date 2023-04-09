/**
 * @file example-1.c
 * @brief Example usage of libconf library to read and parse a configuration
 * file
 *
 * This file demonstrates the usage of the libconf library to read and parse a
 * configuration file. It reads a configuration file from the command line
 * arguments and retrieves integer, string, float, and double values from it.
 * Finally, it prints out the values.
 *
 */

#include "../include/libconf.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	if (argc <= 1) {
		fprintf(stderr, "Error: No config file given.\n");
		return -1;
	}

	/* Read in, and parse conf file */
	conf_data* data = conf_load(argv[1]);
	if (data == NULL) {
		fprintf(stderr, "Error: Could not parse configuration file.\n");
		return -1;
	}

	/* Get values from conf file */
	int			ival = conf_get_int(data, "int_val", 0);
	const char* sval = conf_get_string(data, "string_val", "");
	float		fval = conf_get_float(data, "float_val", 0.0);
	double		dval = conf_get_double(data, "double_val", 0.0);

	/* Print values */
	printf("ival=%d, sval=%s, fval=%f, dval=%f\n", ival, sval, fval, dval);
	conf_free(data);
	return 0;
}
