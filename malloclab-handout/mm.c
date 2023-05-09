/**********************************************************************************************************************************
 * mm.c - 基于书中的方法(隐式空闲链表 + 首次匹配 + 立即合并), 实现了mm_init, mm_malloc, mm_free，由于tracefile相关文件缺失，暂时不考虑实现mm_remalloc. 
 ***********************************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "master of Kongfu",
    /* First member's full name */
    "LiDu",
    /* First member's email address */
    "lidunot_fear@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* double word (8) alignment */
#define ALIGNMENT 8
#define WORD 4
#define DWORD 8
#define DEFAULT_BLOCK (1<<4) 

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define PUT(p, state) (*(unsigned int *)(p) = (state))
#define STATE(size, allocate_bit) ((size) | (allocate_bit))
#define GETSIZE(p) (*(unsigned int *)(p) & ~0x7)
#define GETALLOCATE(p) (*(unsigned int *)(p) & 0x1)
#define HEADER(block_ptr) ((char *)(block_ptr) - WORD)
#define FOOTER(block_ptr) ((char *)(block_ptr) + GETSIZE(HEADER(block_ptr)) - DWORD)
#define NEXT_BLOCK(block_ptr) ((char *)(block_ptr) + GETSIZE(HEADER(block_ptr)))
#define PREV_BLOCK(block_ptr) ((char *)(block_ptr) - GETSIZE((char *)(block_ptr) - DWORD))
#define GETSTATE(p) (*(unsigned int *)(p))
#define RESET_EMPTY(p) (*(unsigned int *)(p) = GETSTATE(p) & ~0x1)

/*********************************
 * 观察运行时内存分配情况时定义此宏
 *******************************/
//#define TEST_UTI 1



/*
 * 一些私有的函数声明
 */
void allocate_block(void *block, size_t size);
void *request_heap(size_t size);
void *merge(void *block);


/*
 * first_block始终指向堆中的第一个块
 */
void *first_block;

/* 
 * mm_init - 初始化堆 
 * 初始堆由三部分组成，一个头块尾部4byte，一个空闲块2MB，一个尾块头部4byte
 * 头块状态码：大小 0，分配位 1 
 * 尾块状态码：大小 0，分配位 1
 */
int mm_init(void)
{
	first_block = mem_sbrk(DWORD + DEFAULT_BLOCK);	
	if (first_block == NULL) {
		return -1;
	}

	PUT(first_block, 1);
	first_block = (char *)first_block + DWORD;
	PUT(HEADER(first_block), STATE(DEFAULT_BLOCK, 0));
	PUT(FOOTER(first_block), STATE(DEFAULT_BLOCK, 0));
	PUT(HEADER(NEXT_BLOCK(first_block)), 1);
	#ifdef TEST_UTI
		printf("堆初始化：分配%ubytes空闲块\n\n", DEFAULT_BLOCK);
	#endif

	return 0;	
}

/* 
 * mm_malloc - 应用请求分配内存空间。mm_malloc分配成功后返回可用地址，分配失败返回NULL
 * 扫描空闲块，进行首次适配。如果没有符合条件空闲块，则对堆进行扩展 
 */
void *mm_malloc(size_t size)
{
	if (size == 0) {
		printf("你在请求0byte的分配");
		return NULL;
	}
	size_t adjust_size = ALIGN(size + DWORD);
	#ifdef TEST_UTI
		printf("malloc请求%ubytes块:", adjust_size);
	#endif

	void *check_block = first_block;
	const void *_brk = (char *)mem_heap_hi() + 1;
	for ( ; check_block < _brk; check_block = NEXT_BLOCK(check_block)) {
		//如果发现合适的空闲块，将用户请求的空间适配到这个空闲块中
		if (!GETALLOCATE(HEADER(check_block)) && GETSIZE(HEADER(check_block)) >= adjust_size) {
			allocate_block(check_block, adjust_size);
			#ifdef TEST_UTI
				printf("\n");
			#endif	
			return check_block;
		}
	}
	
	//如果没有现存合适的空闲块，则向堆请求扩展一个新的空闲块。
	//拿到合适的空闲块后，再将用户请求分配的空间适配到这个空闲块中
	check_block = request_heap(adjust_size);
	allocate_block(check_block, adjust_size);
	#ifdef TEST_UTI
		printf("\n");
	#endif	
	return check_block;
}


/*
 * mm_free - 释放ptr所指分配块，并与周围空闲块进行合并 
 */
void mm_free(void *ptr)
{
	RESET_EMPTY(HEADER(ptr));
	RESET_EMPTY(FOOTER(ptr));
	#ifdef TEST_UTI
		printf("释放%ubytes已分配块:", GETSIZE(HEADER(ptr)));
	#endif
	merge(ptr);
	#ifdef TEST_UTI
		printf("\n");
	#endif
}


/*
 * mm_realloc - 由于tracefile缺失，暂时不实现 
 */
void *mm_realloc(void *ptr, size_t size)
{
	return NULL;
}


/*
 * allocate_block - 将用户请求分配的空间(已对齐)适配到block所指的空闲块中(空间足够大)
 */
void allocate_block(void *block, size_t size)
{
	size_t block_size = GETSIZE(HEADER(block));
	size_t remain_size = block_size - size;
	//ALIGN(DWORD + 1)表示最小块大小
	if (remain_size < ALIGN(DWORD + 1)) {
		PUT(HEADER(block), STATE(block_size, 1));
		PUT(FOOTER(block), STATE(block_size, 1));
		#ifdef TEST_UTI
			printf("首次适配到%ubytes块中,不切割\n", block_size);
		#endif
	} else {
		PUT(HEADER(block), STATE(size, 1));
		PUT(FOOTER(block), STATE(size, 1));
		PUT(HEADER(NEXT_BLOCK(block)), STATE(remain_size, 0));
		PUT(FOOTER(NEXT_BLOCK(block)), STATE(remain_size, 0));
		#ifdef TEST_UTI
			printf("首次适配到%ubytes块中,切割余下%ubytes空闲块\n", block_size, remain_size);
		#endif
	}
}

/*
 * request_heap - 根据请求块size大小(已对齐)，向堆请求新的空闲块并返回。如果请求失败，返回NULL 
 */
void *request_heap(size_t size) {
	//if (size + ALIGN(DWORD + 1) <= DEFAULT_BLOCK) {
	//	size = DEFAULT_BLOCK;
	//}

	#ifdef TEST_UTI
		printf("没有适配到合适空闲块，向堆请求%ubytes新空闲块:", size);
	#endif
	void *old_brk = mem_sbrk(size);
	if (old_brk == NULL) {
		return NULL;
	}
	PUT(HEADER(old_brk), STATE(size, 0));
	PUT(FOOTER(old_brk), STATE(size, 0));
	PUT(HEADER(NEXT_BLOCK(old_brk)), 1);

	old_brk = merge(old_brk);
	return old_brk;
}

/*
 * merge - 合并block周围可能出现的空闲块，并返回合并后的空闲块指针
 */
void *merge(void *block) {
	size_t next_allocate = GETALLOCATE(HEADER(NEXT_BLOCK(block)));
	size_t prev_allocate = GETALLOCATE((char *)block - DWORD);	
	size_t size;
	size_t next_size;
	size_t prev_size;
	if (next_allocate && prev_allocate) {
		#ifdef TEST_UTI
			printf("\n");
		#endif
		return block;
	} else if (!next_allocate && prev_allocate) {
		size = GETSIZE(HEADER(block));
		next_size = GETSIZE(HEADER(NEXT_BLOCK(block)));
		size += next_size;
		PUT(HEADER(block), STATE(size, 0));
		PUT(FOOTER(block), STATE(size, 0));
		#ifdef TEST_UTI
			printf("与后面%ubytes空闲块合并成%ubytes空闲块\n", next_size, size);
		#endif
		return block;
	} else if (next_allocate && !prev_allocate) {
		size = GETSIZE(HEADER(block));
		prev_size = GETSIZE(HEADER(PREV_BLOCK(block)));
		size += prev_size;
		PUT(FOOTER(block), STATE(size, 0));
		PUT(HEADER(PREV_BLOCK(block)), STATE(size, 0));
		#ifdef TEST_UTI
			printf("与前面%ubytes空闲块合并成%ubytes空闲块\n", prev_size, size);
		#endif
		return PREV_BLOCK(block);
	} else {
		size = GETSIZE(HEADER(block));
		prev_size = GETSIZE(HEADER(PREV_BLOCK(block)));
		next_size = GETSIZE(HEADER(NEXT_BLOCK(block)));
		size += prev_size + next_size;
		PUT(HEADER(PREV_BLOCK(block)), STATE(size, 0));
		PUT(FOOTER(NEXT_BLOCK(block)), STATE(size, 0));
		#ifdef TEST_UTI
			printf("与前面%ubytes空闲块和后面%ubytes空闲块一起合并成%ubytes空闲块\n", prev_size, next_size, size);
		#endif
		return PREV_BLOCK(block);
	}
}
