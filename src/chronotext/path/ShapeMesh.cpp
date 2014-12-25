#include "ShapeMesh.h"

#include "cinder/gl/gl.h"

using namespace ci;
using namespace std;

namespace chr
{
    void ShapeMesh::appendVertices(const Vec2f *vertices, size_t count)
    {
        ShapeMesh::vertices.insert(ShapeMesh::vertices.end(), vertices, vertices + count);
    }
    
    void ShapeMesh::appendIndices(const uint32_t *indices, size_t count)
    {
        ShapeMesh::indices.reserve(ShapeMesh::indices.size() + count);
        
        for (int i = 0; i < count; i++)
        {
            ShapeMesh::indices.push_back((uint16_t)indices[i]);
        }
    }
    
    const vector<Vec2f>& ShapeMesh::getVertices() const
    {
        return vertices;
    }
    
    const vector<uint16_t>& ShapeMesh::getIndices() const
    {
        return indices;
    }
    
    void ShapeMesh::clear()
    {
        vertices.clear();
        indices.clear();
    }
    
    void ShapeMesh::draw()
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, 0, vertices.data());
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, indices.data());
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}
