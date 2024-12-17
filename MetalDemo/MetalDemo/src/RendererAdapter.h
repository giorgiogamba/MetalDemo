//
//  RendererAdapter.h
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#import "QuartzCore/CAMetalLayer.h"
#import "Metal/MTLDevice.h"
#import "MetalKit/MTKView.h"

#import "Renderer.hpp"

// MTKViewDelegate permits to use draw methods invokes with fps rate
@interface RendererAdapter : NSObject<MTKViewDelegate>
{
    Renderer* pRenderer;
}

- (nonnull instancetype) initWithMTKView: (MTKView*) pView;

@end
