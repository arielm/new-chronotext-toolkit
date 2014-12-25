/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012-2014, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

#include "GLUtils.h"

#include "chronotext/Log.h"
#include "chronotext/utils/MathUtils.h"

using namespace std;
using namespace ci;

namespace chr
{
    namespace utils
    {
        namespace gl
        {
            void bindTexture(const ci::gl::Texture &texture)
            {
                glBindTexture(GL_TEXTURE_2D, texture.getId());
            }
            
            void beginTexture(const ci::gl::Texture &texture)
            {
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                glEnable(GL_TEXTURE_2D);
                
                glBindTexture(GL_TEXTURE_2D, texture.getId());
            }
            
            void endTexture()
            {
                glDisable(GL_TEXTURE_2D);
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            
            /*
             * XXX: INCLUDES WORKAROUND FOR ci::gl::Texture::getCleanWidth() AND CO. WHICH ARE NOT WORKING ON GL-ES
             */
            
            void drawTextureFromCenter(const ci::gl::Texture &texture)
            {
                drawTexture(texture, texture.getWidth() * texture.getMaxU() * 0.5f, texture.getHeight() * texture.getMaxV() * 0.5f);
            }
            
            /*
             * XXX: INCLUDES WORKAROUND FOR ci::gl::Texture::getCleanWidth() AND CO. WHICH ARE NOT WORKING ON GL-ES
             */
            
            void drawTexture(const ci::gl::Texture &texture, float rx, float ry)
            {
                float u = texture.getMaxU();
                float v = texture.getMaxV();
                
                float x1 = -rx;
                float y1 = -ry;
                
                float x2 = x1 + texture.getWidth() * u;
                float y2 = y1 + texture.getHeight() * v;
                
                const float vertices[] =
                {
                    x1, y1,
                    x2, y1,
                    x2, y2,
                    x1, y2
                };
                
                const float coords[] =
                {
                    0, 0,
                    u, 0,
                    u, v,
                    0, v
                };
                
                glTexCoordPointer(2, GL_FLOAT, 0, coords);
                glVertexPointer(2, GL_FLOAT, 0, vertices);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
            
            /*
             * XXX: ONLY WORKS FOR "TRUE" POWER-OF-TWO TEXTURES
             */
            
            void drawTextureInRect(const ci::gl::Texture &texture, const Rectf &rect, float ox, float oy)
            {
                const float vertices[] =
                {
                    rect.x1, rect.y1,
                    rect.x2, rect.y1,
                    rect.x2, rect.y2,
                    rect.x1, rect.y2
                };
                
                float u1 = (rect.x1 - ox) / texture.getWidth();
                float v1 = (rect.y1 - oy) / texture.getHeight();
                float u2 = (rect.x2 - ox) / texture.getWidth();
                float v2 = (rect.y2 - oy) / texture.getHeight();
                
                const float coords[] =
                {
                    u1, v1,
                    u2, v1,
                    u2, v2,
                    u1, v2
                };
                
                glTexCoordPointer(2, GL_FLOAT, 0, coords);
                glVertexPointer(2, GL_FLOAT, 0, vertices);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
            
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
                
                const float m[] =
                {
                    x, 0, 0,  0,
                    0, y, 0,  0,
                    a, b, c, -1,
                    0, 0, d,  0
                };
                
                return m;
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
                    vertices.emplace_back(bounds.x1, y);
                    vertices.emplace_back(bounds.x2, y);
                }
                
                for (int ix = 0; ix < nx; ix++)
                {
                    float x = x1 + ix * sx;
                    vertices.emplace_back(x, bounds.y1);
                    vertices.emplace_back(x, bounds.y2);
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
                for (int i = 0; i < 4; i++) colors.push_back(color1);
                for (int i = 0; i < 4; i++) colors.push_back(color2);
                
                glEnableClientState(GL_VERTEX_ARRAY);
                glEnableClientState(GL_COLOR_ARRAY);
                
                glVertexPointer(2, GL_FLOAT, 0, vertices);
                glColorPointer(4, GL_FLOAT, 0, colors.data());
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4 * 2);
                
                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_COLOR_ARRAY);
            }
            
            void dumpCamera(const Camera &cam, const string &name)
            {
                auto eyepoint = cam.getEyePoint();
                auto orientation = cam.getOrientation();
                
                LOGI << name << ".setEyePoint(Vec3f(" << eyepoint.x << ", " << eyepoint.y << ", " << eyepoint.z << "));" << endl;
                LOGI << name << ".setCenterOfInterest(" << cam.getCenterOfInterest() << ");" << endl;
                LOGI << name << ".setOrientation(Quatf(" << orientation.w << ", " << orientation.v.x << ", " << orientation.v.y << ", " << orientation.v.z << "));" << endl;
                LOGI << name << ".setPerspective(" << cam.getFov() << ", getWindowAspectRatio(), " << cam.getNearClip() << ", " << cam.getFarClip() << ");" << endl;
            }
        }
    }
}
