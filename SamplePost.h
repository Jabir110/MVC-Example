//
//  SamplePost.h
//  PromoRhino
//
//  Created by Jabir on 28/05/17.
//  Copyright © 2017 agile. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SamplePost : NSObject


#pragma mark - LOGIN
#pragma mark -
-(void)loginUserWithEmail:(NSString *)emailAddress password:(NSString *)password completionHandler:(void (^)(BOOL isSuccess))completionHandler;

#pragma mark - SIGN UP
#pragma mark -
-(void)signUpUser:(PRUser *)userToSignUp withCompletionHandler:(void (^)(BOOL isSuccess,NSDictionary *dictResponse))completionHandler;

@end
