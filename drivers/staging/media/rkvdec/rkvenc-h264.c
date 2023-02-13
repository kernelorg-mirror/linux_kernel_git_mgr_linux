// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2022 Pengutronix
 *	Michael Grzeschik <m.grzeschik@pengutronix.de>
 */

#include <linux/of.h>

#include <media/v4l2-h264.h>
#include <media/v4l2-mem2mem.h>

#include "rkvenc-vepu540.h"
#include "rkvpu.h"

struct rkvenc_h264_run {
	struct rkvpu_run base;
	const struct v4l2_ctrl_h264_encode_params *encode_params;
	const struct v4l2_ctrl_h264_encode_rc *encode_rc;
	const struct v4l2_ctrl_h264_sps *sps;
	const struct v4l2_ctrl_h264_pps *pps;
	const struct v4l2_ctrl_h264_scaling_matrix *scaling_matrix;
	struct vb2_buffer *ref_buf[V4L2_H264_NUM_DPB_ENTRIES];
};

struct rkvenc_h264_ctx {
	struct regmap *l1_regmap;
	struct regmap *l2_regmap;
	struct regmap_field *l1_fields[L1_MAX_FIELDS];
	struct regmap_field *l2_fields[L2_MAX_FIELDS];
};

static const struct regmap_config rkvenc_l1_regmap_cfg = {
	.name			= "rkvenc_l1",
	.reg_bits		= 32,
	.reg_stride		= 4,
	.val_bits		= 32,
	.reg_format_endian	= REGMAP_ENDIAN_NATIVE,
	.val_format_endian	= REGMAP_ENDIAN_NATIVE,
	.cache_type		= REGCACHE_NONE,
	/*
	 * No concurrent access expected - driver has one interrupt handler,
	 * regmap is not shared, no driver or user-space API.
	 */
	.disable_locking	= true,
	.max_register		= RKVENC_VEPU540_L1_MMU_ACKG(1),
};

static const struct regmap_config rkvenc_l2_regmap_cfg = {
	.name			= "rkvenc_l2",
	.reg_bits		= 32,
	.reg_stride		= 4,
	.val_bits		= 32,
	.reg_format_endian	= REGMAP_ENDIAN_NATIVE,
	.val_format_endian	= REGMAP_ENDIAN_NATIVE,
	.cache_type		= REGCACHE_NONE,
	/*
	 * No concurrent access expected - driver has one interrupt handler,
	 * regmap is not shared, no driver or user-space API.
	 */
	.disable_locking	= true,
	.max_register		= RKVENC_VEPU540_L2_I32_SOBEL_E(9),
};

#define CHROMA_KLUT_TAB_SIZE    24

static unsigned int h264e_klut_weight[30] = {
	0x0a000010, 0x00064000, 0x14000020, 0x000c8000,
	0x28000040, 0x00194000, 0x50800080, 0x0032c000,
	0xa1000100, 0x00658000, 0x42800200, 0x00cb0001,
	0x85000400, 0x01964002, 0x0a000800, 0x032c8005,
	0x14001000, 0x0659400a, 0x28802000, 0x0cb2c014,
	0x51004000, 0x1965c028, 0xa2808000, 0x32cbc050,
	0x4500ffff, 0x659780a1, 0x8a81fffe, 0xCC000142,
	0xFF83FFFF, 0x000001FF,
};

static int h264_aq_tthd_default[16] = {
	0,  0,  0,  0,
	3,  3,  5,  5,
	8,  8,  8,  15,
	15, 20, 25, 25,
};

static int h264_P_aq_step_default[16] = {
	-8, -7, -6, -5,
	-4, -3, -2, -1,
	0,  1,  2,  3,
	4,  5,  7,  8,
};

static int h264_I_aq_step_default[16] = {
	-8, -7, -6, -5,
	-4, -3, -2, -1,
	0,  1,  3,  3,
	4,  5,  8,  8,
};

static void setup_vepu541_normal(struct rkvpu_ctx *ctx)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;

	regmap_field_write(h264_ctx->l1_fields[SAFE_CLR], 0);
	regmap_field_write(h264_ctx->l1_fields[FORCE_CLR], 0);

	regmap_field_write(h264_ctx->l1_fields[LKT_ADDR], 0);

	regmap_field_write(h264_ctx->l1_fields[ENC_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[LKT_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[SCLR_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[ENC_SLICE_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[OFLW_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[BRSP_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[BERR_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[RERR_DONE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[WDG_DONE_EN], 0);

	regmap_field_write(h264_ctx->l1_fields[ENC_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[LKT_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[SCLR_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[ENC_SLICE_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[OFLW_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[BRSP_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[BERR_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[RERR_DONE_MSK], 0);
	regmap_field_write(h264_ctx->l1_fields[WDG_DONE_MSK], 0);

	regmap_field_write(h264_ctx->l1_fields[VS_LOAD_THD], 0);
	regmap_field_write(h264_ctx->l1_fields[RFP_LOAD_THRD], 0);

	regmap_field_write(h264_ctx->l1_fields[CMVW_BUS_ORDR], 0);
	regmap_field_write(h264_ctx->l1_fields[DSPW_BUS_ORDR], 0);
	regmap_field_write(h264_ctx->l1_fields[RFPW_BUS_ORDR], 0);
	regmap_field_write(h264_ctx->l1_fields[SRC_BUS_EDIN], 0);
	regmap_field_write(h264_ctx->l1_fields[MEIW_BUS_EDIN], 0);
	regmap_field_write(h264_ctx->l1_fields[BSW_BUS_EDIN], 7);
	regmap_field_write(h264_ctx->l1_fields[LKTR_BUS_EDIN], 0);
	regmap_field_write(h264_ctx->l1_fields[ROIR_BUS_EDIN], 0);
	regmap_field_write(h264_ctx->l1_fields[LKTW_BUS_EDIN], 0);
	regmap_field_write(h264_ctx->l1_fields[AFBC_BSIZE], 1);

	/* vepu540 */
	regmap_field_write(h264_ctx->l1_fields[VPU540_AXI_BRSP_CKE], 0);
	regmap_field_write(h264_ctx->l1_fields[VPU540_DSPR_OTSD], 1);
}

static bool rkvenc_alpha_swap(int format)
{
	switch (format) {
		case V4L2_PIX_FMT_ARGB32:
		case V4L2_PIX_FMT_ABGR32:
			return 1;
		default:
			return 0;
	}

	return 0;
}

static bool rkvenc_rbuv_swap(int format)
{
	switch (format) {
		case V4L2_PIX_FMT_ARGB32:
		case V4L2_PIX_FMT_RGBA32:
		case V4L2_PIX_FMT_BGR24:
		case V4L2_PIX_FMT_NV61:
		case V4L2_PIX_FMT_NV21:
		case V4L2_PIX_FMT_YVYU:
		case V4L2_PIX_FMT_VYUY:
		case V4L2_PIX_FMT_YUV444M:
			return 1;
		default:
			return 0;
	}

	return 0;
}

static bool rkvenc_src_endian(int format)
{
	switch (format) {
		case V4L2_PIX_FMT_RGB565:
			return 7;
		default:
			return 0;
	}

	return 0;
}

static bool rkvenc_fmt_is_rgb(int format)
{
	switch (format) {
		case V4L2_PIX_FMT_ARGB32:
		case V4L2_PIX_FMT_RGBA32:
		case V4L2_PIX_FMT_BGR32:
		case V4L2_PIX_FMT_RGB565:
			return 1;
		default:
			return 0;
	}

	return 0;
}

static int rkvenc_hw_fmt(int format)
{
	switch (format) {
		case V4L2_PIX_FMT_UYVY:
			return 9;
		case V4L2_PIX_FMT_YUYV:
			return 8;
		case V4L2_PIX_FMT_YUV420M:
		case V4L2_PIX_FMT_YUV420:
			return 7;
		case V4L2_PIX_FMT_NV21M:
		case V4L2_PIX_FMT_NV21:
		case V4L2_PIX_FMT_NV12M:
		case V4L2_PIX_FMT_NV12:
			return 6;
		case V4L2_PIX_FMT_YUV422P:
			return 5;
		case V4L2_PIX_FMT_NV61M:
		case V4L2_PIX_FMT_NV61:
		case V4L2_PIX_FMT_NV16M:
		case V4L2_PIX_FMT_NV16:
			return 4;
		case V4L2_PIX_FMT_RGB565:
			return 2;
		case V4L2_PIX_FMT_RGB24:
			return 1;
		case V4L2_PIX_FMT_RGBA32:
		default:
			return 0;
	}

	return 0;
}

static bool rkvenc_yuvfmt_is_sp(int format)
{
	switch (format) {
		case V4L2_PIX_FMT_NV61:
		case V4L2_PIX_FMT_NV16:
		case V4L2_PIX_FMT_NV21:
		case V4L2_PIX_FMT_NV12:
			return 1;
		default:
			return 0;
	}

	return 0;
}

static void setup_vepu541_prep(struct rkvpu_ctx *ctx)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_pix_format_mplane *src_fmt;
	const struct v4l2_format *f;
	int y_stride;
	int c_stride;

	f = &ctx->src_fmt;
	src_fmt = &f->fmt.pix_mp;

	regmap_field_write(h264_ctx->l1_fields[PIC_WD8_M1],
			   ALIGN(src_fmt->width, 16) / 8 - 1);
	regmap_field_write(h264_ctx->l1_fields[PIC_WFILL],
			   ALIGN(src_fmt->width, 16) - src_fmt->width);
	regmap_field_write(h264_ctx->l1_fields[PIC_HD8_M1],
			   ALIGN(src_fmt->height, 16) / 8 - 1);
	regmap_field_write(h264_ctx->l1_fields[PIC_HFILL],
			   ALIGN(src_fmt->height, 16) - src_fmt->height);

	regmap_field_write(h264_ctx->l1_fields[SRC_BUS_EDIN], rkvenc_src_endian(src_fmt->pixelformat));

	regmap_field_write(h264_ctx->l1_fields[SRC_CFMT], rkvenc_hw_fmt(src_fmt->pixelformat));
	regmap_field_write(h264_ctx->l1_fields[ALPHA_SWAP], rkvenc_alpha_swap(src_fmt->pixelformat));
	regmap_field_write(h264_ctx->l1_fields[RBUV_SWAP], rkvenc_rbuv_swap(src_fmt->pixelformat));
	regmap_field_write(h264_ctx->l1_fields[SRC_RANGE], 0);
	regmap_field_write(h264_ctx->l1_fields[OUT_FMT_CFG], 0);

	//FIXME
	/*if (MPP_FRAME_FMT_IS_FBC(src_fmt->pixelformat)) {
		y_stride = mpp_frame_get_fbc_hdr_stride(task->frame);
		if (!y_stride)
			y_stride = ALIGN(prep->hor_stride, 16);
	} else
	*/

	y_stride = (f->fmt.pix.bytesperline) ? (f->fmt.pix.bytesperline) : (src_fmt->width);

	c_stride = rkvenc_yuvfmt_is_sp(src_fmt->pixelformat) ?
		y_stride : y_stride / 2;

	if (rkvenc_fmt_is_rgb(src_fmt->pixelformat)) {
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_B2Y], 25);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_G2Y], 129);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_R2Y], 66);

		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_B2U], 112);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_G2U], -74);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_R2U], -38);

		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_B2V], -18);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_G2V], -94);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_R2V], 112);

		regmap_field_write(h264_ctx->l1_fields[CSC_OFST_Y], 15);
		regmap_field_write(h264_ctx->l1_fields[CSC_OFST_U], 128);
		regmap_field_write(h264_ctx->l1_fields[CSC_OFST_V], 128);
	} else {
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_B2Y], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_G2Y], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_R2Y], 0);

		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_B2U], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_G2U], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_R2U], 0);

		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_B2V], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_G2V], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_WGT_R2V], 0);

		regmap_field_write(h264_ctx->l1_fields[CSC_OFST_Y], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_OFST_U], 0);
		regmap_field_write(h264_ctx->l1_fields[CSC_OFST_V], 0);
	}

	regmap_field_write(h264_ctx->l1_fields[AFBCD_EN], 0); // No AFBC Format for now
	regmap_field_write(h264_ctx->l1_fields[SRC_STRD0], y_stride);
	regmap_field_write(h264_ctx->l1_fields[SRC_STRD1], c_stride);

	regmap_field_write(h264_ctx->l1_fields[SRC_MIRR], 0); // disable for now
	regmap_field_write(h264_ctx->l1_fields[SRC_ROT], 0); // disable for now
	regmap_field_write(h264_ctx->l1_fields[TXA_EN], 1);

	regmap_field_write(h264_ctx->l1_fields[SLI_CRS_EN], 1);

	regmap_field_write(h264_ctx->l1_fields[PIC_OFST_Y], 0);
	regmap_field_write(h264_ctx->l1_fields[PIC_OFST_X], 0);
}

static void setup_vepu541_codec(struct rkvpu_ctx *ctx, struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_ctrl_h264_encode_params *encode_params;
	const struct v4l2_ctrl_h264_sps *sps;
	const struct v4l2_ctrl_h264_pps *pps;

	encode_params = run->encode_params;
	sps = run->sps;
	pps = run->pps;

	regmap_field_write(h264_ctx->l1_fields[ENC_STND], 0);
	// FIXME: CUR_FRM_REF needs to be 0 to avoid paging errors in iommu
	regmap_field_write(h264_ctx->l1_fields[CUR_FRM_REF],
			   /* encode_params->nal_reference_idc > */ 0 );
	regmap_field_write(h264_ctx->l1_fields[BS_SCP], 1);
	regmap_field_write(h264_ctx->l1_fields[LAMB_MOD_SEL],
			   (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) ? 0 : 1);
	regmap_field_write(h264_ctx->l1_fields[ATR_THD_SEL], 0);
	regmap_field_write(h264_ctx->l1_fields[NODE_INT], 0);

	regmap_field_write(h264_ctx->l1_fields[NAL_REF_IDC],
			   encode_params->nal_reference_idc);
	regmap_field_write(h264_ctx->l1_fields[NAL_UNIT_TYPE],
			   encode_params->nalu_type);

	regmap_field_write(h264_ctx->l1_fields[MAX_FNUM],
			   sps->log2_max_frame_num_minus4);
	regmap_field_write(h264_ctx->l1_fields[DRCT_8X8],
			   !!(sps->flags & V4L2_H264_SPS_FLAG_DIRECT_8X8_INFERENCE));
	regmap_field_write(h264_ctx->l1_fields[MPOC_LM4],
			   sps->log2_max_pic_order_cnt_lsb_minus4);

	regmap_field_write(h264_ctx->l1_fields[ETPY_MODE],
			   !!(pps->flags & V4L2_H264_PPS_FLAG_ENTROPY_CODING_MODE));
	regmap_field_write(h264_ctx->l1_fields[TRNS_8X8],
			   !!(pps->flags & V4L2_H264_PPS_FLAG_TRANSFORM_8X8_MODE));
	regmap_field_write(h264_ctx->l1_fields[CSIP_FLAG],
			   !!(pps->flags & V4L2_H264_PPS_FLAG_CONSTRAINED_INTRA_PRED));
	regmap_field_write(h264_ctx->l1_fields[NUM_REF0_IDX],
			   pps->num_ref_idx_l0_default_active_minus1);
	regmap_field_write(h264_ctx->l1_fields[NUM_REF1_IDX],
			   pps->num_ref_idx_l1_default_active_minus1);
	regmap_field_write(h264_ctx->l1_fields[PIC_INIT_QP],
			   pps->pic_init_qp_minus26 + 26);
	regmap_field_write(h264_ctx->l1_fields[CB_OFST],
			   pps->chroma_qp_index_offset);
	regmap_field_write(h264_ctx->l1_fields[CR_OFST],
			   pps->second_chroma_qp_index_offset);
	regmap_field_write(h264_ctx->l1_fields[WGHT_PRED],
			   !!(pps->flags & V4L2_H264_PPS_FLAG_WEIGHTED_PRED));
	regmap_field_write(h264_ctx->l1_fields[DBF_CP_FLG],
			   !!(pps->flags & V4L2_H264_PPS_FLAG_DEBLOCKING_FILTER_CONTROL_PRESENT));

	regmap_field_write(h264_ctx->l1_fields[SLI_TYPE],
			   (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) ? (2) : (0));
	regmap_field_write(h264_ctx->l1_fields[PPS_ID],
			   encode_params->pic_parameter_set_id);
	regmap_field_write(h264_ctx->l1_fields[DRCT_SMVP], 0);
	regmap_field_write(h264_ctx->l1_fields[NUM_REF_OVRD],
			   encode_params->num_ref_idx_override);
	regmap_field_write(h264_ctx->l1_fields[CBC_INIT_IDC],
			   encode_params->cabac_init_idc);
	regmap_field_write(h264_ctx->l1_fields[FRM_NUM], encode_params->frame_num);

	regmap_field_write(h264_ctx->l1_fields[IDR_PIC_ID],
			   (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) ?
				encode_params->idr_pic_id : (unsigned int)(-1));
	regmap_field_write(h264_ctx->l1_fields[POC_LSB], sps->pic_order_cnt_type);


	regmap_field_write(h264_ctx->l1_fields[DIS_DBLK_IDC],
			   encode_params->disable_deblocking_filter_idc);
	regmap_field_write(h264_ctx->l1_fields[SLI_ALPH_OFST],
			   encode_params->slice_alpha_c0_offset_div2);

#if 0
	{
	H264eRplmo rplmo; // reference picture list modification operation
	H264eMmco mmco; // memory management control operation
	int ret;

	h264e_reorder_rd_rewind(slice->reorder);
	/* reorder process */
	ret = h264e_reorder_rd_op(slice->reorder, &rplmo);
	if (!ret) {
		regmap_field_write(h264_ctx->l1_fields[REF_LIST0_RODR], 1);
		regmap_field_write(h264_ctx->l1_fields[RODR_PIC_IDX],
				   rplmo.modification_of_pic_nums_idc);

		switch (rplmo.modification_of_pic_nums_idc) {
			case 0:
			case 1:
				regmap_field_write(h264_ctx->l1_fields[RODR_PIC_NUM],
						   rplmo.abs_diff_pic_num_minus1);
				break;
			case 2:
				regmap_field_write(h264_ctx->l1_fields[RODR_PIC_NUM],
						   rplmo.long_term_pic_idx);
				break;
			default:
				mpp_err_f("invalid modification_of_pic_nums_idc %d\n",
					  rplmo.modification_of_pic_nums_idc);
				break;
		}
	} else {
		// slice->ref_pic_list_modification_flag;
		regmap_field_write(h264_ctx->l1_fields[REF_LIST0_RODR], 0);
		regmap_field_write(h264_ctx->l1_fields[RODR_PIC_IDX], 0);
		regmap_field_write(h264_ctx->l1_fields[RODR_PIC_NUM], 0);
	}

#endif
	/* clear all mmco arg first */
	regmap_field_write(h264_ctx->l1_fields[NOPP_FLG], 0);
	regmap_field_write(h264_ctx->l1_fields[LTRF_FLG], 0);
	regmap_field_write(h264_ctx->l1_fields[ARPM_FLG], 0);
	regmap_field_write(h264_ctx->l1_fields[MMCO4_PRE], 0);
	regmap_field_write(h264_ctx->l1_fields[MMCO_TYPE0], 0);
	regmap_field_write(h264_ctx->l1_fields[MMCO_PARM0], 0);
	regmap_field_write(h264_ctx->l1_fields[MMCO_TYPE1], 0);
	regmap_field_write(h264_ctx->l1_fields[MMCO_PARM1], 0);
	regmap_field_write(h264_ctx->l1_fields[MMCO_TYPE2], 0);
	regmap_field_write(h264_ctx->l1_fields[MMCO_PARM2], 0);
	regmap_field_write(h264_ctx->l1_fields[LONG_TERM_FRAME_IDX0], 0);
	regmap_field_write(h264_ctx->l1_fields[LONG_TERM_FRAME_IDX1], 0);
	regmap_field_write(h264_ctx->l1_fields[LONG_TERM_FRAME_IDX2], 0);

#if 0
	h264e_marking_rd_rewind(slice->marking);

	/* only update used parameter */
	if (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) {
#endif
		regmap_field_write(h264_ctx->l1_fields[NOPP_FLG],
				   encode_params->no_output_of_prior_pics);
		regmap_field_write(h264_ctx->l1_fields[LTRF_FLG],
				   encode_params->long_term_reference_flag);
#if 0
		return 0;
	}

	if (h264e_marking_is_empty(slice->marking))
		return 0;

	regmap_field_write(h264_ctx->l1_fields[ARPM_FLG], 1);

	/* max 3 mmco */
	int type = 0;
	int param_0 = 0;
	int param_1 = 0;

	h264e_marking_rd_op(slice->marking, &mmco);
	type = mmco.mmco;
	switch (type) {
		case 1:
			param_0 = mmco.difference_of_pic_nums_minus1;
			break;
		case 2:
			param_0 = mmco.long_term_pic_num;
			break;
		case 3:
			param_0 = mmco.difference_of_pic_nums_minus1;
			param_1 = mmco.long_term_frame_idx;
			break;
		case 4:
			param_0 = mmco.max_long_term_frame_idx_plus1;
			break;
		case 5:
			break;
		case 6:
			param_0 = mmco.long_term_frame_idx;
			break;
		default:
			mpp_err_f("unsupported mmco 0 %d\n", type);
			type = 0;
			break;
	}

	regmap_field_write(h264_ctx->l1_fields[MMCO_TYPE0], type);
	regmap_field_write(h264_ctx->l1_fields[MMCO_PARM0], param_0);
	regmap_field_write(h264_ctx->l1_fields[LONG_TERM_FRAME_IDX0], param_1);

	if (h264e_marking_is_empty(slice->marking))
		break;

	h264e_marking_rd_op(slice->marking, &mmco);
	type = mmco.mmco;
	param_0 = 0;
	param_1 = 0;
	switch (type) {
		case 1:
			param_0 = mmco.difference_of_pic_nums_minus1;
			break;
		case 2:
			param_0 = mmco.long_term_pic_num;
			break;
		case 3:
			param_0 = mmco.difference_of_pic_nums_minus1;
			param_1 = mmco.long_term_frame_idx;
			break;
		case 4:
			param_0 = mmco.max_long_term_frame_idx_plus1;
			break;
		case 5:
			break;
		case 6:
			param_0 = mmco.long_term_frame_idx;
			break;
		default:
			mpp_err_f("unsupported mmco 0 %d\n", type);
			type = 0;
			break;
	}

	regmap_field_write(h264_ctx->l1_fields[MMCO_TYPE1], type);
	regmap_field_write(h264_ctx->l1_fields[MMCO_PARM1], param_0);
	regmap_field_write(h264_ctx->l1_fields[LONG_TERM_FRAME_IDX1], param_1);

	if (h264e_marking_is_empty(slice->marking))
		break;

	h264e_marking_rd_op(slice->marking, &mmco);
	type = mmco.mmco;
	param_0 = 0;
	param_1 = 0;
	switch (type) {
		case 1:
			param_0 = mmco.difference_of_pic_nums_minus1;
			break;
		case 2: {
			param_0 = mmco.long_term_pic_num;
			break;
		case 3:
			param_0 = mmco.difference_of_pic_nums_minus1;
			param_1 = mmco.long_term_frame_idx;
			break;
		case 4:
			param_0 = mmco.max_long_term_frame_idx_plus1;
			break;
		case 5:
			break;
		case 6:
			param_0 = mmco.long_term_frame_idx;
			break;
		default:
			mpp_err_f("unsupported mmco 0 %d\n", type);
			type = 0;
			break;
	}

	regmap_field_write(h264_ctx->l1_fields[MMCO_TYPE2], type);
	regmap_field_write(h264_ctx->l1_fields[MMCO_PARM2], param_0);
	regmap_field_write(h264_ctx->l1_fields[LONG_TERM_FRAME_IDX2], param_1);
	}
#endif
}

static void setup_vepu541_rdo_pred(struct rkvpu_ctx *ctx, struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_ctrl_h264_encode_params *encode_params;
	const struct v4l2_ctrl_h264_sps *sps;
	const struct v4l2_ctrl_h264_pps *pps;

	encode_params = run->encode_params;
	sps = run->sps;
	pps = run->pps;

	if (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) {
		regmap_field_write(h264_ctx->l1_fields[CHRM_KLUT_OFST], 0);
		regmap_bulk_write(h264_ctx->l1_regmap, RKVENC_VEPU540_L1_KLUT_WGT(0),
				  &h264e_klut_weight[0], CHROMA_KLUT_TAB_SIZE);
	} else {
		regmap_field_write(h264_ctx->l1_fields[CHRM_KLUT_OFST], 3);
		regmap_bulk_write(h264_ctx->l1_regmap, RKVENC_VEPU540_L1_KLUT_WGT(0),
				  &h264e_klut_weight[4], CHROMA_KLUT_TAB_SIZE);
	}

	regmap_field_write(h264_ctx->l1_fields[VTHD_Y], 9);
	regmap_field_write(h264_ctx->l1_fields[VTHD_C], 63);

	regmap_field_write(h264_ctx->l1_fields[RECT_SIZE],
			   (sps->profile_idc == V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE &&
				sps->level_idc <= V4L2_MPEG_VIDEO_H264_LEVEL_3_0) ? 1 : 0);
	regmap_field_write(h264_ctx->l1_fields[INTER_4X4], 0);
	regmap_field_write(h264_ctx->l1_fields[VLC_LMT],
			   (sps->profile_idc < V4L2_MPEG_VIDEO_H264_PROFILE_MAIN) &&
			        !encode_params->flags & V4L2_H264_ENCODE_FLAG_ENTROPY_CODING_MODE);
	regmap_field_write(h264_ctx->l1_fields[CHRM_SPCL], 1);
	regmap_field_write(h264_ctx->l1_fields[RDO_MASK], 24);
	regmap_field_write(h264_ctx->l1_fields[CCWA_E], 1);
	regmap_field_write(h264_ctx->l1_fields[SCL_LST_SEL],
			   pps->flags & V4L2_H264_PPS_FLAG_SCALING_MATRIX_PRESENT);
	regmap_field_write(h264_ctx->l1_fields[SCL_LST_SEL_],
			   pps->flags & V4L2_H264_PPS_FLAG_SCALING_MATRIX_PRESENT);
	regmap_field_write(h264_ctx->l1_fields[ATR_E], 1);
	regmap_field_write(h264_ctx->l1_fields[ATF_EDG], 0);
	regmap_field_write(h264_ctx->l1_fields[ATF_LVL_E], 0);
	regmap_field_write(h264_ctx->l1_fields[ATF_INTRA_E], 1);
	regmap_field_write(h264_ctx->l1_fields[SATD_BYPS_FLG], 0);
}

static void setup_vepu541_rc_base(struct rkvpu_ctx *ctx, struct rkvenc_h264_run *run)
{
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_ctrl_h264_encode_params *encode_params = run->encode_params;
	const struct v4l2_ctrl_h264_encode_rc *encode_rc = run->encode_rc;
	const struct v4l2_ctrl_h264_sps *sps = run->sps;
	int mb_w = sps->pic_width_in_mbs_minus1 + 1;
	int mb_h = sps->pic_height_in_map_units_minus1 + 1; //sps->pic_height_in_mbs;
	unsigned int qp_target = encode_rc->qp;
	unsigned int qp_min = encode_rc->qp_min;
	unsigned int qp_max = encode_rc->qp_max;
	unsigned int qpmap_mode = 1;
	int mb_target_bits_mul_16 = DIV_ROUND_UP((encode_rc->target_bits << 4), (mb_w * mb_h));
	int mb_target_bits;
	int negative_bits_thd;
	int positive_bits_thd;

	v4l2_info(&rkvpu->v4l2_dev, "bittarget %d qp [%d %d %d]\n",
			encode_rc->target_bits, qp_min, qp_target, qp_max);

	if (mb_target_bits_mul_16 >= 0x100000) {
		mb_target_bits_mul_16 = 0x50000;
	}

	mb_target_bits = (mb_target_bits_mul_16 * mb_w) >> 4;
	negative_bits_thd = 0 - mb_target_bits / 4;
	positive_bits_thd = mb_target_bits / 4;

	regmap_field_write(h264_ctx->l1_fields[PIC_QP], qp_target);

	regmap_field_write(h264_ctx->l1_fields[RC_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[AQ_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[AQ_MODE], 0);
	regmap_field_write(h264_ctx->l1_fields[RC_CTU_NUM], mb_w);

	regmap_field_write(h264_ctx->l1_fields[RC_QP_RANGE],
			(encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) ?
			0 : 2);
	regmap_field_write(h264_ctx->l1_fields[RC_MAX_QP], qp_max);
	regmap_field_write(h264_ctx->l1_fields[RC_MIN_QP], qp_min);

	regmap_field_write(h264_ctx->l1_fields[CTU_EBIT], mb_target_bits_mul_16);

	regmap_field_write(h264_ctx->l1_fields[QP_ADJ0], -1);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ1], 0);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ2], 0);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ3], 0);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ4], 0);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ5], 0);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ6], 0);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ7], 0);
	regmap_field_write(h264_ctx->l1_fields[QP_ADJ8], 1);

	regmap_field_write(h264_ctx->l1_fields[RC_DTHD0], negative_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD1], positive_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD2], positive_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD3], positive_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD4], positive_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD5], positive_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD6], positive_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD7], positive_bits_thd);
	regmap_field_write(h264_ctx->l1_fields[RC_DTHD8], positive_bits_thd);

	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA0], qp_min);
	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA0], qp_max);
	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA1], qp_min);
	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA1], qp_max);
	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA2], qp_min);

	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA2], qp_max);
	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA3], qp_min);
	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA3], qp_max);
	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA4], qp_min);
	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA4], qp_max);

	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA5], qp_min);
	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA5], qp_max);
	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA6], qp_min);
	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA6], qp_max);
	regmap_field_write(h264_ctx->l1_fields[QPMIN_AREA7], qp_min);

	regmap_field_write(h264_ctx->l1_fields[QPMAX_AREA7], qp_max);
	regmap_field_write(h264_ctx->l1_fields[QPMAP_MODE], qpmap_mode);
}

static void setup_vepu541_io_buf(struct rkvpu_ctx *ctx,
				 struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_pix_format_mplane *src_fmt;
	const struct v4l2_format *f;
	int ver_stride;
	int hor_stride;
	int src_plane2offset = 0;
	int src_plane3offset = 0;
	struct vb2_v4l2_buffer *src_buf = run->base.bufs.src;
	struct vb2_v4l2_buffer *dst_buf = run->base.bufs.dst;

	f = &ctx->src_fmt;
	src_fmt = &f->fmt.pix_mp;

	ver_stride = (f->fmt.pix.bytesperline) ?
		     (f->fmt.pix.bytesperline) : (src_fmt->width);
	hor_stride = src_fmt->height;

	/*
	if (MPP_FRAME_FMT_IS_FBC(fmt)) {
		off_in[0] = mpp_frame_get_fbc_offset(frm);
		off_in[1] = 0;
	} else
	*/
	if (!rkvenc_fmt_is_rgb(src_fmt->pixelformat)) {
		switch (src_fmt->pixelformat) {
			case V4L2_PIX_FMT_NV61:
			case V4L2_PIX_FMT_NV21:
			case V4L2_PIX_FMT_NV12:
			case V4L2_PIX_FMT_NV16:
				src_plane2offset = hor_stride * ver_stride;
				src_plane3offset = hor_stride * ver_stride;
				break;
			case V4L2_PIX_FMT_YUV422P:
				src_plane2offset = hor_stride * ver_stride;
				src_plane3offset = hor_stride * ver_stride * 3 / 2;
				break;
			case V4L2_PIX_FMT_YUV420:
				src_plane2offset = hor_stride * ver_stride;
				src_plane3offset = hor_stride * ver_stride * 5 / 4;
				break;
			default:
				break;
		}
	}

	regmap_field_write(h264_ctx->l1_fields[ADR_SRC0],
			vb2_dma_contig_plane_dma_addr(&src_buf->vb2_buf, 0));

	if (src_buf->vb2_buf.num_planes > 1)
		regmap_field_write(h264_ctx->l1_fields[ADR_SRC1],
				vb2_dma_contig_plane_dma_addr(&src_buf->vb2_buf, 1) + src_plane2offset);
	else
		regmap_field_write(h264_ctx->l1_fields[ADR_SRC1], 0xbeefdead);

	if (src_buf->vb2_buf.num_planes > 2)
		regmap_field_write(h264_ctx->l1_fields[ADR_SRC2],
				vb2_dma_contig_plane_dma_addr(&src_buf->vb2_buf, 2) + src_plane3offset);
	else
		regmap_field_write(h264_ctx->l1_fields[ADR_SRC2], 0xdeadbeef);

	f = &ctx->dst_fmt;
	regmap_field_write(h264_ctx->l1_fields[BSBT_ADDR],
			(vb2_dma_contig_plane_dma_addr(&dst_buf->vb2_buf, 0) + f->fmt.pix_mp.plane_fmt[0].sizeimage));
	regmap_field_write(h264_ctx->l1_fields[BSBB_ADDR],
			vb2_dma_contig_plane_dma_addr(&dst_buf->vb2_buf, 0));
	regmap_field_write(h264_ctx->l1_fields[BSBR_ADDR],
			vb2_dma_contig_plane_dma_addr(&dst_buf->vb2_buf, 0));
	regmap_field_write(h264_ctx->l1_fields[ADR_BSBS],
			vb2_dma_contig_plane_dma_addr(&dst_buf->vb2_buf, 0) /* + mpp_packet_get_length(pkt) */ );
}

static void setup_vepu541_roi(struct rkvpu_ctx *ctx, struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
#if 0
	const struct v4l2_ctrl_h264_sps *sps;
	sps = run->sps;

	if (ctx->roi_data2) {
		MppEncROICfg2 *cfg = ( MppEncROICfg2 *)ctx->roi_data2;

		regmap_field_write(h264_ctx->l1_fields[ROI_ENC], 1);
		regmap_field_write(h264_ctx->l1_fields[ROI_ADDR],
				   mpp_buffer_get_fd(cfg->base_cfg_buf));
	} else if (ctx->qpmap) {
		regmap_field_write(h264_ctx->l1_fields[ROI_ENC], 1);
		regmap_field_write(h264_ctx->l1_fields[ROI_ADDR],
				   mpp_buffer_get_fd(ctx->qpmap));
	} else {
		MppEncROICfg *roi = ctx->roi_data;
		unsigned int w = sps->pic_width_in_mbs * 16;
		unsigned int h = sps->pic_height_in_mbs * 16;

		/* roi setup */
		if (roi && roi->number && roi->regions) {
			int roi_buf_size = vepu541_get_roi_buf_size(w, h);

			if (!ctx->roi_buf || roi_buf_size != ctx->roi_buf_size) {
				if (NULL == ctx->roi_grp)
					mpp_buffer_group_get_internal(&ctx->roi_grp,
							   MPP_BUFFER_TYPE_ION);
				else if (roi_buf_size != ctx->roi_buf_size) {
					if (ctx->roi_buf) {
						mpp_buffer_put(ctx->roi_buf);
						ctx->roi_buf = NULL;
					}
					mpp_buffer_group_clear(ctx->roi_grp);
				}

				if (!ctx->roi_grp)
					return;

				if (NULL == ctx->roi_buf)
					mpp_buffer_get(ctx->roi_grp,
							   &ctx->roi_buf, roi_buf_size);

				ctx->roi_buf_size = roi_buf_size;
			}

			if (!ctx->roi_grp)
				return;

			int fd = mpp_buffer_get_fd(ctx->roi_buf);
			void *buf = mpp_buffer_get_ptr(ctx->roi_buf);

			regmap_field_write(h264_ctx->l1_fields[ROI_ENC], 1);
			regmap_field_write(h264_ctx->l1_fields[ROI_ADDR], fd);

			vepu541_set_roi(buf, roi, w, h);
		} else {
#endif
			regmap_field_write(h264_ctx->l1_fields[ROI_ENC], 0);
			regmap_field_write(h264_ctx->l1_fields[ROI_ADDR], 0);
#if 0
		}
	}
#endif
}

static unsigned int rec_luma_size(unsigned int width, unsigned int height)
{
	return round_up(width, MB_DIM) * round_up(height, MB_DIM);
}


static void setup_vepu541_recn_refr(struct rkvpu_ctx *ctx,
				    struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_format *f = &ctx->src_fmt;
	const struct v4l2_pix_format_mplane *src_fmt = &f->fmt.pix_mp;
	struct vb2_v4l2_buffer *dst_buf = run->base.bufs.dst;
	const struct v4l2_ctrl_h264_encode_params *encode_params;
	struct rkvpu_enc_buf *enc_buf;
	struct rkvpu_aux_buf *rec_buf;
	struct rkvpu_aux_buf *ds_buf;

	enc_buf = rkvpu_get_enc_buf(dst_buf);
	rec_buf = &enc_buf->rec_buf;
	ds_buf = &enc_buf->ds_buf;

	encode_params = run->encode_params;

	regmap_field_write(h264_ctx->l1_fields[RFPW_H_ADDR], rec_buf->dma);
	regmap_field_write(h264_ctx->l1_fields[RFPW_B_ADDR], rec_buf->dma +
		rec_luma_size(src_fmt->width, src_fmt->height)); // fbc_hdr_size;
	regmap_field_write(h264_ctx->l1_fields[DSPW_ADDR], ds_buf->dma);

	if (encode_params->slice_type == V4L2_H264_SLICE_TYPE_P) {
		struct vb2_queue *queue;
		struct vb2_v4l2_buffer *ref_buf;
		u64 reference_ts;
		struct vb2_buffer *buf;

		queue = v4l2_m2m_get_vq(ctx->fh.m2m_ctx,
					V4L2_BUF_TYPE_VIDEO_CAPTURE);

		reference_ts = encode_params->reference_ts;
		buf = vb2_find_buffer(queue, reference_ts);
		if (!buf)
			return;

		ref_buf = to_vb2_v4l2_buffer(buf);
		enc_buf = rkvpu_get_enc_buf(ref_buf);
		rec_buf = &enc_buf->rec_buf;
		ds_buf = &enc_buf->ds_buf;

		regmap_field_write(h264_ctx->l1_fields[RFPR_H_ADDR], rec_buf->dma);
		regmap_field_write(h264_ctx->l1_fields[RFPR_B_ADDR], rec_buf->dma +
			rec_luma_size(src_fmt->width, src_fmt->height)); // fbc_hdr_size;

		regmap_field_write(h264_ctx->l1_fields[DSPR_ADDR], ds_buf->dma);
	}
}

static void setup_vepu541_split(struct rkvpu_ctx *ctx)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
#if 0
	switch (split_mode) {
		case MPP_ENC_SPLIT_NONE:
#endif
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_MODE], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CPST], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_MAX_NUM_M1], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_FLSH], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CNUM_M1], 0);

			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_BYTE], 0);
			regmap_field_write(h264_ctx->l1_fields[SLEN_FIFO], 0);
#if 0
			break;
		case MPP_ENC_SPLIT_BY_BYTE:
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT], 1);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_MODE], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CPST], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_MAX_NUM_M1], 500);
			regmap_field_write(h264_ctx->l1_fields[SLI_FLSH], 1);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CNUM_M1], 0);

			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_BYTE],
					   cfg->split_arg);
			regmap_field_write(h264_ctx->l1_fields[SLEN_FIFO], 0);
			break;
		case MPP_ENC_SPLIT_BY_CTU:
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT], 1);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_MODE], 1);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CPST], 0);
			regmap_field_write(h264_ctx->l1_fields[SLI_MAX_NUM_M1], 500);
			regmap_field_write(h264_ctx->l1_fields[SLI_FLSH], 1);
			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CNUM_M1],
					   cfg->split_arg - 1);

			regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_BYTE], 0);
			regmap_field_write(h264_ctx->l1_fields[SLEN_FIFO], 0);
			break;
		default:
			pr_info("invalide slice split mode \n");
			break;
	}
#endif
}

static void setup_vepu540_force_slice_split(struct rkvpu_ctx *ctx, int width)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	int mb_w = ALIGN(width, 16) >> 4;

	regmap_field_write(h264_ctx->l1_fields[SLI_SPLT], 1);
	regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_MODE], 1);
	regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CPST], 0);
	regmap_field_write(h264_ctx->l1_fields[SLI_MAX_NUM_M1], 500);
	regmap_field_write(h264_ctx->l1_fields[SLI_FLSH], 1);
	regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_CNUM_M1], mb_w - 1);

	regmap_field_write(h264_ctx->l1_fields[SLI_SPLT_BYTE], 0);
	regmap_field_write(h264_ctx->l1_fields[SLEN_FIFO], 0);
	regmap_field_write(h264_ctx->l1_fields[SLI_CRS_EN], 0);
}

static void setup_vepu541_me(struct rkvpu_ctx *ctx, struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_ctrl_h264_encode_params *encode_params;
	const struct v4l2_ctrl_h264_sps *sps;
	const struct v4l2_ctrl_h264_pps *pps;

	int w_temp = 1296;
	int h_temp = 1;
	int h_val_0 = 1;
	int h_val_1 = 18;
	int temp0, temp1;
	int swin_scope_wd16;
	unsigned int pic_temp;
	int cime_linebuf_w;
	int pic_w, pic_wd64;
	int cime_w = 176;
	int cime_h = 112;
	int cime_blk_w_max = 44;
	int cime_blk_h_max = 28;
	u16 pic_height_in_mbs;
	u16 pic_width_in_mbs;
	u32 pic_wd8_m1;
	u32 cme_srch_h;
	u32 cme_srch_v;
	u32 cme_rama_h;

	encode_params = run->encode_params;
	sps = run->sps;
	pps = run->pps;

	pic_height_in_mbs = sps->pic_height_in_map_units_minus1 + 1;
	pic_width_in_mbs = sps->pic_width_in_mbs_minus1 + 1;

	/*
	 * Step 1. limit the mv range by level_idc
	 * For level 1 and level 1b the vertical MV range is [-64,+63.75]
	 * For level 1.1, 1.2, 1.3 and 2 the vertical MV range is [-128,+127.75]
	 */
	switch (sps->level_idc) {
		case V4L2_MPEG_VIDEO_H264_LEVEL_1_0:
		case V4L2_MPEG_VIDEO_H264_LEVEL_1B:
			cime_blk_h_max = 12;
			break;
		case V4L2_MPEG_VIDEO_H264_LEVEL_1_1:
		case V4L2_MPEG_VIDEO_H264_LEVEL_1_2:
		case V4L2_MPEG_VIDEO_H264_LEVEL_1_3:
		case V4L2_MPEG_VIDEO_H264_LEVEL_2_0:
			cime_blk_h_max = 28;
			break;
		default:
			cime_blk_h_max = 28;
			break;
	}

	if (cime_w < cime_blk_w_max * 4)
		cime_blk_w_max = cime_w / 4;

	if (cime_h < cime_blk_h_max * 4)
		cime_blk_h_max = cime_h / 4;

	/*
	 * Step 2. limit the mv range by image size
	 */
	if (cime_blk_w_max / 4 * 2 > (pic_width_in_mbs * 2 + 1) / 2)
		cime_blk_w_max = (pic_width_in_mbs * 2 + 1) / 2 / 2 * 4;

	if (cime_blk_h_max / 4 > ALIGN(pic_height_in_mbs * 16, 64) / 128 * 4)
		cime_blk_h_max = ALIGN(pic_height_in_mbs * 16, 64) / 128 * 16;

	regmap_field_write(h264_ctx->l1_fields[CME_SRCH_H], cime_blk_w_max / 4);
	regmap_field_write(h264_ctx->l1_fields[CME_SRCH_V], cime_blk_h_max / 4);
	regmap_field_write(h264_ctx->l1_fields[RME_SRCH_H], 7);
	regmap_field_write(h264_ctx->l1_fields[RME_SRCH_V], 5);
	regmap_field_write(h264_ctx->l1_fields[DLT_FRM_NUM], 0);

	if (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) {
		regmap_field_write(h264_ctx->l1_fields[PMV_MDST_H], 0);
		regmap_field_write(h264_ctx->l1_fields[PMV_MDST_V], 0);
	} else {
		regmap_field_write(h264_ctx->l1_fields[PMV_MDST_H], 5);
		regmap_field_write(h264_ctx->l1_fields[PMV_MDST_V], 5);
	}
	regmap_field_write(h264_ctx->l1_fields[MV_LIMIT], 2);
	regmap_field_write(h264_ctx->l1_fields[PMV_NUM], 2);

	// VEPU540 specific
	regmap_field_read(h264_ctx->l1_fields[PIC_WD8_M1], &pic_wd8_m1);
	pic_temp = ((pic_wd8_m1 + 1) * 8 + 63) / 64 * 64;
	cime_linebuf_w = pic_temp / 64;

	regmap_field_write(h264_ctx->l1_fields[CME_LINEBUF_W], cime_linebuf_w);

	while ((w_temp > ((h_temp - h_val_0) * cime_linebuf_w * 4 + ((h_val_1 - h_temp) * 4 * 7)))
			&& (h_temp < 17))
		h_temp = h_temp + h_val_0;

	if (w_temp < ((h_temp - h_val_0) * cime_linebuf_w * 4 + ((h_val_1 - h_temp) * 4 * 7)))
		h_temp = h_temp - h_val_0;

	regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], h_temp);

	regmap_field_read(h264_ctx->l1_fields[CME_SRCH_H], &cme_srch_h);
	swin_scope_wd16 = (cme_srch_h + 3) / 4 * 2 + 1;

	regmap_field_read(h264_ctx->l1_fields[CME_SRCH_V], &cme_srch_v);
	temp0 = 2 * cme_srch_v + 1;

	regmap_field_read(h264_ctx->l1_fields[CME_RAMA_H], &cme_rama_h);
	if (temp0 > cme_rama_h)
		temp0 = cme_rama_h;

	temp1 = 0;

	pic_w = pic_width_in_mbs * 16;
	pic_wd64 = (pic_w + 63) / 64;

	if (pic_wd64 >= swin_scope_wd16)
		temp1 = swin_scope_wd16;
	else
		temp1 = pic_wd64 * 2;

	regmap_field_write(h264_ctx->l1_fields[CME_RAMA_MAX],
			   pic_wd64 * (temp0 - 1) + temp1);

	// TODO: VEPU541 (8K)
#if 0
	int swin_all_4_ver;
	int swin_all_16_hor;

	if (pic_w > 3584)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 8);
	else if (pic_w > 3136)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 9);
	else if (pic_w > 2816)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 10);
	else if (pic_w > 2560)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 11);
	else if (pic_w > 2368)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 12);
	else if (pic_w > 2176)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 13);
	else if (pic_w > 2048)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 14);
	else if (pic_w > 1856)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 15);
	else if (pic_w > 1792)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 16);
	else
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_H], 17);

	regmap_field_read(h264_ctx->l1_fields[CME_SRCH_V], &cme_srch_v);
	swin_all_4_ver = 2 * cme_srch_v + 1;

	regmap_field_read(h264_ctx->l1_fields[CME_SRCH_H], &cme_srch_h);
	swin_all_16_hor = (cme_srch_h * 4 + 15) / 16 * 2 + 1;

	regmap_field_read(h264_ctx->l1_fields[CME_RAMA_H]), &cme_rama_h);
	if (swin_all_4_ver < cme_rama_h)
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_MAX],
				   (swin_all_4_ver - 1) * pic_wd64 + swin_all_16_hor);
	else
		regmap_field_write(h264_ctx->l1_fields[CME_RAMA_MAX],
				   (cme_rama_h - 1) * pic_wd64 + swin_all_16_hor);
#endif
}

#define H264E_LAMBDA_TAB_SIZE       52

static unsigned int h264e_lambda_default[58] = {
	0x00000003, 0x00000005, 0x00000006, 0x00000007,
	0x00000009, 0x0000000b, 0x0000000e, 0x00000012,
	0x00000016, 0x0000001c, 0x00000024, 0x0000002d,
	0x00000039, 0x00000048, 0x0000005b, 0x00000073,
	0x00000091, 0x000000b6, 0x000000e6, 0x00000122,
	0x0000016d, 0x000001cc, 0x00000244, 0x000002db,
	0x00000399, 0x00000489, 0x000005b6, 0x00000733,
	0x00000912, 0x00000b6d, 0x00000e66, 0x00001224,
	0x000016db, 0x00001ccc, 0x00002449, 0x00002db7,
	0x00003999, 0x00004892, 0x00005b6f, 0x00007333,
	0x00009124, 0x0000b6de, 0x0000e666, 0x00012249,
	0x00016dbc, 0x0001cccc, 0x00024492, 0x0002db79,
	0x00039999, 0x00048924, 0x0005b6f2, 0x00073333,
	0x00091249, 0x000b6de5, 0x000e6666, 0x00122492,
	0x0016dbcb, 0x001ccccc,
};

static void setup_vepu541_l2(struct rkvpu_ctx *ctx, struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	const struct v4l2_ctrl_h264_encode_params *encode_params;
	int i;

	encode_params = run->encode_params;

	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY4_0], 1);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY4_1], 4);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY4_2], 9);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY4_3], 36);

	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDC8_0], 1);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDC8_1], 4);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDC8_2], 9);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDC8_3], 36);

	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY8_0], 1);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY8_1], 4);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY8_2], 9);
	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHDY8_3], 36);

	regmap_field_write(h264_ctx->l2_fields[IPRD_TTHD_UL], 0x0);

	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY8_0], 0x30);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY8_1], 0x3c);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY8_2], 0x28);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY8_3], 0x30);

	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY4_0], 0x30);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY4_1], 0x3c);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY4_2], 0x28);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY4_3], 0x30);

	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY16_0], 0x30);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY16_1], 0x3c);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY16_2], 0x28);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTY16_3], 0x30);

	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTC8_0], 0x24);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTC8_1], 0x2a);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTC8_2], 0x1c);
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGTC8_3], 0x20);

	/* 000556ab */
	regmap_field_write(h264_ctx->l2_fields[QNT_BIAS_P], 171);

	regmap_field_write(h264_ctx->l2_fields[ATR_THD0], 1);
	regmap_field_write(h264_ctx->l2_fields[ATR_THD1], 4);

	if (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I) {
		regmap_field_write(h264_ctx->l2_fields[QNT_BIAS_I], 683);
		regmap_field_write(h264_ctx->l2_fields[ATR_THD2], 36);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV16_WGT0], 16);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV16_WGT1], 16);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV16_WGT2], 16);

		regmap_field_write(h264_ctx->l2_fields[ATR_LV8_WGT0], 32);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV8_WGT1], 32);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV8_WGT2], 32);

		regmap_field_write(h264_ctx->l2_fields[ATR_LV4_WGT0], 20);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV4_WGT1], 18);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV4_WGT2], 16);
	} else {
		regmap_field_write(h264_ctx->l2_fields[QNT_BIAS_I], 583);
		regmap_field_write(h264_ctx->l2_fields[ATR_THD2], 81);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV16_WGT0], 28);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV16_WGT1], 27);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV16_WGT2], 23);

		regmap_field_write(h264_ctx->l2_fields[ATR_LV8_WGT0], 32);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV8_WGT1], 32);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV8_WGT2], 32);

		regmap_field_write(h264_ctx->l2_fields[ATR_LV4_WGT0], 28);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV4_WGT1], 27);
		regmap_field_write(h264_ctx->l2_fields[ATR_LV4_WGT2], 23);
	}

	regmap_field_write(h264_ctx->l2_fields[ATR_QP], 45);
	regmap_field_write(h264_ctx->l2_fields[ATF_TTHD0], 0);
	regmap_field_write(h264_ctx->l2_fields[ATF_TTHD1], 64);
	regmap_field_write(h264_ctx->l2_fields[ATF_TTHD2], 144);
	regmap_field_write(h264_ctx->l2_fields[ATF_TTHD3], 2500);

	regmap_field_write(h264_ctx->l2_fields[ATF_STHD_10], 80);
	regmap_field_write(h264_ctx->l2_fields[ATF_STHD_MAX], 280);

	regmap_field_write(h264_ctx->l2_fields[ATF_STHD_11], 144);
	regmap_field_write(h264_ctx->l2_fields[ATF_STHD_20], 192);

	regmap_field_write(h264_ctx->l2_fields[ATF_WGT10], 26);
	regmap_field_write(h264_ctx->l2_fields[ATF_WGT11], 24);

	regmap_field_write(h264_ctx->l2_fields[ATF_WGT12], 19);
	regmap_field_write(h264_ctx->l2_fields[ATF_WGT20], 22);

	regmap_field_write(h264_ctx->l2_fields[ATF_WGT21], 19);
	regmap_field_write(h264_ctx->l2_fields[ATF_WGT30], 19);

	regmap_field_write(h264_ctx->l2_fields[ATF_OFST10], 3500);
	regmap_field_write(h264_ctx->l2_fields[ATF_OFST11], 3500);

	regmap_field_write(h264_ctx->l2_fields[ATF_OFST12], 0);
	regmap_field_write(h264_ctx->l2_fields[ATF_OFST20], 3500);

	regmap_field_write(h264_ctx->l2_fields[ATF_OFST21], 1000);
	regmap_field_write(h264_ctx->l2_fields[ATF_OFST30], 0);

	// HEVC specific
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGT_QP0], 0);
	/* ~ */
	regmap_field_write(h264_ctx->l2_fields[IPRD_WGT_QP51], 0);

	regmap_bulk_write(h264_ctx->l2_regmap, RKVENC_VEPU540_L2_RDO_WGTA_QP(0),
			  &h264e_lambda_default[6], H264E_LAMBDA_TAB_SIZE);

	regmap_bulk_write(h264_ctx->l2_regmap, RKVENC_VEPU540_L2_RDO_WGTB_QP(0),
			  &h264e_lambda_default[5], H264E_LAMBDA_TAB_SIZE);

	regmap_field_write(h264_ctx->l2_fields[MADI_MODE], 0);

	regmap_bulk_write(h264_ctx->l2_regmap, RKVENC_VEPU540_L2_AQ_TTHD(0),
			  &h264_aq_tthd_default, 16);

	if (encode_params->slice_type == V4L2_H264_SLICE_TYPE_I)
		for (i = 0; i < ARRAY_SIZE(h264_P_aq_step_default); i++)
			regmap_field_write(h264_ctx->l2_fields[AQ_STEP0 + i],
				h264_I_aq_step_default[i] & 0x3f);
	else
		for (i = 0; i < ARRAY_SIZE(h264_P_aq_step_default); i++)
			regmap_field_write(h264_ctx->l2_fields[AQ_STEP0 + i],
				h264_P_aq_step_default[i] & 0x3f);

	regmap_field_write(h264_ctx->l2_fields[MVD_PNLT_E], 1);
	regmap_field_write(h264_ctx->l2_fields[MVD_PNLT_COEF], 1);
	regmap_field_write(h264_ctx->l2_fields[MVD_PNLT_CNST], 16000);
	regmap_field_write(h264_ctx->l2_fields[MVD_PNLT_LTHD], 0);
	regmap_field_write(h264_ctx->l2_fields[MVD_PNLT_HTHD], 0);

	regmap_field_write(h264_ctx->l2_fields[ATR1_THD0], 1);
	regmap_field_write(h264_ctx->l2_fields[ATR1_THD1], 4);
	regmap_field_write(h264_ctx->l2_fields[ATR1_THD2], 49);
}

static int h264e_vepu541_gen_regs(struct rkvpu_ctx *ctx, struct rkvenc_h264_run *run)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct vb2_v4l2_buffer *src_buf = run->base.bufs.src;
	const struct v4l2_format *f = &ctx->src_fmt;
	const struct v4l2_pix_format_mplane *src_fmt = &f->fmt.pix_mp;
	struct vb2_v4l2_buffer *dst_buf = run->base.bufs.dst;
	struct rkvpu_enc_buf *enc_buf;
	struct rkvpu_aux_buf *me_buf;

	enc_buf = rkvpu_get_enc_buf(dst_buf);
	me_buf = &enc_buf->me_buf;
	v4l2_info(&rkvpu->v4l2_dev, "frame %d generate regs now", src_buf->sequence);

	setup_vepu541_l2(ctx, run);

	setup_vepu541_normal(ctx);
	setup_vepu541_prep(ctx);
	setup_vepu541_codec(ctx, run);
	setup_vepu541_rdo_pred(ctx, run);
	setup_vepu541_rc_base(ctx, run);
	setup_vepu541_io_buf(ctx, run);

	setup_vepu541_recn_refr(ctx, run);

	regmap_field_write(h264_ctx->l1_fields[MEIW_ADDR], me_buf->dma);
	setup_vepu541_roi(ctx, run);

#if 0
	regmap_field_write(h264_ctx->l1_fields[PIC_OFST_Y],
			   mpp_frame_get_offset_y(task->frame));
	regmap_field_write(h264_ctx->l1_fields[PIC_OFST_X],
			   mpp_frame_get_offset_x(task->frame));

#endif
	setup_vepu541_split(ctx);

	/* only limitation on vepu540 */
	if (src_fmt->width > 1920)
		setup_vepu540_force_slice_split(ctx, src_fmt->width);

	setup_vepu541_me(ctx, run /*, TODO: ctx->is_vepu540 */);

	//FIXME
//	vepu540_set_osd(&ctx->osd_cfg);
	//TODO: vepu541_set_osd(&ctx->osd_cfg);

	return 0;
}

static int rkvenc_l2_read(void *context, unsigned int reg,
				      unsigned int *val)
{
	struct rkvpu_ctx *ctx = context;
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	int ret = 0;

	ret = regmap_write(h264_ctx->l1_regmap,
			   RKVENC_VEPU540_L1_L2CFG_ADDR, reg);
	if (ret < 0)
		return ret;

	ret = regmap_read(h264_ctx->l1_regmap,
			  RKVENC_VEPU540_L1_L2CFG_RDATA, val);
	if (ret < 0)
		return ret;

	return ret;
}

static int rkvenc_l2_write(void *context, unsigned int reg,
				     unsigned int val)
{
	struct rkvpu_ctx *ctx = context;
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	int ret = 0;

	ret = regmap_write(h264_ctx->l1_regmap,
			   RKVENC_VEPU540_L1_L2CFG_ADDR, reg);
	if (ret < 0)
		return ret;

	ret = regmap_write(h264_ctx->l1_regmap,
			   RKVENC_VEPU540_L1_L2CFG_WDATA, val);
	if (ret < 0)
		return ret;

	return ret;
}

static const struct regmap_bus regmap_l2_bus = {
	.reg_read = rkvenc_l2_read,
	.reg_write = rkvenc_l2_write,
	.max_raw_read = 4,
	.max_raw_write = 4,
	.reg_format_endian_default = REGMAP_ENDIAN_NATIVE,
	.val_format_endian_default = REGMAP_ENDIAN_NATIVE,
};

static int rkvenc_h264_init_regmap(struct rkvpu_ctx *ctx)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct device *dev = rkvpu->dev;
	int ret;

	h264_ctx->l1_regmap = devm_regmap_init_mmio(dev, rkvpu->regs,
					      &rkvenc_l1_regmap_cfg);
	if (IS_ERR(h264_ctx->l1_regmap))
		return dev_err_probe(dev, PTR_ERR(h264_ctx->l1_regmap),
				     "failed to initialize regmap\n");

	ret = devm_regmap_field_bulk_alloc(dev, h264_ctx->l1_regmap,
					   h264_ctx->l1_fields,
					   rkvenc_vepu540_l1_fields,
					   L1_MAX_FIELDS);

	h264_ctx->l2_regmap = devm_regmap_init(dev, &regmap_l2_bus, ctx,
					       &rkvenc_l2_regmap_cfg);
	if (IS_ERR(h264_ctx->l2_regmap))
		return dev_err_probe(dev, PTR_ERR(h264_ctx->l2_regmap),
				     "failed to initialize regmap\n");

	ret = devm_regmap_field_bulk_alloc(dev, h264_ctx->l2_regmap,
					   h264_ctx->l2_fields,
					   rkvenc_vepu540_l2_fields,
					   L2_MAX_FIELDS);

	return ret;
}

static int rkvenc_h264_start(struct rkvpu_ctx *ctx)
{
	struct rkvenc_h264_ctx *h264_ctx;
	int ret;

	h264_ctx = kzalloc(sizeof(*h264_ctx), GFP_KERNEL);
	if (!h264_ctx)
		return -ENOMEM;

	ctx->priv = h264_ctx;

	ret = rkvenc_h264_init_regmap(ctx);
	if (ret)
		return ret;

	return 0;
}

static void rkvenc_h264_stop(struct rkvpu_ctx *ctx)
{
}

static void rkvenc_h264_run_preamble(struct rkvpu_ctx *ctx,
				     struct rkvenc_h264_run *run)
{
	struct v4l2_ctrl *ctrl;

	ctrl = v4l2_ctrl_find(&ctx->ctrl_hdl,
			      V4L2_CID_STATELESS_H264_ENCODE_PARAMS);
	run->encode_params = ctrl ? ctrl->p_cur.p : NULL;
	ctrl = v4l2_ctrl_find(&ctx->ctrl_hdl,
			      V4L2_CID_STATELESS_H264_ENCODE_RC);
	run->encode_rc = ctrl ? ctrl->p_cur.p : NULL;
	ctrl = v4l2_ctrl_find(&ctx->ctrl_hdl,
			      V4L2_CID_STATELESS_H264_SPS);
	run->sps = ctrl ? ctrl->p_cur.p : NULL;

	ctrl = v4l2_ctrl_find(&ctx->ctrl_hdl,
			      V4L2_CID_STATELESS_H264_PPS);
	run->pps = ctrl ? ctrl->p_cur.p : NULL;

	rkvpu_run_preamble(ctx, &run->base);
}

static int rkvenc_h264_run(struct rkvpu_ctx *ctx)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct rkvenc_h264_run run;

	rkvenc_h264_run_preamble(ctx, &run);

	h264e_vepu541_gen_regs(ctx, &run);

	rkvpu_run_postamble(ctx, &run.base);

	schedule_delayed_work(&rkvpu->watchdog_work, msecs_to_jiffies(2000));

	/* Start encoding! */
	regmap_field_write(h264_ctx->l1_fields[LKT_NUM], 0);
	regmap_field_write(h264_ctx->l1_fields[CLK_GATE_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[RESETN_HW_EN], 0);
	regmap_field_write(h264_ctx->l1_fields[ENC_DONE_TMVP_EN], 1);
	regmap_field_write(h264_ctx->l1_fields[RKVENC_CMD], 1);

	return 0;
}

static int rkvenc_h264_irq(struct rkvpu_ctx *ctx)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	enum vb2_buffer_state state;
	u32 status;
	int ret;

	ret = regmap_read(h264_ctx->l1_regmap,
			  RKVENC_VEPU540_L1_INT_STA, &status);
        if (!status || ret)
                return IRQ_NONE;

	state = (status & BIT(0)) ?
		VB2_BUF_STATE_DONE : VB2_BUF_STATE_ERROR;

	ret = regmap_write(h264_ctx->l1_regmap,
			   RKVENC_VEPU540_L1_INT_MSK, 0x100);
        if (ret)
                return IRQ_NONE;

	ret = regmap_write(h264_ctx->l1_regmap,
			   RKVENC_VEPU540_L1_INT_CLR, 0xffffffff);
        if (ret)
                return IRQ_NONE;

	ret = regmap_write(h264_ctx->l1_regmap,
			   RKVENC_VEPU540_L1_INT_STA, 0);
        if (ret)
                return IRQ_NONE;

	return state;
}

static void rkvdec_h264_done(struct rkvpu_ctx *ctx,
			    struct vb2_v4l2_buffer *src_buf,
			    struct vb2_v4l2_buffer *dst_buf,
			    enum vb2_buffer_state result)
{
	struct rkvenc_h264_ctx *h264_ctx = ctx->priv;
	int bsl;

	regmap_field_read(h264_ctx->l1_fields[BS_LGTH], &bsl);
	vb2_set_plane_payload(&dst_buf->vb2_buf, 0, bsl);

	return;
}

#if 0
static int rkvenc_h264_watchdog(struct rkvpu_ctx *ctx)
{
	struct rkvpu_dev *rkvpu = ctx->dev;

	/* TODO: Do all necessary things */

	return 0;
}

static int rkvenc_h264_try_ctrl(struct rkvpu_ctx *ctx, struct v4l2_ctrl *ctrl)
{
	if (ctrl->id == V4L2_CID_STATELESS_H264_SPS)
		return rkvdec_h264_validate_sps(ctx, ctrl->p_new.p_h264_sps);

	return 0;
}
#endif

const struct rkvpu_ops rkvenc_h264_fmt_ops = {
//	.adjust_fmt = rkvenc_h264_adjust_fmt,
	.start = rkvenc_h264_start,
	.stop = rkvenc_h264_stop,
	.run = rkvenc_h264_run,
	.irq = rkvenc_h264_irq,
	.done = rkvdec_h264_done,
#if 0
	.watchdog = rkvenc_h264_watchdog,
	.try_ctrl = rkvenc_h264_try_ctrl,
#endif
};
