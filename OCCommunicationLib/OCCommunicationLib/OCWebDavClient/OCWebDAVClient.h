//
//  OCWebDAVClient.h
//  OCWebDAVClient
//
// This class is based in https://github.com/zwaldowski/DZWebDAVClient. Copyright (c) 2012 Zachary Waldowski, Troy Brant, Marcus Rohrmoser, and Sam Soffes.
//
// Copyright (C) 2014 ownCloud Inc. (http://www.owncloud.org/)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//


#import "AFHTTPRequestOperationManager.h"
#import "OCHTTPRequestOperation.h"

@class OCCommunication;
@class OCChunkDto;
@class OCChunkInputStream;

/** The key for a uniform (MIME) type identifier returned from the property request methods. */
extern NSString *OCWebDAVContentTypeKey;

/** The key for a unique entity identifier returned from the property request methods. */
extern NSString *OCWebDAVETagKey;

/** The key for a content identifier tag returned from the property request methods. This is only supported on some servers, and usually defines whether the contents of a collection (folder) have changed. */
extern NSString *OCWebDAVCTagKey;

/** The key for the creation date of an entity. */
extern NSString *OCWebDAVCreationDateKey;

/** The key for last modification date of an entity. */
extern NSString *OCWebDAVModificationDateKey;

@interface OCWebDAVClient : AFHTTPRequestOperationManager

@property (readwrite, nonatomic, strong) NSMutableDictionary *defaultHeaders;
//On redirections AFNetworking lose the request method on iOS6 and set a GET, we use this as workarround
@property (nonatomic, strong) NSString *requestMethod;
//We use this variable to return the url of a redirected server to detect if we receive any sesion expired on SSO server
@property (nonatomic, strong) NSString *redirectedServer;
//We use this variable to get the Cookies from the storage provider
@property (nonatomic, strong) NSString *originalUrlServer;

@property (nonatomic, strong) NSString *postStringForShare;

/**
 Sets the "Authorization" HTTP header set in request objects made by the HTTP client to a basic authentication value with Base64-encoded username and password. This overwrites any existing value for this header.
 
 @param username The HTTP basic auth username
 @param password The HTTP basic auth password
 */
- (void)setAuthorizationHeaderWithUsername:(NSString *)username
                                  password:(NSString *)password;

/**
 Sets the "Authorization" HTTP header set in request objects made by the HTTP client to a basic authentication value with Base64-encoded username and password. This overwrites any existing value for this header.
 
 @param cookieString The HTTP token to login on SSO Servers
 */
- (void)setAuthorizationHeaderWithCookie:(NSString *) cookieString;

/**
 Sets the "Authorization" HTTP header set in request objects made by the HTTP client to a token-based authentication value, such as an OAuth access token. This overwrites any existing value for this header.
 
 @param token The authentication token
 */
- (void)setAuthorizationHeaderWithToken:(NSString *)token;


/**
 Sets the "User-Agent" HTTP header
 
 @param userAgent -> String that indentifies the client app. Ex: "iOS-ownCloud"
 */
- (void)setUserAgent:(NSString *)userAgent;

/**
 Enqueues an operation to copy the object at a path to another path using a `COPY` request.
 
 @param source The path to copy.
 @param destination The path to copy the item to.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and the network error that occurred.
 */
- (void)copyPath:(NSString *)source toPath:(NSString *)destination
 onCommunication:(OCCommunication *)sharedOCCommunication
         success:(void(^)(OCHTTPRequestOperation *, id))success
         failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

/**
 Enqueues an operation to move the object at a path to another path using a `MOVE` request.
 
 @param source The path to move.
 @param destination The path to move the item to.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and the network error that occurred.
 */
- (void)movePath:(NSString *)source toPath:(NSString *)destination
 onCommunication:(OCCommunication *)sharedOCCommunication
         success:(void(^)(OCHTTPRequestOperation *, id))success
         failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

/**
 Enqueues an operation to delete the object at a path using a `DELETE` request.
 
 @param path The path for which to create a directory.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and the network error that occurred.
 */
- (void)deletePath:(NSString *)path
   onCommunication:(OCCommunication *)sharedOCCommunication
           success:(void(^)(OCHTTPRequestOperation *, id))success
           failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;



/**
 Enqueues a request to list the properties of a single entity using a `PROPFIND` request for the specified path.
 
 @param path The path for which to list the properties.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a dictionary with the properties of the entity.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 
 @see listPath:success:failure:
 @see recursiveListPath:success:failure:
 */
- (void)propertiesOfPath:(NSString *)path
         onCommunication: (OCCommunication *)sharedOCCommunication
                 success:(void(^)(OCHTTPRequestOperation *, id ))success
                 failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

/**
 Enqueues a request to list the contents of a single collection and
 the properties of each object, including the properties of the
 collection itself, using a `PROPFIND` request.
 
 @param path The directory for which to list the contents.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a dictionary with the properties of the directory and its contents.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 
 @see propertiesOfPath:success:failure:
 @see recursiveListPath:success:failure:
 */
- (void)listPath:(NSString *)path
 onCommunication: (OCCommunication *)sharedOCCommunication
         success:(void(^)(OCHTTPRequestOperation *operation, id responseObject))success
         failure:(void(^)(OCHTTPRequestOperation *operation, NSError *error))failure;


/**
 Enqueues a request to list the contents of a single collection and
 the properties of each object, including the properties of the
 collection itself, using a `PROPFIND` request.
 
 @param path The directory for which to list the contents.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param token User Session token
 @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a dictionary with the properties of the directory and its contents.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 
 @see propertiesOfPath:success:failure:
 @see recursiveListPath:success:failure:
 */
- (void)listPath:(NSString *)path
 onCommunication:(OCCommunication *)sharedOCCommunication withUserSessionToken:(NSString *)token
         success:(void(^)(OCHTTPRequestOperation *, id, NSString *token))success
         failure:(void(^)(OCHTTPRequestOperation *, NSError *, NSString *token))failure;


/**
 Enqueues an operation to download the contents of a file directly to disk using a `GET` request.
 
 @param remoteSource The path to be fetched, relative to the HTTP client's base URL.
 @param localDestination A local URL to save the contents of a remote file to.
 @param isLIFO Boolean value to indicate if must be use FIFO queue or LIFO queue
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and the network error that occurred.
 
 @see getPath:success:failure:
 */

- (NSOperation *)downloadPath:(NSString *)remoteSource toPath:(NSString *)localDestination withLIFOSystem:(BOOL)isLIFO onCommunication:(OCCommunication *)sharedOCCommunication progress:(void(^)(NSUInteger, long long, long long))progress success:(void(^)(OCHTTPRequestOperation *, id))success failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure shouldExecuteAsBackgroundTaskWithExpirationHandler:(void (^)(void))handler;



/**
 Creates an `NSURLSessionDownloadTask` with the specified request for a local file.
 
 @param remoteSource is a string with the path of the file in the server 
 @param localDestination is a string with the local device path for store the file
 @param defaultPriority is a bool with a flag to indicate if the download must be download inmediately of not.
 @param progress A progress object monitoring the current upload progress.
 @param success A block callback, to be fired upon successful completion, with NSURLResponse and string of URL of the filePath
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 *
 @warning NSURLSession and NSRULSessionUploadTask only can be supported in iOS 7.
 */
- (NSURLSessionDownloadTask *)downloadWithSessionPath:(NSString *)remoteSource toPath:(NSString *)localDestination defaultPriority:(BOOL)defaultPriority onCommunication:(OCCommunication *)sharedOCCommunication withProgress:(NSProgress * __autoreleasing *) progressValue success:(void(^)(NSURLResponse *response, NSURL *filePath))success failure:(void(^)(NSURLResponse *response, NSError *error))failure;

/**
 Enqueues a request to check the server to know the kind of authentication needed.
 
 @param path The path of the server.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and the network error that occurred.
 */
- (void)checkServer:(NSString *)path onCommunication:
(OCCommunication *)sharedOCCommunication
            success:(void(^)(OCHTTPRequestOperation *, id))success
            failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

/**
 Enqueues a request to creates a directory using a `MKCOL` request for the specified path.
 
 @param path The path for which to create a directory.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and the network error that occurred.
 */
- (void)makeCollection:(NSString *)path
       onCommunication:(OCCommunication *)sharedOCCommunication
               success:(void(^)(OCHTTPRequestOperation *, id))success
               failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

/**
 Enqueues an operation to upload the contents of a specified local
 file to a remote path using a `PUT` request.
 
 @param localSource A URL for a local file whose contents will be written the server.
 @param remoteDestination A remote path, relative to the HTTP client's base URL, to write the data to.
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 
 @see putURL:path:success:failure:
 */

- (NSOperation *)putLocalPath:(NSString *)localSource atRemotePath:(NSString *)remoteDestination onCommunication:(OCCommunication *)sharedOCCommunication   progress:(void(^)(NSUInteger, long long))progress success:(void(^)(OCHTTPRequestOperation *, id))success failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure forceCredentialsFailure:(void(^)(NSHTTPURLResponse *, NSError *))forceCredentialsFailure shouldExecuteAsBackgroundTaskWithExpirationHandler:(void (^)(void))handler;


/**
 Creates an `NSURLSessionUploadTask` with the specified request for a local file.
 
 @param localSource is a string with the path of the file to upload
 @param remoteDestination A remote path, relative to the HTTP client's base URL, to write the data to.
 @param progress A progress object monitoring the current upload progress.
 @param success A block callback, to be fired upon successful completion, with NSURLResponse and string of redirected server.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 *
 @warning NSURLSession and NSRULSessionUploadTask only can be supported in iOS 7.
 */
- (NSURLSessionUploadTask *)putWithSessionLocalPath:(NSString *)localSource atRemotePath:(NSString *)remoteDestination onCommunication:(OCCommunication *)sharedOCCommunication withProgress:(NSProgress * __autoreleasing *) progressValue success:(void(^)(NSURLResponse *, NSString *))success failure:(void(^)(NSURLResponse *, id, NSError *))failure failureBeforeRequest:(void(^)(NSError *)) failureBeforeRequest;


/**
 Enqueues an operation to upload the contents of a specified local
 file to a remote path using a `PUT` request.
 
 @param currentChunkDto is an object that contain the current chunk that we will upload
 @param chunkInputStream is an object OCChunkInputStream with the stream of the chunk that we will upload
 @param remoteDestination A remote path, relative to the HTTP client's base URL, to write the data to.
 @param success A block callback, to be fired upon successful completion, with no arguments.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 
 @see putURL:path:success:failure:
 */
- (NSOperation *)putChunk:(OCChunkDto *) currentChunkDto fromInputStream:(OCChunkInputStream *)chunkInputStream andInputStreamForRedirection:(OCChunkInputStream *) chunkInputStreamForRedirection atRemotePath:(NSString *)remoteDestination onCommunication:(OCCommunication *)sharedOCCommunication  progress:(void(^)(NSUInteger, long long))progress success:(void(^)(OCHTTPRequestOperation *, id))success failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure forceCredentialsFailure:(void(^)(NSHTTPURLResponse *, NSError *))forceCredentialsFailure shouldExecuteAsBackgroundTaskWithExpirationHandler:(void (^)(void))handler;


///-----------------------------------
/// @name requestForUserNameByCookie
///-----------------------------------

/**
 * Method to obtain the User name by the cookie of the session
 *
 * @param NSString the cookie of the session
 *
 */
- (void) requestUserNameByCookie:(NSString *) cookieString onCommunication:
(OCCommunication *)sharedOCCommunication success:(void(^)(OCHTTPRequestOperation *, id))success
                         failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;


///-----------------------------------
/// @name Get the status of the server
///-----------------------------------

/**
 * Method to get the json of the status.php common in the ownCloud servers
 *
 * @param serverPath -> url of the server
 * @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 * @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a data with the json file.
 * @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and error.
 *
 */
- (void) getTheStatusOfTheServer:(NSString *)serverPath onCommunication:
(OCCommunication *)sharedOCCommunication success:(void(^)(OCHTTPRequestOperation *, id))success
                            failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

///-----------------------------------
/// @name Get All the shared files and folders of a server
///-----------------------------------

/**
 Method to get all the shared files fo an account by the server path and api
 
 @param serverPath -> The url of the server including the path of the Share API.
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a dictionary with the properties of the directory and its contents.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 */
- (void)listSharedByServer:(NSString *)serverPath
 onCommunication: (OCCommunication *)sharedOCCommunication
         success:(void(^)(OCHTTPRequestOperation *operation, id responseObject))success
         failure:(void(^)(OCHTTPRequestOperation *operation, NSError *error))failure;


///-----------------------------------
/// @name Get All the shared files and folders of concrete folder
///-----------------------------------

/**
 Method to get all the shared files fo an account by the server path and api
 
 @param serverPath -> The url of the server including the path of the Share API.
 @param path -> The path of the folder that we want to know the shared
 @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a dictionary with the properties of the directory and its contents.
 @param failure A block callback, to be fired upon the failure of either the request or the parsing of the request's data, with two arguments: the request operation and the network or parsing error that occurred.
 */
- (void)listSharedByServer:(NSString *)serverPath andPath:(NSString *) path
           onCommunication:(OCCommunication *)sharedOCCommunication
                   success:(void(^)(OCHTTPRequestOperation *, id))success
                   failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

///-----------------------------------
/// @name shareFileOrFolderByServer 
///-----------------------------------

/**
 * Method to share a file or folder with password
 *
 * @param serverPath -> NSString: Server path where we want to share a file or folder. Ex: http://10.40.40.20/owncloud/ocs/v1.php/apps/files_sharing/api/v1/shares
 * @param filePath -> NSString: Path of the server where is the file. Ex: /File.pdf
 * @param password -> NSString: Password
 * @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 * @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a data with the json file.
 * @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and error.
 *
 */
- (void)shareByLinkFileOrFolderByServer:(NSString *)serverPath andPath:(NSString *) filePath andPassword:(NSString *)password
                        onCommunication:(OCCommunication *)sharedOCCommunication
                                success:(void(^)(OCHTTPRequestOperation *, id))success
                                failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

///-----------------------------------
/// @name shareFileOrFolderByServer
///-----------------------------------

/**
 * Method to share a file or folder
 *
 * @param serverPath -> NSString: Server path where we want to share a file or folder. Ex: http://10.40.40.20/owncloud/ocs/v1.php/apps/files_sharing/api/v1/shares
 * @param filePath -> NSString: Path of the server where is the file. Ex: /File.pdf
 * @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 * @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a data with the json file.
 * @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and error.
 *
 */
- (void)shareByLinkFileOrFolderByServer:(NSString *)serverPath andPath:(NSString *) filePath
                  onCommunication:(OCCommunication *)sharedOCCommunication
                          success:(void(^)(OCHTTPRequestOperation *, id))success
                          failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

///-----------------------------------
/// @name unShareFileOrFolderByServer
///-----------------------------------

/**
 * Method to unshare a file or folder
 *
 * @param serverPath -> NSString: Server path with the id of the file or folder that we want to unshare Ex: http://10.40.40.20/owncloud/ocs/v1.php/apps/files_sharing/api/v1/shares/44
 * @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 * @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a data with the json file.
 * @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and error.
 */
- (void)unShareFileOrFolderByServer:(NSString *)serverPath
                    onCommunication:(OCCommunication *)sharedOCCommunication
                            success:(void(^)(OCHTTPRequestOperation *, id))success
                            failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

///-----------------------------------
/// @name isShareFileOrFolderByServer
///-----------------------------------

/**
 * Method to know if a share item still shared
 *
 * @param serverPath -> NSString: Server path with the id of the file or folder that we want know if is shared Ex: http://10.40.40.20/owncloud/ocs/v1.php/apps/files_sharing/api/v1/shares/44
 * @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 * @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a data with the json file.
 * @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and error.
 */
- (void)isShareFileOrFolderByServer:(NSString *)serverPath
                    onCommunication:(OCCommunication *)sharedOCCommunication
                            success:(void(^)(OCHTTPRequestOperation *, id))success
                            failure:(void(^)(OCHTTPRequestOperation *, NSError *))failure;

///-----------------------------------
/// @name updateShareItem
///-----------------------------------

/**
 * Method to update a share link
 *
 * @param shareId -> NSInterger: Share id (You can get the shares id in the calls listSharedByServer....)
 * @param serverPath -> NSString: Server path with the id of the file or folder that we want know if is shared Ex: http://10.40.40.20/owncloud/ocs/v1.php/apps/files_sharing/api/v1/shares/44
 * @param sharedOCCommunication Singleton of communication to add the operation on the queue.
 * @param success A block callback, to be fired upon successful completion, with two arguments: the request operation and a data with the json file.
 * @param failure A block callback, to be fired upon the failure of the request, with two arguments: the request operation and error.
 */
- (void) updateShareItem:(NSInteger)shareId ofServerPath:(NSString*)serverPath withPasswordProtect:(NSString*)password andExpirationTime:(NSString*)expirationTime
         onCommunication:(OCCommunication *)sharedOCCommunication
                 success:(void(^)(OCHTTPRequestOperation *operation, id response))success
                 failure:(void(^)(OCHTTPRequestOperation *operation, NSError *error))failure;
@end
