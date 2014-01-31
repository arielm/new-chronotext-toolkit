#pragma once

struct Frame
{
    float x;
    float y;
    float scaleX;
    float scaleY;
    float rotation;
    float alpha;
    
    Frame(float x, float y, float scaleX, float scaleY, float rotation, float alpha)
    :
    x(x),
    y(y),
    scaleX(scaleX),
    scaleY(scaleY),
    rotation(rotation),
    alpha(alpha)
    {}
};
