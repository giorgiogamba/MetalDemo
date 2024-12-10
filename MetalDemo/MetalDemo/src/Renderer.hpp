//
//  Renderer.hpp
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

class Renderer
{
    
public:
    
    Renderer(CA::MetalDrawable* const pDrawable, MTL::Device* const pDevice);
    ~Renderer();
    
    void draw() const;
    
private:
    
    CA::MetalDrawable* pDrawable;
    MTL::Device* const pDevice;
    MTL::CommandQueue* const pCommandQueue;
};
