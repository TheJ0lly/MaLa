VM="main/mvm.c"
COMMON="src/*.c"
CC_FLAGS="-Wall -Wextra -Wswitch -Wpedantic -Werror -std=gnu11"

echo "Compiling VM"

gcc $VM $COMMON -I./include -Wall -Wextra -Wswitch -o ./bin/mvm

echo "VM Compilation Finished"