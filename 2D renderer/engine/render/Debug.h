#pragma once
#include "core/MathLibrary.h"

namespace Debug
{
    void Render(M4 cameraVPMatrix);
    void DrawSquare(float size, V3 pos, V3 color);
    void DrawLine(V3 start, V3 end, V3 color);
}