/*
 * IMP Group common function header file.
 *
 * Copyright (C) 2014 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
 */

#ifndef __GROUP_H__
#define __GROUP_H__

#include <imp_sys.h>
#include <system/constraints.h>
#include <system/module.h>

typedef struct Device Device;
typedef struct Group Group;

struct Group {
	Device *device;
	Module *module;
	int group_index;
	int nr_channels;
	FrameInfo *for_channel_data[NR_MAX_CHN_IN_GROUP];
	int update_cb;

	int (*OnGroupDataUpdate)(Group *modulex, FrameInfo *data);
};

Module *get_module(DeviceID dev_id, int grp_id);
void clear_all_modules(void);

Group *create_group(DeviceID mod_id, int dev_id,
					const char *mod_name,
					int (*update_cb)(Group *, FrameInfo *));

int destroy_group(Group *group, DeviceID dev_id);

#endif /* __GROUP_H__ */
