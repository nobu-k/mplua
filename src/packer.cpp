#include "packer.hpp"

namespace msgpack {
namespace lua {

Packer::Packer(Buffer* buf) : buffer_(buf), tmp_lua_state_(NULL) {
  buffer_->initPacker(this, &packer_);
}

Packer::~Packer() {
  delete buffer_;
}

int Packer::pack(lua_State* L) {
}

} // namespace lua
} // namespace msgpack
