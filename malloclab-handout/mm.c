/**********************************************************************************************************************************
 * mm.c - 参考dlmalloc.c实现自己malloc和free(使用了全局数组，违反了部分规则)。由于tracefile部分缺失，暂时不考虑实现remalloc. 
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


/****************************************************************************************************
 * 算法细节：
 * 1使用分离适配(见教材)，分配器维护一个空闲链表数组。数组长度45，0-32bin维护
 * size 16-256bytes的空闲块，空闲块使用双向链表连接，LIFO。33-44维护size 
 * 512b-1Mb的空闲块，空闲块使用一种特殊二叉树(相同大小块用平行的双向链表连接)
 * 连接，按size大小构建二叉树。
 *
 * 2malloc时，先确定需要的块大小size。然后到最匹配的bin里寻找合适的空闲块。
 * size<256bytes,采用首次适配。如果size>256bytes，采用最优适配。适配到后，删
 * 除适配空闲块，修改块格式，根据需要切割出剩余空闲块后插入对应bin中。
 * 如果没有适配到，看下一个bin
 * 如果所有bin都没有合适空闲块，向堆请求新块
 * 
 * 3free时，修改块格式后，进行立即合并，并将合并后的新空闲块插入对应bin中。
 *
 * 4块的格式：已分配块和空闲块有不同的格式
 *	已分配块：块头+数据空间
 *			块头：size+上一个块分配状态位+本块分配状态位
 *	空闲块：< 256bytes 块头(见下)+上一个空闲块地址+下一个空闲块地址+数据空间+块尾
 *			块头：size+上一个块分配位+本块分配位
 *			> 256bytes 块头(见下)+上一个空闲块地址+下一个空闲块地址+左子树地址+右子树地址+数据空间+块尾
 *			块头：size+本块是否处于相等链中+上一个块分配位+本块分配位
 *****************************************************************************************************/


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


/*******************
 * global val bins
 *******************/
void *bins[45];

/*******************
 * 用到的私有函数声明
 ********************/

/*
 * mm_init
 */
int mm_init(void)
{
	
}

/* 
 * mm_malloc
 */
void *mm_malloc(size_t size)
{

}

/* 
 * mm_free
 */
void mm_free(void *ptr)
{

}

/* 
 * mm_remalloc
 */
void *mm_remalloc(void *ptr, size_t size)
{

}
