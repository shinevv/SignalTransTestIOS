//
//  VVLogInControlle.m
//  VVRoom
//
//  Created by Apple on 2017/11/23.
//  Copyright © 2017年 Apple. All rights reserved.
//

#import "VVLogInControlle.h"
#import "ios-ntp.h"
#import <VVRoomPeerconnection/Shinevv.h>

@interface VVLogInControlle ()<VVConnectionDelegate, VVChatDelegate, UITextFieldDelegate, VVMediaDelegate>
{
    NetworkClock * netClock;
    NSString* myPeerId;
    NSInteger iCount;
    NSInteger index;
    NSTimer* sendTimer;
    NSMutableArray* mesArr;
    NSTimer* reciveTimer;
    dispatch_queue_t queue;
    NSInteger talDelay;
}

@property (nonatomic, assign)NSInteger nArgDelay;
@end

@implementation VVLogInControlle

- (void)viewDidLoad {
    [super viewDidLoad];
    _syncTime.hidden = YES;
    index = 1;
    netClock = [NetworkClock sharedNetworkClock];
    [[Shinevv shareManager] addShinevvDelegate:(id)self];
    _infoText.text = @"已同步服务器时间！\n";
    myPeerId = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    _inputText.delegate = self;
    _numLab.text = [NSString stringWithFormat:@"%ld", _inputText.text.length];
    mesArr = [NSMutableArray new];
    iCount = 100;
    reciveTimer = [NSTimer scheduledTimerWithTimeInterval:0.2 target:self selector:@selector(reciveTime) userInfo:nil repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:reciveTimer forMode:NSRunLoopCommonModes];
    queue = dispatch_queue_create("queueName", DISPATCH_QUEUE_SERIAL);
    [self joinroom];
    
    [self timerFireMethod:nil];
}

- (void) timerFireMethod:(NSTimer *) theTimer {
    //    _sysClockLabel.text =
    NSLog(@"System Clock: %@",[NSDate date]);
    //    _netClockLabel.text = [NSString stringWithFormat:@"Network Clock: %@", netClock.networkTime];
    NSLog(@"Network Clock: %@", netClock.networkTime);
    //    _offsetLabel.text = [NSString stringWithFormat:@"Clock Offet: %5.3f mSec", netClock.networkOffset * 1000.0];
    NSLog(@"Clock Offet: %5.3f mSec", netClock.networkOffset * 1000.0);
}

- (void)onConnected{

}

- (void)OnCreateLocalAudio:(bool) status{
    if (status) {
        [[Shinevv shareManager] modifyAudioStatus:false];
    }
}

- (void)OnCreateLocalVideo:(bool) status
{
    if (status) {
        [[Shinevv shareManager] modifyVideoStatus:false];
    }
}

- (void)onConnectFail{
    
}
- (void)joinroom
{
    //连接服务器
    [[Shinevv shareManager]joinRoom:@"vvroom.shinevv.cn"
                           WithPort:3443
                          WithToken:@"06175684da8706a0da7e0a6fb2aa8d02"
                    WithDisplayName:[NSString stringWithFormat:@"100%d",arc4random()%10000]
                         WithRoomId:@"123"
                           WithRole:@"student"
                         WithPeerID:myPeerId
                      WithMediaType:nil];
    
}

- (IBAction)sendMsg:(id)sender {
    _sendBut.enabled = NO;
    if (_sendNum.text&&_sendNum.text.length) {
        iCount = [_sendNum.text intValue];
    }
    sendTimer = [NSTimer scheduledTimerWithTimeInterval:0.2f target:self selector:@selector(sendText) userInfo:nil repeats:YES];

}

- (void)sendText{
    if (index > iCount) {
        index = 1;
        _sendBut.enabled = YES;
        [sendTimer invalidate];
        sendTimer = nil;
        return;
    }
    
    [self appendText:[NSString stringWithFormat:@"\n\n$$$$$$$$$$$$$$$-%ld", index]];
    NSString* sendTitm = [self timeStampFromUTCDate:netClock.networkTime];
    [self appendText:[NSString stringWithFormat:@"发送:%@毫秒", sendTitm]];
    NSString* sendText = [NSString stringWithFormat:@"%@==%@&%ld", _inputText.text, sendTitm, index];
    [[Shinevv shareManager] sendChatMessage:sendText];
    [self appendText:[NSString stringWithFormat:@"发送内容:\n%@", sendText]];
    index ++;
}

- (void)appendText:(NSString*)strText{
    __weak VVLogInControlle* ws = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        ws.aveDelay.text = [NSString stringWithFormat:@"%ld", ws.nArgDelay];
        ws.infoText.text = [NSString stringWithFormat:@"%@%@\n",ws.infoText.text, strText];
        [ws.infoText scrollRectToVisible:CGRectMake(0, ws.infoText.contentSize.height-15, ws.infoText.contentSize.width, 10) animated:NO];
    });

    
//    _clearBut.hidden = NO;
}
- (IBAction)clearBut:(id)sender {
    _infoText.text = @"";
    _aveDelay.text = @"0";
}

//接收到im消息回调
- (void)onReceiveImMes:(NSString *)mes
{

    NSString* recTime = [self timeStampFromUTCDate:netClock.networkTime];
    [mesArr addObject:@{@"recTime":recTime, @"mes":mes}];


}

- (void)reciveTime{
    if (mesArr.count>0) {
        NSArray* arr = [NSArray arrayWithArray:mesArr];
        [mesArr removeAllObjects];
        
            dispatch_async(queue, ^{
                NSArray* disArr = [NSArray arrayWithArray:arr];
                for (NSDictionary* dic in disArr) {
                    [self showText:dic];
                }
            });
        
    }
}

- (void)showText:(NSDictionary*)dic{
    NSString* recTime =  dic[@"recTime"];
    NSString* mes = dic[@"mes"];
    NSDictionary* mesDic = [[self dictionaryWithJsonString:mes] objectForKey:@"message"];
    NSString* text = mesDic[@"text"];
    NSArray* arr = [text componentsSeparatedByString:@"=="];
    NSString* strIndex = [[[arr lastObject] componentsSeparatedByString:@"&"] lastObject];
    NSString* strSend = [[[arr lastObject] componentsSeparatedByString:@"&"] firstObject];
    long long delay = [recTime longLongValue] - [strSend longLongValue];
    if ([strIndex isEqualToString:@"1"]) {
        talDelay = delay;
    }else{
        talDelay += delay;
    }
    self.nArgDelay = talDelay / [strIndex intValue];
    NSString* showText = [NSString stringWithFormat:@"\n###############-%@\n发送:%@毫秒\n接收:%@毫秒\n网络延时:%lld毫秒\n接收内容:\n%@", strIndex,strSend,recTime,delay,text];
    [self appendText:showText];
//    [self appendText:[NSString stringWithFormat:@"发送:%@毫秒", strSend]];
//    [self appendText:[NSString stringWithFormat:@"接收:%@毫秒", recTime]];
//    [self appendText:[NSString stringWithFormat:@"网络延时:%lld毫秒", [recTime longLongValue] - [strSend longLongValue]]];
//    [self appendText:[NSString stringWithFormat:@"接收内容:\n%@", text]];
}

- (void)onSendChatMessageFail:(NSString *)mes{
    
}
 //将当前时间（UTCDate）转为时间戳
 -(NSString *)timeStampFromUTCDate:(NSDate *)UTCDate{
    
     NSTimeInterval timeInterval = [UTCDate timeIntervalSince1970];
     // *1000,是精确到毫秒；这里是精确到秒;
     NSString *result = [NSString stringWithFormat:@"%.0f",timeInterval*1000];
     return result;

 }


- (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString
{
    if (jsonString == nil) {
        return nil;
    }
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSError *err;
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
    
    if(err){
        NSLog(@"json解析失败：%@",err);
        return nil;
    }
    return dic;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string{
    NSInteger len = _inputText.text.length - range.length + string.length;
    _numLab.text = [NSString stringWithFormat:@"%ld", len];
    return true;
}

@end
