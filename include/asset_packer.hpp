// Copyright (c) Evangelion Manuhutu 2024

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <any>

#include "base.hpp"

#include "asset_type.hpp"

namespace pac
{
struct ImageInfo
{
    std::string name;
    char *data;
    int width = 0, height = 0, channels = -1;
    size_t size = 0;
    const AssetType type = AssetType::Image;
};

using ImageMap = std::unordered_map<std::string, Ref<ImageInfo>>;

struct AssetPackerCreateInfo
{
    std::vector<std::any> headers;
    size_t size = 0;
};

class AssetPacker
{
public:
    AssetPacker() = default;
    AssetPacker(const AssetPackerCreateInfo &create_info);

    void serialize(const std::filesystem::path &filepath);
    void deserialize(const std::filesystem::path &filepath);

    Ref<ImageInfo> image_load(const std::string &filepath);
    ImageMap get_image_map();

private:
    AssetPackerCreateInfo m_CreateInfo;
    ImageMap m_Images;
};


}