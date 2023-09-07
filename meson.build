project('libconf', 'c', version: '1.0', license: 'MIT')
project_description = 'Lightweight C library for parsing configuration files using Key=Value format.'
project_source_files = [ 'source/libconf.c' ]
project_header_files = [ 'include/libconf.h' ]
project_test_files = ['tests/test_libconf.c']
public_headers = include_directories('include')

# ========================== Build Library ===========================

build_args = [
  '-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
]

project_target = shared_library(
    'conf',
    project_source_files,
    version : meson.project_version(),
    install : true,
    c_args : build_args,
    include_directories : public_headers,
)

# ========================== Install Library ==========================

install_headers(project_header_files)
pkg = import('pkgconfig')
pkg.generate(project_target, 
    name: 'libconf',
    description: project_description)

# ========================== Test Library =============================

cmocka_dep = dependency('cmocka')

test_target = executable(
    'test_libconf',
    [project_source_files, project_test_files],
    dependencies : [cmocka_dep],
    include_directories : public_headers,
)

test('cmocka unit tests', test_target)
