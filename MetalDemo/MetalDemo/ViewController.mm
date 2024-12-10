//
//  ViewController.m
//  MetalDemo
//
//  Created by Giorgio Gamba on 10/12/24.
//

#import "ViewController.h"
#import "RendererAdapter.h"

@implementation ViewController
{
    MTKView* view;
    
    RendererAdapter* pRendererAdapter;
}


- (void)viewDidLoad {
    [super viewDidLoad];

    view = (MTKView *) self.view; // Set correct storyboard class in order to make the cast work
    view.device = MTLCreateSystemDefaultDevice(); // alloctaes to gpu
    pRendererAdapter = [RendererAdapter alloc];
    [pRendererAdapter draw:view.currentDrawable device:view.device];
}

@end
