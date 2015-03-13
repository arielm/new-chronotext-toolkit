/*
 * THE NEW CHRONOTEXT TOOLKIT: https://github.com/arielm/new-chronotext-toolkit
 * COPYRIGHT (C) 2012, ARIEL MALKA ALL RIGHTS RESERVED.
 *
 * THE FOLLOWING SOURCE-CODE IS DISTRIBUTED UNDER THE SIMPLIFIED BSD LICENSE:
 * https://github.com/arielm/new-chronotext-toolkit/blob/master/LICENSE.md
 */

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
