#ifdef __OBJC__
#import <UIKit/UIKit.h>
#else
#ifndef FOUNDATION_EXPORT
#if defined(__cplusplus)
#define FOUNDATION_EXPORT extern "C"
#else
#define FOUNDATION_EXPORT extern
#endif
#endif
#endif

#import "CatapushHeaders.h"

FOUNDATION_EXPORT double catapush_ios_sdk_podVersionNumber;
FOUNDATION_EXPORT const unsigned char catapush_ios_sdk_podVersionString[];

