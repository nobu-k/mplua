#ifndef MSGPACK_LUA_PACKER_HPP_
#define MSGPACK_LUA_PACKER_HPP_

namespace msgpack {
namespace lua {

class Buffer;

/**
 * Metatable for this class:
 * callback = nil or function (serialized string)
 */
class Packer {
private:
  Packer(const Packer&);
  Packer& operator =(const Packer&);

public:
  Packer(Buffer* buf);
  ~Packer();

  int pack(lua_State* L);
  Buffer* buffer() { return buffer_; }
  const Buffer* buffer() const { return buffer_; }
  lua_State* state() { return tmp_lua_state_; }

private:
  msgpack_packer packer_;
  Buffer* buffer_;

  lua_State* tmp_lua_state_;
};

} // namespace lua
} // namespace msgpack

#endif
