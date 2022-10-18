#include "config.h"
#include "Lightning.h"


Lightning::Lightning(V3 _pos, V3 _rgb, float _intensity)
{
    pos = _pos;
    rgb = _rgb;
    intensity = _intensity;
}

Lightning::~Lightning()
{

}


void Lightning::setPos(V3 _pos)
{
    pos = _pos;
}

V3 Lightning::getPos()
{
    return pos;
}

void Lightning::setColor(V3 _rgb)
{
    rgb = _rgb;
}

V3 Lightning::getColor()
{
    return rgb;
}

void Lightning::setIntensity(float _intensity)
{
    intensity = _intensity;
}

float Lightning::getIntensity()
{
    return intensity;
}

void Lightning::bindLight( std::shared_ptr<ShaderResource> shader, V3 cameraPos)
{
    shader->setV3(rgb, "lightColor");
    shader->setV3(pos, "lightPos");
    shader->setFloat(1, "lightIntensity");

    shader->setV3(cameraPos, "camPos");
    shader->setFloat(1, "specularIntensity");
}
