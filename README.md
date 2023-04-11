# libconf

`libconf` is a C library for parsing configuration files in the format of `Key=Value` pairs. The library supports parsing values of various types, including strings, integers, longs, floats, and doubles.

## Building and Installing

`libconf` can be built using the included `makefile` file, which requires `make` and the `clang` C compiler. If you are on a Red Hat-based Linux distribution, you can install them using the following command:

```bash
sudo dnf -y install make clang clang-extra-tools # install build tools
sudo dnf -y install cmocka-devel                 # install external libraries
```

To build and install the library, run the following commands:

```bash
make && sudo make install 
```

This will build the library and install it to the default location (`/usr/local/lib`) along with the header files.

## License

`libconf` is released under the MIT License. See the `LICENSE` file for more information.

## Unit Tests

To run the unit tests, which require the `cmocka` library, run the following command from the build directory:

```bash
make test
```

## Usage

To use `libconf` in your project, include the header file in your source code:

```c
#include <libconf.h>
```

### Parsing Configuration Files

`libconf` is able to parse config files in a `Key=Value` manner. Lines starting with `#` are treated as comments. Leading and trailing 
whitespaces in the key and value are ignored. The following is an example of a valid configuration file:

```makefile
# This is a comment
name=John Doe
age=42
weight=73.5
```

To parse a configuration file, create a new `conf_data* ` object using the `conf_load` function:

```c
conf_data *data = conf_load("example.conf");
```

The `conf_load` function takes a single argument, which is the path to the configuration file. If the file cannot be opened, the function will return `NULL`.

### Getting Values

Once the configuration file has been parsed, you can retrieve values using the `conf_get_*` functions. The following functions are available:

```c
int conf_get_int(const conf_data* data, const char* key, int default_value)
long conf_get_long(const conf_data* data, const char* key, long default_value);
float conf_get_float(const conf_data* data, const char* key, float default_value);
double conf_get_double(const conf_data* data, const char* key, double default_value);
const char* conf_get_string(const conf_data* data, const char* key, const char* default_value);
char conf_get_char(const conf_data* data, const char* key, char default_value);
```

Each function takes three arguments. The first argument is a pointer to the `conf_data` object, the second argument is the key of the value to retrieve. If the key is not found, the function will return `NULL`. The third argument is the default value to return if the key is not found.

**Hint**: `int` values are stored as `long` values, so you can use the `conf_get_long` function to retrieve `int` values. The same applies to `float` and `double` values, which are stored as `double` values. However, make sure that the read value fits the type you are trying to store it in.

### Freeing Memory

When you are done using the `conf_data` object, you should free the memory using the `conf_free` function:

```c
conf_free(data);
```

### Example 

Here's an example of how to use `libconf` to parse a configuration file:

```c
#include <stdio.h>
#include <libconf.h>

int main(int argc, char *argv[]) {
  conf_parser_t *parser = conf_parser_new("example.conf");
  if (parser == NULL) {
    printf("Error: could not open configuration file.\n");
    return 1;
  }

  const char *name = conf_parser_get_string(parser, "name");
  int age = conf_parser_get_int(parser, "age");
  float weight = conf_parser_get_float(parser, "weight");

  printf("Name: %s\n", name);
  printf("Age: %d\n", age);
  printf("Weight: %f\n", weight);

  conf_parser_free(parser);

  return 0;
}
```

Assuming you have a configuration file named `example.conf` with the following contents:

```makefile
name=John Doe
age=42
weight=73.5
```

Running the program will output:

```makefile
Name: John Doe
Age: 42
Weight: 73.500000
```