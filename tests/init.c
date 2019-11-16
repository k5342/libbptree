#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

bptree_test_result testcase_1(){
	bptree_t *bpt = bptree_init(-1);
	if (bpt == NULL){
		return BPTREE_TEST_PASSED;
	}
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_2(){
	bptree_t *bpt = bptree_init(0);
	if (bpt == NULL){
		return BPTREE_TEST_PASSED;
	}
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_3(){
	bptree_t *bpt = bptree_init(10);
	if (bpt == NULL)      { return BPTREE_TEST_FAILED; }
	if (bpt->root == NULL){ return BPTREE_TEST_FAILED; }
	if (bpt->nkeys != 10) { return BPTREE_TEST_FAILED; }
	return BPTREE_TEST_PASSED;
}

bptree_test_result testcase_4(){
	bptree_t *bpt = bptree_init(10);
	if (bpt == NULL)            { return BPTREE_TEST_FAILED; }
	if (bpt->root == NULL)      { return BPTREE_TEST_FAILED; }
	if (bpt->root->used != 0)   { return BPTREE_TEST_FAILED; }
	if (bpt->root->is_leaf != 1){ return BPTREE_TEST_FAILED; }
	return BPTREE_TEST_PASSED;
}

int main(int argc, char *argv[]){
	do_single_test("check bptree_init argument validation #1", testcase_1);
	do_single_test("check bptree_init argument validation #2", testcase_2);
	do_single_test("check bptree_t", testcase_3);
	do_single_test("check root node", testcase_4);
}
