lib_LTLIBRARIES = libmsgpack-lua.la

libmsgpack_lua_la_includedir = $(includedir)/msgpack/lua
libmsgpack_lua_la_include_HEADERS = \
  lua_objects.hpp

libmsgpack_lua_la_SOURCES = \
  msgpack.cpp \
  lua_objects.hpp \
  lua_objects.cpp \
  packer.hpp \
  packer.cpp \
  packer_impl.hpp \
  packer_impl.cpp \
  unpacker.hpp \
  unpacker.cpp
libmsgpack_lua_la_CXXFLAGS = \
  $(LUA_CFLAGS)
libmsgpack_lua_la_LIBADD = \
  $(LUA_LIBS) -lmsgpack

# Because mplua.la does not start with "lib", module option is necessary.
libmsgpack_lua_la_LDFLAGS = \
  -module

install-exec-hook:
	ln -f -s $(libdir)/libmsgpack-lua.so $(LUA_CMOD_DIR)/msgpack.so
