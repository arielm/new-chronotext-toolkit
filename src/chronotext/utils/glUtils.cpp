#include "chronotext/utils/glUtils.h"
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;

const Matrix44f getPerspectiveMatrix(float fovy, float aspect, float zNear, float zFar)
{
    float ymax = zNear * math<float>::tan(fovy * PI / 360);
    float ymin = -ymax;
    
    float xmin = ymin * aspect;
    float xmax = ymax * aspect;
    
    return getFrustumMatrix(xmin, xmax, ymin, ymax, zNear, zFar);
}

/*
 * SPECIAL VERSION TAKING IN COUNT PAN AND ZOOM
 */
const Matrix44f getPerspectiveMatrix(float fovy, float zNear, float zFar, float width, float height, float panX, float panY, float zoom)
{
    float halfHeight = zNear * math<float>::tan(fovy * PI / 360) / zoom;
    float halfWidth = halfHeight * width / height;
    
    float offsetX = -panX * (halfWidth * 2 / width);
    float offsetY = -panY * (halfHeight * 2 / height);
    
    return getFrustumMatrix(-halfWidth + offsetX, halfWidth + offsetX, -halfHeight + offsetY, halfHeight + offsetY, zNear, zFar);
}

/*
 * BASED ON CODE FROM http://www.mesa3d.org
 */
const Matrix44f getFrustumMatrix(float left, float right, float bottom, float top, float znear, float zfar)
{
    float x = (2 * znear) / (right - left);
    float y = (2 * znear) / (top - bottom);
    float a = (right + left) / (right - left);
    float b = (top + bottom) / (top - bottom);
    float c = -(zfar + znear) / ( zfar - znear);
    float d = -(2 * zfar * znear) / (zfar - znear);
    
    float m[] =
    {
        x, 0, 0,  0,
        0, y, 0,  0,
        a, b, c, -1,
        0, 0, d,  0
    };
    
    return Matrix44f(m);
}

void dumpCamera(const ci::Camera &cam, const string &name)
{
    Vec3f worldUp = cam.getWorldUp();
    Vec3f eyepoint = cam.getEyePoint();
    Vec3f centerOfInterestPoint = cam.getCenterOfInterestPoint();
    Quatf orientation = cam.getOrientation();
    Vec3f axis = orientation.getAxis();
    
    cout << name << ".setWorldUp(Vec3f(" << worldUp.x << ", " << worldUp.y << ", " << worldUp.z << "));" << endl;
    cout << name << ".setEyePoint(Vec3f(" << eyepoint.x << ", " << eyepoint.y << ", " << eyepoint.z << "));" << endl;
    cout << name << ".setCenterOfInterestPoint(Vec3f(" << centerOfInterestPoint.x << ", " << centerOfInterestPoint.y << ", " << centerOfInterestPoint.z << "));" << endl;
    cout << name << ".setOrientation(Quatf(Vec3f(" << axis.x << ", " << axis.y << ", " << axis.z << "), " << orientation.getAngle() << "));" << endl;
    cout << name << ".setPerspective(" << cam.getFov() << ", getWindowAspectRatio(), " << cam.getNearClip() << ", " << cam.getFarClip() << ");" << endl;
}
