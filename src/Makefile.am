lib_LTLIBRARIES = mplua.la

mplua_la_SOURCES = \
  msgpack.cpp \
  packer.hpp \
  packer.cpp \
  buffer.hpp \
  buffer.cpp
mplua_la_CXXFLAGS = \
  $(LUA_CFLAGS)
mplua_la_LIBADD = \
  $(LUA_LIBS) -lmsgpack

# Because mplua.la does not start with "lib", module option is necessary.
mplua_la_LDFLAGS = \
  -module