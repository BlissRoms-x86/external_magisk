static int switch_mnt_ns(int pid);
static int read_ns(const int pid, struct stat *st);
static int parse_ppid(int pid);
int main(int argc, char **argv);
int give_root(int pid);
int do_give_root();

