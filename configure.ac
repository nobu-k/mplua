#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT([mplua], [0.2.0], [nobu.k.jp+mplua@gmail.com])
AC_CONFIG_SRCDIR([src/msgpack.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LIBTOOL

# Expand prefix
expanded_prefix=$prefix
if test "x$prefix" = xNONE; then
  expanded_prefix=${ac_default_prefix}
fi

# Checks for compiler version
if $CC --version >/dev/null 2>/dev/null; then
  ax_prog_cc_version=`$CC --version | head -n 1`
else
  ax_prog_cc_version="unknown"
fi

# Checks for pkg-config
AC_PATH_PROGS(PKG_CONFIG, pkg-config, no)
if test "x$PKG_CONFIG" = xno; then
  AC_MSG_ERROR([pkg-config not found])
fi

# Checks for MessagePack
# TODO

# Checks for Lua
AC_ARG_WITH([lua],
  AS_HELP_STRING([--with-lua=Lua package name],
                 [used to look-up Lua package, like lua, lua5.1, etc.]),
  [lua_pkg_name="$withval"],
  [lua_pkg_name="lua"])

if ! $PKG_CONFIG $lua_pkg_name --exists; then
  lua_pkg_name="lua5.1"
  if ! $PKG_CONFIG $lua_pkg_name --exists; then
    AC_MSG_ERROR([Lua not found. Please install Lua or use --with-lua.])
  fi
fi

LUA_CFLAGS=`$PKG_CONFIG $lua_pkg_name --cflags`
LUA_LIBS=`$PKG_CONFIG $lua_pkg_name --libs`
LUA_CMOD_DIR=`$PKG_CONFIG $lua_pkg_name --define-variable=prefix=${expanded_prefix} --variable=INSTALL_CMOD`

AC_SUBST(LUA_CFLAGS)
AC_SUBST(LUA_LIBS)
AC_SUBST(LUA_CMOD_DIR)

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

# Default Compiler Option
if test x$CC = xgcc; then
  CFLAGS="$CFLAGS -O2 -g -Wswitch-enum -Wall -D_REENTRANT -Wformat=2 -pipe -D_FORTIFY_SOURCE=1 -Wcast-qual -Wcast-align -Wpointer-arith -Wwrite-strings -fno-omit-frame-pointer"
fi

if test x$CXX = xg++; then
  CXXFLAGS="$CXXFLAGS -O2 -g -Wswitch-enum -Wall -D_REENTRANT -Wformat=2 -pipe -D_FORTIFY_SOURCE=1 -Wcast-qual -Wcast-align -Wpointer-arith -Wwrite-strings -Woverloaded-virtual -fno-omit-frame-pointer"
fi

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

AC_MSG_RESULT([[
[Modules]
Lua: $lua_pkg_name
Lua C Module dir: $LUA_CMOD_DIR]])

AC_MSG_RESULT([[
[Build information]
Package:            $PACKAGE_STRING
build (compile on): $ac_cv_build
Compiler:           $CC
Compiler version:   $ax_prog_cc_version
CFLAGS='$CFLAGS'
CXXFLAGS='$CXXFLAGS']])