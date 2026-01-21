project(
    'radio',
    ['c'],
    default_options: ['c_std=c99'],
    version: '1.0',
    license: 'GPL-2.0',
)

bash_completion = configure_file(
    input: 'radio.comp',
    output: 'radio',
    copy: true,
    #configuration: conf_data
)

bash_comp = dependency('bash-completion', required: false)
if bash_comp.found()
    bash_compdir = bash_comp.get_variable(pkgconfig: 'completionsdir')
else
    bash_compdir = join_paths(datadir, 'bash-completion', 'completions')
endif

c_args = [
    '-Wall',
    '-Wextra',
    '-O2',
    '-D_GNU_SOURCE',
]

# libraries

app_deps = [
    dependency('tinyc'),
]

app_sources = [
    'main.c',
]

executable(
    meson.project_name(),
    c_args: c_args,
    dependencies: app_deps,
    sources: app_sources,
    install: true,
)

install_data(bash_completion, install_dir: bash_compdir)

