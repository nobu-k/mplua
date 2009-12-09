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

#ifndef MSGPACK_LUA_BUFFER_HPP_
#define MSGPACK_LUA_BUFFER_HPP_

#include <vector>
#include <lua.hpp>
#include <msgpack.hpp>

namespace msgpack {
namespace lua {

class Packer;

class Buffer {
public:
  virtual ~Buffer();

  /**
   * @param p Packer* whose buffer() == this
   * @param packer msgpack_packer to be used
   */
  virtual void initPacker(Packer* p, msgpack_packer* packer) = 0;

  /**
   * @return The number of upvalues
   *
   * L has arguments of msgpack.Packer() on its stack.
   */
  virtual int initUpvalues(lua_State* L) = 0;

  /**
   * @return The number of return values
   */
  virtual int returnValues(lua_State* L) = 0;
};

/**
 * @brief A buffer class which directly returns the serialized data
 */
class DirectBuffer : public Buffer {
public:
  DirectBuffer();
  virtual ~DirectBuffer();

  virtual void initPacker(Packer* p, msgpack_packer* packer);
  virtual int initUpvalues(lua_State* L);
  virtual int returnValues(lua_State* L);

private:
  static int callback(void* data, const char* buf, unsigned int len);
  
private:
  std::vector<char> retval_;
};

/**
 * @brief A buffer class which redirects the serialized data to
 * the callback function defined in Lua.
 */
class LuaBuffer : public Buffer {
public:
  LuaBuffer();
  virtual ~LuaBuffer();

  virtual void initPacker(Packer* p, msgpack_packer* packer);
  virtual int initUpvalues(lua_State* L);
  virtual int returnValues(lua_State* L);

private:
  static int callback(void* data, const char* buf, unsigned int len);
};

} // namespace lua
} // namespace msgpack

#endif
