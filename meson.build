project('tarea3','cpp',
        default_options : ['cpp_std=c++20'],  
        version : '0.0.2')

# Find Jack dependencies
jack_dep = dependency('jack')
sndfile_dep = dependency('sndfile')
boost_dep = dependency('boost', modules : ['program_options','system'])
google_benchmark_dep = dependency('benchmark', required : true)

all_deps = [jack_dep,sndfile_dep,boost_dep]
sources = files('main.cpp', 'jack_client.cpp','passthrough_client.cpp',
                'sndfile_thread.cpp','waitkey.cpp', 'filter_client.cpp', 'biquad.cpp', 'volume_controller.cpp', 'plf_nanotimer.h', 'stats_handler.h')

executable('tarea3',sources,dependencies:all_deps)

benchmark_tarea3=executable('benchmark_tarea3',
                            files('benchmark.cpp','biquad.cpp', 'jack_client.cpp', 'volume_controller.cpp', 'sndfile_thread.cpp'),
                            dependencies:[boost_dep, google_benchmark_dep, jack_dep, sndfile_dep],
                            cpp_args : ['-std=c++20'],
                            build_by_default : false)

benchmark('benchmark_suite', benchmark_tarea3)
