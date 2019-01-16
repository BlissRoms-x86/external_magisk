#include <sched.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mount.h>
#include "giveroot.h"

static int switch_mnt_ns(int pid) {
	char mnt[32];
	snprintf(mnt, sizeof(mnt), "/proc/%d/ns/mnt", pid);
	if(access(mnt, R_OK) == -1) return 1; // Maybe process died..

	int fd, ret;
	fd = open(mnt, O_RDONLY);
	if (fd < 0) return 1;
	// Switch to its namespace
	ret = setns(fd, 0);
	close(fd);
	return ret;
};

static int read_ns(const int pid, struct stat *st) {
	char path[32];
	sprintf(path, "/proc/%d/ns/mnt", pid);
	return stat(path, st);
};

static int parse_ppid(int pid) {
	char path[32];
	int ppid;
	sprintf(path, "/proc/%d/stat", pid);
	FILE *stat = fopen(path, "re");
	if (stat == nullptr)
		return -1;
	/* PID COMM STATE PPID ..... */
	fscanf(stat, "%*d %*s %*c %d", &ppid);
	fclose(stat);
	return ppid;
};

int give_root(int pid) {
	int ppid;
	if (access("/proc/1/ns/mnt", F_OK) != 0)
		return 2; // No mount namespaces in kernel
	struct stat ns, pns;
	read_ns(pid, &ns);
	if ((ppid = parse_ppid(pid)) < 0 || read_ns(ppid, &pns))
		return 3; //Process died.
//	while (read_ns(pid, &ns) == 0 && ns.st_dev == pns.st_dev && ns.st_ino == pns.st_ino)
//		usleep(500);
	if (switch_mnt_ns(pid) == 0) {
		if (do_give_root() != 0) {
			return 5; //Failed to mount.
		};
	} else {
		return 4;
	};
	return 0;
};

int do_give_root() {
	return mount("/system/etc/nomagic", "/system/xbin", "", MS_PRIVATE|MS_BIND, "");
};
