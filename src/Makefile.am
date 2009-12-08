pkglibdir = $(LUA_CMOD_DIR)

pkglib_LTLIBRARIES = msgpack.la

msgpack_la_SOURCES = \
  msgpack.cpp \
  packer.hpp \
  packer.cpp \
  buffer.hpp \
  buffer.cpp \
  unpacker.hpp \
  unpacker.cpp
msgpack_la_CXXFLAGS = \
  $(LUA_CFLAGS)
msgpack_la_LIBADD = \
  $(LUA_LIBS) -lmsgpack

# Because mplua.la does not start with "lib", module option is necessary.
msgpack_la_LDFLAGS = \
  -module