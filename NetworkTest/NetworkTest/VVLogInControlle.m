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
}
@end

@implementation VVLogInControlle

- (void)viewDidLoad {
    [super viewDidLoad];
    //self.navigationController.navigationBar.barStyle = UIBarStyleBlack;
    _syncTime.hidden = YES;
    netClock = [NetworkClock sharedNetworkClock];
    [[Shinevv shareManager] addShinevvDelegate:(id)self];
//    [[Shinevv shareManager] modifyAudioStatus:false];
//    [[Shinevv shareManager] modifyVideoStatus:false];
    _infoText.text = @"已同步服务器时间！\n";
    myPeerId = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
    _inputText.delegate = self;
    _numLab.text = [NSString stringWithFormat:@"%ld", _inputText.text.length];
    [self joinroom];

}

- (void)onConnected{
//    [[Shinevv shareManager] modifyAudioStatus:false];
//    [[Shinevv shareManager] modifyVideoStatus:false];
}

- (void)OnCreateLocalAudio:(bool) status{
    NSLog(@"本地音频创建%@", status?@"成功":@"失败");
    if (status) {
        [[Shinevv shareManager] modifyAudioStatus:false];
    }
    
}

- (void)OnCreateLocalVideo:(bool) status
{
    NSLog(@"本地视频创建%@", status?@"成功":@"失败");
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
    [self appendText:@"\n\n$$$$$$$$$$$$$$$"];
    NSString* sendTitm = [self timeStampFromUTCDate:netClock.networkTime];
    [self appendText:[NSString stringWithFormat:@"发送:%@毫秒", sendTitm]];
    NSString* sendText = [NSString stringWithFormat:@"%@==%@", _inputText.text, sendTitm];
    [[Shinevv shareManager] sendChatMessage:sendText];
    [self appendText:[NSString stringWithFormat:@"发送内容:\n%@", sendText]];
}

- (void)appendText:(NSString*)strText{
    _infoText.text = [NSString stringWithFormat:@"%@%@\n",_infoText.text, strText];
    [_infoText scrollRectToVisible:CGRectMake(0, _infoText.contentSize.height-15, _infoText.contentSize.width, 10) animated:YES];
    
    _clearBut.hidden = NO;
}
- (IBAction)clearBut:(id)sender {
    _infoText.text = @"";
    UIButton* btn = sender;
    btn.hidden = YES;
}

//接收到im消息回调
- (void)onReceiveImMes:(NSString *)mes
{

    NSString* recTime = [self timeStampFromUTCDate:netClock.networkTime];
    NSDictionary* mesDic = [[self dictionaryWithJsonString:mes] objectForKey:@"message"];
    NSString* text = mesDic[@"text"];
    NSArray* arr = [text componentsSeparatedByString:@"=="];
    NSString* strSend = [arr lastObject];
    [self appendText:@"\n\n###############"];
    [self appendText:[NSString stringWithFormat:@"发送:%@毫秒", strSend]];
    [self appendText:[NSString stringWithFormat:@"接收:%@毫秒", recTime]];
    [self appendText:[NSString stringWithFormat:@"网络延时:%lld毫秒", [recTime longLongValue] - [strSend longLongValue]]];
    [self appendText:[NSString stringWithFormat:@"接收内容:\n%@", text]];

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
