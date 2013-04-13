/**
 * stack.h
 * Copyright (C) 2013 Jan Viktorin
 */

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct stack {
	void *data;
	struct stack *next;
};

static inline
int stack_empty(struct stack **s)
{
	assert(s != NULL);
	return *s == NULL;
}

static inline
size_t stack_depth(struct stack **s)
{
	const struct stack *curr = *s;
	size_t depth = 0;

	for(; curr != NULL; curr = curr->next)
		depth += 1;

	return depth;
}

static inline
int stack_push(struct stack **s, void *data)
{
	struct stack *news = malloc(sizeof(struct stack));
	if(news == NULL)
		return 1;

	news->data = data;
	news->next = *s;
	*s = news;
	return 0;
}

static inline
int stack_push_dup(struct stack **s, const void *data, size_t dlen)
{
	void *dup = malloc(dlen);
	if(dup == NULL)
		return -1;

	memcpy(dup, data, dlen);
	
	if(stack_push(s, dup)) {
		free(dup);
		return 1;
	}

	return 0;
}

static inline
void *stack_pop(struct stack **s)
{
	if(stack_empty(s))
		return NULL;

	struct stack *curs = *s;
	void *data = curs->data;
	*s = curs->next;

	memset(curs, 0, sizeof(struct stack));
	free(curs);
	return data;
}

static inline
void *stack_move(struct stack **src, struct stack **dst)
{
	if(stack_empty(src))
		return NULL;

	struct stack *curs = *src;
	void *data = curs->data;
	*src = curs->next;

	curs->next = *dst;
	*dst = curs;

	return data;
}

static inline
void *stack_top(struct stack **s)
{
	if(stack_empty(s))
		return NULL;

	struct stack *curr = *s;
	return curr->data;
}

#endif

