
#include "isp-ctrl.h"
#include "ovisp-debugtool.h"

#define ISP_WRITEB_ARRAY(vals)	\
	isp_writeb_array(isp, vals, ARRAY_SIZE(vals))

struct isp_regb_vals {
	unsigned int reg;
	unsigned char value;
};

static struct isp_regb_vals isp_exposure_l2[] = {
	{0x1c146, 0x14},
	{0x1c5a0, 0x14},
};

static struct isp_regb_vals isp_exposure_l1[] = {
	{0x1c146, 0x1d},
	{0x1c5a0, 0x1d},
};

static struct isp_regb_vals isp_exposure_h0[] = {
	{0x1c146, 0x28},
	{0x1c5a0, 0x50},
};

static struct isp_regb_vals isp_exposure_h1[] = {
	{0x1c146, 0x68},
	{0x1c5a0, 0x68},
};

static struct isp_regb_vals isp_exposure_h2[] = {
	{0x1c146, 0x78},
	{0x1c5a0, 0x78},
};

static struct isp_regb_vals isp_iso_auto[] = {
	{0x1c150, 0x00},
	{0x1c151, 0x7c},
	{0x1c154, 0x00},
	{0x1c155, 0x10},
};

static struct isp_regb_vals isp_iso_100[] = {
	{0x1c150, 0x00},
	{0x1c151, 0x14},
	{0x1c154, 0x00},
	{0x1c155, 0x10},
};

static struct isp_regb_vals isp_iso_200[] = {
	{0x1c150, 0x00},
	{0x1c151, 0x23},
	{0x1c154, 0x00},
	{0x1c155, 0x1d},
};

static struct isp_regb_vals isp_iso_400[] = {
	{0x1c150, 0x00},
	{0x1c151, 0x44},
	{0x1c154, 0x00},
	{0x1c155, 0x3c},
};

static struct isp_regb_vals isp_iso_800[] = {
	{0x1c150, 0x00},
	{0x1c151, 0x7c},
	{0x1c154, 0x00},
	{0x1c155, 0x7c},
};

static struct isp_regb_vals isp_contrast_l3[] = {
	{0x1c4c0, 0x1a},
	{0x1c4c1, 0x2d},
	{0x1c4c2, 0x3c},
	{0x1c4c3, 0x48},
	{0x1c4c4, 0x53},
	{0x1c4c5, 0x5e},
	{0x1c4c6, 0x69},
	{0x1c4c7, 0x74},
	{0x1c4c8, 0x80},
	{0x1c4c9, 0x8d},
	{0x1c4ca, 0x9a},
	{0x1c4cb, 0xa9},
	{0x1c4cc, 0xb9},
	{0x1c4cd, 0xcd},
	{0x1c4ce, 0xe4},
};

static struct isp_regb_vals isp_contrast_l2[] = {
	{0x1c4c0, 0x1c},
	{0x1c4c1, 0x2f},
	{0x1c4c2, 0x3e},
	{0x1c4c3, 0x49},
	{0x1c4c4, 0x54},
	{0x1c4c5, 0x5e},
	{0x1c4c6, 0x68},
	{0x1c4c7, 0x73},
	{0x1c4c8, 0x7e},
	{0x1c4c9, 0x8a},
	{0x1c4ca, 0x97},
	{0x1c4cb, 0xa6},
	{0x1c4cc, 0xb6},
	{0x1c4cd, 0xca},
	{0x1c4ce, 0xe2},
};

static struct isp_regb_vals isp_contrast_l1[] = {
	{0x1c4c0, 0x1d},
	{0x1c4c1, 0x31},
	{0x1c4c2, 0x40},
	{0x1c4c3, 0x4a},
	{0x1c4c4, 0x54},
	{0x1c4c5, 0x5e},
	{0x1c4c6, 0x67},
	{0x1c4c7, 0x71},
	{0x1c4c8, 0x7c},
	{0x1c4c9, 0x87},
	{0x1c4ca, 0x94},
	{0x1c4cb, 0xa3},
	{0x1c4cc, 0xb3},
	{0x1c4cd, 0xc7},
	{0x1c4ce, 0xe0},
};

static struct isp_regb_vals isp_contrast_h0[] = {
	{0x1c4c0, 0x1f},
	{0x1c4c1, 0x33},
	{0x1c4c2, 0x42},
	{0x1c4c3, 0x4c},
	{0x1c4c4, 0x55},
	{0x1c4c5, 0x5e},
	{0x1c4c6, 0x67},
	{0x1c4c7, 0x70},
	{0x1c4c8, 0x7a},
	{0x1c4c9, 0x85},
	{0x1c4ca, 0x91},
	{0x1c4cb, 0xa0},
	{0x1c4cc, 0xb0},
	{0x1c4cd, 0xc5},
	{0x1c4ce, 0xdf},
};

static struct isp_regb_vals isp_contrast_h1[] = {
	{0x1c4c0, 0x20},
	{0x1c4c1, 0x34},
	{0x1c4c2, 0x43},
	{0x1c4c3, 0x4c},
	{0x1c4c4, 0x55},
	{0x1c4c5, 0x5d},
	{0x1c4c6, 0x66},
	{0x1c4c7, 0x6e},
	{0x1c4c8, 0x78},
	{0x1c4c9, 0x83},
	{0x1c4ca, 0x8e},
	{0x1c4cb, 0x9d},
	{0x1c4cc, 0xad},
	{0x1c4cd, 0xc3},
	{0x1c4ce, 0xdd},
};

static struct isp_regb_vals isp_contrast_h2[] = {
	{0x1c4c0, 0x21},
	{0x1c4c1, 0x35},
	{0x1c4c2, 0x44},
	{0x1c4c3, 0x4d},
	{0x1c4c4, 0x55},
	{0x1c4c5, 0x5d},
	{0x1c4c6, 0x65},
	{0x1c4c7, 0x6d},
	{0x1c4c8, 0x76},
	{0x1c4c9, 0x81},
	{0x1c4ca, 0x8c},
	{0x1c4cb, 0x9b},
	{0x1c4cc, 0xab},
	{0x1c4cd, 0xc1},
	{0x1c4ce, 0xdc},
};

static struct isp_regb_vals isp_contrast_h3[] = {
	{0x1c4c0, 0x22},
	{0x1c4c1, 0x36},
	{0x1c4c2, 0x45},
	{0x1c4c3, 0x4e},
	{0x1c4c4, 0x56},
	{0x1c4c5, 0x5d},
	{0x1c4c6, 0x65},
	{0x1c4c7, 0x6c},
	{0x1c4c8, 0x75},
	{0x1c4c9, 0x7f},
	{0x1c4ca, 0x8a},
	{0x1c4cb, 0x99},
	{0x1c4cc, 0xa9},
	{0x1c4cd, 0xbf},
	{0x1c4ce, 0xdb},
};

static struct isp_regb_vals isp_saturation_l2[] = {
	{0x1c4eb, 0x60},
	{0x1c4ec, 0x50},
};

static struct isp_regb_vals isp_saturation_l1[] = {
	{0x1c4eb, 0x70},
	{0x1c4ec, 0x60},
};

static struct isp_regb_vals isp_saturation_h0[] = {
	{0x1c4eb, 0x80},
	{0x1c4ec, 0x70},
};

static struct isp_regb_vals isp_saturation_h1[] = {
	{0x1c4eb, 0x90},
	{0x1c4ec, 0x80},
};

static struct isp_regb_vals isp_saturation_h2[] = {
	{0x1c4eb, 0xa0},
	{0x1c4ec, 0x90},
};

static struct isp_regb_vals isp_scene_auto[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x00},
};

static struct isp_regb_vals isp_scene_normal[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x00},
};

static struct isp_regb_vals isp_scene_action[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x00},
};

static struct isp_regb_vals isp_scene_portrait[] = {
	{0x1c5ae, 0x98}, //pre gain

	{0x65320, 0x00},
};

static struct isp_regb_vals isp_scene_landscape[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0xb4},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xd0},
};

static struct isp_regb_vals isp_scene_night[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0xe5},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xcc},
};

static struct isp_regb_vals isp_scene_night_portrait[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0xe5},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xcc},
};

static struct isp_regb_vals isp_scene_theatre[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0xe5},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xcc},
};

static struct isp_regb_vals isp_scene_beach[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0x87},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xd6},
};

static struct isp_regb_vals isp_scene_snow[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0x98},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xd6},
};

static struct isp_regb_vals isp_scene_sunset[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0x98},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x01},
	{0x65307, 0x10},
};

static struct isp_regb_vals isp_scene_steadyphoto[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0x98},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xd6},
};

static struct isp_regb_vals isp_scene_fireworks[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0x98},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xd6},
};

static struct isp_regb_vals isp_scene_sports[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x00},
};

static struct isp_regb_vals isp_scene_party[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0x98},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xd6},
};

static struct isp_regb_vals isp_scene_candlelight[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x65320, 0x01},
	{0x65300, 0x00},
	{0x65301, 0x98},
	{0x65302, 0x00},
	{0x65303, 0x80},
	{0x65304, 0x00},
	{0x65305, 0x80},
	{0x65306, 0x00},
	{0x65307, 0xd6},
};

static struct isp_regb_vals isp_scene_barcode[] = {
	{0x1c5ae, 0x80}, // pre gain restore

	{0x1cd0a, 0x00},
	{0x1c5da, 0x03},
	{0x1c5db, 0xff},
	{0x1cddd, 0x01},
};

static struct isp_regb_vals isp_white_balance_auto[] = {
	{0x66206, 0x0d},
	{0x66207, 0x0d},
	{0x66208, 0x0f},
	{0x66209, 0x71},
	{0x6620a, 0x61},
	{0x6620b, 0xd8},
	{0x6620c, 0xa0},
	{0x6620d, 0x40},
	{0x6620e, 0x34},
	{0x6620f, 0x65},
	{0x66210, 0x55},
	{0x66201, 0x52},

	{0x1c5cd, 0x01},
	{0x1c5ce, 0x00},
	{0x1c5cf, 0xf0},
	{0x1c5d0, 0x01},
	{0x1c5d1, 0x20},
	{0x1c5d2, 0x03},
	{0x1c5d3, 0x00},
	{0x1c5d4, 0x50},
	{0x1c5d5, 0xc0},
	{0x1c5d6, 0x88},
	{0x1c5d7, 0xd8},
	{0x1c5d8, 0x40},
	{0x1c1c2, 0x00},
	{0x1c1c3, 0x20},
	{0x1c17c, 0x01},
};

static struct isp_regb_vals isp_white_balance_daylight[] = {
	{0x1c4f0, 0x00},
	{0x1c4f1, 0xa4},
	{0x1c4f2, 0x00},
	{0x1c4f3, 0x80},
	{0x1c4f4, 0x00},
	{0x1c4f5, 0xc1},
	{0x1c17c, 0x03},
};

static struct isp_regb_vals isp_white_balance_cloudy_daylight[] = {
	{0x1c4f6, 0x00},
	{0x1c4f7, 0xb0},
	{0x1c4f8, 0x00},
	{0x1c4f9, 0x80},
	{0x1c4fa, 0x00},
	{0x1c4fb, 0xbd},
	{0x1c17c, 0x04},
};
static struct isp_regb_vals isp_white_balance_incandescent[] = {
	{0x1c4fc, 0x01},
	{0x1c4fd, 0x3a},
	{0x1c4fe, 0x00},
	{0x1c4ff, 0x8a},
	{0x1c500, 0x00},
	{0x1c501, 0x80},
	{0x1c17c, 0x05},
};

static struct isp_regb_vals isp_white_balance_fluorescent[] = {
	{0x1c502, 0x01},
	{0x1c503, 0x12},
	{0x1c504, 0x00},
	{0x1c505, 0x80},
	{0x1c506, 0x00},
	{0x1c507, 0xbb},
	{0x1c17c, 0x06},
};

static struct isp_regb_vals isp_white_balance_warm_fluorescent[] = {
	{0x1c508, 0x01},
	{0x1c509, 0x02},
	{0x1c50a, 0x00},
	{0x1c50b, 0x80},
	{0x1c50c, 0x00},
	{0x1c50d, 0xa0},
	{0x1c17c, 0x07},
};

static struct isp_regb_vals isp_white_balance_twilight[] = {
	{0x1c50e, 0x00},
	{0x1c50f, 0xa8},
	{0x1c510, 0x00},
	{0x1c511, 0x80},
	{0x1c512, 0x00},
	{0x1c513, 0xd0},
	{0x1c17c, 0x08},
};

static struct isp_regb_vals isp_white_balance_shade[] = {
	{0x1c514, 0x00},
	{0x1c515, 0x98},
	{0x1c516, 0x00},
	{0x1c517, 0x80},
	{0x1c518, 0x00},
	{0x1c519, 0xc8},
	{0x1c17c, 0x09},
};

static struct isp_regb_vals isp_brightness_l3[] = {
	{0x1c5ac, 0x20},
	{0x1c5ad, 0x20},
	{0x1c5ae, 0x20},
};

static struct isp_regb_vals isp_brightness_l2[] = {
	{0x1c5ac, 0x40},
	{0x1c5ad, 0x40},
	{0x1c5ae, 0x40},	
};

static struct isp_regb_vals isp_brightness_l1[] = {
	{0x1c5ac, 0x60},
	{0x1c5ad, 0x60},
	{0x1c5ae, 0x60},
};

static struct isp_regb_vals isp_brightness_h0[] = {
	{0x1c5ac, 0x80},
	{0x1c5ad, 0x80},
	{0x1c5ae, 0x80},
};

static struct isp_regb_vals isp_brightness_h1[] = {
	{0x1c5ac, 0xa0},
	{0x1c5ad, 0xa0},
	{0x1c5ae, 0xa0},
};

static struct isp_regb_vals isp_brightness_h2[] = {
	{0x1c5ac, 0xc0},
	{0x1c5ad, 0xc0},
	{0x1c5ae, 0xc0},
};

static struct isp_regb_vals isp_brightness_h3[] = {
	{0x1c5ac, 0xe0},
	{0x1c5ad, 0xe0},
	{0x1c5ae, 0xe0},
};

static struct isp_regb_vals isp_sharpness_l2[] = {
	{0x6560c, 0x00},
	{0x6560d, 0x0b},
};

static struct isp_regb_vals isp_sharpness_l1[] = {
	{0x6560c, 0x00},
	{0x6560d, 0x06},
};

static struct isp_regb_vals isp_sharpness_h0[] = {
	{0x6560c, 0x00},
	{0x6560d, 0x10},
};

static struct isp_regb_vals isp_sharpness_h1[] = {
	{0x6560c, 0x00},
	{0x6560d, 0x20},
};

static struct isp_regb_vals isp_sharpness_h2[] = {
	{0x6560c, 0x00},
	{0x6560d, 0x30},
};

static struct isp_regb_vals isp_flicker_50hz[] = {
	{0x1c13f, 0x02},
};

static struct isp_regb_vals isp_flicker_60hz[] = {
	{0x1c13f, 0x01},
};

static void isp_writeb_array(struct isp_device *isp,
	struct isp_regb_vals *vals, int num)
{
	int i;

	for (i = 0; i < num; i++)
		isp_reg_writeb(isp, vals[i].value, vals[i].reg);
}

int isp_set_exposure(struct isp_device *isp, int val)
{
	switch (val) {
	case EXPOSURE_L2:
		ISP_WRITEB_ARRAY(isp_exposure_l2);
		break;
	case EXPOSURE_L1:
		ISP_WRITEB_ARRAY(isp_exposure_l1);
		break;
	case EXPOSURE_H0:
		ISP_WRITEB_ARRAY(isp_exposure_h0);
		break;
	case EXPOSURE_H1:
		ISP_WRITEB_ARRAY(isp_exposure_h1);
		break;
	case EXPOSURE_H2:
		ISP_WRITEB_ARRAY(isp_exposure_h2);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_exposure);

int isp_set_iso(struct isp_device *isp, int val)
{
	switch (val) {
	case ISO_AUTO:
		ISP_WRITEB_ARRAY(isp_iso_auto);
		break;
	case ISO_100:
		ISP_WRITEB_ARRAY(isp_iso_100);
		break;
	case ISO_200:
		ISP_WRITEB_ARRAY(isp_iso_200);
		break;
	case ISO_400:
		ISP_WRITEB_ARRAY(isp_iso_400);
		break;
	case ISO_800:
		ISP_WRITEB_ARRAY(isp_iso_800);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_iso);

int isp_set_contrast(struct isp_device *isp, int val)
{
	switch (val) {
	case CONTRAST_L3:
		ISP_WRITEB_ARRAY(isp_contrast_l3);
		break;
	case CONTRAST_L2:
		ISP_WRITEB_ARRAY(isp_contrast_l2);
		break;
	case CONTRAST_L1:
		ISP_WRITEB_ARRAY(isp_contrast_l1);
		break;
	case CONTRAST_H0:
		ISP_WRITEB_ARRAY(isp_contrast_h0);
		break;
	case CONTRAST_H1:
		ISP_WRITEB_ARRAY(isp_contrast_h1);
		break;
	case CONTRAST_H2:
		ISP_WRITEB_ARRAY(isp_contrast_h2);
		break;
	case CONTRAST_H3:
		ISP_WRITEB_ARRAY(isp_contrast_h3);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_contrast);

int isp_set_saturation(struct isp_device *isp, int val)
{
	switch (val) {
	case SATURATION_L2:
		ISP_WRITEB_ARRAY(isp_saturation_l2);
		break;
	case SATURATION_L1:
		ISP_WRITEB_ARRAY(isp_saturation_l1);
		break;
	case SATURATION_H0:
		ISP_WRITEB_ARRAY(isp_saturation_h0);
		break;
	case SATURATION_H1:
		ISP_WRITEB_ARRAY(isp_saturation_h1);
		break;
	case SATURATION_H2:
		ISP_WRITEB_ARRAY(isp_saturation_h2);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_saturation);

int isp_set_scene(struct isp_device *isp, int val)
{
	u32 max_exposure;
	u8 l,h;

	max_exposure = (isp->fmt_data.vts > 0x20) ? (isp->fmt_data.vts - 0x20) : isp->fmt_data.vts;
	if((SCENE_MODE_ACTION == val) || (SCENE_MODE_SPORTS == val))
		max_exposure /= 2;
	h = (max_exposure >> 8)&0xff;
	l = max_exposure & 0xff;

	switch (val) {
	case SCENE_MODE_AUTO:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_auto);
		break;
	case SCENE_MODE_NORMAL:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_normal);
		break;
	case SCENE_MODE_ACTION:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_action);
		break;
	case SCENE_MODE_PORTRAIT:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_portrait);
		break;
	case SCENE_MODE_LANDSCAPE:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_landscape);
		break;
	case SCENE_MODE_NIGHT:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_night);
		break;
	case SCENE_MODE_NIGHT_PORTRAIT:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_night_portrait);
		break;
	case SCENE_MODE_THEATRE:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_theatre);
		break;
	case SCENE_MODE_BEACH:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_beach);
		break;
	case SCENE_MODE_SNOW:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_snow);
		break;
	case SCENE_MODE_SUNSET:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_sunset);
		break;
	case SCENE_MODE_STEADYPHOTO:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_steadyphoto);
		break;
	case SCENE_MODE_FIREWORKS:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_fireworks);
		break;
	case SCENE_MODE_SPORTS:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_sports);
		break;
	case SCENE_MODE_PARTY:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_party);
		break;
	case SCENE_MODE_CANDLELIGHT:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_candlelight);
		break;
	case SCENE_MODE_BARCODE:
		isp_reg_writeb(isp, h, 0x1c158);
		isp_reg_writeb(isp, l, 0x1c159);
		ISP_WRITEB_ARRAY(isp_scene_barcode);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_scene);

int isp_set_effect(struct isp_device *isp, int val)
{
	u8 reg;
	reg = isp_reg_readb(isp, 0x65002);

	switch (val) {
	case EFFECT_NONE:
		reg &= (~(1<<2));
		isp_reg_writeb(isp, reg, 0x65002);
		isp_reg_writeb(isp, 0x00, 0x65b00);
		break;
	case EFFECT_MONO:
		reg |= (1<<2);
		isp_reg_writeb(isp, 0x20, 0x65b00);
		isp_reg_writeb(isp, reg, 0x65002);
		break;
	case EFFECT_NEGATIVE:
		reg |= (1<<2);
		isp_reg_writeb(isp, 0x40, 0x65b00);
		isp_reg_writeb(isp, reg, 0x65002);
		break;
	case EFFECT_SOLARIZE:
		break;
	case EFFECT_SEPIA:
		break;
	case EFFECT_POSTERIZE:
		break;
	case EFFECT_WHITEBOARD:
		break;
	case EFFECT_BLACKBOARD:
		break;
	case EFFECT_AQUA:
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_effect);

int isp_set_white_balance(struct isp_device *isp, int val)
{
	switch (val) {
	case WHITE_BALANCE_AUTO:
		ISP_WRITEB_ARRAY(isp_white_balance_auto);
		break;
	case WHITE_BALANCE_INCANDESCENT:
		ISP_WRITEB_ARRAY(isp_white_balance_incandescent);
		break;
	case WHITE_BALANCE_FLUORESCENT:
		ISP_WRITEB_ARRAY(isp_white_balance_fluorescent);
		break;
	case WHITE_BALANCE_WARM_FLUORESCENT:
		ISP_WRITEB_ARRAY(isp_white_balance_warm_fluorescent);
		break;
	case WHITE_BALANCE_DAYLIGHT:
		ISP_WRITEB_ARRAY(isp_white_balance_daylight);
		break;
	case WHITE_BALANCE_CLOUDY_DAYLIGHT:
		ISP_WRITEB_ARRAY(isp_white_balance_cloudy_daylight);
		break;
	case WHITE_BALANCE_TWILIGHT:
		ISP_WRITEB_ARRAY(isp_white_balance_twilight);
		break;
	case WHITE_BALANCE_SHADE:
		ISP_WRITEB_ARRAY(isp_white_balance_shade);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_white_balance);

int isp_set_sharpness(struct isp_device *isp, int val)
{
	switch (val) {
	case SHARPNESS_L2:
		ISP_WRITEB_ARRAY(isp_sharpness_l2);
		break;
	case SHARPNESS_L1:
		ISP_WRITEB_ARRAY(isp_sharpness_l1);
		break;
	case SHARPNESS_H0:
		ISP_WRITEB_ARRAY(isp_sharpness_h0);
		break;
	case SHARPNESS_H1:
		ISP_WRITEB_ARRAY(isp_sharpness_h1);
		break;
	case SHARPNESS_H2:
		ISP_WRITEB_ARRAY(isp_sharpness_h2);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_sharpness);

int isp_set_brightness(struct isp_device *isp, int val)
{
	switch (val) {
	case BRIGHTNESS_L3:
		ISP_WRITEB_ARRAY(isp_brightness_l3);
		break;
	case BRIGHTNESS_L2:
		ISP_WRITEB_ARRAY(isp_brightness_l2);
		break;
	case BRIGHTNESS_L1:
		ISP_WRITEB_ARRAY(isp_brightness_l1);
		break;
	case BRIGHTNESS_H0:
		ISP_WRITEB_ARRAY(isp_brightness_h0);
		break;
	case BRIGHTNESS_H1:
		ISP_WRITEB_ARRAY(isp_brightness_h1);
		break;
	case BRIGHTNESS_H2:
		ISP_WRITEB_ARRAY(isp_brightness_h2);
		break;
	case BRIGHTNESS_H3:
		ISP_WRITEB_ARRAY(isp_brightness_h3);
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_brightness);

int isp_set_flicker(struct isp_device *isp, int val)
{
	switch (val) {
	case FLICKER_AUTO:
		break;
	case FLICKER_50Hz:
		ISP_WRITEB_ARRAY(isp_flicker_50hz);
		break;
	case FLICKER_60Hz:
		ISP_WRITEB_ARRAY(isp_flicker_60hz);
		break;
	case FLICKER_OFF:
		break;
	default:
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(isp_set_flicker);

int isp_set_hflip(struct isp_device *isp, int val)
{
	return 0;
}
EXPORT_SYMBOL(isp_set_hflip);

int isp_set_vflip(struct isp_device *isp, int val)
{
	return 0;
}
EXPORT_SYMBOL(isp_set_vflip);

