//
//  VVLogInControlle.h
//  VVRoom
//
//  Created by Apple on 2017/11/23.
//  Copyright © 2017年 Apple. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface VVLogInControlle : UIViewController

@property (weak, nonatomic) IBOutlet UILabel *syncTime;

@property (weak, nonatomic) IBOutlet UITextField *inputText;
@property (weak, nonatomic) IBOutlet UIButton *sendBut;
@property (weak, nonatomic) IBOutlet UILabel *numLab;

@property (weak, nonatomic) IBOutlet UITextView *infoText;

@property (weak, nonatomic) IBOutlet UIButton *clearBut;
@property (weak, nonatomic) IBOutlet UITextField *sendNum;
@property (weak, nonatomic) IBOutlet UILabel *aveDelay;
@end
