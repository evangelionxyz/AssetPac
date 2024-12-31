// Copyright (c) Evangelion Manuhutu 2024

#pragma once
#include <fstream>
#include <stdint.h>
#include "stb_image.h"

#include "asset_type.hpp"

namespace pac
{

class Asset
{
public:
    virtual ~Asset() = default;
    virtual void serialize(std::ofstream &out) = 0;
    virtual void deserialize(std::ifstream &in) = 0;
    virtual AssetType get_asset_type() = 0;
};

class Texture : public Asset
{
public:
    Texture() = default;
    Texture(const char *file);
    ~Texture();

    int get_width() {return m_Width; }
    int get_height() {return m_Height; }
    int get_num_channels() {return m_Channels; } 
    size_t get_raw_size() { return m_Width * m_Height * m_Channels; }

    void serialize(std::ofstream &out) override;
    void serialize_raw_data(std::ofstream &out);

    void deserialize(std::ifstream &in) override;
    static AssetType get_static_asset_type() { return AssetType::Image; }
    AssetType get_asset_type() override { return get_static_asset_type(); }

private:
    const char *m_Filepath;
    char *m_Data = nullptr;
    size_t m_FileSize = 0;

    // raw data
    unsigned char *m_RawData = nullptr;
    int m_Width = 0, m_Height = 0, m_Channels = 0;
};

}