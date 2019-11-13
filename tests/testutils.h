const int data_counts;

typedef struct _context {
	bptree_t *bpt;
	int *values;
	int npassed;
	int nfailed;
} context;

context *create_context(int size);
void *destroy_context(context *c);

int do_test(char *print_string, void (*prepare)(context *c), void (*verify)(context *c));

int lcg(int prev, int n);

void check_search(context *c);

void insert_in_asc(context *c);
void insert_in_desc(context *c);
void insert_in_random(context *c);

