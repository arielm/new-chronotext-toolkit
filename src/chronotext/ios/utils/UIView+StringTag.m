#import "UIView+StringTag.h"

@implementation UIView (StringTag)

static NSString *kStringTagKey = @"StringTagKey";

- (NSString*) stringTag
{
    return objc_getAssociatedObject(self, kStringTagKey);
}

- (void) setStringTag:(NSString*)stringTag
{
    objc_setAssociatedObject(self, kStringTagKey, stringTag, OBJC_ASSOCIATION_COPY_NONATOMIC);
}

@end
