
#import "NSDictionary+DefautValues.h"

@implementation NSDictionary (DefautValues)

- (int) intForKey:(NSString*)key
{
    return [[self objectForKey:key] intValue];
}

- (int) intForKey:(NSString*)key defaultValue:(int)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [[self objectForKey:key] intValue];
    }
    
    return defaultValue;
}

- (unsigned int) unsignedIntForKey:(NSString*)key
{
    return [[self objectForKey:key] unsignedIntValue];
}

- (unsigned int) unsignedIntForKey:(NSString*)key defaultValue:(unsigned int)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [[self objectForKey:key] unsignedIntValue];
    }
    
    return defaultValue;
}

- (float) floatForKey:(NSString*)key
{
    return [[self objectForKey:key] floatValue];
}

- (float) floatForKey:(NSString*)key defaultValue:(float)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [[self objectForKey:key] floatValue];
    }
    
    return defaultValue;
}

- (NSString*) stringForKey:(NSString*)key
{
    return [self objectForKey:key];
}

- (NSString*) stringForKey:(NSString*)key defaultValue:(NSString*)defaultValue
{
    if ([[self allKeys] containsObject:key])
    {
        return [self objectForKey:key];
    }
    
    return defaultValue;
}

@end
