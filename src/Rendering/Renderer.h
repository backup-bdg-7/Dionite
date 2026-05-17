// ============================================================================
// Dionite — Rendering: abstract renderer interface (Metal/Vulkan platform impls)
// ============================================================================
#pragma once
#include "Core/Math/Matrix.h"
#include "Core/Math/Vector.h"
#include <string>
#include <vector>

namespace dionite::render {

struct Vertex {
    math::Vec3 position;
    math::Vec3 normal;
    float u, v;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    int materialId = 0;
};

struct DrawCommand {
    int meshId;
    math::Mat4 transform;
    int materialId = 0;
    float r = 1, g = 1, b = 1, a = 1;
};

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual bool init(void* nativeWindow, int width, int height) = 0;
    virtual void resize(int width, int height) = 0;
    virtual int  uploadMesh(const Mesh& m) = 0;
    virtual int  uploadTexture(const std::string& path) = 0;
    virtual void beginFrame() = 0;
    virtual void setCamera(const math::Mat4& view, const math::Mat4& proj) = 0;
    virtual void draw(const DrawCommand& d) = 0;
    virtual void drawSprite2D(float x, float y, float w, float h, int textureId,
                              float r=1,float g=1,float b=1,float a=1) = 0;
    virtual void drawText2D(float x, float y, const std::string& text,
                            float scale=1.f, float r=1, float g=1, float b=1) = 0;
    virtual void endFrame() = 0;
    virtual void shutdown() = 0;
};

} // namespace dionite::render
