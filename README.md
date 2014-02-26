RDProcess — you're now as knowledgeable as Activity Monitor is
=========

RDProcess is a light library that allows you to obtain as many information about any running process in your system as it possible to collect (it can even handle some Sandbox-related info, such as containter path for a process).  
It's built on top of Launch Services, AppKit and some scary old-school POSIX things.  

## Interface  

The library has only one class: `RDProcess` which implements  the methods listed below:

##### Initialization via PID


`- (instancetype)initWithPID: (pid_t)aPid;`  
Initializes a process with a PID, and collects the following information about it:  
* PID value
* Process name  (using either `LunchServices` API or `argv[0]`-based technique)  
* Bundle ID (if the process is a bundled application, this value will be `nil` otherwise)  
* An executable path (using either `LaunchServices` API or `proc_pidpath()`, or `agrv[0]`-based technique)  

---------
##### Bundle ID-based initialization


* `+ (NSArray *)allProcessesWithBundleID: (NSString *)bundleID;`  
Returns an array of all launched processes with a specifed Bundle ID; each item is initialized using [`-initWithPID:`](#initialization-via-pid)  

* `+ (void)enumerateProcessesWithBundleID: (NSString *)bundleID usingBlock: (RDProcessEnumerator)block;` 
Iterates a list of launched processes with a specifed Bundle ID using a block; `RDProcessEnumerator` type's definition is folowing:
  ```objc
  typedef void (^RDProcessEnumerator)(id process, NSString *bundleID, BOOL *stop);
  ```  

* `+ (instancetype)youngestProcessWithBundleID: (NSString *)bundleID`  
Returns the most recently launched process with a Bundle ID.

* `+ (instancetype)oldestProcessWithBundleID: (NSString *)bundleID;`  
Returns the most oldest process with a Bundle ID.

---------
##### Setters

* `- (BOOL)setProcessName: (NSString *)new_proc_name;`  
This method sets a new value for `LaunchServices`' «Display Name» key of the process.  

> Note that some utils like `ps` or `top` rather depend on an `argv[0]` value than on the «Display Name», so the process name may remain unchanged there.

---------
##### Getters

###### General information

* `- (pid_t)pid;`  
Just a PID of the process. This value if fetched from `LaunchService` database each time you call this method.  

* `- (NSString *)processName;`  
A name of the process (using either `LaunchServices` API or `argv[0]`-based value).  
> Note, that this method won't return `nil` value, but a result value may be invalid in any other way, so it's up to you to verify it.  
> *This should only happen* for processes that weren't launched via `Launch Services` **and** have a currupted `argv` array.    

###### Bundles and executables

* `- (NSString *)bundleID`  
A Bundle ID of the process (or `nil` for non-bundled applications).  

* `- (NSURL *)bundleURL`  
An URL of the process' bundle (or `nil` for non-bundled applications).  

* `- (NSString *)bundlePath`  
A path of the process' bundle (or `nil` for non-bundled applications).  

* `- (NSURL *)executableURL`  
An URL of the process' main executable file.  

* `- (NSString *)executablePath;`  
A path of the process' main executable file.  

###### Users and groups

* `- (uid_t)ownerUserID;`
An ID (*UID*) of a user who owns this process.  

* `- (NSString *)ownerUserName`  
A «short» name of the user who owns this process.  

* `- (NSString *)ownerFullUserName;`  
A full name of the user who owns this process.  

* `- (NSDictionary *)ownerGroups;`  
A list of which the owner user is member of. The result dictionary has the following format: keys are groupd IDs, values are groups names.  

###### Sandbox  

* `- (BOOL)isSandboxedByOSX;`  
Checks if the the process was launched inside the Sandbox environment, i.e. it's sandboxed by OS X.
> Note: this method returns `YES` for any process with **invalid PID**, so you should also check if `-sandboxContainerPath` isn't `nil`.

* `- (NSString *)sandboxContainerPath;`  
A path of the process' sandbox container directory (or `nil` if it's not sanboxed). 

* `- (NSURL *)sandboxContainerURL;`  
An URL of the process's sandbox containter directory (or `nil` of it's not sandboxed).  

* `- (BOOL)canWriteToFileAtPath: (NSString *)file_path;`   
`- (BOOL)canWriteToFileAtURL:  (NSURL    *)file_url;`  
Checks if the process can write to a specified file.  

* `- (BOOL)canReadFileAtPath: (NSString *)file_path;`  
`- (BOOL)canReadFileAtURL:  (NSURL    *)file_url;`  
Checks if the process can read a specifed file.

###### Launch arguments and environment variables  

Until the current user is a member of `procmod` group, these methods will work only for processes owned by this user (for other's processes they just return `nil`).  

* `- (NSArray *)launchArguments;`   
An Objective-C representaion of the process' `argv` array.  

* `- (NSDictionary *)environmentVariables;`  
A dictionary containing all the environment variables.  
> Note: all values are percent escaped.


## TODO

* Check if process was sandboxed by user  
* Enable\Disable sandbox for the process  
* Description of the process' executable architecture (smth like *"Intel (64-bit)"*)  
* Check for 64 bit  
* CPU usage information  
* Memory usage information    
* Threads information (active, idle, count)  

------

If you found any bug(s) or something, please open an issue or a pull request — I'd appreciate your help! `(^,,^)`

*Dmitry Rodionov, 2014*  
*i.am.rodionovd@gmail.com*
