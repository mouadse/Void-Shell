valgrind -s --track-fds=yes --tool=memcheck --leak-check=full --show-leak-kinds=all --suppressions=readline.supp  ./tester
