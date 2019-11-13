const int data_counts;

typedef enum _bptree_test_result {
	BPTREE_TEST_PASSED,
	BPTREE_TEST_FAILED,
} bptree_test_result;

typedef enum _bptree_error_type {
	BPTREE_ERROR_FATAL,
	BPTREE_ERROR_WARN,
} bptree_error_type;

typedef struct _bptree_test_context {
	bptree_t *bpt;
	int *values;
	int npassed;
	int nfailed;
} bptree_test_context;

bptree_test_context *bptree_create_test_context(int size);
void *bptree_destroy_test_context(bptree_test_context *c);

bptree_test_result do_test(char *print_string, 
		bptree_test_result (*prepare)(bptree_test_context *c), 
		bptree_test_result (*verify)(bptree_test_context *c));

int lcg(int prev, int n);

bptree_test_result check_search(bptree_test_context *c);

bptree_test_result insert_in_asc(bptree_test_context *c);
bptree_test_result insert_in_desc(bptree_test_context *c);
bptree_test_result insert_in_random(bptree_test_context *c);

