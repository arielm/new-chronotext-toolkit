#include "Path3D.h"

#include "chronotext/utils/MathUtils.h"
#include "chronotext/utils/Utils.h"

using namespace std;
using namespace ci;

void Path3D::clear()
{
    x.clear();
    y.clear();
    z.clear();
    ax.clear();
    ay.clear();
    az.clear();
    len.clear();
    
    size = 0;
}

float Path3D::getLength()
{
    if (size > 0)
    {
        return len[size - 1];
    }
    else
    {
        return 0;
    }
}

void Path3D::add(float xx, float yy, float zz, float aax, float aaz)
{
    x.push_back(xx);
    y.push_back(yy);
    z.push_back(zz);
    
    if (size > 0)
    {
        float dx = xx - x[size - 1];
        float dy = yy - y[size - 1];
        float dz = zz - z[size - 1];
        
        float l = math<float>::sqrt(dx * dx + dy * dy + dz * dz);
        len.push_back(len[size - 1] + l);
        
        ax[size - 1] = aax;
        ay[size - 1] = math<float>::atan2(dz, l);
        az[size - 1] = aaz;
    }
    else
    {
        len.push_back(0);
    }
    
    ax.push_back(0);
    ay.push_back(0);
    az.push_back(0);
    
    size++;
}

Path3D::Value Path3D::pos2Value(float pos) const
{
    if (size > 0)
    {
        float length = len[size - 1];
        pos = math<float>::clamp(pos, 0, length);
        
        int index = search((float*)len.data(), pos, 1, size);
        float ratio = (pos - len[index]) / (len[index + 1] - len[index]);
        
        Path3D::Value value;
        value.x = x[index] + (x[index + 1] - x[index]) * ratio;
        value.y = y[index] + (y[index + 1] - y[index]) * ratio;
        value.z = z[index] + (z[index + 1] - z[index]) * ratio;
        value.ax = ax[index] + (ax[index + 1] - ax[index]) * ratio;
        value.ay = ay[index] + (ay[index + 1] - ay[index]) * ratio;
        value.az = az[index] + (az[index + 1] - az[index]) * ratio;
        value.pos = pos;
        
        return value;
    }
    else
    {
        return Value(0, 0, 0, 0, 0, 0, 0);
    }
}
