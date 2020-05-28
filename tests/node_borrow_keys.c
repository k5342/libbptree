#include <bptree.h>
#include <stdio.h>
#include <stdlib.h>
#include "testutils.h"

// create
// [
//   [
//     {0x0 0 0x1 1 0x2 2 0x3 3 0x4 4 0x5 5|XXXX}
//     20
//     {0x14 20 0x15 21 0x16 22 0x17 23 0x18 24 0x19 25|XXXX}
//     40
//     {0x28 40 0x29 41 0x2a 42 0x2b 43 0x2c 44 0x2d 45|XXXX}
//     60
//     {0x3c 60 0x3d 61 0x3e 62 0x3f 63 0x40 64 0x41 65|XXXX}
//     80
//     {0x50 80 0x51 81 0x52 82 0x53 83 0x54 84 0x55 85|XXXX}
//     100
//     {0x64 100 0x65 101 0x66 102 0x67 103 0x68 104 0x69 105|XXXX}
//   ]
//   120
//   [
//     {0x78 120 0x79 121 0x7a 122 0x7b 123 0x7c 124 0x7d 125|XXXX}
//     140
//     {0x8c 140 0x8d 141 0x8e 142 0x8f 143 0x90 144 0x91 145|XXXX}
//     160
//     {0xa0 160 0xa1 161 0xa2 162 0xa3 163 0xa4 164 0xa5 165|XXXX}
//     180
//     {0xb4 180 0xb5 181 0xb6 182 0xb7 183 0xb8 184 0xb9 185|XXXX}
//     200
//     {0xc8 200 0xc9 201 0xca 202 0xcb 203 0xcc 204 0xcd 205|XXXX}
//     220
//     {0xdc 220 0xdd 221 0xde 222 0xdf 223 0xe0 224 0xe1 225|XXXX}
//   ]
//   240
//   [
//     {0xf0 240 0xf1 241 0xf2 242 0xf3 243 0xf4 244 0xf5 245|XXXX}
//     260
//     {0x104 260 0x105 261 0x106 262 0x107 263 0x108 264 0x109 265|XXXX}
//     280
//     {0x118 280 0x119 281 0x11a 282 0x11b 283 0x11c 284 0x11d 285|XXXX}
//     300
//     {0x12c 300 0x12d 301 0x12e 302 0x12f 303 0x130 304 0x131 305|XXXX}
//     320
//     {0x140 320 0x141 321 0x142 322 0x143 323 0x144 324 0x145 325|XXXX}
//     340
//     {0x154 340 0x155 341 0x156 342 0x157 343 0x158 344 0x159 345|(nil)}
//   ]
// ]
void setup_environment(bptree_t *bpt){
	for (int i = 0; i < 6 * 3; i++){
		for (int j = 10 * i; j < 10 * i + 6; j++){
			bptree_insert(bpt, (bptree_key_t)(10 * i + j), (void *)(10 * i + j));
		}
	}
	bptree_print(bpt);
}

bptree_test_result testcase_check_node_borrow_keys_from_left(){
	bptree_t *bpt = bptree_init(10);
	setup_environment(bpt);
	
	bptree_node_t *left_node, *right_node, *last_node;
	left_node = bpt->root->children[0];
	right_node = bpt->root->children[1];
	last_node = bpt->root->children[2];
	
	bptree_node_t *expected_left_node_children[] = {
		left_node->children[0],
		left_node->children[1],
		left_node->children[2],
		left_node->children[3],
	};
	
	bptree_node_t *expected_right_node_children[] = {
		left_node->children[4],
		left_node->children[5],
		right_node->children[0],
		right_node->children[1],
		right_node->children[2],
		right_node->children[3],
		right_node->children[4],
		right_node->children[5],
	};
	
	bptree_node_borrow_keys(bpt, left_node, right_node, right_node, 2, 0);
	bptree_node_t *expected_root_children[] = {left_node, right_node, last_node};
	int expected_root_keys[] = {80, 240};
	for(int i = 0; i < 2; i++){
		if (expected_root_keys[i] != bpt->root->keys[i]){
			printf("Error: bpt->root->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_root_keys[i], bpt->root->keys[i]);
			goto fail;
		}
	}
	for(int i = 0; i < 3; i++){
		if (expected_root_children[i] != bpt->root->children[i]){
			printf("Error: bpt->root->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_root_children[i], bpt->root->children[i]);
			goto fail;
		}
	}
	
	int expected_left_node_keys[] = {20, 40, 60};
	for(int i = 0; i < 3; i++){
		if (expected_left_node_keys[i] != left_node->keys[i]){
			printf("Error: left_node->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_left_node_keys[i], left_node->keys[i]);
			goto fail;
		}
		if (expected_left_node_children[i] != left_node->children[i]){
			printf("Error: left_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_left_node_children[i], left_node->children[i]);
			goto fail;
		}
	}
	if (expected_left_node_children[3] != left_node->children[3]){
		printf("Error: left_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
				3, expected_left_node_children[3], left_node->children[3]);
		goto fail;
	}
	
	int expected_right_node_keys[] = {100, 120, 140, 160, 180, 200, 220};
	for(int i = 0; i < 7; i++){
		if (expected_right_node_keys[i] != right_node->keys[i]){
			printf("Error: right_node->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_right_node_keys[i], right_node->keys[i]);
			goto fail;
		}
		if (expected_right_node_children[i] != right_node->children[i]){
			printf("Error: right_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_right_node_children[i], right_node->children[i]);
			goto fail;
		}
	}
	if (expected_right_node_children[7] != right_node->children[7]){
		printf("Error: right_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
				7, expected_right_node_children[7], right_node->children[7]);
		goto fail;
	}
	
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

bptree_test_result testcase_check_node_borrow_keys_from_right(){
	bptree_t *bpt = bptree_init(10);
	setup_environment(bpt);
	
	bptree_node_t *left_node, *right_node, *last_node;
	left_node = bpt->root->children[0];
	right_node = bpt->root->children[1];
	last_node = bpt->root->children[2];
	
	bptree_node_t *expected_left_node_children[] = {
		left_node->children[0],
		left_node->children[1],
		left_node->children[2],
		left_node->children[3],
		left_node->children[4],
		left_node->children[5],
		right_node->children[0],
		right_node->children[1],
	};
	
	bptree_node_t *expected_right_node_children[] = {
		right_node->children[2],
		right_node->children[3],
		right_node->children[4],
		right_node->children[5],
	};
	
	bptree_node_borrow_keys(bpt, left_node, right_node, left_node, 2, 0);
	bptree_node_t *expected_root_children[] = {left_node, right_node, last_node};
	int expected_root_keys[] = {160, 240};
	for(int i = 0; i < 2; i++){
		if (expected_root_keys[i] != bpt->root->keys[i]){
			printf("Error: bpt->root->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_root_keys[i], bpt->root->keys[i]);
			goto fail;
		}
	}
	for(int i = 0; i < 3; i++){
		if (expected_root_children[i] != bpt->root->children[i]){
			printf("Error: bpt->root->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_root_children[i], bpt->root->children[i]);
			goto fail;
		}
	}
	
	int expected_left_node_keys[] = {20, 40, 60, 80, 100, 120, 140};
	for(int i = 0; i < 7; i++){
		if (expected_left_node_keys[i] != left_node->keys[i]){
			printf("Error: left_node->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_left_node_keys[i], left_node->keys[i]);
			goto fail;
		}
		if (expected_left_node_children[i] != left_node->children[i]){
			printf("Error: left_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_left_node_children[i], left_node->children[i]);
			goto fail;
		}
	}
	if (expected_left_node_children[7] != left_node->children[7]){
		printf("Error: left_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
				7, expected_left_node_children[7], left_node->children[7]);
		goto fail;
	}
	
	int expected_right_node_keys[] = {180, 200, 220};
	for(int i = 0; i < 3; i++){
		if (expected_right_node_keys[i] != right_node->keys[i]){
			printf("Error: right_node->keys[%d] is wrong. (expected: %lld, actual: %lld)\n",
					i, expected_right_node_keys[i], right_node->keys[i]);
			goto fail;
		}
		if (expected_right_node_children[i] != right_node->children[i]){
			printf("Error: right_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
					i, expected_right_node_children[i], right_node->children[i]);
			goto fail;
		}
	}
	if (expected_right_node_children[3] != right_node->children[3]){
		printf("Error: right_node->children[%d] is wrong. (expected: %p, actual: %p)\n",
				3, expected_right_node_children[3], right_node->children[3]);
		goto fail;
	}
	
	bptree_free(bpt);
	return BPTREE_TEST_PASSED;
fail:
	bptree_print(bpt);
	bptree_free(bpt);
	return BPTREE_TEST_FAILED;
}

int main(int argc, char *argv[]){
	do_single_test("check borrow_keys from left node", testcase_check_node_borrow_keys_from_left);
	do_single_test("check borrow_keys from right node", testcase_check_node_borrow_keys_from_right);
}
