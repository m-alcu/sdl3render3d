#pragma once

#include <memory>
#include "Background.hpp"
#include "Desert.hpp"
#include "Imagepng.hpp"

enum class BackgroundType {
    DESERT,
    IMAGE_PNG,
};

static const char* backgroundNames[] = {
    "Desert",
    "Image PNG"
};

class BackgroundFactory {
public:
    static std::unique_ptr<Background> createBackground(BackgroundType type);
};