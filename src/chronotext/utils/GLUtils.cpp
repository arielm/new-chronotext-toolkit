/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE MODIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "chronotext/utils/GLUtils.h"
#include "chronotext/utils/Utils.h"
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;

namespace chronotext
{
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
    
    void drawGrid(const Rectf &bounds, float size, const Vec2f &offset)
    {
        drawGrid(bounds, size, size, offset);
    }
    
    void drawGrid(const Rectf &bounds, float sx, float sy, const Vec2f &offset)
    {
        float x1 = bounds.x1 - boundf(bounds.x1 - offset.x, sx);
        float y1 = bounds.y1 - boundf(bounds.y1 - offset.y, sy);
        
        int nx = (int)math<float>::ceil(bounds.getWidth() / sx) + 1;
        int ny = (int)math<float>::ceil(bounds.getHeight() / sy) + 1;
        
        vector<Vec2f> vertices;
        vertices.reserve((nx + ny) * 4);
        
        for (int iy = 0; iy < ny; iy++)
        {
            float y = y1 + iy * sy;
            vertices.push_back(Vec2f(bounds.x1, y));
            vertices.push_back(Vec2f(bounds.x2, y));
        }
        
        for (int ix = 0; ix < nx; ix++)
        {
            float x = x1 + ix * sx;
            vertices.push_back(Vec2f(x, bounds.y1));
            vertices.push_back(Vec2f(x, bounds.y2));
        }
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices.data());
        glDrawArrays(GL_LINES, 0, vertices.size());
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
    void drawFullScreenQuad()
    {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        
        const float vertices[] =
        {
            -1, -1,
            +1, -1,
            +1, +1,
            -1, +1
        };
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    
    void drawVerticalGradient(const Rectf &bounds, const ColorA &color1, const ColorA &color2, float v1, float v2)
    {
        float x1 = bounds.x1;
        float x2 = bounds.x2;

        float y0 = bounds.y1;
        float y1 = y0 + bounds.getHeight() * v1;
        float y2 = y0 + bounds.getHeight() * v2;
        float y3 = bounds.y2;
        
        const float vertices[] =
        {
            x1, y0, x2, y0,
            x1, y1, x2, y1,
            x1, y2, x2, y2,
            x1, y3, x2, y3,
        };
        
        vector<ColorA> colors;
        colors.push_back(color1); colors.push_back(color1);
        colors.push_back(color1); colors.push_back(color1);
        colors.push_back(color2); colors.push_back(color2);
        colors.push_back(color2); colors.push_back(color2);
        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glColorPointer(4, GL_FLOAT, 0, colors.data());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, colors.size());
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
    
    void dumpCamera(const Camera &cam, const string &name)
    {
        Vec3f worldUp = cam.getWorldUp();
        Vec3f eyepoint = cam.getEyePoint();
        Vec3f centerOfInterestPoint = cam.getCenterOfInterestPoint();
        Quatf orientation = cam.getOrientation();
        Vec3f axis = orientation.getAxis();
        
        LOGI << name << ".setWorldUp(Vec3f(" << worldUp.x << ", " << worldUp.y << ", " << worldUp.z << "));" << endl;
        LOGI << name << ".setEyePoint(Vec3f(" << eyepoint.x << ", " << eyepoint.y << ", " << eyepoint.z << "));" << endl;
        LOGI << name << ".setCenterOfInterestPoint(Vec3f(" << centerOfInterestPoint.x << ", " << centerOfInterestPoint.y << ", " << centerOfInterestPoint.z << "));" << endl;
        LOGI << name << ".setOrientation(Quatf(Vec3f(" << axis.x << ", " << axis.y << ", " << axis.z << "), " << orientation.getAngle() << "));" << endl;
        LOGI << name << ".setPerspective(" << cam.getFov() << ", getWindowAspectRatio(), " << cam.getNearClip() << ", " << cam.getFarClip() << ");" << endl;
    }
}
