/* ***************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef _IOT_LOG_FILE_H_
#define _IOT_LOG_FILE_H_

#include "iot_internal.h"
#include "iot_main.h"
#include "iot_debug.h"
#include "iot_bsp_debug.h"

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define IOT_LOG_FILE_TRUE 1
#define IOT_LOG_FILE_FALSE 0

#define IOT_LOG_FILE_COLOR_RED "\033[0;31m"
#define IOT_LOG_FILE_COLOR_CYAN "\033[0;36m"
#define IOT_LOG_FILE_COLOR_END "\033[0;m"

#define IOT_LOG_FILE_DEBUG_ENABLE 0
#if IOT_LOG_FILE_DEBUG_ENABLE
#define IOT_LOG_FILE_DEBUG(fmt, args...) printf(IOT_LOG_FILE_COLOR_CYAN fmt IOT_LOG_FILE_COLOR_END, ##args)
#else
#define IOT_LOG_FILE_DEBUG(fmt, args...)
#endif

#define IOT_LOG_FILE_ERROR(fmt, args...) printf(IOT_LOG_FILE_COLOR_RED fmt IOT_LOG_FILE_COLOR_END, ##args)

#define IOT_LOG_FILE_MAX_STRING_SIZE 128 /* Max input string size */
#define IOT_LOG_FILE_MARGIN_CNT 1		 /* magin count */

#define IOT_LOG_FILE_EVENT_SYNC_REQ_BIT (1u << 0u)
#define IOT_LOG_FILE_EVENT_BIT_ALL (IOT_LOG_FILE_EVENT_SYNC_REQ_BIT)

#define IOT_LOG_FILE_TASK_NAME "iot-log-file-task"
#define IOT_LOG_FILE_TASK_STACK_SIZE (1024 * 5)
#define IOT_LOG_FILE_TASK_PRIORITY (IOT_TASK_PRIORITY + 1)

#define IOT_LOG_FILE_ADDR CONFIG_STDK_IOT_CORE_LOG_FILE_ADDR
#define IOT_LOG_FILE_SIZE CONFIG_STDK_IOT_CORE_LOG_FILE_SIZE
#define IOT_LOG_FILE_RAM_BUF_SIZE CONFIG_STDK_IOT_CORE_LOG_FILE_RAM_BUF_SIZE
#define IOT_LOG_FILE_FLASH_SECTOR_SIZE CONFIG_STDK_IOT_CORE_LOG_FILE_FLASH_SECTOR_SIZE

#define IOT_LOG_FILE_HEADER_SIZE (sizeof(struct iot_log_file_header_tag))
#define IOT_LOG_FILE_BUF_SIZE (2 * IOT_LOG_FILE_FLASH_SECTOR_SIZE)

#define IOT_LOG_FILE_FIRST_SECTOR (IOT_LOG_FILE_ADDR / IOT_LOG_FILE_FLASH_SECTOR_SIZE)
#define IOT_LOG_FILE_MAX_ADDR (IOT_LOG_FILE_ADDR + IOT_LOG_FILE_SIZE)

typedef struct
{
	unsigned int start_addr;
	unsigned int cur_addr;
	unsigned int log_size;
} iot_log_file_handle_t;

typedef enum
{
	NORMAL,
	NO_MAGIC,
	LOAD_FAIL,
} iot_log_file_header_state_t;

struct iot_log_file_buf_tag
{
	bool enable;
	unsigned int cnt;
	char buf[IOT_LOG_FILE_RAM_BUF_SIZE];
};

struct iot_log_file_sector_tag
{
	unsigned int num;
	unsigned int offset;
};

struct iot_log_file_header_tag
{
	char magic_code[4];
	unsigned int file_size;
	unsigned int written_size;
	struct iot_log_file_sector_tag sector;
	unsigned int checksum;
};

struct iot_log_file_ctx
{
	iot_os_eventgroup *events;
	struct iot_log_file_buf_tag log_buf;
	struct iot_log_file_header_tag file_header;
	char file_buf[IOT_LOG_FILE_BUF_SIZE];
	bool file_opened;
};


/**
 * @brief Initialize a log file system.
 * 
 * @retval IOT_ERROR_NONE log file init successful.
 * @retval IOT_ERROR_MEM_ALLOC log file task alloc failed.
 */
iot_error_t iot_log_file_init(void);

/**
 * @brief Store log data to log file.
 * 
 * @return The length of the stored data. -1 is failure.
 */
int iot_log_file_store(char *log_data);

/**
 * @brief Log file synchronize with ram log data.
 * 
 * @details This function store log data on ram to flash memory
 */
void iot_log_file_sync(void);

/**
 * @brief Remove Iot log file
 * 
 * @details This function remove log data
 * @retval IOT_ERROR_NONE Log file remove successful.
 * @retval IOT_ERROR_BAD_REQ Log file remove failed.
 */
iot_error_t iot_log_file_remove(void);

/**
 * @brief Open Iot log file to read
 * 
 * @details This function make ready to read, if this function is called, log will be saved any more.
 * @param[out] filesize Log file size
 * @return Pointer of log file handle
 */
iot_log_file_handle_t *iot_log_file_open(unsigned int *filesize);

/**
 * @brief Read file data using file handle
 * 
 * @details You can read log file as much as you want using file handle
 * @param[out] buffer Buffer where read data will be located
 * @param[in] size Size to read
 * @param[in] file_handle Handle to access file
 * @retval IOT_ERROR_NONE log file read successful.
 * @retval IOT_ERROR_READ_FAIL log file read failed.
 */
iot_error_t iot_log_file_read(void *buffer, unsigned int size, iot_log_file_handle_t *file_handle);

/**
 * @brief Close opened log file
 * 
 * @details This function makes close opend file handle,
 			And saving log data to flash memory will be started from this function called.
 * @param[in] file_handle A file to close
 */
void iot_log_file_close(iot_log_file_handle_t *file_handle);

#ifdef __cplusplus
}
#endif

#endif /* _IOT_LOG_FILE_H_ */

