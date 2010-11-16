pkglibdir = $(LUA_CMOD_DIR)

pkglib_LTLIBRARIES = msgpack.la

msgpack_la_SOURCES = \
  msgpack.cpp \
  lua_objects.hpp \
  lua_objects.cpp \
  packer.hpp \
  packer.cpp \
  packer_impl.hpp \
  packer_impl.cpp \
  unpacker.hpp \
  unpacker.cpp
msgpack_la_CXXFLAGS = \
  $(LUA_CFLAGS)
msgpack_la_LIBADD = \
  $(LUA_LIBS) -lmsgpack

# Because mplua.la does not start with "lib", module option is necessary.
msgpack_la_LDFLAGS = \
  -module