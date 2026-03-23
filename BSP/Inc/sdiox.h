/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef __SDIOX_H
#define __SDIOX_H

/*SDIO初始化，应该先初始化SDIO后再初始化FATFS*/
void SdioInit(void);
void SdioDeInit(void);
#endif

