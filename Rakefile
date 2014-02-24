task :default => [:build, :run, :cleanup]

TARGET = "rdproc_test"

task :build do
	system("mkdir build") if (!File.exists?(Dir.getwd+"/build"))
	system("clang -DDEBUG -framework AppKit -o build/#{TARGET} *.m")
end

task :run do
	# Inspecting the demo app itself (console application)
	system("./build/#{TARGET}")
	puts "\n\n\n"
	# Inspecting the Finder (GUI application)
	finder_pid = `ps -A | grep -m1 Finder | awk '{print $1}'`
	system("./build/#{TARGET} #{finder_pid}")
	puts "\n\n\n"
	# Inspecting a process that doesn't exist (invalid PID)
	system("./build/#{TARGET} -12312")
end

task :cleanup do
	system("rm -Rf ./build")
end
