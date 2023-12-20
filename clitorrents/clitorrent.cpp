#include <iostream>
#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <chrono>
#include <thread>

// g++ -O3 clitorrent.cpp -o clitorrent -ltorrent-rasterbar -lpthread

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <magnet_link>" << std::endl;
        return 1;
    }

    lt::session s;

    std::string magnet_link = argv[1];

    lt::add_torrent_params p;
    lt::error_code ec;
    p = lt::parse_magnet_uri(magnet_link, ec);

    if (ec)
    {
        std::cout << "Error: " << ec.message() << std::endl;
        return 1;
    }

    p.save_path = "./";
    lt::torrent_handle h = s.add_torrent(p);

    while (!h.status().is_seeding)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Progress: " << h.status().progress * 100 << "%" << std::endl;
    }

    std::cout << "Download completed" << std::endl;

    return 0;
}
