#### httpd
简单的http服务器，支持get与post请求，只能处理文本和图片文件，服务器的根目录为httpd目录，配置目录为conf目录，日志操作需要安装syslog或者syslog-ng。

#### install
```javascript
windows-cygwin-gcc 
meson build
cd build
ninja install

linux-gcc
meson build
cd build
ninja install
```

#### 简单请求
  1. 127.0.0.1:8888
  2. 127.0.0.1:8888/hi.txt
  3. 127.0.0.1:8888/cgi-bin/adder?111&333
  4. 127.0.0.1:8888/weiruan.jpg
