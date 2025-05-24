#pragma once

#include <memory>
#include "Background.hpp"
#include "Desert.hpp"

enum class BackgroundType {
    DESERT,
    IMAGE_PNG
};

class BackgroundFactory {
public:
    static std::unique_ptr<Background> createBackground(BackgroundType type);
};