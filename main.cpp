#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include "commands_big.h"
#include "block_manager.h"
#include "OAHash.h"
#include "hash.h"
#include "assert.h"


typedef unsigned short len_t;

void f() { 
    char data[10];
    char *key = data;
    strncpy(key, "ABCDE", 5);
    len_t *len_slot = (len_t*)(data + KEY_SIZE);
    *len_slot = 123;
    len_t len = *((len_t*)(data + KEY_SIZE));
    char *v = data + KEY_SIZE + sizeof(len_t);
    *v = 'X';
    
}


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
