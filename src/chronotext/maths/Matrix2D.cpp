#include "chronotext/maths/Matrix2D.h"

using namespace std;
using namespace ci;

namespace chronotext
{
    void Matrix2D::setToIdentity()
    {
        m00 = m11 = 1;
        m01 = m02 = m10 = m12 = 0;
    }
    
    void Matrix2D::translate(float x, float y)
    {
        m02 = x * m00 + y * m01 + m02;
        m12 = x * m10 + y * m11 + m12;
    }
    
    void Matrix2D::scale(float s)
    {
        m00 *= s;
        m01 *= s;
        
        m10 *= s;
        m11 *= s;
    }
    
    void Matrix2D::rotate(float a)
    {
        float c = math<float>::cos(a);
        float s = math<float>::sin(a);
        
        float temp1 = m00;
        float temp2 = m01;
        m00 =  c * temp1 + s * temp2;
        m01 =  c * temp2 - s * temp1;
        
        temp1 = m10;
        temp2 = m11;
        m10 =  c * temp1 + s * temp2;
        m11 =  c * temp2 - s * temp1;
    }
    
    Vec2f Matrix2D::transform(float x, float y) const
    {
        return Vec2f(m00 * x + m01 * y + m02, m10 * x + m11 * y + m12);
    }
}
