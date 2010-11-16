/*
 * MessagePack for Lua
 *
 * Copyright (C) 2009 Nobuyuki Kubota 
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <lua.hpp>

#include "lua_objects.hpp"
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
// TODO: Modify these function to catch std::exception when
// fixing 'luaL_error with C++' problem.
/**
 * @brief pack function which is provided as a module function.
 */
int pack(lua_State* L) {
  sbuffer buffer;
  packer<sbuffer> pk(&buffer);
  LuaObjects obj(L, 1);

  obj.msgpack_pack(pk);
  lua_pushlstring(L, buffer.data(), buffer.size());
  return 1;
}

/**
 * @brief packTable function which is provided as a module function.
 */
int packTable(lua_State* L) {
  sbuffer buffer;
  packer<sbuffer> pk(&buffer);
  LuaObjects obj(L, 1);

  obj.packTable(pk);
  lua_pushlstring(L, buffer.data(), buffer.size());
  return 1;
}

/**
 * @brief packArray function which is provided as a module function.
 */
int packArray(lua_State* L) {
  sbuffer buffer;
  packer<sbuffer> pk(&buffer);
  LuaObjects obj(L, 1);

  obj.packArray(pk);
  lua_pushlstring(L, buffer.data(), buffer.size());
  return 1;
}

int unpack(lua_State* L) {
  
}

const char* const MpLuaPkgName = "msgpack";
const struct luaL_Reg MpLuaLib[] = {
  {"Packer", &createPacker},
  {"pack", &pack},
  {"packTable", &packTable},
  {"packArray", &packArray},
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
