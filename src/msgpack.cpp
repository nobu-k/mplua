#include <lua.hpp>

#include "packer.hpp"
#include "unpacker.hpp"

namespace msgpack {
namespace lua {

/**
 * class Packer {
 *   pack(object)
 *   packTable(table)
 *   packArray(array)
 * }
 */
int createPacker(lua_State* L) {
  return Packer::create(L);
}

/**
 * class Unpacker {
 *   feed()
 *   next()
 *   operator () -- equals to next()
 * }
 */
int createUnpacker(lua_State* L) {
  return Unpacker::create(L);
}

namespace {
const char* const MpLuaPkgName = "msgpack";
const struct luaL_Reg MpLuaLib[] = {
  {"Packer", &createPacker},
  {"Unpacker", &createUnpacker},
  {NULL, NULL}
};
} // namespace
} // namespace lua
} // namespace msgpack

extern "C" {
  /**
   * pack: automatically detect types
   * pack_#{TypeName}: pack with the specific type
   */
  int luaopen_msgpack(lua_State* L) {
    msgpack::lua::Packer::registerUserdata(L);
    msgpack::lua::Unpacker::registerUserdata(L);
    luaL_register(L, msgpack::lua::MpLuaPkgName, msgpack::lua::MpLuaLib);
    return 1;
  }
}
