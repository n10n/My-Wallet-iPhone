/*
 * 
 * Copyright (c) 2012, Ben Reeves. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#import <Foundation/Foundation.h>
#import "JSBridgeWebView.h"
#import "MultiAddressResponse.h"


@interface transactionProgressListeners : NSObject {}
@property(nonatomic, copy) void (^on_success)();
@property(nonatomic, copy) void (^on_start)();
@property(nonatomic, copy) void (^on_error)(NSString*error);
@property(nonatomic, copy) void (^on_begin_signing)();
@property(nonatomic, copy) void (^on_sign_progress)(int input);
@property(nonatomic, copy) void (^on_finish_signing)();
@end

@interface Key : NSObject {
    int tag;
}
@property(nonatomic, strong) NSString * addr;
@property(nonatomic, strong) NSString * priv;
@property(nonatomic, strong) NSString * label;
@property(nonatomic, assign) int tag;
@end

@class Wallet;

@protocol WalletDelegate <NSObject>
@optional
-(void)didSetLatestBlock:(LatestBlock*)block;
-(void)didGetMultiAddressResponse:(MulitAddressResponse*)response;
-(void)walletDidDecrypt;
-(void)walletFailedToDecrypt;
-(void)walletDidLoad;
-(void)walletFailedToLoad;
-(void)didBackupWallet;
-(void)didFailBackupWallet;
-(void)walletJSReady;
-(void)didGenerateNewAddress:(NSString*)address;
-(void)networkActivityStart;
-(void)networkActivityStop;
-(void)didParsePairingCode:(NSDictionary *)dict;
-(void)errorParsingPairingCode:(NSString*)message;
-(void)didCreateNewAccount:(NSString*)guid sharedKey:(NSString*)sharedKey password:(NSString*)password;
-(void)errorCreatingNewAccount:(NSString*)message;
-(void)askForPrivateKey:(NSString*)address success:(void(^)(id))_success error:(void(^)(id))_error;
-(void)didWalletDecryptStart;
-(void)didWalletDecryptFinish;
-(void)didFailPutPin:(NSString*)value;
-(void)didPutPinSuccess:(NSDictionary*)dictionary;
-(void)didFailGetPin:(NSString*)value;
-(void)didFailGetPinTimeout;
-(void)didFailGetPinNoResponse;
-(void)didFailGetPinInvalidResponse;
-(void)didGetPinSuccess:(NSDictionary*)dictionary;

@end

@interface Wallet : NSObject <UIWebViewDelegate, JSBridgeWebViewDelegate> {
}

//Core Wallet Init Properties
@property(nonatomic, strong) NSString * guid;
@property(nonatomic, strong) NSString * sharedKey;
@property(nonatomic, strong) NSString * password;

@property(nonatomic, strong) id<WalletDelegate> delegate;
@property(nonatomic, strong) JSBridgeWebView * webView;

@property(nonatomic) uint64_t final_balance;
@property(nonatomic) uint64_t total_sent;
@property(nonatomic) uint64_t total_received;

@property(nonatomic, strong) NSMutableDictionary * transactionProgressListeners;

#pragma mark Init Methods
-(id)init;

-(void)loadGuid:(NSString*)_guid sharedKey:(NSString*)_sharedKey;
-(void)loadGuid:(NSString *)_guid;
-(void)loadBlankWallet;
- (void)loadWalletLogin;

-(NSDictionary*)addressBook;

+(NSString*)generateUUID;

-(void)setLabel:(NSString*)label ForAddress:(NSString*)address;

-(void)archiveAddress:(NSString*)address;
-(void)unArchiveAddress:(NSString*)address;
-(void)removeAddress:(NSString*)address;

-(void)sendPaymentTo:(NSString*)toAddress from:(NSString*)fromAddress satoshiValue:(NSString*)value listener:(transactionProgressListeners*)listener;

-(void)generateNewKey;

-(NSString*)labelForAddress:(NSString*)address;
-(NSInteger)tagForAddress:(NSString*)address;

-(void)addToAddressBook:(NSString*)address label:(NSString*)label;

-(BOOL)isValidAddress:(NSString*)string;
-(BOOL)isWatchOnlyAddress:(NSString*)address;

-(void)cancelTxSigning;

-(BOOL)addKey:(NSString*)privateKeyString;

//Fetch String Array Of Addresses
-(NSArray*)activeAddresses;
-(NSArray*)allAddresses;
-(NSArray*)archivedAddresses;

-(BOOL)isDoubleEncrypted;
-(BOOL)isInitialized;
-(BOOL)hasEncryptedWalletData;

-(BOOL)validateSecondPassword:(NSString*)secondPassword;

-(void)getHistory;
-(void)getWalletAndHistory;

-(uint64_t)getAddressBalance:(NSString*)address;
-(uint64_t)parseBitcoinValue:(NSString*)input;

-(CurrencySymbol*)getLocalSymbol;
-(CurrencySymbol*)getBTCSymbol;

-(void)clearLocalStorage;

-(void)parsePairingCode:(NSString*)code;

-(NSString*)detectPrivateKeyFormat:(NSString*)privateKeyString;

-(NSInteger)getWebsocketReadyState;

-(void)newAccount:(NSString*)password email:(NSString *)email;

-(void)pinServerPutKeyOnPinServerServer:(NSString*)key value:(NSString*)value pin:(NSString*)pin;
-(void)apiGetPINValue:(NSString*)key pin:(NSString*)pin withWalletDownload:(BOOL)withWalletDownload;

-(NSString*)encrypt:(NSString*)data password:(NSString*)password pbkdf2_iterations:(int)pbkdf2_iterations;
-(NSString*)decrypt:(NSString*)data password:(NSString*)password pbkdf2_iterations:(int)pbkdf2_iterations;

    
@end
