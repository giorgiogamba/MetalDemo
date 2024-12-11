//
//  Renderer.cpp
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <iostream>

#include "Renderer.hpp"

Renderer::Renderer(CA::MetalDrawable* const pDrawable, MTL::Device* const pDevice)
    : pDrawable(pDrawable)
    , pDevice(pDevice)
    , pCommandQueue(pDevice->newCommandQueue())
{}

Renderer::~Renderer()
{
    pCommandQueue->release();
}

void Renderer::draw() const
{
    std::cout << "Window Configuration" << std::endl;
    
    // Passes drawing information to the GPU by command queue
    MTL::CommandBuffer* pCommandBuffer = pCommandQueue->commandBuffer();
    
    // RenderPassDsc receives pixels from previous steps in rendering pipeline
    // Does 3 things: color attachments, deptch attachments, stencil attachments
    MTL::RenderPassDescriptor* renderPassDsc = MTL::RenderPassDescriptor::alloc()->init();
    renderPassDsc->colorAttachments()->object(0)->setTexture(pDrawable->texture());
    renderPassDsc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    renderPassDsc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(1.f, 0.f, 0.f, 1.f)); // Red
    
    MTL::RenderCommandEncoder* renderCommandEdr = pCommandBuffer->renderCommandEncoder(renderPassDsc);
    renderCommandEdr->endEncoding();
    pCommandBuffer->presentDrawable(pDrawable);
    pCommandBuffer->commit();
    
    renderPassDsc->release();
}

