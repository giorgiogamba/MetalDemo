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
    , pRenderPipelineState(nullptr, [](MTL::RenderPipelineState* const p) { p->release(); })
{
    buildShaders();
}

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
    
    // Cpu population, then GPU transfer
    
    // Defines triangle through buffer
    const std::vector<float> triangle = // defines xyz coordinates for each vertex of the triangle in anticlockwise order
    {
        -0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f,
        0.f, -1.f, 0.f
    };

    // Use smart pointers instead of simple pointers in order to use the reference counting which invokes the lambda (which releases resources)
    // when the pointer has not references anymore
    const std::unique_ptr<MTL::Buffer, void(*)(MTL::Buffer * const)> pVertexBuffer(pDevice->newBuffer(triangle.data(), sizeof(float) * sizeof(triangle), MTL::ResourceStorageModeShared), [](MTL::Buffer * const buffer) { buffer->release(); });
    
    MTL::RenderCommandEncoder* renderCommandEdr = pCommandBuffer->renderCommandEncoder(renderPassDsc);
    
    // In order to connect each rendering step/state with out shaders, we use the renderign pipeline state
    renderCommandEdr->setRenderPipelineState(pRenderPipelineState.get());
    
    // GPU has a limited number of buffers that can allocated depending on the device we are runnign on
    renderCommandEdr->setVertexBuffer(pVertexBuffer.get(), 0, 5);
    renderCommandEdr->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::Integer(0), NS::UInteger(sizeof(triangle) / 3));
    
    renderCommandEdr->endEncoding();
    pCommandBuffer->presentDrawable(pDrawable);
    pCommandBuffer->commit();
    
    renderPassDsc->release();
}

void Renderer::buildShaders()
{
    // Creates the rendering library, the pointers to the rendering functions and conencts everything together
    
    const std::unique_ptr<MTL::Library, void(*)(MTL::Library* const)> pLibrary(pDevice->newDefaultLibrary(), [](MTL::Library* const l){ l->release(); });
    
    const std::unique_ptr<MTL::Function, void(*)(MTL::Function* const)> pVertexShaderFunction(pLibrary.get()->newFunction(NS::String::string("vertexShader", NS::UTF8StringEncoding)), [](MTL::Function* const f){ f->release(); });
    const std::unique_ptr<MTL::Function, void(*)(MTL::Function* const)> pFragmentShaderFunction(pLibrary.get()->newFunction(NS::String::string("fragmentShader", NS::UTF8StringEncoding)), [](MTL::Function* const f){ f->release(); });
    
    const std::unique_ptr<MTL::RenderPipelineDescriptor, void(*)(MTL::RenderPipelineDescriptor* const)> pRenderPipelineDescriptor(MTL::RenderPipelineDescriptor::alloc()->init(), [](MTL::RenderPipelineDescriptor* const d){ d->release(); });
    pRenderPipelineDescriptor.get()->setVertexFunction(pVertexShaderFunction.get());
    pRenderPipelineDescriptor.get()->setFragmentFunction(pFragmentShaderFunction.get());
    pRenderPipelineDescriptor.get()->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    
    // Error handling
    NS::Error* pError = nullptr;
    pRenderPipelineState.reset(pDevice->newRenderPipelineState(pRenderPipelineDescriptor.get(), &pError));
    if (!pRenderPipelineState)
        __builtin_printf("%s", pError->localizedDescription()->utf8String());
}

