//
//  RendererAdapter.h
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#import "QuartzCore/CAMetalLayer.h"
#import "Metal/MTLDevice.h"

#import "Renderer.hpp"

@interface RendererAdapter : NSObject
{
    Renderer* pRenderer;
}

- (void)draw:(id <CAMetalDrawable>) drawable device: (id <MTLDevice>) device;
@end
