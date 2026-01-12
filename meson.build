project(
    'radio',
    ['c'],
    default_options: ['c_std=c99'],
    version: '1.0',
    license: 'GPL-2.0',
)

c_args = [
    '-Wall',
    '-Wextra',
    '-O2',
    '-D_GNU_SOURCE',
]

# libraries

app_deps = [
    dependency('tinyc'),
    #dependency('dbus-1'),
    #dependency('gdlib'),
    #dependency('gio-2.0'),
    #dependency('glib-2.0'),
    #dependency('gudev-1.0'),
    #dependency('gumbo'),
    #dependency('libcurl'),
    #dependency('libnotify'),
    #dependency('libpcre'),
    #dependency('libusb'),
    #dependency('libxml-2.0'),
    #dependency('polkit-gobject-1'),
]

app_sources = [
    'main.c',
]

executable(
    meson.project_name(),
    c_args: c_args,
    dependencies: app_deps,
    sources: app_sources,
    install: false,
)


