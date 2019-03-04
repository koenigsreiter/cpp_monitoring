# uncomment as necessary
project('simonmon', 'cpp',
        license : ['Boost'],
        version : '0.1',
        default_options : ['cpp_std=c++17', 'warning_level=3', 'werror=true'])

thread_dep = dependency('threads')

# begin asio: https://think-async.com/Asio/
add_global_arguments('-I' + get_option('asio_include_dir'), language : 'cpp')
add_global_arguments('-DASIO_STANDALONE', language : 'cpp')
add_global_arguments('-DASIO_NO_DEPRECATED', language : 'cpp')
# end asio

# clipp: https://github.com/muellan/clipp/
#add_global_arguments('-I' + get_option('clipp_include_dir'), language : 'cpp')

# CLI11: https://github.com/CLIUtils/CLI11
add_global_arguments('-I' + get_option('cli11_include_dir'), language : 'cpp')

# spdlog: https://github.com/gabime/spdlog
add_global_arguments('-I' + get_option('spdlog_include_dir'), language : 'cpp')

# json: https://github.com/nlohmann/json
add_global_arguments('-I' + get_option('json_include_dir'), language : 'cpp')

# type_safe: https://github.com/foonathan/type_safe
add_global_arguments('-I' + get_option('type_safe_include_dir'), language : 'cpp')

# # begin protobuf: https://developers.google.com/protocol-buffers/
# # protobuf: must be installed independently and has to be found...
protoc = find_program('protoc', required : true)
grpc_cpp_plugin = find_program('grpc_cpp_plugin', required : true)
protobuf_dep = dependency('protobuf', required : true)
grpc_dep = dependency('grpc', required : true)
grpcpp_dep = dependency('grpc++', required : true)

gen = generator(protoc, \
  output    : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h', '@BASENAME@.grpc.pb.cc', '@BASENAME@.grpc.pb.h'], \
  arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/src', 
               '--plugin=protoc-gen-grpc=' + grpc_cpp_plugin.path(),
               '--grpc_out=@BUILD_DIR@',
               '--cpp_out=@BUILD_DIR@', 
               '@CURRENT_SOURCE_DIR@/src/@PLAINNAME@'])

# change proto file accordingly
generated = gen.process('src/messages.proto')
# end protobuf

# subdir('SUBDIR1')

inc_dir = include_directories('include/common')
inc_server = include_directories('include/server')
inc_client = include_directories('include/client')

src_client = ['src/client/main.cpp', 'src/client/json_parser.cpp', generated]
src_server = ['src/server/main.cpp', 'src/server/health_checker.cpp', 
              'src/server/monitor.cpp', 'src/server/tcp_monitor.cpp', generated]

executable('monitoring_client',
           sources : src_client,
           include_directories : [inc_dir, inc_client],
           dependencies : [thread_dep, protobuf_dep, grpc_dep, grpcpp_dep]
          )

executable('monitoring_server',
           sources : src_server,
           include_directories : [inc_dir, inc_server],
           dependencies : [thread_dep, protobuf_dep, grpc_dep, grpcpp_dep]
          )
