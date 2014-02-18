/**
 * $ clang -framework AppKit -framework Security *.m -o rdproc_test
 */
#import "RDProcess.h"
#include <mach-o/dyld.h>

static void print_usage(const char *prog_name);

int main(int argc, char const *argv[])
{
	pid_t pid = (-1);
	if (argc < 2) {
		print_usage(argv[0]);
		pid = getpid();
	} else {
		pid = strtol(argv[1], NULL, 10);
	}

	RDProcess *proc = [[RDProcess alloc] initWithPID: pid];
	if (!proc) {
		NSLog(@"Could not create RDProcess with invalid PID (%d)", pid);
		return (1);
	}
	NSLog(@"Proc general: %@", proc);

	NSLog(@"PID: %d", proc.pid);
	NSLog(@"Name: %@", proc.processName);
	NSLog(@"Bundle ID: %@", proc.bundleID);
	NSLog(@"Bundle URL: %@", proc.bundleURL);
	NSLog(@"Executable URL: %@", proc.executableURL);
	NSLog(@"Owner: %@, %@ (%d)", proc.ownerUserName, proc.ownerFullUserName, proc.ownerUserID);

	NSDictionary *tmp = proc.ownerGroups;
	if (tmp.count > 0) {
		NSMutableString *owner_groups = [[NSMutableString alloc] init];
		[tmp enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL *stop) {
		    [owner_groups appendFormat:@"%@(%@), ", obj, key];
		}];

		NSLog(@"Owner groups (%lu): %@",
			[tmp allKeys].count, [owner_groups substringToIndex: owner_groups.length-2]);
		[owner_groups release];
	}

	NSLog(@"Sandboxed by OS X (unreliable): %@", proc.isSandboxedByOSX ? @"YES" : @"NO");
	NSLog(@"Sandbox container: %@", proc.sandboxContainerPath);

	NSArray *paths = @[
		@"/usr/bin",
		@"~/Library/",
		@"~/Desktop",
		@"/",
		@"~/Library/Container/com.apple.Preview/Data/Library"
	];
	if (proc.sandboxContainerPath) {
		paths = [paths arrayByAddingObject: proc.sandboxContainerPath];
	}
	[paths enumerateObjectsUsingBlock: ^(id path, NSUInteger idx, BOOL *stop){
		NSLog(@"Sandbox file permissions {%@%@} for [%@]:\t",
			[proc canReadFileAtPath: [path stringByExpandingTildeInPath]] ? @"R" : @"-",
			[proc canWriteToFileAtPath: [path stringByExpandingTildeInPath]] ? @"W" : @"-",
			path);
	}];

	NSLog(@"Arguments: %@", proc.launchArguments);
	NSLog(@"Environment: %@", proc.environmentVariables);

	// proc.processName = [proc.processName stringByAppendingString: @" (RDProcess)"];

	NSLog(@"All processes:");
	[RDProcess enumerateProcessesWithBundleID: proc.bundleID
			usingBlock:^(id process, NSString *bundleID, BOOL *stop){
				NSLog(@"\t* %@", process);
	}];
	NSLog(@"And again:");
	NSLog(@"%@", [RDProcess allProcessesWithBundleID: proc.bundleID]);
	NSLog(@"The youngest process: %@", [RDProcess youngestProcessWithBundleID: proc.bundleID]);
	NSLog(@"The oldest process: %@", [RDProcess oldestProcessWithBundleID: proc.bundleID]);
	[proc release];

	return 0;
}


static void print_usage(const char *prog_name)
{
	printf("Usage: %s [pid]\nIf no pid specified, getpid() is used\n\n", prog_name);
}
