//
//  Parametric.h
//  Tracks and EQ10
//
//  Created by David Ross on 05/03/2019.
//  Copyright © 2019 David Ross. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface Parametric : NSObject

- (void)reset;
- (CGFloat)filterLeft:(CGFloat)x;
- (CGFloat)filterRight:(CGFloat)x;
- (void)calcCoeffs:(CGFloat)gain :(CGFloat)frequency :(CGFloat)q;

@end

NS_ASSUME_NONNULL_END
