/*
	$Source: /cvs/repository/io/Bluetooth/IOBluetoothFamily/IOBluetoothUserLib/OBEX/NSDictionaryOBEXExtensions.h,v $

	Contains:	

	Version:	xxx put version here xxx

	Copyright:	(C) 2002 by Apple Computer, Inc., all rights reserved.

	File Ownership:

		DRI:				Jason Giles

		Other Contact:		xxx put other contact here xxx

		Technology:			xxx put technology here xxx

	Writers:

		(JG)	Jason Giles

	Change History (most recent first):

		$Log: NSDictionaryOBEXExtensions.h,v $
		Revision 1.1  2002/10/10 20:10:41  bubba
		New files; objc wrapper for their CF counterparts.
		
*/

#import <IOKit/IOKitLib.h>
#import <stdlib.h>

#import <IOBluetooth/OBEX.h>


@interface NSMutableDictionary (NSDictionaryOBEXExtensions)

/* creation */

+(NSDictionary*)withOBEXHeadersData:(const void*)inHeadersData headersDataSize:(size_t)inDataSize;

/* Utilities */

-(NSMutableData *)getHeaderBytes;

/* Adding headers to the dictionary */

-(OBEXError)addTargetHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addHTTPHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addBodyHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength endOfBody:(BOOL)isEndOfBody;
-(OBEXError)addWhoHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addConnectionIDHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addByteSequenceHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addObjectClassHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addAuthorizationChallengeHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addAuthorizationResponseHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addTimeISOHeader:(const void*)inHeaderData	length:(uint32_t)inHeaderDataLength;
-(OBEXError)addTypeHeader:(NSString*)type;
-(OBEXError)addLengthHeader:(uint32_t)length;
-(OBEXError)addTime4ByteHeader:(uint32_t)time4Byte;
-(OBEXError)addCountHeader:(uint32_t)inCount;
-(OBEXError)addDescriptionHeader:(NSString*)inDescriptionString;
-(OBEXError)addNameHeader:(NSString*)inNameString;


@end
