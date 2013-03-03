#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#import "CinderDelegate.h"

@interface GLViewController : GLKViewController
{
    GLKView *glView;
    CinderDelegate *cinderDelegate;
    
    int interfaceOrientation;
    
    BOOL started;
    BOOL stopped;
    
	int ticks;
	NSTimeInterval t0;
	NSTimeInterval elapsed;
}

@property (nonatomic, assign) GLKView *glView;
@property (nonatomic, assign) CinderDelegate *cinderDelegate;

@end
