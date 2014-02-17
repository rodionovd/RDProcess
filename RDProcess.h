#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

@interface RDProcess : NSObject

- (id)init __attribute__((unavailable("use -initWithPID: instead")));
- (id)initWithPID: (pid_t)aPid;

- (pid_t)pid;
- (NSString *)processName;
/**
 * Sets a new title for a process.
 *
 * @brief
 *     1) This method sets a new value for LaunchServices' Display Name key of the process;
 *     2) If we set a name for current process {pid == getpid()}
 *        OR  if the current user is a member of `procmod` group,
 *        than it also pathes argv[0] with a new name.
 *
 * @discussion
 *     It will *completely* set a new name only for a current process.
 *     For other ones it will only change the LaunchServices' "Display Name" value, which implies
 *     that some utilities (e.g. System Monitor.app) will follow that change but other ones
 *     (e.g. ps, top) won't, because they depend on argv[0] value which we can't change
 *     without being inside `procmod` group.
 *
 * @param
 *     {NSString *} new title for the process
 * @return
 *     {BOOL}       Always NO (0)
 */
- (BOOL)setProcessName: (NSString *)new_proc_name;

// these will return (obviously) `nil` for non-bundled applications
- (NSString *)bundleID;
- (NSURL    *)bundleURL;
- (NSString *)bundlePath;

- (NSURL    *)executableURL;
- (NSString *)executablePath;

- (uid_t)ownerUserID;
- (NSString *)ownerUserName;
- (NSString *)ownerFullUserName;
// @{
//    /* Keys */ /* Values */
//    group_id0 : group_name0,
//    group_id1 : group_name1,
//    ... ... ... ... ... ...
//    group_idN : group_nameN,
// }
- (NSDictionary *)ownerGroups;


/**
 * Check if the process is sanboxed by OS X.
 *
 * @note
 *     this method returns YES for any process with invalid PID, so you should also check if
 *     [proc sandboxContainerPath] is not nil.
 *
 * @return {BOOL} YES or NO, or neither.
 */
- (BOOL)isSandboxedByOSX;
/**
 * Sandbox contatiner path for the process (if it has one).
 * @return
 *     {NSString *} containter path or `nil` if the process is not sandboxed.
 */
- (NSString *)sandboxContainerPath;
- (NSURL    *)sandboxContainerURL;
- (BOOL)canWriteToFileAtPath: (NSString *)file_path;
- (BOOL)canWriteToFileAtURL:  (NSURL *)file_url;
- (BOOL)canReadFileAtPath: (NSString *)file_path;
- (BOOL)canReadFileAtURL:  (NSURL *)file_url;


/**
 * ARGV and ENV values of a process
 *
 * @brief
 *     Until the current user is not a member of `procmod` group, these method will work only for
 *     processes owned by this user (for other's processes they return `nil`).
 */
- (NSArray *)launchArguments;
/* @note variable values are percent escaped */
- (NSDictionary *)environmentVariables;





/* ------------------------{ NOT IMPLEMENTED YET }------------------------ */

/**
 * More sandbox stuff
 */
- (int)_enableSandbox __attribute__((unavailable("not implemented yet")));
- (BOOL)_isSandboxedByUser __attribute__((unavailable("not implemented yet")));
// gonna crash it down
- (int)_disableSandbox __attribute__((unavailable("not implemented yet")));

// Intel
- (NSString *)architectureString __attribute__((unavailable("not implemented yet")));
// smth like "Intel (64 bit)"
- (NSString *)kindString __attribute__((unavailable("not implemented yet")));
- (BOOL)is64bit __attribute__((unavailable("not implemented yet")));


// 0-100%
- (NSUInteger)CPUUsagePercentages __attribute__((unavailable("not implemented yet")));
// msec
- (NSUInteger)CPUTimeMsec __attribute__((unavailable("not implemented yet")));

- (NSUInteger)threadsCount __attribute__((unavailable("not implemented yet")));
- (NSUInteger)activeThreadsCount __attribute__((unavailable("not implemented yet")));
- (NSUInteger)inactiveThreadsCount __attribute__((unavailable("not implemented yet")));
- (NSUInteger)openPortsCount __attribute__((unavailable("not implemented yet")));

- (NSUInteger)memoryUsageRealBytes __attribute__((unavailable("not implemented yet")));
- (NSUInteger)memoryUsageRealPrivateBytes __attribute__((unavailable("not implemented yet")));
- (NSUInteger)memoryUsageRealSharedBytes __attribute__((unavailable("not implemented yet")));
- (NSUInteger)memoryUsageVirtualPrivateBytes __attribute__((unavailable("not implemented yet")));

- (NSUInteger)messagesSent __attribute__((unavailable("not implemented yet")));
- (NSUInteger)messagesReceived __attribute__((unavailable("not implemented yet")));

@end
