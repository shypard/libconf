project('libconf-example', 'c', version : '1.0', license : 'MIT')
project_description  = 'Example project on how to use libconf'
project_source_files = [ 'example-1.c' ]

# ========================== Build Example ===========================

build_args = [
  '-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
]

project_target = executable(
    meson.project_name(),
    project_source_files,
    c_args : build_args,
    dependencies : dependency('libconf')
)