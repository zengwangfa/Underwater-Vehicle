/*
 * user_finsh_cmd.c
 *
 *  Created on: 2013年12月7日
 *      Author: Armink
 *      Notes:  更新固件 Update 方法
 */
#define LOG_TAG    "cmd"
#include "init.h"
#include <rtthread.h>
#include <shell.h>
#include <finsh.h>
#include <elog.h>

#include <easyflash.h>
#include <ymodem.h>
#include "flash.h"

extern char *VehicleModeName[2];
extern u8 VehicleMode;

static size_t update_file_total_size, update_file_cur_size;
static uint32_t crc32_checksum = 0;
static enum rym_code ymodem_on_begin(struct rym_ctx *ctx, rt_uint8_t *buf, rt_size_t len) {
    char *file_name, *file_size;

    /* calculate and store file size */
    file_name = (char *) &buf[0];
		file_size = (char *) &buf[rt_strlen(file_name) + 1];
    update_file_total_size = atol(file_size);
    /* 4 bytes align */
    update_file_total_size = (update_file_total_size + 3) / 4 * 4;
    update_file_cur_size = 0;
    crc32_checksum = 0;

    /* erase backup section */
    if (ef_erase_bak_app(update_file_total_size)) {
        /* if erase fail then end session */
        return RYM_CODE_CAN;
    }

    return RYM_CODE_ACK;
}

static enum rym_code ymodem_on_data(struct rym_ctx *ctx, rt_uint8_t *buf, rt_size_t len) {
    if (update_file_cur_size + len <= update_file_total_size) {
        crc32_checksum = ef_calc_crc32(crc32_checksum, buf, len);
    } else {
        crc32_checksum = ef_calc_crc32(crc32_checksum, buf, update_file_total_size - update_file_cur_size);
    }
    /* write data of application to backup section  */
    if (ef_write_data_to_bak(buf, len, &update_file_cur_size, update_file_total_size)) {
        /* if write fail then end session */
        return RYM_CODE_CAN;
    }
    return RYM_CODE_ACK;
}

void update(uint8_t argc, char **argv) {
    char new_char[2], c_file_size[11] = { 0 }, c_crc32_checksum[11] = { 0 };
    struct rym_ctx rctx;

    elog_set_output_enabled(false);
    rt_kprintf("Waring: This operator will not recovery. If you want, press 'Y'.\n");
    new_char[0] = getchar();
    rt_kprintf("%c", new_char[0]);
    new_char[1] = getchar();
    rt_kprintf("%c", new_char[1]);
    if ((new_char[0] != 'y') && (new_char[0] != 'Y')) {
        goto __exit;
    }
    rt_kprintf("Please select the application firmware file and use Ymodem to send.\n");

    if (!rym_recv_on_device(&rctx, rt_console_get_device(), RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
            ymodem_on_begin, ymodem_on_data, NULL, RT_TICK_PER_SECOND)) {
        /* wait some time for terminal response finish */
        rt_thread_delay(RT_TICK_PER_SECOND);
        /* save the downloaded firmware crc32 checksum ENV */
        ef_set_env("iap_need_crc32_check", "1");
        rt_snprintf(c_crc32_checksum, sizeof(c_crc32_checksum), "%lu", crc32_checksum);
        ef_set_env("iap_crc32_checksum", c_crc32_checksum);
        /* set need copy application from backup section flag is 1, backup application length */
        ef_set_env("iap_need_copy_app", "1");
        rt_sprintf(c_file_size, "%ld", update_file_total_size);
        ef_set_env("iap_copy_app_size", c_file_size);
        ef_save_env();
        rt_kprintf("Download firmware to flash OK.\n");
        rt_kprintf("System now will restart...\n");
        NVIC_SystemReset();
    } else {
        /* wait some time for terminal response finish */
        rt_thread_delay(RT_TICK_PER_SECOND);
        rt_kprintf("Update firmware fail.\n");
    }

__exit:
    elog_set_output_enabled(true);
}
MSH_CMD_EXPORT(update, Update user application firmware);

void get_memory_situation(void)
{
		 static rt_uint32_t total_mem, used_mem, max_used_mem;
	   rt_memory_info(&total_mem, &used_mem, &max_used_mem);
     rt_kprintf("Total_Mem:%ld  Used_Mem:%ld  Max_Used_Mem:%ld\n",total_mem,used_mem,max_used_mem);
}
MSH_CMD_EXPORT(get_memory_situation,get memory situation);


/*  设置机器工作模式 */
static int set_vehicle_mode(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
				log_e("Proper Usage: set_vehicle_mode auv / rov");//用法:设置工作模式
				result = -RT_ERROR;
        goto _exit;
    }
		if( !strcmp(argv[1],"auv") ){ //设置为工作模式 strcmp 检验两边相等 返回0
				VehicleMode = AUV_Mode;
				Flash_Update();
				log_i("Current Mode:%s\r\n",VehicleModeName[VehicleMode]);
		}

		else if( !strcmp(argv[1],"rov") ){ //设置为 ROV
				VehicleMode = ROV_Mode;
				Flash_Update();
				log_i("Current Mode:%s\r\n",VehicleModeName[VehicleMode]);
		}
		else {
				log_e("Proper Usage: set_vehicle_mode auv / rov");
				goto _exit;
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(set_vehicle_mode,set_vehicle_mode auv / rov);


