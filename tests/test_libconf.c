/**
 * @file test_libconf.c
 * @brief Unit tests for the conf library functions.
 *
 * This file contains a set of unit tests for the functions of the conf library.
 * The tests use the cmocka library for testing and cover functions for loading
 * and parsing configuration files.
 *
 */

#include "libconf.h"

#include <errno.h>
#include <fcntl.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define CONF_PATH "test.conf"
#define FLOAT_VALUE 3.14159
#define DOUBLE_VALUE 2.71828
#define INT_VALUE 42
#define LONG_VALUE 3000000000
#define STRING_VALUE "string_value"

static void setup(void)
{
	FILE* f = fopen(CONF_PATH, "w");
	if (f == NULL) {
		fail_msg("Failed to open file '%s'", CONF_PATH);
		exit(1);
	}

	fprintf(f, "string_key=%s\n", STRING_VALUE);
	fprintf(f, "int_key=%d\n", INT_VALUE);
	fprintf(f, "float_key=%f\n", FLOAT_VALUE);
	fprintf(f, "double_key=%f\n", DOUBLE_VALUE);
	fprintf(f, "long_key=%ld\n", LONG_VALUE);
	fprintf(f, "string_key_whitespaces1=  %s  \n", STRING_VALUE);
	fprintf(f, "string_key_whitespaces2 =%s  \n", STRING_VALUE);
	fclose(f);
}

static void test_conf_load(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	conf_free(conf);
}

static void test_conf_load_invalid(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load("invalid.conf");
	assert_null(conf);
}

static void test_conf_parse_key_not_found(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	const char* str_val = conf_get_string(conf, "invalid_key", "failed");
	assert_string_equal(str_val, "failed");

	conf_free(conf);
}

static void test_conf_remove_whitespaces(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	const char* str_val1 =
		conf_get_string(conf, "string_key_whitespaces1", "failed");
	assert_string_equal(str_val1, STRING_VALUE);

	const char* str_val2 =
		conf_get_string(conf, "string_key_whitespaces2", "failed");
	assert_string_equal(str_val2, STRING_VALUE);
	conf_free(conf);
}

static void test_conf_parse_string(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	const char* str_val = conf_get_string(conf, "string_key", "failed");
	assert_string_equal(str_val, STRING_VALUE);

	conf_free(conf);
}

static void test_conf_parse_integer(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	int int_val = conf_get_int(conf, "int_key", -1);
	assert_int_equal(int_val, INT_VALUE);

	conf_free(conf);
}

static void test_conf_parse_long(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	long long_val = conf_get_long(conf, "long_key", -1);
	assert_int_equal(long_val, LONG_VALUE);

	conf_free(conf);
}

static void test_conf_parse_float(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	float float_val = conf_get_float(conf, "float_key", -1.0);
	assert_float_equal(float_val, FLOAT_VALUE, 1e-6);

	conf_free(conf);
}

static void test_conf_parse_double(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	double double_val = conf_get_double(conf, "double_key", -1.0);
	assert_float_equal(double_val, DOUBLE_VALUE, 1e-6);

	conf_free(conf);
}

int main(void)
{
	setup();

	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_conf_load),
		cmocka_unit_test(test_conf_load_invalid),
		cmocka_unit_test(test_conf_parse_string),
		cmocka_unit_test(test_conf_parse_integer),
		cmocka_unit_test(test_conf_parse_long),
		cmocka_unit_test(test_conf_parse_float),
		cmocka_unit_test(test_conf_parse_double),
		cmocka_unit_test(test_conf_parse_key_not_found),
		cmocka_unit_test(test_conf_remove_whitespaces),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
