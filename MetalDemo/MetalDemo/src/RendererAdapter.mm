//
//  RendererAdapter.m
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#import "RendererAdapter.h"

@implementation RendererAdapter

- (nonnull instancetype) initWithMTKView: (MTKView*)pView
{
    self = [super init];
    pRenderer = new Renderer((__bridge MTL::Device *) pView.device);
    return self;
}

-(void)drawInMTKView:(MTKView *)view
{
    pRenderer->drawFrame((__bridge CA::MetalDrawable *)view.currentDrawable);
}

-(void)mtkView:(MTKView *)view drawableSizeWillChange:(CGSize)size
{
    // Empty override
}

-(void)dealloc
{
    delete pRenderer;
}

@end
