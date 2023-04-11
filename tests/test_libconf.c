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

/* Configuration file path */
#define CONF_PATH "test.conf"

/* Key definitions */
#define S_KEY "string_key"
#define I_KEY "int_key"
#define F_KEY "float_key"
#define D_KEY "double_key"
#define L_KEY "long_key"
#define S_KEY_WS_IN_VALUE "string_key_ws_in_value"
#define S_KEY_WS_IN_KEY_BEFORE "string_key_ws_in_key_before"
#define S_KEY_WS_IN_KEY_AFTER "string_key_ws_in_key_after"

/* Value definitions */
#define F_VALUE 3.14159
#define D_VALUE 2.71828
#define I_VALUE 42
#define L_VALUE 3000000000
#define S_VALUE "string value"

/* Various settings */
#define FLOAT_PRECISION 1e-6

/**
 * @brief Setup function for the tests. Creates a configuration file.
 */
static void setup(void)
{
	FILE* conf = fopen(CONF_PATH, "w");
	if (conf == NULL) {
		fail_msg("Failed to open file '%s'", CONF_PATH);
		exit(1);
	}

	fprintf(conf, "%s=%s\n", S_KEY, S_VALUE);
	fprintf(conf, "%s=%d\n", I_KEY, I_VALUE);
	fprintf(conf, "%s=%f\n", F_KEY, F_VALUE);
	fprintf(conf, "%s=%f\n", D_KEY, D_VALUE);
	fprintf(conf, "%s=%ld\n", L_KEY, L_VALUE);
	fprintf(conf, "%s= %s  \n", S_KEY_WS_IN_VALUE, S_VALUE);
	fprintf(conf, "%s =%s  \n", S_KEY_WS_IN_KEY_AFTER, S_VALUE);
	fprintf(conf, " %s=%s  \n", S_KEY_WS_IN_KEY_BEFORE, S_VALUE);
	fclose(conf);
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

static void test_conf_remove_whitespaces_in_value(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	const char* str_val = conf_get_string(conf, S_KEY_WS_IN_VALUE, "failed");
	assert_string_equal(str_val, S_VALUE);

	conf_free(conf);
}

static void test_conf_remove_whitespaces_in_key_before(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	const char* str_val =
		conf_get_string(conf, S_KEY_WS_IN_KEY_BEFORE, "failed");
	assert_string_equal(str_val, S_VALUE);

	conf_free(conf);
}

static void test_conf_remove_whitespaces_in_key_after(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	const char* str_val =
		conf_get_string(conf, S_KEY_WS_IN_KEY_AFTER, "failed");
	assert_string_equal(str_val, S_VALUE);

	conf_free(conf);
}

static void test_conf_parse_string(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	const char* str_val = conf_get_string(conf, S_KEY, "failed");
	assert_string_equal(str_val, S_VALUE);

	conf_free(conf);
}

static void test_conf_parse_integer(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	int int_val = conf_get_int(conf, I_KEY, -1);
	assert_int_equal(int_val, I_VALUE);

	conf_free(conf);
}

static void test_conf_parse_long(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	long long_val = conf_get_long(conf, L_KEY, -1);
	assert_int_equal(long_val, L_VALUE);

	conf_free(conf);
}

static void test_conf_parse_float(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	float float_val = conf_get_float(conf, F_KEY, -1.0);
	assert_float_equal(float_val, F_VALUE, FLOAT_PRECISION);

	conf_free(conf);
}

static void test_conf_parse_double(void** state)
{
	(void)state; /* unused */

	conf_data* conf = conf_load(CONF_PATH);
	assert_non_null(conf);

	double double_val = conf_get_double(conf, D_KEY, -1.0);
	assert_float_equal(double_val, D_VALUE, FLOAT_PRECISION);

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
		cmocka_unit_test(test_conf_remove_whitespaces_in_value),
		cmocka_unit_test(test_conf_remove_whitespaces_in_key_before),
		cmocka_unit_test(test_conf_remove_whitespaces_in_key_after),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
