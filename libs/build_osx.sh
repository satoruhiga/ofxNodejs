#!/bin/sh

ROOT=$(cd $(dirname $0) && pwd)
cd $ROOT

if [ ! -d build ]; then
	mkdir build
fi

cd build

if [ ! -d node ]; then
	curl -L -O http://nodejs.org/dist/v0.10.35/node-v0.10.35.tar.gz
	tar xvzf node-v0.10.35.tar.gz
	mv node-v0.10.35 node
	rm node-v0.10.35.tar.gz
fi

cd node

cat << EOF >> patch.diff

--- node.gyp	2014-12-23 06:22:32.000000000 +0900
+++ node.gyp.changed	2015-01-18 04:54:06.000000000 +0900
@@ -61,7 +61,7 @@
   'targets': [
     {
       'target_name': 'node',
-      'type': 'executable',
+      'type': 'static_library',

       'dependencies': [
         'node_js2c#host',
@@ -85,7 +85,7 @@
         'src/node_file.cc',
         'src/node_http_parser.cc',
         'src/node_javascript.cc',
-        'src/node_main.cc',
+        # 'src/node_main.cc',
         'src/node_os.cc',
         'src/node_script.cc',
         'src/node_stat_watcher.cc',
@@ -158,7 +158,7 @@
               # Do not let unused OpenSSL symbols to slip away
               'xcode_settings': {
                 'OTHER_LDFLAGS': [
-                  '-Wl,-force_load,<(PRODUCT_DIR)/libopenssl.a',
+                  # '-Wl,-force_load,<(PRODUCT_DIR)/libopenssl.a',
                 ],
               },
               'conditions': [
@@ -293,7 +293,7 @@
           'xcode_settings': {
             'OTHER_LDFLAGS': [
-              '-Wl,-force_load,<(V8_BASE)',
+              # '-Wl,-force_load,<(V8_BASE)',
             ],
           },
         }],         

EOF

# apply patch
patch -u -f < patch.diff

./configure --dest-cpu x64
make -j4

# merge static library
libtool -static -o libnode.a ./out/Release/libcares.a ./out/Release/libchrome_zlib.a ./out/Release/libhttp_parser.a ./out/Release/libnode.a ./out/Release/libopenssl.a ./out/Release/libuv.a ./out/Release/libv8_base.a ./out/Release/libv8_nosnapshot.a ./out/Release/libv8_snapshot.a

# copy
mkdir -p ../../node/lib/osx/
cp libnode.a ../../node/lib/osx/libnode.a

mkdir -p ../../node/include
cp src/v8_typed_array.h ../../node/include/
cp src/node_buffer.h ../../node/include/
cp src/node_object_wrap.h ../../node/include/
cp src/node_stat_watcher.h ../../node/include/
cp src/node_string.h ../../node/include/
cp src/node_version.h ../../node/include/
cp src/node.h ../../node/include/

cp -R deps/cares/include/* ../../node/include/
cp -R deps/uv/include/* ../../node/include/
cp -R deps/http_parser/*.h ../../node/include/
cp -R deps/openssl/openssl/include/* ../../node/include/
cp -R deps/v8/include/* ../../node/include/
