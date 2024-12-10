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
}

