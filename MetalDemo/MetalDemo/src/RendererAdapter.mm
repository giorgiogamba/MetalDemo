//
//  RendererAdapter.m
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#import "RendererAdapter.h"

@implementation RendererAdapter

-(void)draw:(id <CAMetalDrawable>) drawable device: (id <MTLDevice>) device
{
    // bridge makes casts without transferring ownership of the object
    pRenderer = new Renderer((__bridge CA::MetalDrawable *) drawable, (__bridge MTL::Device *) device);
    pRenderer->draw();
}

-(void)dealloc
{
    delete pRenderer;
}

@end
