#include "rootserver.cpp"

int main(int argc, char **argv) {
        if (argc == 1)
                return 0; //No need to watch for processes...
        GetStdoutFromCommand(cb);
        return 2; // This should never be reached.
};

