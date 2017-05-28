# MVC-Example
Objective C

HEADER FILE
============
#pragma mark - LOGIN
#pragma mark -
-(void)loginUserWithEmail:(NSString *)emailAddress password:(NSString *)password completionHandler:(void (^)(BOOL isSuccess))completionHandler;

#pragma mark - SIGN UP
#pragma mark -
-(void)signUpUser:(PRUser *)userToSignUp withCompletionHandler:(void (^)(BOOL isSuccess,NSDictionary *dictResponse))completionHandler;


IMPLIMENTATION FILE
====================

#pragma mark - LOGIN
#pragma mark -
-(void)loginUserWithEmail:(NSString *)emailAddress password:(NSString *)password completionHandler:(void (^)(BOOL isSuccess))completionHandler{
    
//    NSDictionary *dictParams = @{@"email":emailAddress,@"password":password };
    NSDictionary *dictParams = @{@"user_email":emailAddress,@"password":password};
    
    [self postDataToUrl:MAKE_URL(kAPILogin) params:dictParams withShowLoading:YES hideLoading:NO completionHandler:^(BOOL isSuccess, NSDictionary *dictResponse) {

        if(isSuccess){
            [CURRENT_USER.strPassword setString:password];
            
            NSDictionary *dictUserInfo = [dictResponse valueForKeyPath:@"response.user"] ;
            if(dictUserInfo.allKeys.count>0){
                [CURRENT_USER setCurrentUserInfo:dictUserInfo];
            }
            
            SET_USER_DEFAULTS(kUserDefaultsEmailAddressKey, emailAddress);
            SET_USER_DEFAULTS(kUserDefaultsPasswordKey, password);
        }
        
        if(completionHandler){
            completionHandler(isSuccess);
        }
    }];
}

#pragma mark - SIGN UP
#pragma mark -
-(void)signUpUser:(PRUser *)userToSignUp withCompletionHandler:(void (^)(BOOL isSuccess,NSDictionary *dictResponse))completionHandler{
    NSDictionary *dictParams = @{@"user_email":userToSignUp.userEmailAddress,
                                 @"password":userToSignUp.strPassword ,
                                 @"user_firstname" : userToSignUp.userFirstName,
                                 @"subscription" : @{@"planCode" : @"Free"},
                                 @"user_lastname" : userToSignUp.userLastName };
   
    
    [self postDataToUrl:MAKE_URL(kAPISignUp) params:dictParams withShowLoading:YES hideLoading:YES completionHandler:^(BOOL isSuccess, NSDictionary *dictResponse) {
        if(isSuccess){
            NSDictionary *dictUser = [dictResponse objectForKey:@"result"];
            CURRENT_USER.userID = getValue(dictUser, @"user_id");
//            [AIAlertUtils displayAlertWithMessage:getValue(dictResponse, @"message") otherButtonTitles:nil preferredAlertStyle:UIAlertControllerStyleAlert withCompletion:nil];
            
        }
        if(completionHandler)
            completionHandler(isSuccess,dictResponse);
    }];
}


/////////////////////////////////////////////
////// ******* GENERAL METHODS ******* //////
/////////////////////////////////////////////
#pragma mark - GENERAL METHODS

#pragma mark - POST
#pragma mark -
- (void)postDataToUrl:(NSString *)urlToPost
               params:(NSDictionary *)parameters
      withShowLoading:(BOOL)shouldShowLoader
          hideLoading:(BOOL)shouldHideLoader
    completionHandler:(void (^)(BOOL isSuccess,
                                NSDictionary *dictResponse))completionHandler {
    
    if (!IS_INTERNET_AVAILABLE) {
        [AIHudUtils hideHUD];
        [AIAlertUtils noInternet];
        completionHandler(NO, nil);
        return;
    }
    
    if(shouldShowLoader){
        [AIHudUtils showHUD];
    }
    
    [CLIENT POST:urlToPost
      parameters:parameters
        progress:^(NSProgress *_Nonnull uploadProgress) {
            
        }
         success:^(NSURLSessionDataTask *_Nonnull task,
                   id _Nullable responseObject) {
             
             if(shouldHideLoader){
                 [AIHudUtils hideHUD];
             }

             
#if DEBUG
             NSLog(@"==================================================");
             NSLog(@" URL %@  \n  PARAMS : %@  \n  "
                   @"RESPONSE : %@",
                   task.originalRequest.URL, parameters, responseObject);
#endif
             if (isSucessResponse) {
                 
                 if (completionHandler) {
                     completionHandler(YES, responseObject);
                 }
             }else {
                 [AIHudUtils hideHUD];
                if (completionHandler)
                    completionHandler(NO, responseObject);
                 
                 NSString *strMessage;
                 
                 if ([responseObject valueForKey:@"message"]) {
                     strMessage = getValue(responseObject, @"message");
                 }else{
                     if([responseObject valueForKeyPath:@"response.message"]){
                         strMessage = [responseObject valueForKeyPath:@"response.message"];
                     }else{
                          strMessage = [responseObject valueForKeyPath:@"result.message"];
                     }
                    
                 }
                 
                 [AIAlertUtils displayAlertWithMessage:strMessage otherButtonTitles:nil preferredAlertStyle:UIAlertControllerStyleAlert withCompletion:nil];
             }
         }
         failure:^(NSURLSessionDataTask *_Nullable task, NSError *_Nonnull error) {
                 [AIHudUtils hideHUD];
#if DEBUG
             NSLog(@"==================================================");
             NSLog(@" URL %@  \n  PARAMS : %@  \n  ERROR : "
                   @"%@",
                   task.originalRequest.URL, parameters, error);
#endif
             if (completionHandler) {
                 completionHandler(NO, nil);
             }
             
              [AIAlertUtils displayAlertWithMessage:error.localizedDescription otherButtonTitles:nil preferredAlertStyle:UIAlertControllerStyleAlert withCompletion:nil];
         }];
}


