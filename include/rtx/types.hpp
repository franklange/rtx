#pragma once

#include <bln_net/udp_endpoint.hpp>
#include <bln_net/udp_socket.hpp>
#include <bln_net/udp_packet.hpp>

#include <cstdint>
#include <unordered_map>
#include <unordered_set>

namespace rtx {

using u32 = std::uint32_t;

using socket  = bln_net::udp::socket;
using packet  = bln_net::udp::packet;

using remote  = bln_net::udp::endpoint;
using remotes = std::unordered_set<remote, bln_net::udp::ep_hash>;

using topic   = std::string;
using topics  = std::unordered_map<topic, remotes>;

} // namespace rtx
