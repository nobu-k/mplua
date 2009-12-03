#include "packer.hpp"

#include "buffer.hpp"

// TODO: check error codes of msgpack

namespace msgpack {
namespace lua {

Packer::Packer(Buffer* buf) : buffer_(buf), tmp_lua_state_(NULL) {
  buffer_->initPacker(this, &packer_);
}

Packer::~Packer() {
  delete buffer_;
}

int Packer::pack(lua_State* L) {
  tmp_lua_state_ = L;

  int n = lua_gettop(L); // the number of arguments
  for (int i = 2; i <= n; i++) { // The bottom is the packer its self
    pack(L, i);
  }
  
  tmp_lua_state_ = NULL;
  return buffer_->returnValues(L);
}

int Packer::packTable(lua_State* L) {
  tmp_lua_state_ = L;

  int n = lua_gettop(L);
  for (int i = 2; i <= n; i++) {
    int t = lua_type(L, i);
    if (t == LUA_TTABLE) {
      packTableAsTable(L, i);

    } else {
      // TODO: raise an error: invalid type
      return 0;
    }
  }

  tmp_lua_state_ = NULL;
  return buffer_->returnValues(L);
}

int Packer::packArray(lua_State* L) {
  tmp_lua_state_ = L;

  int n = lua_gettop(L);
  for (int i = 2; i <= n; i++) {
    int t = lua_type(L, i);
    if (t == LUA_TTABLE) {
      packTableAsArray(L, i);

    } else {
      // TODO: raise an error: invalid type
      return 0;
    }
  }

  tmp_lua_state_ = NULL;
  return buffer_->returnValues(L);
}

void Packer::pack(lua_State* L, int index) {
  int t = lua_type(L, index);
  switch (t) {
  case LUA_TNUMBER: packNumber(L, index); break;
  case LUA_TBOOLEAN: packBoolean(L, index); break;
  case LUA_TSTRING:  packString(L, index); break;
  case LUA_TTABLE: packTable(L, index); break;
  case LUA_TUSERDATA: packUserdata(L, index); break;

  case LUA_TNIL:
  case LUA_TTHREAD:
  case LUA_TLIGHTUSERDATA:
  default:
    // TODO: raise an error: invalid type
    break;
  }
}

void Packer::packNumber(lua_State* L, int index) {
  double n = lua_tonumber(L, index);
  int64_t i = static_cast<int64_t>(n);
  if (i == n) msgpack_pack_int64(&packer_, i);
  else msgpack_pack_double(&packer_, n);
}

void Packer::packBoolean(lua_State* L, int index) {
  int b = lua_toboolean(L, index);
  if (b == 0) msgpack_pack_false(&packer_);
  else msgpack_pack_true(&packer_);
}

void Packer::packString(lua_State* L, int index) {
  size_t len;
  const char* str = lua_tolstring(L, index, &len);
  if (str == NULL) {
    // TODO: raise an error
    return;
  }

  msgpack_pack_raw(&packer_, len);
  msgpack_pack_raw_body(&packer_, str, len);
}

void Packer::packTable(lua_State* L, int index) {
  // TODO: support serialize meta-method for Lua classes.

  // check if this is an array
  // NOTE: This code strongly depends on the internal implementation
  // of Lua5.1. The table in Lua5.1 consists of two parts: the array part
  // and the hash part. The array part is placed before the hash part.
  // Therefore, it is possible to obtain the first key of the hash part
  // by using the return value of lua_objlen as the argument of lua_next.
  // If lua_next return 0, it means the table does not have the hash part,
  // that is, the table is an array.
  //
  // Due to the specification of Lua, the table with non-continous integral
  // keys is detected as a table, not an array.

  bool isArray = false;
  size_t len = lua_objlen(L, index);
  lua_pushnumber(L, len);
  if (lua_next(L, index) == 0) isArray = true;
  else lua_pop(L, 2);

  if (isArray) packTableAsArray(L, index);
  else packTableAsTable(L, index);
}

void Packer::packTableAsTable(lua_State* L, int index) {
  // calc the size of the table
  // TODO: Make this faster!!
  size_t len = 0;
  lua_pushnil(L);
  while (lua_next(L, index) != 0) {
    len++; lua_pop(L, 1);
  }
  msgpack_pack_map(&packer_, len);

  int n = lua_gettop(L); // used as a positive index
  lua_pushnil(L);
  while (lua_next(L, index) != 0) {  
    pack(L, n + 1); // -2:key
    pack(L, n + 2); // -1:value
    lua_pop(L, 1); // removes value, keeps key for next iteration
  }
}

void Packer::packTableAsArray(lua_State* L, int index) {
  int n = lua_gettop(L);
  size_t len = lua_objlen(L, index);
  
  for (size_t i = 1; i <= len; i++) {
    lua_rawgeti(L, index, i);
    pack(L, n + 1);
    lua_pop(L, 1);
  }
}

void Packer::packTableAsClass(lua_State* L, int index) {
  // TODO: implement
}

void Packer::packUserdata(lua_State* L, int index) {
  // TODO: support userdata serialization.
  // Calling __serialize meta-method may be good.

  // TODO: raise an error temporally
}

} // namespace lua
} // namespace msgpack
