// Copyright (c) Evangelion Manuhutu 2024
#include "asset_packer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <iostream>

namespace pac
{

AssetPacker::AssetPacker(const AssetPackerCreateInfo &create_info)
    : m_CreateInfo(create_info)
{
}

void AssetPacker::serialize(const std::filesystem::path &filepath)
{
    std::ofstream out(filepath, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
    {
        std::cerr << "[AssetPacker] Failed to open: " << filepath << '\n';
        return;
    }

    for (const auto &header : m_CreateInfo.headers)
    {
        std::cout << header.type().name() << "--\n";

        if (header.type().name() == "i")
        {
            int value = std::any_cast<int>(header);
            out.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
        else if (header.type() == typeid(bool))
        {
            bool value = std::any_cast<bool>(header);
            out.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
        else if (header.type().name() == "f")
        {
            float value = std::any_cast<float>(header);
            out.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
        else if (header.type().name() == "d")
        {
            double value = std::any_cast<double>(header);
            out.write(reinterpret_cast<char*>(&value), sizeof(value));
        }
        else if (header.type().name() == "PKc")
        {
            auto value = std::any_cast<std::string>(header);
            size_t length = value.size();
            out.write(reinterpret_cast<char*>(&length), sizeof(length)); // Write the size first
            out.write(value.data(), length); // Then the string data
        }
        else if (header.type().name() == "c")
        {
            auto value = std::any_cast<std::vector<char>>(header);
            size_t length = value.size();
            out.write(reinterpret_cast<char*>(&length), sizeof(length)); // Write the size first
            out.write(value.data(), length); // Then the binary data
        }
        else
        {
            std::cerr << "[AssetPacker] Unsupported type in std::any\n";
        }
    }

    // serialize images
    for (auto &[name, image_info] : m_Images)
    {
        int type = static_cast<int>(image_info->type);
        out.write(reinterpret_cast<const char *>(&type), sizeof(type));

        out.write(name.data(), name.size());
        out.write(reinterpret_cast<const char *>(&image_info->width), sizeof(image_info->width));
        out.write(reinterpret_cast<const char *>(&image_info->height), sizeof(image_info->height));
        out.write(reinterpret_cast<const char *>(&image_info->channels), sizeof(image_info->channels));
    }

    m_CreateInfo.size = out.tellp();
    out.close();
}

void AssetPacker::deserialize(const std::filesystem::path &filepath)
{

}

Ref<ImageInfo> AssetPacker::image_load(const std::string &filepath)
{
    size_t ext_start = filepath.find_last_of('.');
    size_t ext_end = filepath.size();
    std::string extension = filepath.substr(ext_start, ext_end);

    Ref<ImageInfo> image_info = CreateRef<ImageInfo>();
    image_info->name = filepath.substr(0, ext_end);

    std::fstream in(filepath, std::ios::binary | std::ios::in);
    image_info->size = in.tellg();
    image_info->data = new char[image_info->size];
    in.read(image_info->data, image_info->size);
    
    stbi_load_from_memory((const unsigned char *)image_info->data, image_info->size,
        &image_info->width, &image_info->height, &image_info->channels, 0);

    m_Images[image_info->name] = image_info;

    return image_info;
}

ImageMap AssetPacker::get_image_map()
{
    return m_Images;
}



}