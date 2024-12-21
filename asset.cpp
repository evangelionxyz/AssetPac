// Copyright (c) Evangelion Manuhutu

#include "asset.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char *file): m_Filepath(file)
{
    std::ifstream input(file, std::ios::binary | std::ios::ate);
    if (!input.is_open()) 
    {
        std::cerr << "Failed to open image file. " << file << "\n";
        return;
    }

    m_FileSize = input.tellg();
    m_Data = new char[m_FileSize];
    input.seekg(0, std::ios::beg);
    input.read(m_Data, m_FileSize);
    input.close();


    m_RawData = stbi_load(file, &m_Width, &m_Height, &m_Channels, 0);
    std::cout << "Loaded Image: " << file << ", size: " << m_FileSize << " bytes\n";
    std::cout << "Loaded Image: " << file << ", raw_size: " << get_raw_size() << " bytes\n";
}

Texture::~Texture()
{
    if (m_RawData) stbi_image_free(m_RawData);
    delete[] m_Data;
}

void Texture::serialize(std::ofstream &out)
{
    int type = static_cast<int>(get_asset_type());
    out.write(reinterpret_cast<const char *>(&type), sizeof(type));
    out.write(reinterpret_cast<const char *>(&m_FileSize), sizeof(m_FileSize));
    out.write(m_Data, m_FileSize);
}

void Texture::serialize_raw_data(std::ofstream &out)
{
    int type = static_cast<int>(get_asset_type());
    out.write(reinterpret_cast<const char *>(&type), sizeof(type));

    size_t data_size = get_raw_size();
    out.write(reinterpret_cast<const char *>(&data_size), sizeof(data_size));
    out.write(reinterpret_cast<const char *>(&m_Width), sizeof(m_Width));
    out.write(reinterpret_cast<const char *>(&m_Height), sizeof(m_Height));
    out.write(reinterpret_cast<const char *>(&m_Channels), sizeof(m_Channels));
    out.write(reinterpret_cast<const char *>(m_RawData), data_size);
}

void Texture::deserialize(std::ifstream &in)
{
    in.read(reinterpret_cast<char *>(&m_FileSize), sizeof(m_FileSize));
    m_Data = new char[m_FileSize];
    in.read(m_Data, m_FileSize);

    m_RawData = stbi_load_from_memory((const stbi_uc *)m_Data, m_FileSize, &m_Width, &m_Height, &m_Channels, 0);
    std::cout << "Loaded Image: " << "size: " << m_FileSize << " bytes\n";
    std::cout << "Loaded Image: " << "raw_size: " << get_raw_size() << " bytes\n";
}
