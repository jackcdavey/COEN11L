#include <stdio.h>    /** defines the printf function */
#include <stdlib.h>   /** defines malloc */
#include <string.h>   /** defines string functions */
#include <limits.h>   /** defines USHRT_MAX */

/* structure definitions */
typedef struct hist_node {
    char * encryption_key;
    char * url;
    struct hist_node * prev;
    struct hist_node * next;
} hist_node;

typedef struct history {
    char * encryption_key;
    hist_node * head;
    hist_node * tail;
    hist_node * current;
    int max_size;
    int size;
    int cidx; // current index
} history;



/* function prototypes */
// history data structure
history * history_create(const int);
char * encKey();
void history_free(history **);
char history_add(history *, const char *);
char * history_current(history *);
char history_back(history *);
char history_forward(history *);
void history_print(const history *);
void history_print_back(const history *);
void history_print_forward(const history *);
char history_save(const history *, const char *, const char *);
history * history_read(const char *);
char history_file_iter(FILE *, int *, int *, int *, char **, char *);
char history_save_current(const history *, const char *);
// private functions - should not be included in a public header file
hist_node * _history_node_create(const char *);
void _history_node_free(hist_node **);
int _free_history_nodes_rec(hist_node ** h);
void update_enc_key(history * h, char * newKey);
// history tests
void run_tests();
char test_is_url_valid();
char test_history_add_byond_n(history **, const int);
char test_history_back_then_add(history **, const int, const int);
char test_history_back_forward(history **, const int, const int);
char test_history_forward_after_tail(history **, const int, const int);
char test_history_forward_current(history **, const int, const int);
// interactive functions
char is_url_valid(const char * url);
void usage();
char mode_interactive(const char * fname);
char mode_print(const char * fname);

/* macros */
#define max(a,b) ( ((a) > (b)) ? (a) : (b) )
#define min(a,b) ( ((a) < (b)) ? (a) : (b) )

/* global variables */
// NONE for the data structure


int main(const int argc, const char ** argv)
{
    
    if(argc < 3){
        fprintf(stderr, "  Invalid program usage.\n");
        usage();
        return 1;
    }
    
    char mode = 0;
    if(strlen(argv[1]) < 1){
        fprintf(stderr, "  Mode cannot be empty.");
        usage();
        return 1;
    }
    mode = argv[1][0];
    switch(mode){
        case 'i':
            return mode_interactive(argv[2]);
        case 'p':
            return mode_print(argv[2]);
        case 't':
            run_tests();
            break;
        case 'h':
            usage();
            break;
        default:
            fprintf(stderr, "Invalid mode.");
            usage();
            return 1;
    }
    
    return 0;
}


/**
 * Print usage information
 */
void usage()
{
    printf("Usage: lab08 <mode> <filepath>.\n");
    printf("Valid modes:\n");
    printf("    h : print this help information\n");
    printf("    i : interactive history access\n");
    printf("    p : print history data from file\n");
    printf("    t : execute tests\n");
    printf("Valid interactive commands:\n");
    printf("    c : print current node\n");
    printf("    b : go back 1 step and print current node\n");
    printf("    f : go forward 1 step and print current node\n");
    printf("    a : add URL to history\n");
    printf("    m : print history metadata (max_size, size, cidx)\n");
    printf("    p : print entire list\n");
    printf("    B : print history nodes backwards from current node\n");
    printf("    F : print history nodes forwards from current node\n");
    printf("    P : print history by iterating through the history file\n");
    printf("    s : save history to file\n");
    printf("    C : write current node index to file\n");
    printf("    r : re-load history from file\n");
    printf("    q : save history file and exit\n");
    printf("\n");
}


char mode_interactive(const char * fname)
{
    history * h = history_read(fname);
    if(!h){
        fprintf(stderr, "  Invalid history file path: %s.\nNew history will be created. Max size?\n", fname);
        int max_size = 0;
        scanf("%d%*c", &max_size);
        h = history_create(max_size);
    }
    
    char command = 0;
    char buff[2048];
    
    while(1){
        printf("Command?  ");
        scanf("%c%*c", &command);
        char * key = encKey();
        switch(command){
            case 'q':
            case 'Q':
                history_save(h, fname, key);
                history_free(&h);
                return 0;
            case 'c':
                printf("Current URL: index %d, %s\n", h->cidx, h->current ? h->current->url : "NULL");
                break;
            case 'b':
                history_back(h);
                printf("Current URL: index %d, %s\n", h->cidx, h->current ? h->current->url : "NULL");
                break;
            case 'f':
                history_forward(h);
                printf("Current URL: index %d, %s\n", h->cidx, h->current ? h->current->url : "NULL");
                break;
            case 'a':
                printf("URL to add:\n");
                scanf(" %1024[^\n]%*c", buff);
                while(!is_url_valid(buff)){
                    printf("Invalid URL. It should start with http:// or https:// and contain only valid characters.\n");
                    printf("URL to add:\n");
                    scanf(" %1024[^\n]%*c", buff);
                }
                history_add(h, buff);
                printf("Current URL: index %d, %s\n", h->cidx, h->current ? h->current->url : "NULL");
                break;
            case 'm':
                printf("Metadata: size %d, max_size %d, index %d.\n", h->size, h->max_size, h->cidx);
                break;
            case 'p':
                history_print(h);
                break;
            case 'B':
                history_print_back(h);
                break;
            case 'F':
                history_print_forward(h);
                break;
            case 'P':
                if(mode_print(fname) != 0){
                    fprintf(stderr, "  File %s no longer exists or could not be read correctly!\n", fname);
                }
                break;
            case 's':
                if(history_save(h, fname, key)){
                    printf("History saved to %s.\n", fname);
                } else {
                    printf("History could not be saved to %s.\n", fname);
                }
                break;
            case 'C':
                if(!history_save_current(h, fname)){
                    printf("Could not save current node index in %s.\n", fname);
                } else {
                    printf("Current node index updated successfully in %s.\n", fname);
                }
                break;
            case 'r':
                history_free(&h);
                h = history_read(fname);
                if(!h){
                    fprintf(stderr, "  Invalid history file path: %s.\nNew history will be created. Max size?\n", fname);
                    int max_size = 0;
                    scanf("%d%*c", &max_size);
                    h = history_create(max_size);
                }
                printf("History re-loaded from %s.\n", fname);
                break;
            default:
                fprintf(stderr, "  Invalid command. Try again.\n");
                usage();
                break;
        }
    }
    return 0;
}

char mode_print(const char * fname){
    FILE * fin = fopen(fname, "rb");
    if(!fin){
        return 3;
    }
    int buff_size = 2048;
    char * buff = (char *) malloc(sizeof(char) * (buff_size + 1));
    int idx = 0;
    int cidx = 0;
    char current;
    printf("History:\n");
    while(history_file_iter(fin, &idx, &cidx, &buff_size, &buff, &current)){
        printf(" %c %s\n", current ? '*' : ' ', buff);
    }
    printf("\n");
    free(buff);
    fclose(fin);
    return 0;
}

/**
 * Create a history node with a copy of the url as payload.
 * @param  url URL payload
 * @return     Pointer to newly created node
 */
hist_node * _history_node_create(const char * url){
    hist_node * n = (hist_node *) malloc(sizeof(hist_node));
    if(!n){
        return NULL;
    }
    char * eK="blah";
    n->url = (char*) malloc((strlen(url)+1) * sizeof(char));
    n->encryption_key = (char*) malloc((strlen(eK)+1) * sizeof(char));
    strcpy(n->url, url);
    n->prev = NULL;
    n->next = NULL;
    n->encryption_key= eK;
    return n;
}

/**
 * Free memory for a history node. Sets pointer to node to NULL after free.
 * @param n Reference to a node
 */
void _history_node_free(hist_node ** n){
    if(!n || (*n) == NULL){
        return;
    }
    // free the payload
    free((*n)->url);
    // free the node struct
    free(*n);
    *n = NULL;
}

/**
 * Recursively free nodes starting at current node
 * @param  h pointer to current node
 * @return   Number of nodes that were freed
 */
int _free_history_nodes_rec(hist_node ** h){
    int n = 0;
    if((*h)->next){
        n = _free_history_nodes_rec(&((*h)->next));
    }
    _history_node_free(h);
    return n + 1;
}

/**
 * Create a history data structure
 * @return  Pointer to newly created history data structure
 */
history * history_create(const int max_size){
    history * h = (history *) malloc(sizeof(history));
    if(!h){
        return NULL;
    }
    memset(h, 0, sizeof(history));
    h->max_size = max_size;
    return h;
}

/**
 * Free a history data structure
 * @param hp Reference to history data structure
 */
void history_free(history ** hp){
    if(!hp || (*hp) == NULL){
        return;
    }
    hist_node *p, *c = (*hp)->head;
    // free nodes within list
    while(c){
        p = c;
        c = c->next;
        _history_node_free(&p);
    }
    // free data structure
    free(*hp);
    *hp = NULL;
}

/**
 * Add url to history, to the right of current
 * @param h   History data structure
 * @param url URL to add
 * @return   1 if successful (position changed), 0 otherwise
 */
char history_add(history * h, const char * url){
    if(!h || h->max_size < 1){
        return 0;
    }
    // free any elements to the right of current
    if(h->current && h->current->next){
        h->size -= _free_history_nodes_rec(&(h->current->next));
        h->cidx = h->size - 1 < 0 ? 0 : h->size - 1;
    }
    // create new node
    hist_node * n = _history_node_create(url);
    if(!n){
        return 0;
    }
    if(h->current){
        n->prev = h->current;
        h->current->next = n;
    }
    h->tail = h->current = n;
    if(h->size == 0){ // empty history
        h->head = n;
        h->size++;
    } else if(h->size == h->max_size){ // full history
        // forget first element in the list; size does not change
        hist_node * c = h->head;
        h->head = h->head->next;
        _history_node_free(&c);
    } else { // size < max_size
        h->size++;
    }
    h->cidx = h->size - 1;
    return 1;
}

/**
 * Get current URL, or NULL if list is empty.
 * @param  h History data structure
 * @return   URL string, or NULL if list is empty.
 */
char * history_current(history * h){
    if(!h || !h->current){
        return NULL;
    }
    return h->current->url;
}

/**
 * Advance history one position backward from current, up to head.
 * @param  h History data structure
 * @return   1 if successful (position changed), 0 otherwise
 */
char history_back(history * h){
    if(!h || !h->current || !h->current->prev){
        return 0;
    }
    h->current = h->current->prev;
    h->cidx--;
    return 1;
}

/**
 * Advance history one position forward from current, up to tail.
 * @param  h History data structure
 * @return   1 if successful (position changed), 0 otherwise
 */
char history_forward(history * h){
    if(!h || !h->current || !h->current->next){
        return 0;
    }
    h->current = h->current->next;
    h->cidx++;
    return 1;
}

/**
 * Print contents of history, noting current position
 * @param h History data structure
 */
void history_print(const history * h){
    if(!h){
        return;
    }
    hist_node *p = h->head;
    printf("History:\n");
    while(p){
        printf(" %c %s\n", p == h->current ? '*' : ' ', p->url);
        p = p->next;
    }
    printf("\n");
}

/**
 * Print contents of history, backwards from current position.
 * @param h History data structure
 */
void history_print_back(const history * h){
    if(!h){
        return;
    }
    hist_node *p = h->current;
    printf("Back history:\n");
    while(p){
        printf("%s\n", p->url);
        p = p->prev;
    }
    printf("\n");
}

/**
 * Print contents of history, forwards from current position.
 * @param h History data structure
 */
void history_print_forward(const history * h){
    if(!h){
        return;
    }
    hist_node *p = h->current;
    printf("Forward history:\n");
    while(p){
        printf("%s\n", p->url);
        p = p->next;
    }
    printf("\n");
}

void update_enc_key(history * h, char * newKey){
    h->encryption_key = newKey;
    //history_save(h, fname, newKey);
}

/**
 * Write a node to the file
 * @param n    Node to write
 * @param fout File to write to
 */
char _history_write_node(const hist_node * n, int nidx, FILE * fout)
{
    if(!n || !fout){
        return 0;
    }
    
    // write size of string (limit to size of unsigned short)
    int l = strlen(n->url);
    if(l > (size_t) USHRT_MAX){
        printf("Warning: Truncating URL for node %d to %d characters on file save.\n", nidx+1, USHRT_MAX);
        l = USHRT_MAX;
    }
    unsigned short ul = l;
    if(fwrite(&ul, sizeof(unsigned short), 1, fout) != 1){
        return 0;
    }
    
    // write string (no null char)
    printf("madeit");
    char writeThis;
    char * key= n->encryption_key;
    char *u;
    strncpy(u, n->url, strlen(n->url));
    int keyLength = strlen(n->encryption_key);
    for(int i=0; i<strlen(u); ++i){
        u[i]^=*(key + (i % keyLength-1));
    }
    if(fwrite(u, sizeof(char), l, fout) != l){
        return 0;
    }
    
    // done
    return 1;
}

char * encKey(hist_node * n){
    char *key = n->encryption_key;
    return key;
}

/**
 * Save history in a binary file
 * @param h     History to save
 * @param fname File path
 * @return       1 if success, 0 otherwise
 */
char history_save(const history * h, const char * fname, const char * key){
    FILE * fout = NULL;
    if(!(fout = fopen(fname, "wb"))){
        return 0;
    }
    // write current node index
    if(fwrite(&(h->cidx), sizeof(int), 1, fout) != 1){
        return 0;
    }
    // write max size of list
    if(fwrite(&(h->max_size), sizeof(int), 1, fout) != 1){
        return 0;
    }
    // write size of list
    if(fwrite(&(h->size), sizeof(int), 1, fout) != 1){
        return 0;
    }
    // write nodes
    hist_node * p = h->head;
    int i = 0;
    while(p){
        if(!_history_write_node(p, i, fout)){
            return 0;
        }
        i++;
        p = p->next;
    }
    
    fclose(fout);
    
    return 1;
}


hist_node * _history_read_node(FILE * fin){
    if(!fin){
        return NULL;
    }
    
    // read size of string (limit is size of unsigned short)
    unsigned short ul = 0;
    if(fread(&ul, sizeof(unsigned short), 1, fin) != 1){
        return NULL;
    }
    // allocate memory for the node and the string
    hist_node * n = (hist_node*) malloc(sizeof(hist_node));
    if(!n){
        return n;
    }
    n->prev = n->next = NULL;
    n->url = (char*) malloc(sizeof(char) * (1 + (size_t) ul));
    if(!n->url){
        free(n);
        return NULL;
    }
    
    // read string (no null char)
    if(fread(n->url, sizeof(char), ul, fin) != ul){
        free(n->url);
        free(n);
        return NULL;
    }
    
    // set null char to end string
    n->url[ul] = '\0';
    
    // done
    return n;
}

/**
 * Load history from a binary file
 * @param  fname File path
 * @return       History data structure
 */
history * history_read(const char * fname)
{
    history * h = NULL;
    int max_size, size, cidx;
    FILE * fin = NULL;
    
    if(!(fin = fopen(fname, "rb"))){
        return NULL;
    }
    
    // attempt to read history metadata
    if(fread(&cidx, sizeof(int), 1, fin) != 1){
        return NULL;
    }
    if(fread(&max_size, sizeof(int), 1, fin) != 1){
        return NULL;
    }
    if(fread(&size, sizeof(int), 1, fin) != 1){
        return NULL;
    }
    
    // validate data
    if(size < 0 || cidx < 0 || max_size < 0 || cidx >= size || size > max_size){
        return NULL;
    }
    
    // create history data structure
    h = history_create(max_size);
    if(!h){
        return NULL;
    }
    
    if(size == 0){
        return h;
    }
    
    // add nodes
    hist_node * n = _history_read_node(fin);
    h->head = h->tail = n;
    if(cidx == 0){
        h->current = n;
    }
    for(int i=1; i < size; ++i){
        n = _history_read_node(fin);
        if(!n){
            history_free(&h);
            return NULL;
        }
        if(cidx == i){
            h->current = n;
        }
        n->prev = h->tail;
        h->tail->next = n;
        h->tail = n;
    }
    h->size = size;
    h->cidx = cidx;
    
    // close file
    fclose(fin);
    
    return h;
}

/**
 * Iterator for history binary file
 * @param fin        File pointer opened with rb (or rwb) for history binary file
 * @param idx        Index in the list
 * @param cidx       Index of the current node
 * @param max_strlen Pointer to current size of string buffer
 * @param strp       Pointer to current string buffer - return argument
 * @param current    Pointer to current node specifier - return argument (0 - not current, 1 - current)
 * @return           1 if success, 0 otherwise
 */
char history_file_iter(FILE * fin, int * idx, int * cidx, int * max_strlen, char ** strp, char * current){
    if(!fin){
        return 0;
    }
    size_t pos = ftell(fin);
    // if at the start of the file, read cidx
    if(pos == 0){
        if(fread(cidx, sizeof(int), 1, fin) != 1){
            return 0;
        }
        *idx = -1;
    }
    // if necessary, jump to the first record
    if(pos < 3 * sizeof(int)){
        fseek(fin, 3 * sizeof(int), SEEK_SET);
    }
    unsigned short ul = 0;
    if(fread(&ul, sizeof(unsigned short), 1, fin) != 1){
        return 0;
    }
    // re-allocate memory for the string buffer if necessary
    if(*max_strlen < (int) ul + 1){
        *strp = realloc(*strp, sizeof(char) * ((int) ul + 1));
        if(!(*strp)){
            return 0;
        }
        *max_strlen = (int) ul;
    }
    // read URL string (no null char)
    if(fread(*strp, sizeof(char), ul, fin) != ul){
        return 0;
    }
    // set null char to end string
    (*strp)[ul] = '\0';
    // increment index
    (*idx)++;
    // set current status
    *current = *idx == *cidx;
    // done
    return 1;
}

/**
 * Save current position in the file storage of a history data structure
 * @param  h     History data structure
 * @param  fname File path
 * @return       1 if success, 0 otherwise
 */
char history_save_current(const history * h, const char * fname)
{
    FILE * fout = NULL;
    if(!(fout = fopen(fname, "r+b"))){
        return 0;
    }
    if(fseek(fout, 2 * sizeof(int), SEEK_SET) != 0){
        fclose(fout);
        return 0;
    }
    // read size of list
    int size = 0;
    if(fread(&size, sizeof(int), 1, fout) != 1){
        fclose(fout);
        return 0;
    }
    if(h->size != size){
        fclose(fout);
        return 0; // size miss-match
    }
    if(fseek(fout, 0, SEEK_SET) != 0){
        fclose(fout);
        return 0;
    }
    if(fwrite(&(h->cidx), sizeof(int), 1, fout) != 1){
        fclose(fout);
        return 0;
    }
    fclose(fout);
    return 1;
}

/**
 * Check if current url is valid
 * @param  url URL to check
 * @return     1 if valid, 0 otherwise
 */
char is_url_valid(const char * url)
{
    static const char * allowed = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ./_+-&%#$@?~";
    // verify URL starts with http or https
    if(strstr(url, "http") != url){
        return 0; // does not start with http
    }
    url += 4;
    // check for secure http
    if(*url == 's'){
        url++;
    }
    if(strstr(url, "://") != url){
        return 0; // does not start with ://
    }
    url += 3;
    if(!url){
        return 0; // only has http:// or https://
    }
    // check that the rest of the URL has only allowed chars and at least one .
    unsigned short ndots = 0;
    while(*url){
        if(strchr(allowed, *url) == NULL){
            return 0;
        }
        if(*url == '.'){
            ndots++;
        }
        url++;
    }
    if(ndots < 1){
        return 0;  // should have at least one dot
    }
    // could add more checks, e.g., domain should be one in a list of valid domains, etc.
    return 1;
}



//
// history tests
//

// test data global vars

const char urls[][50] = {
    "https://www.apple.com",
    "https://www.google.com",
    "https://www.msn.com",
    "https://www.facebook.com",
    "https://www.whystudy.com",
    "https://www.tolearn.com",
    "https://www.cisbest.com",
};
const int n_urls = 7;

/**
 * Execute all tests
 */
void run_tests(){
    
    int list_sizes[] = {0, 1, 5, 10, 13};
    int back_forward_steps[] = {1, 5, 14};
    int print_from_position[] = {0, 1, 5, 14};
    history * h = NULL;
    
    printf("test_is_url_valid: %s\n", test_is_url_valid() ? "pass" : "fail");
    
    for(int i=0; i < sizeof(list_sizes)/sizeof(int); ++i){
        printf("test_history_add_byond_n(%d): %s\n", list_sizes[i],
               test_history_add_byond_n(&h, list_sizes[i]) ? "pass" : "fail");
        history_free(&h);
        for(int j=0; j < sizeof(back_forward_steps)/sizeof(int); ++j){
            printf("test_history_back_then_add(%d, %d): %s\n", list_sizes[i], back_forward_steps[j],
                   test_history_back_then_add(&h, list_sizes[i], back_forward_steps[j]) ? "pass" : "fail");
            history_free(&h);
        }
        for(int j=0; j < sizeof(back_forward_steps)/sizeof(int); ++j){
            printf("test_history_back_forward(%d, %d): %s\n", list_sizes[i], back_forward_steps[j],
                   test_history_back_forward(&h, list_sizes[i], back_forward_steps[j]) ? "pass" : "fail");
            history_free(&h);
        }
        for(int j=0; j < sizeof(back_forward_steps)/sizeof(int); ++j){
            printf("test_history_forward_after_tail(%d, %d): %s\n", list_sizes[i], back_forward_steps[j],
                   test_history_forward_after_tail(&h, list_sizes[i], back_forward_steps[j]) ? "pass" : "fail");
            history_free(&h);
        }
        for(int j=0; j < sizeof(print_from_position)/sizeof(int); ++j){
            printf("test_history_forward_current(%d, %d): %s\n", list_sizes[i], print_from_position[j],
                   test_history_forward_current(&h, list_sizes[i], print_from_position[j]) ? "pass" : "fail");
            history_free(&h);
        }
    }
}

char test_is_url_valid()
{
    for(int i=0; i < n_urls; ++i){
        if(!is_url_valid(urls[i])){
            return 0;
        }
    }
    if(is_url_valid("www.google.com")){
        return 0;
    }
    if(is_url_valid("http://www.google.com\t")){
        return 0;
    }
    if(is_url_valid("http:/ /www.google.com")){
        return 0;
    }
    return 1;
}

/**
 * Test adding more than max_size elements in the history
 * @param  hp       Reference pointer to a history data structure
 * @param  max_size Max size of list
 * @return          1 if test pass, 0 otherwise
 */
char test_history_add_byond_n(history ** hp, const int max_size){
    history * h;
    if(*hp){
        history_free(hp);
    }
    h = *hp = history_create(max_size);
    if(!h){
        return 0;
    }
    
    // ensure history created successfully
    if(!h){
        return 0; // each return 0 could be a separate test...
    }
    if(h->max_size != max_size){
        return 0;
    }
    if(h->size != 0){
        return 0;
    }
    // fill history with URLs
    for(int i=0; i < max_size; ++i){
        history_add(h, urls[i % n_urls]);
        if(h->size != i+1){
            return 0;
        }
        if(!h->current || strcmp(h->current->url, urls[i % n_urls]) != 0){
            return 0;
        }
        if(h->tail != h->current){
            return 0;
        }
        if(!h->head || strcmp(h->head->url, urls[0]) != 0){
            return 0;
        }
    }
    // must be able to remember history for the rest of the tests
    if(max_size < 1){
        return 1;
    }
    // add 1 URL beyond max_size urls
    history_add(h, urls[max_size % n_urls]);
    if(h->size != max_size){
        return 0;
    }
    // this check is only valid if max_size > 1
    if(max_size > 1 && (!h->head || strcmp(h->head->url, urls[1]) != 0)){
        return 0;
    }
    // add one more URL
    history_add(h, urls[(max_size+1) % n_urls]);
    if(h->size != max_size){
        return 0;
    }
    // this check is only valid if max_size > 2
    if(max_size > 2 && (!h->head || strcmp(h->head->url, urls[2]) != 0)){
        return 0;
    }
    return 1;
}

/**
 * Test going back #steps elements, then adding an element.
 * @param  max_size Max size of list
 * @param  steps    Number of steps to go back
 * @return          1 if test pass, 0 otherwise
 */
char test_history_back_then_add(history ** hp, const int max_size, const int steps){
    history * h;
    if(*hp){
        history_free(hp);
    }
    h = *hp = history_create(max_size);
    if(!h){
        return 0;
    }
    
    // must be able to remember history for the rest of the tests
    if(max_size < 1){
        return 1;
    }
    // fill history with URLs
    for(int i=0; i < max_size; ++i){
        history_add(h, urls[i % n_urls]);
    }
    // current is now at tail. Go back #steps
    for(int i=max_size-1, j=0; j < steps; ++j){
        history_back(h);
        //check current url
        i = max(i-1, 0);
        if(!h->current || strcmp(history_current(h), urls[i % n_urls]) != 0){
            return 0;
        }
    }
    // if we went back at least max_size steps, we should be at head
    if(steps >= max_size && h->current != h->head){
        return 0;
    }
    // at it should have urls[0] as payload
    if(steps >= max_size && (!h->current || strcmp(history_current(h), urls[0]) != 0)){
        return 0;
    }
    // now add a url
    history_add(h, urls[0]);
    // verify size
    int sz = max_size; // initial size
    sz -= steps; // back #steps
    sz = max(1, sz); // we can only go back to first url, not beyond
    sz += 1; // we added a URL
    sz = min(max_size, sz); // we cannot have more than max_size URLs
    if(h->size != sz){
        return 0;
    }
    // now check that linked list node count matches size
    hist_node *p = h->head;
    int s;
    for(s=0; p; s++){ // note the for loop ending condition does not use s
        p = p->next;
    }
    if(s != sz){
        return 0;
    }
    return 1;
}

/**
 * Test going back #steps elements, forward #steps/2 elements, then verify current.
 * @param  max_size Max size of list
 * @param  steps    Number of steps to go back, from tail.
 * @return          1 if test pass, 0 otherwise
 */
char test_history_back_forward(history ** hp, const int max_size, const int steps){
    history * h;
    if(*hp){
        history_free(hp);
    }
    h = *hp = history_create(max_size);
    if(!h){
        return 0;
    }
    
    // must be able to remember history for the rest of the tests
    if(max_size < 1){
        return 1;
    }
    // fill history with URLs
    for(int i=0; i < max_size; ++i){
        history_add(h, urls[i % n_urls]);
    }
    // current is now at tail. Go back #steps.
    for(int j=0; j < steps; ++j){
        history_back(h);
    }
    // now go forward #steps/2
    for(int j=0; j < steps/2; ++j){
        history_forward(h);
    }
    // find url index: min(max_size - 1, max(0, max_size - 1 - steps) + steps/2)
    int idx = max_size - 1; // current index/size
    idx -= steps; // go back #steps
    idx = max(0, idx); // we don't go back beyond the head of the list
    idx += steps/2; // go forward #steps/2
    idx = min(max_size-1, idx); // we don't go beyond the tail of the list
    if(!h->current || strcmp(h->current->url, urls[ idx % n_urls ]) != 0){
        return 0;
    }
    return 1;
}

/**
 * Test going forward #steps elements, then verify current.
 * @param  max_size Max size of list
 * @param  steps    Number of steps to go forward, from head.
 * @return          1 if test pass, 0 otherwise
 */
char test_history_forward_after_tail(history ** hp, const int max_size, const int steps){
    history * h;
    if(*hp){
        history_free(hp);
    }
    h = *hp = history_create(max_size);
    if(!h){
        return 0;
    }
    
    // must be able to remember history for the rest of the tests
    if(max_size < 1){
        return 1;
    }
    // fill history with URLs
    for(int i=0; i < max_size; ++i){
        history_add(h, urls[i % n_urls]);
    }
    // go forward #steps
    for(int j=0; j < steps; ++j){
        history_forward(h);
    }
    // verify current is still at tail
    if(!h->current || h->current != h->tail || h->size != max_size){
        return 0;
    }
    if(strcmp(history_current(h), urls[ (max_size-1) % n_urls ]) != 0){
        return 0;
    }
    
    return 1;
}

/**
 * Test going forward #steps elements from head, then verify current.
 * @param  max_size Max size of list
 * @param  steps    Number of steps to go forward, from head.
 * @return          1 if test pass, 0 otherwise
 */
char test_history_forward_current(history ** hp, const int max_size, const int steps){
    history * h;
    if(*hp){
        history_free(hp);
    }
    h = *hp = history_create(max_size);
    if(!h){
        return 0;
    }
    
    // must be able to remember history for the rest of the tests
    if(max_size < 1){
        return 1;
    }
    // fill history with URLs
    for(int i=0; i < max_size; ++i){
        history_add(h, urls[i % n_urls]);
    }
    // make current the start of the list
    h->current = h->head;
    // go forward #steps
    for(int j=0; j < steps; ++j){
        history_forward(h);
    }
    // verify current
    int idx = 0; // we're starting at the head of the list
    idx += steps; // advancing #steps
    idx = min(max_size-1, idx); // cannot go beyond the end of the list
    if(strcmp(history_current(h), urls[ idx % n_urls ]) != 0){
        return 0;
    }
    
    return 1;
}
