//
//  ViewController.h
//  kapusha-samples
//
//  Created by Ivan Avdeev on 10.11.12.
//
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include "../sys/IViewport.h"

@interface ViewController : GLKViewController

- (id) initWithViewport:(kapusha::IViewport*)viewport;

@end
