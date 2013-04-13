/**
 * dtree_procfs_stack_test.c
 * Copyright (C) 2013 Jan Viktorin
 */

#include "stack.h"
#include "test.h"

void test_stack_empty(void)
{
	test_start();

	struct stack *a = (struct stack *) 0xDEADBEEF;
	fail_on_true(stack_empty(&a), "Stack seems to be empty here");

	a = NULL;
	fail_on_false(stack_empty(&a), "Stack seems to not be empty here");
	
	test_end();
}

void test_stack_top(void)
{
	test_start();

	struct stack *a = NULL;
	fail_on_false(stack_empty(&a), "Stack is not empty here");
	fail_on_false(stack_top(&a) == NULL, "Top of the stack is not NULL");

	const char *HELLO = "hello";
	halt_on_true(stack_push(&a, (void *) HELLO), "Failed to push HELLO")
	fail_on_true(stack_empty(&a), "Stack is empty here");
	fail_on_true(stack_top(&a) == NULL, "Top of the stack is NULL");
	fail_on_false(stack_top(&a) == HELLO, "The top does not point to HELLO");

	stack_pop(&a);
	test_end();
}

void test_stack_push_pop(void)
{
	test_start();

	struct stack *st = NULL;
	void *A = (void *) 0x4354523;
	void *B = (void *) 0x0918401;
	void *C = (void *) 0x0809481;
	void *D = (void *) 0x9084019;

	halt_on_true(stack_push(&st, A), "Failed to push A");
	fail_on_true(stack_empty(&st), "Stack is empty, but A is there");
	fail_on_false(stack_depth(&st) == 1, "Stack's depth is not 1");
	fail_on_false(stack_top(&st) == A, "Top value is not A");

	halt_on_true(stack_push(&st, B), "Failed to push B");
	fail_on_true(stack_empty(&st), "Stack is empty, but A, B is there");
	fail_on_false(stack_depth(&st) == 2, "Stack's depth is not 2");
	fail_on_false(stack_top(&st) == B, "Top value is not B");

	halt_on_false(stack_pop(&st) == B, "Popped value is not B");
	fail_on_true(stack_empty(&st), "Stack is empty, but A is there");
	fail_on_false(stack_depth(&st) == 1, "Stack's depth is not 1");
	fail_on_false(stack_top(&st) == A, "Top value is not A");

	halt_on_true(stack_push(&st, C), "Failed to push C");
	fail_on_true(stack_empty(&st), "Stack is empty, but C, C is there");
	fail_on_false(stack_depth(&st) == 2, "Stack's depth is not 2");
	fail_on_false(stack_top(&st) == C, "Top value is not C");

	halt_on_true(stack_push(&st, D), "Failed to push D");
	fail_on_true(stack_empty(&st), "Stack is empty, but D, D is there");
	fail_on_false(stack_depth(&st) == 3, "Stack's depth is not 3");
	fail_on_false(stack_top(&st) == D, "Top value is not D");

	fail_on_false(stack_pop(&st) == D, "Popped value is not D");
	fail_on_true(stack_empty(&st), "Stack is empty, but A, C is there");
	fail_on_false(stack_pop(&st) == C, "Popped value is not C");
	fail_on_true(stack_empty(&st), "Stack is empty, but A is there");
	fail_on_false(stack_pop(&st) == A, "Popped value is not A");
	fail_on_false(stack_empty(&st), "Stack is not empty");
	fail_on_false(stack_pop(&st) == NULL, "Popped value is not NULL");

	test_end();
}

void test_stack_move(void)
{
	test_start();

	struct stack *left  = NULL;
	struct stack *right = NULL;

	void *A = (void *) 0x8970198;
	void *B = (void *) 0x1298730;
	void *C = (void *) 0x09812a0;

	fail_on_true(stack_push(&left, A), "Failed to push A to left");
	fail_on_true(stack_push(&left, B), "Failed to push B to left");
	fail_on_true(stack_push(&right, C), "Failed to push C to right");

	fail_on_false(stack_depth(&left) == 2, "Depth of left is not 2");
	fail_on_false(stack_depth(&right) == 1, "Depth of right is not 1");

	fail_on_false(stack_move(&left, &right) == B, "Moving whatever but not B");
	fail_on_false(stack_depth(&left) == 1, "Depth of left is not 1");
	fail_on_false(stack_depth(&right) == 2, "Depth of right is not 2");

	fail_on_false(stack_move(&left, &right) == A, "Moving whatever but not B");
	fail_on_false(stack_depth(&left) == 0, "Depth of left is not 0");
	fail_on_false(stack_depth(&right) == 3, "Depth of right is not 3");

	fail_on_false(stack_pop(&right) == A, "Popping whatever but not A");
	fail_on_false(stack_pop(&right) == B, "Popping whatever but not B");
	fail_on_false(stack_pop(&right) == C, "Popping whatever but not C");

	test_end();
}

int main(void)
{
	test_stack_empty();
	test_stack_top();
	test_stack_push_pop();
	test_stack_move();
	return 0;
}
