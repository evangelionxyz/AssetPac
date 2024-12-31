#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>

#include "asset.hpp"
#include "asset_packer.hpp"

using namespace pac;

const char HEADER[4] = {'O', 'R', 'G', 'N'};
const char AUTHOR[4] = {'E', 'V', 'A', 'N'};
const float VERSION = 0.1f;

void serialize(const std::string &filepath, const std::vector<std::unique_ptr<Asset>> &assets)
{
    std::ofstream game_asset(filepath, std::ios::binary | std::ios::trunc);

    if (game_asset.is_open())
    {
        game_asset.write(HEADER, sizeof(HEADER));
        game_asset.write(AUTHOR, sizeof(AUTHOR));
        game_asset.write(reinterpret_cast<const char*>(&VERSION), sizeof(VERSION));

        for (const auto &asset : assets)
        {
            asset->serialize(game_asset);
        }

        game_asset.close();

        std::cout << "Game asset written as binary data.\n";
    } else {
        std::cerr << "Failed to open as binary file.\n";
    }
}

void deserialize(const std::string &filepath, std::vector<std::unique_ptr<Asset>> &assets)
{
    std::ifstream game_asset(filepath, std::ios::binary);
    if (game_asset.is_open())
    {
        // header
        char header[5];
        game_asset.read(header, 4);
        header[4] = '\0';
        if(std::strncmp(header, "ORGN", 4) != 0)
        {
            std::cerr << "Invalid file format.\n";
            return;
        }

        std::cout << "Header: " << header << '\n';

        // game name
        char author[5];
        game_asset.read(author, 4);
        author[4] = '\0';
        std::cout << "Author: " << author << '\n';

        // File version
        float version;
        game_asset.read(reinterpret_cast<char *>(&version), sizeof(version));
        std::cout << "Version: " << version << '\n';

        // deserialize assets
        while (game_asset.peek() != EOF) 
        {
            int int_type;
            game_asset.read(reinterpret_cast<char *>(&int_type), sizeof(int_type));
            AssetType asset_type = static_cast<AssetType>(int_type);
            if (asset_type == AssetType::Image)
            {
                auto texture = std::make_unique<Texture>();
                texture->deserialize(game_asset);
                assets.push_back(std::move(texture));
            }
        }

        game_asset.close();
    } else {
        std::cerr << "Failed to open as binary file.\n";
    }
}

int show_hint()
{
     std::cout << "hint :\n";
    std::cout << "serializing: -s <asset_pack.bin> <image.jpg ...>\n";
    std::cout << "deserializing: -d <asset_pack.bin>\n";
    return -1;
}

int test_1(int argc, char **argv)
{
    std::vector<std::unique_ptr<Asset>> assets;

    if (argc > 1)
    {
        if (std::strcmp(argv[1], "-s") == 0)
        {
            std::cout << "Serializing mode\n";
            std::string game_data_filepath = argv[2];
            if (game_data_filepath.empty())
                return show_hint();

            for (int i = 3; i < argc; ++i)
            {
                std::string asset_filepath = argv[i];
                size_t ext_start = asset_filepath.find_last_of('.');
                size_t ext_end = asset_filepath.size();
                std::string extension = asset_filepath.substr(ext_start, ext_end);

                if (extension == ".jpg")
                {
                    assets.push_back(std::make_unique<Texture>(asset_filepath.c_str()));
                }
                else
                {
                    std::cerr << "Invalid extension: " << extension << '\n';
                }
            }
            serialize(game_data_filepath, assets);
        }

        else if (std::strcmp(argv[1], "-d") == 0)
        {
            std::cout << "Deserializing mode\n";
            std::string game_data_filepath = argv[2];
            if (game_data_filepath.empty())
                return show_hint();

            deserialize("game_data.bin", assets);
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    std::cout << "Hello World\n";
    AssetPackerCreateInfo create_info;
    create_info.headers = {
        "ORIGIN",
        "EVANGELION",
        1,
        'A',
        0.1,
        0.1f,
        false
    };

    AssetPacker asset_packer(create_info);
    asset_packer.serialize("file.bin");

    return 0;
}