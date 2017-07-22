#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include "commands_big.h"
#include "block_manager.h"

int main() {
    int i;
    int commandCount = sizeof (CommandList) / sizeof (CommandList[0]);
    BlockManager *blckManager = createBlockManager(BLOCK_SIZE);
    for (i = 0; i < commandCount; i++) {
        if (CommandList[i].value == NULL) {
            printf("%s: %s\n", CommandList[i].cmd, CommandList[i].key);
        } else {
            printf("%s: %s %s\n", CommandList[i].cmd, CommandList[i].key, CommandList[i].value);
        }
        executeCommand(blckManager, CommandList[i].cmd, CommandList[i].key, CommandList[i].value);
    }
    deleteBlockManager(blckManager);
    return (0);
}
