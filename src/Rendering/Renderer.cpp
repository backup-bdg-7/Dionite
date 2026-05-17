// ============================================================================
// Dionite — Rendering: NullRenderer for headless / unit tests
// ============================================================================
#include "Renderer.h"
namespace dionite::render {

class NullRenderer : public Renderer {
public:
    bool init(void*, int, int) override { return true; }
    void resize(int, int) override {}
    int  uploadMesh(const Mesh&) override { return nextId_++; }
    int  uploadTexture(const std::string&) override { return nextId_++; }
    void beginFrame() override {}
    void setCamera(const math::Mat4&, const math::Mat4&) override {}
    void draw(const DrawCommand&) override {}
    void drawSprite2D(float, float, float, float, int, float, float, float, float) override {}
    void drawText2D(float, float, const std::string&, float, float, float, float) override {}
    void endFrame() override {}
    void shutdown() override {}
private:
    int nextId_ = 1;
};

} // namespace dionite::render
