project('httpd', 'c',
    version : '0.1.0',
    default_options : [
      'buildtype=debugoptimized',
      'warning_level=0',
      'c_std=gnu99'
    ]
)

# 当前绝对路径
root_path = meson.current_source_dir()

httpd_inc = include_directories('sound_code')
httpd_sources = files(
  'sound_code/conf_file.c',
  'sound_code/http_dispose.c',
  'sound_code/log.c',
  'sound_code/socket.c',
  'sound_code/wrap_unix.c',
  'httpd.c'
)

adder_sources = files(
  'sound_code/adder.c',
)

message('root_path: ' + root_path + '/httpd/cgi-bin')

executable('httpd', 
  httpd_sources, 
  include_directories : httpd_inc, 
)

executable('adder', 
  adder_sources, 
  install : true,
  install_dir : root_path + '/httpd/cgi-bin/',
)

