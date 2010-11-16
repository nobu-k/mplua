MessagePack for Lua
===================
Still writing...

Requirement
===========
MessagePack for C++ and liblua 5.1 is required.

Install
=======
Run ./configure && make && make install.

Usage
=====

Serialization
-------------

::

  require "msgpack"

  -- pack accepts variable length arguments.
  -- data is a string.
  data = msgpack.pack(1, 2, 3, "strings", {"a", "r", "r", "a", "y", "s"},
                      {t = "a", b = "l", e = "s"; 1, 2, 3, 4})

Deserialization
---------------

Simple deserialization::

  require "msgpack"
  a, b, c = msgpack.unpack(msgpack.pack(1, 2, 3))
  -- a == 1, b == 2, c == 3

  ar = msgpack.unpackToArray(msgpack.pack(1, 2, 3))
  -- ar[1] == 1, ar[2] == 2, ar[3] == 3

Stream deserialization::

  require "msgpack"

  u = msgpack.Unpacker()
  u:feed(data)

  for v in u do
    -- v has a serialized data
  end
