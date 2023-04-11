# libconf

`libconf` is a C library for parsing configuration files in the format of `Key=Value` pairs. Lines starting with `#` are treated as comments. The library supports parsing values of various types, including strings, integers, longs, floats, and doubles.

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

TBD 

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