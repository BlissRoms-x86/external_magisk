#include "giveroot.cpp"

int main(int argc, char **argv) {
        if (argc != 2)
                return 1;
        return give_root(atoi(argv[1]));
};

