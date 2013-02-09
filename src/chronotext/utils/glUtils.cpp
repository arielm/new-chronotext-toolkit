#include "chronotext/utils/glUtils.h"
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;

void perspective(float fovy, float aspect, float zNear, float zFar)
{
    float ymax = zNear * math<float>::tan(fovy * PI / 360);
    float ymin = -ymax;
    
    float xmin = ymin * aspect;
    float xmax = ymax * aspect;

#if defined(CINDER_GLES)
    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
#else
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
#endif
}

/*
 * SPECIAL VERSION TAKING IN COUNT PAN AND ZOOM
 */
void perspective(float fovy, float zNear, float zFar, float width, float height, float panX, float panY, float zoom)
{
    float halfHeight = zNear * math<float>::tan(fovy * PI / 360) / zoom;
    float halfWidth = halfHeight * width / height;
    
    float offsetX = -panX * (halfWidth * 2 / width);
    float offsetY = -panY * (halfHeight * 2 / height);
    
#if defined(CINDER_GLES)
    glFrustumf(-halfWidth + offsetX, halfWidth + offsetX, -halfHeight + offsetY, halfHeight + offsetY, zNear, zFar);
#else
    glFrustum(-halfWidth + offsetX, halfWidth + offsetX, -halfHeight + offsetY, halfHeight + offsetY, zNear, zFar);
#endif
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
