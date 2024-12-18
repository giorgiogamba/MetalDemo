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

Renderer::Renderer(MTL::Device* const pDevice)
    : pDevice(pDevice)
    , pCommandQueue(pDevice->newCommandQueue())
    , pRenderPipelineState(nullptr, [](MTL::RenderPipelineState* const p) { p->release(); })
    , deltaTime(0.f)
{
    buildShaders();
}

Renderer::~Renderer()
{
    pCommandQueue->release();
}

void Renderer::drawFrame(const CA::MetalDrawable* const drawable)
{
    // Passes drawing information to the GPU by command queue
    MTL::CommandBuffer* pCommandBuffer = pCommandQueue->commandBuffer();
    
    // RenderPassDsc receives pixels from previous steps in rendering pipeline
    // Does 3 things: color attachments, deptch attachments, stencil attachments
    MTL::RenderPassDescriptor* renderPassDsc = MTL::RenderPassDescriptor::alloc()->init();
    renderPassDsc->colorAttachments()->object(0)->setTexture(drawable->texture());
    renderPassDsc->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
    renderPassDsc->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(1.f, 0.f, 0.f, 1.f)); // Red
    
    // Cpu population, then GPU transfer
    
    // Defines triangle through buffer
    const std::vector<float> vertices = // defines xyz coordinates for each vertex of the triangle in anticlockwise order
    {
        -0.5f, 0.5f, 0.f,
        0.5f, 0.5f, 0.f,
        0.f, -1.f, 0.f,
        -0.5f, -1.f, 0.f
    };
    
    // Index drawing: we define a pool of indices and we define triangles as set of indices
    const std::vector<ushort> indices =
    {
        // First triangle
        0, 1, 2,
        
        // Second triangle
        0, 2, 3
    };
    
    deltaTime += 0.1f;

    // Use smart pointers instead of simple pointers in order to use the reference counting which invokes the lambda (which releases resources)
    // when the pointer has not references anymore
    
    // Buffer containing all vertices for all primitives
    const std::unique_ptr<MTL::Buffer, void(*)(MTL::Buffer * const)> pVertexBuffer(pDevice->newBuffer(vertices.data(), sizeof(float) * sizeof(vertices), MTL::ResourceStorageModeShared), [](MTL::Buffer * const buffer) { buffer->release(); });
    
    // Buffer containing all indices for all primitives
    const std::unique_ptr<MTL::Buffer, void(*)(MTL::Buffer * const)> pIndexBuffer(pDevice->newBuffer(indices.data(), sizeof(ushort) * sizeof(indices), MTL::ResourceStorageModeShared), [](MTL::Buffer * const buffer) { buffer->release(); });
    
    MTL::RenderCommandEncoder* renderCommandEdr = pCommandBuffer->renderCommandEncoder(renderPassDsc);
    
    renderCommandEdr->setVertexBytes(&deltaTime, sizeof(float), 7); // only when the passed data is less than 4kb
    
    // In order to connect each rendering step/state with out shaders, we use the renderign pipeline state
    renderCommandEdr->setRenderPipelineState(pRenderPipelineState.get());
    
    // GPU has a limited number of buffers that can allocated depending on the device we are runnign on
    renderCommandEdr->setVertexBuffer(pVertexBuffer.get(), 0, 5);
    renderCommandEdr->setTriangleFillMode(MTL::TriangleFillModeLines); // wireframe render
    
    // We draw the primitive by pointing its shape and the set of indices composing the primitive, taken from the vertex buffer
    renderCommandEdr->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, indices.size(), MTL::IndexTypeUInt16, pIndexBuffer.get(), 0);
    
    renderCommandEdr->endEncoding();
    pCommandBuffer->presentDrawable(drawable);
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

