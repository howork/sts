#include <string.h>

#include "xUtil.h"
#include "xToken.h"
#include "xSymbolTable.h"

struct _xSTPair;
typedef struct _xSTPair xSTPair;

struct _xSymbolTable {
            int load;
            int capacity;
            xSTPair* slot;
        };


typedef struct _xSTPair {
            char *symbol;
            xToken* token;
        } xSTPair;

        
static int nextPrime(int n);  // n보다 크거나 같은 최소 솟수를 리턴
static unsigned int hashString(char *string, unsigned int modulus); 
      

xSymbolTable* xSymbolTable_create(int capacity) 
{
    capacity = (capacity <= 0 ? xSymbolTable_DEFAULT_CAPACITY : nextPrime(capacity));
    
    xSymbolTable* table = xalloc(xSymbolTable);
    table->capacity = capacity;
    table->load = 0;
    table->slot = xnalloc(xSTPair, capacity);
    for (int i = 0; i < table->capacity; i++) {
        table->slot[i].symbol = NULL;
    }
    
    return table;
}


void xSymbolTable_putHelper(xSymbolTable* table, char *symbol, xToken* token) 
{    
    unsigned int hash = hashString(symbol, table->capacity);

    unsigned int i = hash;
    unsigned int inc = hash / 2 + 1;
    while (table->slot[i].symbol != NULL) {
        // Look for slots contains a key that matches symbol.
        if (strcmp(table->slot[i].symbol, symbol) == 0) {
            break;
        }
        i =  (i + inc) % table->capacity;
    }
    
    if (table->slot[i].symbol != NULL) {
        // We found symbol.  Change the value associated with it to token. */
        xToken_free(table->slot[i].token);
        table->slot[i].token = token;
    }
    else {
        // We reached the slot[] that should contain the new association. 
        // put the (symbol, token) pair into the slots
        // symbol과 token은 복제해서 삽입된다.
        table->slot[i].symbol = xstrdup(symbol);
        table->slot[i].token = xmemdup(xToken, token);
        table->load++;
    }
}


void xSymbolTable_put(xSymbolTable* table, char* symbol, xToken* token) 
{
   // Check to see whether the loading factor is greater or equel maximum loading factor.
    if ((double)table->load/table->capacity >= xSymbolTable_MAX_LOAD_FACTOR) {
        // enlarge table.
        unsigned int odlCapacity = table->capacity;
        xSTPair* oldSlot = table->slot;
        
        table->capacity = nextPrime(table->capacity * 2);
        table->slot = (xSTPair*) malloc(sizeof(xSTPair) * table->capacity);
        for (int i = 0; i < table->capacity; i++) {
            table->slot[i].symbol = NULL;
        }
        
        for (int i = 0; i < odlCapacity; i++) {
            if (oldSlot[i].symbol == NULL) {
                continue;
            }
            xSymbolTable_putHelper(table, oldSlot[i].symbol, oldSlot[i].token);
        }
        free(oldSlot);
    }
    xSymbolTable_putHelper(table, symbol, token);
}

/* The ltht_get function recovers and returns the token associated with a
   given symbol in a given symbol-token hash table.  If the table contains
   no association for the given symbol, a null pointer is returned instead.
 */

xToken* xSymbolTable_get(xSymbolTable* table, char *symbol) 
{
    unsigned int hash = hashString(symbol, table->capacity);
    unsigned int i = hash;
    unsigned int inc = hash / 2 + 1;
    while (table->slot[i].symbol != NULL) {
        if (strcmp(table->slot[i].symbol, symbol) == 0) {
            return table->slot[i].token;
        }
        i =  (i + inc) % table->capacity;
    }
    
    return NULL;
}

/* The ltht_load function determines and returns the number of associations
   currently stored in a given symbol-token hash table.
 */

int xSymbolTable_load(xSymbolTable* table) 
{
    return table->load;
}


/* The free_ltht function deallocates the storage associated with the 
   symbol-token hash table to which a given pointer points.  It also
   replaces that pointer with a null pointer, so that the caller cannot
   accidentally dereference a pointer to deallocated storage.

   Storage for each symbol field within a value is freed when storage for
   the key is freed.
 */

void xSymbolTable_free(xSymbolTable* table) 
{
    free(table);
}


static int isPrime(int n); // 주어진 수가 솟수인지 판별
static int isPrime(int n) 
{
    if (n <= 1) {
        return 0;
    }
    
    if ((n & 0x01) == 0x00) { // even number
        return n == 2;
    }
    
    for (int k = 3; k * k <= n; k += 2) {
        if (n % k == 0) {
            return 0;
        }
    }
    
    return 1;
}

static int nextPrime(int n) 
{
    if (n <= 2) {
        return 2;
    }
    
    n |= 0x01; // change to the odd number
    while (!isPrime(n)) {
        n += 2;
    }
    return n;
}
        
static unsigned int hashString(char *string, unsigned int modulus) 
{
    unsigned long hash = 0;
    char ch;
    
    while ((ch = *string++) != '\0') {
        hash = ch + (hash << 6) + (hash << 16) - hash;
    }
    
    return hash % modulus;
}

