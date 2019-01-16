#include "giveroot.cpp"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <thread>
#include <chrono>
#include <vector>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "rootserver.h"

void GetStdoutFromCommand(void (*callback)(std::string)) {
	pid_t pid = 0;
	int pipefd[2];
	FILE* output;
	std::array<char, 128> buffer;
	std::string line;
	int status;

	pipe(pipefd); //create a pipe
	pid = fork(); //span a child process
	if (pid == 0) {
		// Child. Let's redirect its standard output to our pipe and replace process with tail
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		execl("/system/bin/logcat", "/system/bin/logcat", "-b", "events", "-v", "raw", "-s", "am_proc_bound:I", (char*) NULL);
	}

	//Only parent gets here. Listen to what the tail says
	close(pipefd[1]);
	output = fdopen(pipefd[0], "r");

	while (1) {
		while(fgets(buffer.data(), buffer.size(), output) != nullptr) { //listen to what tail writes to its standard output
			//if you need to kill the tail application, just kill it:
//			std::cout << buffer.data();
			line += std::string(buffer.data());
//			std::cout << "2nd last char---";
//			std::cout << std::string(buffer.data()).back();
//			std::cout << "---2nd last char done.";
			if (std::string(buffer.data()).back() == '\n')
				break;
		};
//		std::cout << line;
		(*callback)(line);
		line.clear();
	}
	//or wait for the child process to terminate
	waitpid(pid, &status, 0);
}


void cb(std::string line) {
//	std::cout << line;
	std::string delimiter = ",";
	line.erase(0,3);
	int pid = stoi(line.substr(0, line.find(delimiter, 0)));
	line.erase(0, line.find(delimiter, 0)+1);
	line.erase(0, line.find(delimiter, 0)+1);
	std::string packageName = line.substr(0, line.find(delimiter, 0));
	packageName = packageName.substr(0, packageName.size()-2);
	std::cout << "Giving root to pid " << pid << ", package name " << packageName << " now.\n";
	std::cout << "give_root returned " << give_root(pid) << '\n';
//	std::cout << tokens[1];
};

