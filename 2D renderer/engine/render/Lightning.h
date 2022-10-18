#pragma once
#include "core/MathLibrary.h"
#include "ShaderResource.h"
#include <memory>

class Lightning
{
    V3 pos;
    V3 rgb;
    float intensity;
public:
    Lightning(V3 _pos, V3 _rgb, float _intensity);
    ~Lightning();

    void setPos(V3 _pos);
    V3 getPos();

    void setColor(V3 _rgb);
    V3 getColor();

    void setIntensity(float _intensity);
    float getIntensity();

    void bindLight(std::shared_ptr<ShaderResource> shader, V3 cameraPos);
};
