#include "../../../ax32_platform_demo/application.h"
#include "../inc/btcom.h"
#include "../inc/btcom_inner.h"

/**
 * 创建保存帧数据的缓存
 *
 */
FRAME_MANAGE_T *btcomBufCreate(void)
{
	FRAME_MANAGE_T *manager;
	int i;

	// 分配空间
	manager = hal_sysMemMalloc(sizeof(FRAME_MANAGE_T), 64);
	if (manager == NULL) {
		deg_Printf("[%s]malloc fail\n",__func__);
		return NULL;
	}
	// 设置链表
	for (i = 0; i < NUM_OF_FRAME-1; i++) {
		manager->buf[i].next = &(manager->buf[i+1]);
	}
	manager->buf[i].next = NULL;
	manager->free = &(manager->buf[0]);
	manager->busy = NULL;
	manager->working = NULL;

	return manager;
}

/**
 * 销毁数据缓存
 *
 */
void btcomBufDestroy(FRAME_MANAGE_T *manager)
{
	if (manager) {
		hal_sysMemFree(manager);
	}
}

/**
 * 选一个空闲的buffer接收数据
 *
 * TRUE-成功；FALSE-失败
 */
bool btcomGetIdleBuf(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *idle = manager->free;

	// 已分配有
	if (manager->working != NULL) {
		return true;
	}
	// 取一个未用buf
	if (idle != NULL) {
		idle->len = 0;
		manager->working = idle;
		manager->free = idle->next;
	}

	return (manager->working == NULL) ? false : true;
}

/**
 * 查找是否有数据
 *
 */
FRAME_BUF_T *btcomTestBusyBuf(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *head, *item, *busy;

	head = item = manager->busy;
	busy = NULL;
	while (item) {
		if (item->total_len > 0) {
			busy = item;
			break;
		}
		if (item->next == head) {
			break;
		}
		item = item->next;
	}

	return busy;
}

/**
 * 标记数据已处理，可回收
 *
 */
void btcomMarkConsumed(FRAME_BUF_T *item)
{
	if (item) {
		item->total_len = 0;
	}
}

/**
 * 回收buffer
 *
 */
void btcomRecycleBuf(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *head, *item,*next,*prev=NULL;

	head = item = manager->busy;
	if (head) {
		prev = head->prev;
	}
	while (item) {
		// 遇到有效的，跳出循环
		if (item->total_len > 0) {
			break;
		}
		// 记录下一个item
		if (item->next == head) {
			next = NULL;
		} else {
			next = item->next;
		}
		// 放入free 表
		item->next = manager->free;
		manager->free = item;

		// 继续下一个
		item = next;
	}
	// 有删除，则重设header
	if (manager->busy != item) {
		manager->busy = item;
		if (manager->busy != NULL) {
			manager->busy->prev = prev;
			prev->next = manager->busy;
		}
	}
}

/**
 * 数据放入待处理链表
 *
 */
void btcomPutBusy(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *item = manager->working;

	if (manager == NULL || item == NULL) {
		return;
	}

	if (manager->busy == NULL) {
		item->next = item;
		item->prev = item;
		manager->busy = item;
	} else {
		item->next = manager->busy;
		item->prev = manager->busy->prev;
		manager->busy->prev->next = item;
		manager->busy->prev = item;
	}
	manager->working = NULL;
}

/**
 * 打印buffer使用情况
 *
 */
void btcomCheckManager(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *item, *head;
	int nFree,nBusy,nWorking;

	btcomRecycleBuf(manager);
	uart_Printf("free=0x%X,busy=0x%X,working=0x%X\n",manager->free,manager->busy,manager->working);

	nFree = 0;
	item = manager->free;
	while (item) {
		nFree++;
		item = item->next;
	}

	nBusy = 0;
	head = item = manager->busy;
	while (item) {
		nBusy++;
		if (item->next == head) {
			break;
		}
		item = item->next;
	}

	if (manager->working) {
		nWorking = 1;
	} else {
		nWorking = 0;
	}

	uart_Printf("nFree=%d,nBusy=%d,nWorking=%d\n",nFree,nBusy,nWorking);
}

