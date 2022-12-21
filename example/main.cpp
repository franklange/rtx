#include <rtx/client.hpp>
#include <rtx/log.hpp>

#include <bln_net/udp_socket_asio.hpp>

#include <cstdint>
#include <string>
#include <thread>

auto main(int argc, char** argv) -> int
{
    const std::string topic{"foo.bar"};

    bln_net::udp::socket_asio s{static_cast<std::uint16_t>(std::stoul(argv[1]))};
    rtx::client c{s, {"127.0.0.1", 8000}};

    std::thread listener{[&c](){
        while (true)
            rtx_log(c.wait());
    }};

    std::thread cmdline{[&c](){
        std::string line;

        while (std::getline(std::cin, line))
            c.send(line);
    }};

    cmdline.join();
    listener.join();

    return 0;
}
