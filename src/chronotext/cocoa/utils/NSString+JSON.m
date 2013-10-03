#import "NSString+JSON.h"

@implementation NSString (JSON)

- (id) toJSON
{
    return [NSJSONSerialization JSONObjectWithData:[self dataUsingEncoding:NSUTF8StringEncoding] options:0 error:nil];
}

@end
