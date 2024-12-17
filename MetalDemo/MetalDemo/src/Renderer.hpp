//
//  Renderer.hpp
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

class Renderer
{
    
public:
    
    Renderer(MTL::Device* const pDevice);
    ~Renderer();
    
    void drawFrame(const CA::MetalDrawable* const drawable);
    
private:
    
    void buildShaders();
    
    MTL::Device* const pDevice;
    MTL::CommandQueue* const pCommandQueue;
    std::unique_ptr<MTL::RenderPipelineState, void(*)(MTL::RenderPipelineState*)> pRenderPipelineState;
};
