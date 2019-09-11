//
//  Parametric.m
//  Tracks and EQ10
//
//  Created by David Ross on 05/03/2019.
//  Copyright © 2019 David Ross. All rights reserved.
//

#import "Parametric.h"

@interface Parametric ()
{
    CGFloat b0, b1, b2;
    CGFloat a0, a1, a2;
    
    CGFloat xL1, xL2;
    CGFloat yL1, yL2;
    
    CGFloat xR1, xR2;
    CGFloat yR1, yR2;
}

@end

@implementation Parametric

- (id)init
{
    self = [super init];
    
    if (self)
    {
        [self reset];
    }
    
    return self;
}

- (void)reset
{
    xL1 = xL2 = 0.0f;
    yL1 = yL2 = 0.0f;
    
    xR1 = xR2 = 0.0f;
    yR1 = yR2 = 0.0f;
    
    b0 = b1 = b2 = 0.0f;
    a0 = a1 = a2 = 0.0f;
}

- (CGFloat)filterLeft:(CGFloat)x
{
    CGFloat yL0 = (b0*x + b1*xL1 + b2*xL2 - a1*yL1 - a2*yL2);
    
    xL2 = xL1;
    xL1 = x;
    
    yL2 = yL1;
    yL1 = yL0;
    
    return yL0;
}

- (CGFloat)filterRight:(CGFloat)x
{
    CGFloat yR0 = (b0*x + b1*xR1 + b2*xR2 - a1*yR1 - a2*yR2);
    
    xR2 = xR1;
    xR1 = x;
    
    yR2 = yR1;
    yR1 = yR0;
    
    return yR0;
}

- (void)calcCoeffs:(CGFloat)gain :(CGFloat)frequency :(CGFloat)q
{
    CGFloat A     = powf(10.0f, (gain / 40.0f));
    CGFloat omega = 2.0f * M_PI * frequency / 44100.0f;
    CGFloat tsin  = sinf(omega);
    CGFloat tcos  = cosf(omega);
    CGFloat alpha = tsin / (2.0f * q);
    
    a0 = (1.0f + alpha/A);
    a1 = (-2.0f * tcos)     / a0;
    a2 = (1.0f - alpha/A)   / a0;
    
    b0 = (1.0f + alpha*A)   / a0;
    b1 = (-2.0f * tcos)     / a0;
    b2 = (1.0f - alpha*A)   / a0;
}

@end
