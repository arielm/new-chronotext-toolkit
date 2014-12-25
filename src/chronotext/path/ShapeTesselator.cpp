#include "ShapeTesselator.h"

using namespace ci;
using namespace std;

namespace chr
{
    static void* memAllocator(void *userData, unsigned int size)
    {
        return malloc(size);
    }
    
    static void* memReallocator(void *userData, void* ptr, unsigned int size)
    {
        return realloc(ptr, size);
    }
    
    static void memFree(void *userData, void *ptr)
    {
        free(ptr);
    }
    
    ShapeTesselator::ShapeTesselator()
    {
        TESSalloc allocator;
        allocator.memalloc = memAllocator;
        allocator.memrealloc = memReallocator;
        allocator.memfree = memFree;
        allocator.meshEdgeBucketSize = 0;
        allocator.meshVertexBucketSize = 0;
        allocator.meshFaceBucketSize = 0;
        allocator.dictNodeBucketSize = 0;
        allocator.regionBucketSize = 0;
        allocator.extraVertices = 0;
        
        tess = tessNewTess(&allocator);
    }
    
    ShapeTesselator::~ShapeTesselator()
    {
        tessDeleteTess(tess);
    }
    
    void ShapeTesselator::add(const Rectf &rect)
    {
        vector<Vec2f> polygon;
        polygon.emplace_back(rect.getUpperLeft());
        polygon.emplace_back(rect.getUpperRight());
        polygon.emplace_back(rect.getLowerRight());
        polygon.emplace_back(rect.getLowerLeft());
        
        add(polygon);
    }
    
    void ShapeTesselator::add(const Shape2d &shape, float approximationScale)
    {
        size_t numContours = shape.getContours().size();
        
        for (size_t p = 0; p < numContours; ++p)
        {
            add(shape.getContour(p), approximationScale);
        }
    }
    
    void ShapeTesselator::add(const Path2d &path, float approximationScale)
    {
        add(path.subdivide(approximationScale));
    }
    
    void ShapeTesselator::add(const FXGDocument &document, float approximationScale)
    {
        for (auto &path : document.getPaths())
        {
            add(path, approximationScale);
        }
    }
    
    void ShapeTesselator::add(const vector<vector<ci::Vec2f>> &polygons)
    {
        for (auto &polygon : polygons)
        {
            add(polygon);
        }
    }
    
    void ShapeTesselator::add(const std::vector<ci::Vec2f> &polygon)
    {
        tessAddContour(tess, 2, polygon.data(), sizeof(float) * 2, polygon.size());
    }
    
    ShapeMesh* ShapeTesselator::createMesh(int windingRule)
    {
        tessTesselate(tess, windingRule, TESS_POLYGONS, 3, 2, 0);
        
        ShapeMesh *mesh = new ShapeMesh;
        mesh->appendVertices((Vec2f*)tessGetVertices(tess), tessGetVertexCount(tess));
        mesh->appendIndices((uint32_t*)tessGetElements(tess), tessGetElementCount(tess) * 3);
        
        return mesh;
    }
}
