//
//  ViewController.m
//  NetworkTest
//
//  Created by 无线视通 on 2018/9/28.
//  Copyright © 2018年 无线视通. All rights reserved.
//

#import "ViewController.h"
#import "ios-ntp.h"

@interface ViewController (){
    NetworkClock *          netClock;           // complex clock
    NetAssociation *        netAssociation;     // one-time server
}

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.view setBackgroundColor:[UIColor whiteColor]];
    
    
    netClock = [NetworkClock sharedNetworkClock];
    
    
    
    //[self timerFireMethod:nil];
    
}

- (void) timerFireMethod:(NSTimer *) theTimer {
//    _sysClockLabel.text =
    NSLog(@"System Clock: %@",[NSDate date]);
//    _netClockLabel.text = [NSString stringWithFormat:@"Network Clock: %@", netClock.networkTime];
    NSLog(@"Network Clock: %@", netClock.networkTime);
//    _offsetLabel.text = [NSString stringWithFormat:@"Clock Offet: %5.3f mSec", netClock.networkOffset * 1000.0];
    NSLog(@"Clock Offet: %5.3f mSec", netClock.networkOffset * 1000.0);
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
