#include "unpacker.hpp"

#include <memory>

namespace msgpack {
namespace lua {
namespace {
template<int (Unpacker::*Memfun)(lua_State*)>
int unpackerProxy(lua_State* L) {
  Unpacker* p =
    *static_cast<Unpacker**>(luaL_checkudata(L, 1, Unpacker::MetatableName));
  return (p->*Memfun)(L);
}
} // namespace

const char* const Unpacker::MetatableName = "msgpack.Unpacker";

void Unpacker::registerUserdata(lua_State* L) {
  luaL_newmetatable(L, msgpack::lua::Unpacker::MetatableName);
  
  // metatable.__index = metatable
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");

  // set __gc
  lua_pushcfunction(L, &Unpacker::finalizer);
  lua_setfield(L, -2, "__gc");

  // set __call
  lua_pushcfunction(L, &unpackerProxy<&Unpacker::next>);
  lua_setfield(L, -2, "__call");

  // register methods
  const struct luaL_Reg Methods[] = {
    {"next", &unpackerProxy<&Unpacker::next>},
    {"feed", &unpackerProxy<&Unpacker::feed>},
    {NULL, NULL}
  };
  luaL_register(L, NULL, Methods);
  lua_pop(L, 1);
}

int Unpacker::create(lua_State* L) {
  // TODO: check the argument of constructor (feeding Lua function)

  Unpacker** p = static_cast<Unpacker**>(lua_newuserdata(L, sizeof(Unpacker*)));
  luaL_getmetatable(L, Unpacker::MetatableName);
  lua_setmetatable(L, -2);
  *p = new Unpacker();
  return 1;
}

int Unpacker::finalizer(lua_State* L) {
  Unpacker* p =
    *static_cast<Unpacker**>(luaL_checkudata(L, 1, Unpacker::MetatableName));
  delete p;
  return 0;
}

Unpacker::Unpacker() {
}

Unpacker::~Unpacker() {
}

int Unpacker::feed(lua_State* L) {
  // TODO: check feeding function
  
  // check arguments first to avoid copying serialized data incompletely
  int n = lua_gettop(L);
  std::vector<std::pair<const char*, size_t> > strs;
  size_t total_size = 0;
  for (int i = 2; i <= n; i++) {
    size_t len;
    const char* v = luaL_checklstring(L, i, &len);
    strs.push_back(std::make_pair(v, len));
    total_size += len;
  }

  // copy strings to the buffer
  unpacker_.reserve_buffer(total_size);
  for (size_t i = 0, offs = 0; i < strs.size(); i++) {
    memcpy(unpacker_.buffer() + offs, strs[i].first, strs[i].second);
    offs += strs[i].second;
  }
  unpacker_.buffer_consumed(total_size);
  return 0;
}

int Unpacker::next(lua_State* L) {
  // feed data until execute returns true
  try {
    while (!unpacker_.execute()) {
      // TODO: check the argument for feeding function and call it
      // TODO: call the feeding function passed to constructor

      // FIXME: currently, this function simply returns
      lua_pushnil(L);
      return 1;
    }

  } catch (const msgpack::unpack_error& e) {
    // TODO: raise an error: deserialization failed
    return 0;
  }

  msgpack::object msg = unpacker_.data();
  std::auto_ptr<msgpack::zone> zone(unpacker_.release_zone());
  unpacker_.reset();
  return unpack(L, msg);
}

int Unpacker::each(lua_State* L) {
  // TODO: implement
  return 0;
}

int Unpacker::unpack(lua_State* L, const msgpack::object& msg) {
  try {
    namespace type = msgpack::type;
    switch (msg.type) {
    case type::BOOLEAN:
      lua_pushboolean(L, msg.via.boolean);
      return 1;

    // Lua internally uses double to represent integer. In addition,
    // lua_Integer is the alias of ptrdiff_t, which can be 32 bits.
    // Therefore, pushing (u)int64_t as double is the best way.
    case type::POSITIVE_INTEGER:
      lua_pushnumber(L, msg.via.u64);
      return 1;

    case type::NEGATIVE_INTEGER:
      lua_pushnumber(L, msg.via.i64);
      return 1;

    case type::DOUBLE:
      lua_pushnumber(L, msg.via.dec);
      return 1;

    case type::RAW:
      lua_pushlstring(L, msg.via.raw.ptr, msg.via.raw.size);
      return 1;

    case type::ARRAY: return unpackArray(L, msg.via.array);
    case type::MAP: return unpackTable(L, msg.via.map);
    case type::NIL:
    default:
      // TODO: raise an error
      return 0;
    }

  } catch (const msgpack::type_error& e) {
    // TODO: raise an error
    return 0;
  }
  return 0;
}

int Unpacker::unpackArray(lua_State* L, const msgpack::object_array& a) {
  lua_newtable(L);
  for (uint32_t i = 0; i < a.size; i++) {
    if (unpack(L, a.ptr[i]) <= 0) {
      // TODO: raise an error
      lua_pop(L, 1);
      return 0;
    }

    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

int Unpacker::unpackTable(lua_State* L, const msgpack::object_map& m) {
  lua_newtable(L);
  for (uint32_t i = 0; i < m.size; i++) {
    if (unpack(L, m.ptr[i].key) <= 0) {
      // TODO: raise an error
      lua_pop(L, 1);
      return 0;
    }

    if (unpack(L, m.ptr[i].val) <= 0) {
      // TODO: raise an error
      lua_pop(L, 2);
      return 0;
    }

    lua_rawset(L, -3);
  }
  return 1;
}

} // namespace lua
} // namespace msgpack
