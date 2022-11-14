/* SPDX-License-Identifier: GPL-2.0 */

#ifndef RKVENC_VEPU540_REGS_H_
#define RKVENC_VEPU540_REGS_H_

#include <linux/regmap.h>

/* L1 */
#define RKVENC_VEPU540_L1_VERSION		0x0
#define RKVENC_VEPU540_L1_STRT		0x4
#define RKVENC_VEPU540_L1_CLR		0x8
#define RKVENC_VEPU540_L1_LKT_ADDR		0xc
#define RKVENC_VEPU540_L1_INT_EN		0x10
#define RKVENC_VEPU540_L1_INT_MSK		0x14
#define RKVENC_VEPU540_L1_INT_CLR		0x18
#define RKVENC_VEPU540_L1_INT_STA		0x1c
#define RKVENC_VEPU540_L1_ENC_IDLE_EN	0x20
#define RKVENC_VEPU540_L1_ENC_RSL		0x30
#define RKVENC_VEPU540_L1_ENC_PIC		0x34
#define RKVENC_VEPU540_L1_ENC_WDG		0x38
#define RKVENC_VEPU540_L1_DTRNS_MAP	0x3c
#define RKVENC_VEPU540_L1_DTRNS_CFG	0x40
#define RKVENC_VEPU540_L1_SRC_FMT		0x44
#define RKVENC_VEPU540_L1_SRC_UDFY		0x48
#define RKVENC_VEPU540_L1_SRC_UDFU		0x4c
#define RKVENC_VEPU540_L1_SRC_UDFV		0x50
#define RKVENC_VEPU540_L1_SRC_UDFO		0x54
#define RKVENC_VEPU540_L1_SRC_PROC		0x58
#define RKVENC_VEPU540_L1_SLI_CFG_H264	0x5c
#define RKVENC_VEPU540_L1_TILE_CFG_HEVC	0x5c
#define RKVENC_VEPU540_L1_TILE_POS		0x60
#define RKVENC_VEPU540_L1_KLUT_OFST	0x64
#define RKVENC_VEPU540_L1_KLUT_WGT(x)	(0x68 + x * 0x4) // -> 0 - 23
#define RKVENC_VEPU540_L1_RC_CFG		0xc8
#define RKVENC_VEPU540_L1_RC_QP		0xcc
#define RKVENC_VEPU540_L1_RC_TGT		0xd0
#define RKVENC_VEPU540_L1_RC_ADJ(x)	(0xd4 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L1_RC_DTHD(x)	(0xdc + x * 0x4) // -> 0 - 8
#define RKVENC_VEPU540_L1_ROI_QTHD(x)	(0x100 + x * 0x4) // -> 0 - 3
#define RKVENC_VEPU540_L1_PIC_OFST		0x110
#define RKVENC_VEPU540_L1_SRC_STRD		0x114
#define RKVENC_VEPU540_L1_ADR_SRC0		0x118
#define RKVENC_VEPU540_L1_ADR_SRC1		0x11c
#define RKVENC_VEPU540_L1_ADR_SRC2		0x120
#define RKVENC_VEPU540_L1_ADR_ROI		0x124
#define RKVENC_VEPU540_L1_ADR_RFPW_H	0x128
#define RKVENC_VEPU540_L1_ADR_RFPW_B	0x12c
#define RKVENC_VEPU540_L1_ADR_RFPR_H	0x130
#define RKVENC_VEPU540_L1_ADR_RFPR_B	0x134
#define RKVENC_VEPU540_L1_ADR_CMVW		0x138
#define RKVENC_VEPU540_L1_ADR_CMVR		0x13c
#define RKVENC_VEPU540_L1_ADR_DSPW		0x140
#define RKVENC_VEPU540_L1_ADR_DSPR		0x144
#define RKVENC_VEPU540_L1_ADR_MEIW		0x148
#define RKVENC_VEPU540_L1_ADR_BSBT		0x14c
#define RKVENC_VEPU540_L1_ADR_BSBB		0x150
#define RKVENC_VEPU540_L1_ADR_BSBR		0x154
#define RKVENC_VEPU540_L1_ADR_BSBS		0x158
#define RKVENC_VEPU540_L1_SLI_SPLT		0x15c
#define RKVENC_VEPU540_L1_SLI_BYTE		0x160
#define RKVENC_VEPU540_L1_MR_RNGE		0x164
#define RKVENC_VEPU540_L1_ME_CFG		0x168
#define RKVENC_VEPU540_L1_ME_CACH		0x16c
#define RKVENC_VEPU540_L1_SYNT_LONG_REFM0	0x170
#define RKVENC_VEPU540_L1_SYNT_LONG_REFM1	0x174
#define RKVENC_VEPU540_L1_OSD_INV_CFG	0x178
#define RKVENC_VEPU540_L1_ADR_LPFW		0x17c
#define RKVENC_VEPU540_L1_ADR_LPFR		0x180
#define RKVENC_VEPU540_L1_IPRD_CSTS	0x194
#define RKVENC_VEPU540_L1_RDO_CFG_H264	0x198
#define RKVENC_VEPU540_L1_RDO_CFG_HEVC	0x198
#define RKVENC_VEPU540_L1_SYNT_NAL		0x19c
#define RKVENC_VEPU540_L1_SYNT_SPS		0x1a0
#define RKVENC_VEPU540_L1_SYNT_PPS		0x1a4
#define RKVENC_VEPU540_L1_SYNT_SLI(x)	(0x1a8 + x * 0x4) // -> 0 - 2
#define RKVENC_VEPU540_L1_SYNT_REFM0	0x1b4
#define RKVENC_VEPU540_L1_SYNT_REFM1	0x1b8
#define RKVENC_VEPU540_L1_OSD_CFG		0x1c0
#define RKVENC_VEPU540_L1_OSD_INV		0x1c4
#define RKVENC_VEPU540_L1_SYNT_REFM2	0x1c8
#define RKVENC_VEPU540_L1_SYNT_REFM3_HEVC	0x1cc
#define RKVENC_VEPU540_L1_OSD_POS(x)	0x1d0
#define RKVENC_VEPU540_L1_ADR_OSD(x)	(0x1f0 + x * 0x4) // -> 0 - 7
#define RKVENC_VEPU540_L1_ST_BSL		0x210
#define RKVENC_VEPU540_L1_ST_SSE_LE32	0x214
#define RKVENC_VEPU540_L1_ST_SSE_QP	0x218
#define RKVENC_VEPU540_L1_ST_SAO		0x21c
#define RKVENC_VEPU540_L1_ST_HEAD_BL	0x220
#define RKVENC_VEPU540_L1_ST_RES_BL	0x224
#define RKVENC_VEPU540_L1_ST_ENC		0x228
#define RKVENC_VEPU540_L1_ST_LKT		0x22c
#define RKVENC_VEPU540_L1_ST_NADR		0x230
#define RKVENC_VEPU540_L1_ST_BSB		0x234
#define RKVENC_VEPU540_L1_ST_BUS		0x238
#define RKVENC_VEPU540_L1_ST_SNUM		0x23c
#define RKVENC_VEPU540_L1_ST_SLEN		0x240
#define RKVENC_VEPU540_L1_ST_PNUM_P64	0x244
#define RKVENC_VEPU540_L1_ST_PNUM_P32	0x248
#define RKVENC_VEPU540_L1_ST_PNUM_P16	0x24c
#define RKVENC_VEPU540_L1_ST_PNUM_P8	0x250
#define RKVENC_VEPU540_L1_ST_PNUM_I32	0x254
#define RKVENC_VEPU540_L1_ST_PNUM_I16	0x258
#define RKVENC_VEPU540_L1_ST_PNUM_I8	0x25c
#define RKVENC_VEPU540_L1_ST_PNUM_I4	0x260
#define RKVENC_VEPU540_L1_ST_BB_QP(x)	(0x264 + x * 0x4) // -> 0 - 51
#define RKVENC_VEPU540_L1_ST_CPLX_TMP	0x334
#define RKVENC_VEPU540_L1_ST_BNUM_CME	0x338
#define RKVENC_VEPU540_L1_ST_CPLX_SPT	0x33c
#define RKVENC_VEPU540_L1_ST_BNUM_B16	0x340
#define RKVENC_VEPU540_L1_ST_CPLX_MAX_B16	0x344
#define RKVENC_VEPU540_L1_L2CFG_ADDR	0x3f0
#define RKVENC_VEPU540_L1_L2CFG_WDATA	0x3f4
#define RKVENC_VEPU540_L1_L2CFG_RDATA	0x3f8
#define RKVENC_VEPU540_L1_OSD_PLT(x)	(0x400 + x * 0x4) // -> 0 - 255
#define RKVENC_VEPU540_L1_ST_WDG		0x85c
#define RKVENC_VEPU540_L1_ST_PPL		0x860
#define RKVENC_VEPU540_L1_ST_SLI_NUM	0x874
#define RKVENC_VEPU540_L1_DBG_DMA_RFPR	0x8e4
#define RKVENC_VEPU540_L1_DBG_DMA_CH_ST	0x8e8
#define RKVENC_VEPU540_L1_MMU_ADDR(x)	(0xf00 + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_ST(x)	(0xf04 + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_CMD(x)	(0xf08 + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_PFA(x)	(0xf0c + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_ZAP(x)	(0xf10 + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_ERR(x)	(0xf14 + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_INT_CLR(x)	(0xf18 + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_INT_MSK(x)	(0xf1c + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_INT_STA(x)	(0xf20 + x * 0x40) // -> 0 - 1
#define RKVENC_VEPU540_L1_MMU_ACKG(x)	(0xf24 + x * 0x40) // -> 0 - 1

enum rkvenc_vepu540_regfields_l1 {
	/*
	 * VERSION
	 * Address: 0x0000 Access type: read only
	 * VEPU version. It contains IP function summary and sub-version informations.
	 */
	/* Sub-version(version 1.1) */
	SUB_VER,
	/* Support H.264 encoding */
	H264_ENC,
	/* Support HEVC encoding */
	H265_ENC,
	/*
	 * The maximum resolution supported
	 * 4'd0: 4096x2304 pixels,
	 * 4'd1: 1920x1088 pixels,
	 * others: reserved
	 */
	PIC_SIZE,
	/*
	 * OSD capability.
	 * 2'd0: 8-area OSD, with 256-color palette
	 * 2'd3: no OSD
	 * others: reserved
	 */
	OSD_CAP,
	/*
	 * pre-process filter capability
	 * 2'd0: basic pre-process filter
	 * 2'd3: no pre-process filter
	 * others: reserved
	 */
	FILTR_CAP,
	/* B frame encoding capability */
	BFRM_CAP,
	/* frame buffer compress capability */
	/* 2'd0: No FBC,
	 * 2'd3: Support AFBC for video source and FBC
	 *       for reconstructured picture
	 * others: reserved
	 */
	FBC_CAP,
	/* IP indentifier for RKVENC default: 0x50 */
	RKVENC_VER,
	/*
	 * ENC_STRT
	 * Address: 0x0004 Access type: read and write/write only
	 * Start cmd register.(auto clock gating enable, auto reset enable and
	 * tmvp adjust enable when frame done are also allocated here.)
	 */
	/*
	 * Number of new nodes in link table.
	 * It's valid only when rkvenc_cmd is 2 or 3.
	 */
	LKT_NUM,
	/*
	 * Rockchip video encoder command:
	 * 2'd0: N/A
	 * 2'd1: one frame encode by register configuration
	 * 2'd2: multi-frame encode start with link table
	 * 2'd3: multi_frame_encode link table update
	 */
	RKVENC_CMD,
	/* RKVENC encoder clock gating enable */
	CLK_GATE_EN,
	/* auto reset core clock domain when frame finished */
	RESETN_HW_EN,
	/* wait tmvp write done by dma */
	ENC_DONE_TMVP_EN,
	/*
	 * ENC_CLR
	 * Address offset: 0x0008 Access type: read and write
	 * ENC_CLR.safe_clr only clears RKVENC DMA and confirms the integrity of
	 * AXI transactions. To execute the global reset of RKVENC, user needs to
	 * configure SOC CRU register which controls RKVENC's asynchronous reset
	 */
	/*
	 * Safe clear. This filed only clears DMA module to confirm the
	 * integrity of AXI transactions
	 */
	SAFE_CLR,
	/*
	 * Force clear. Clear all the sub modules besides regfile and AHB data
	 * path.
	 */
	FORCE_CLR,

	/*
	 * LKT_ADDR
	 * Address offset: 0x000c Access type: read and write
	 * Link table
	 */
	/*
	 * High 28 bits of the address for the first node in current link table
	 * (16bytes aligned)
	 */
	LKT_ADDR,
	/*
	 * INT_EN
	 * Address offset: 0x0010 Access type: read and write
	 * VEPU interrupt enable
	 */
	/* One frame encode finish interrupt enable */
	ENC_DONE_EN,
	/* Link table finish interrupt enable */
	LKT_DONE_EN,
	/* Safe clear finish interrupt enable */
	SCLR_DONE_EN,
	/* Safe clear finish interrupt enable */
	ENC_SLICE_DONE_EN,
	/* Bit stream overflow interrupt enable */
	OFLW_DONE_EN,
	/* AXI write response fifo full interrupt enable */
	BRSP_DONE_EN,
	/* AXI write response channel error interrupt enable */
	BERR_DONE_EN,
	/* AXI read channel error interrupt enable */
	RERR_DONE_EN,
	/* timeout error interrupt enable */
	WDG_DONE_EN,

	/*
	 * INT_MSK
	 * Address offset: 0x0014 Access type: read and write
	 * VEPU interrupt mask
	 */
	/* One frame encode finish interrupt mask */
	ENC_DONE_MSK,
	/* Link table finish interrupt mask */
	LKT_DONE_MSK,
	/* Safe clear finish interrupt mask */
	SCLR_DONE_MSK,
	/* Safe clear finish interrupt mask */
	ENC_SLICE_DONE_MSK,
	/* Bit stream overflow interrupt mask */
	OFLW_DONE_MSK,
	/* AXI write response fifo full interrupt mask */
	BRSP_DONE_MSK,
	/* AXI write response channel error interrupt mask */
	BERR_DONE_MSK,
	/* AXI read channel error interrupt mask */
	RERR_DONE_MSK,
	/* timeout error interrupt mask */
	WDG_DONE_MSK,

	/*
	 * INT_CLR
	 * Address offset: 0x0018 Access type: read and write, write one to clear
	 * VEPU interrupt clear
	 */
	/* One frame encode finish interrupt clear */
	ENC_DONE_CLR,
	/* Link table finish interrupt clear */
	LKT_DONE_CLR,
	/* Safe clear finish interrupt clear */
	SCLR_DONE_CLR,
	/* One slice encode finish interrupt clear */
	ENC_SLICE_DONE_CLR,
	/* Bit stream overflow interrupt clear */
	OFLW_DONE_CLR,
	/* AXI write response fifo full interrupt clear */
	BRSP_DONE_CLR,
	/* AXI write response channel error interrupt clear */
	BERR_DONE_CLR,
	/* AXI read channel error interrupt clear */
	RERR_DONE_CLR,
	/* timeout error interrupt clear */
	WDG_DONE_CLR,

	/*
	 * INT_STA
	 * Address offset: 0x001c Access type: read and write, write one to clear
	 * VEPU interrupt status
	 */
	/* One frame encode finish interrupt status */
	ENC_DONE_STA,
	/* Link table finish interrupt status */
	LKT_DONE_STA,
	/* Safe clear finish interrupt status */
	SCLR_DONE_STA,
	/* One slice encode finish interrupt status */
	ENC_SLICE_DONE_STA,
	/* Bit stream overflow interrupt status */
	OFLW_DONE_STA,
	/* AXI write response fifo full interrupt status */
	BRSP_DONE_STA,
	/* AXI write response channel error interrupt status */
	BERR_DONE_STA,
	/* AXI read channel error interrupt status */
	RERR_DONE_STA,
	/* timeout error interrupt status */
	WDG_DONE_STA,

	/* reg gap 008~011 */
	//reg_008_011[4],

	/*
	 * ENC_RSL
	 * Address offset: 0x0030 Access type: read and write
	 * Resolution
	 */
	/* ceil(picture width/8) - 1 */
	PIC_WD8_M1,
	/* filling pixels to maintain picture width 8 pixels aligned */
	PIC_WFILL,
	/* Ceil(picture_height/8)-1 */
	PIC_HD8_M1,
	/* Filling pixels to maintain picture height 8 pixels aligned */
	PIC_HFILL,

	/*
	 * ENC_PIC
	 * Address offset: 0x0034 Access type: read and write
	 * VEPU common configuration
	 */
	/* Video standard: 0->H.264 */
	ENC_STND,
	/* ROI encode enable */
	ROI_ENC,
	/* Current frame should be refered in future */
	CUR_FRM_REF,
	/* Output ME information */
	MEI_STOR,
	/* Output start code prefix */
	BS_SCP,
	/* 0: select table A, 1: select table B */
	LAMB_MOD_SEL,
	/* QP value for current frame encoding */
	PIC_QP,
	/* sum of reference pictures (indexed by difference POCs), HEVC only */
	TOT_POC_NUM,
	/* bit width to express the maximum ctu number in current picure, HEVC only */
	LOG2_CTU_NUM,
	/* 1'h0: Select atr_thd group 1'h1: Select atr_thd group1 */
	ATR_THD_SEL,
	/* Dual-core handshake Rx ID. */
	DCHS_RXID,
	/* Dual-core handshake tx ID. */
	DCHS_TXID,
	/* Dual-core handshake rx enable. */
	DCHS_RXE,
	/* RDO intra-prediction satd path bypass enable. */
	SATD_BYPS_EN,
	/* Slice length fifo enable. */
	SLEN_FIFO,
	/* Node interrupt enable (only for link table node configuration). */
	NODE_INT,

	/*,
	 * ENC_WDG
	 * Address offset: 0x0038 Access type: read and write
	 * VEPU watch dog configure register
	 */
	/*
	 * Video source loading timeout threshold.
	 * 24'h0: No time limit
	 * 24'hx: x*256 core clock cycles
	 */
	VS_LOAD_THD,
	/*
	 * Reference picture loading timeout threshold.
	 * 8'h0: No time limit
	 * 8'hx: x*256 core clock cycles
	 */
	RFP_LOAD_THRD,

	/*
	 * DTRNS_MAP
	 * Address offset: 0x003c Access type: read and write
	 * Data transaction mapping (endian and order)
	 */
	/* swap the position of 64bits in 128bits for lpf write data between tiles */
	LPFW_BUS_ORDR,
	/* Swap the position of 64 bits in 128 bits for co-located Mv(HEVC only). */
	CMVW_BUS_ORDR,
	/* Swap the position of 64 bits in 128 bits for down-sampled picture. */
	DSPW_BUS_ORDR,
	/* Swap the position of 64 bits in 128 bits for reference picture. */
	RFPW_BUS_ORDR,
	/*
	 * Data swap for video source loading channel.
	 * [3]: Swap 64 bits in 128 bits
	 * [2]: Swap 32 bits in 64 bits
	 * [1]: Swap 16 bits in 32 bits
	 * [0]: Swap 8 bits in 16 bits
	 */
	SRC_BUS_EDIN,
	/*
	 * Data swap for ME information write channel.
	 * [3]: Swap 64 bits in 128 bits
	 * [2]: Swap 32 bits in 64 bits
	 * [1]: Swap 16 bits in 32 bits
	 * [0]: Swap 8 bits in 16 bits
	 */
	MEIW_BUS_EDIN,
	/*
	 * Data swap for bis stream write channel.
	 * [2]: Swap 32 bits in 64 bits
	 * [1]: Swap 16 bits in 32 bits
	 * [0]: Swap 8 bits in 16 bits
	 */
	BSW_BUS_EDIN,
	/*
	 * Data swap for link table read channel.
	 * [3]: Swap 64 bits in 128 bits
	 * [2]: Swap 32 bits in 64 bits
	 * [1]: Swap 16 bits in 32 bits
	 * [0]: Swap 8 bits in 16 bits
	 */
	LKTR_BUS_EDIN,
	/*
	 * Data swap for ROI configuration read channel.
	 * [3]: Swap 64 bits in 128 bits
	 * [2]: Swap 32 bits in 64 bits
	 * [1]: Swap 16 bits in 32 bits
	 * [0]: Swap 8 bits in 16 bits
	 */
	ROIR_BUS_EDIN,
	/*
	 * Data swap for link table write channel.
	 * [3]: Swap 64 bits in 128 bits
	 * [2]: Swap 32 bits in 64 bits
	 * [1]: Swap 16 bits in 32 bits
	 * [0]: Swap 8 bits in 16 bits
	 */
	LKTW_BUS_EDIN,
	/*
	 * AFBC video source loading burst size.
	 * 1'h0: 32 bytes
	 * 1'h1: 64 bytes
	 */
	AFBC_BSIZE,

	/*,
	 * DTRNS_CFG
	 * Address offset: 0x0040 Access type: read and write
	 * (AXI bus) Data transaction configuration
	 */
	/*
	 * AXI write response channel check enable.
	 * [6]: Reconstructed picture write response check enable.
	 * [5]: ME information write response check enable.
	 * [4]: CTU information write response check enable.
	 * [3]: Down-sampled picture write response check enable.
	 * [2]: Bit stream write response check enable.
	 * [1]: Link table mode write reponse check enable.
	 * [0]: Reserved for video preprocess.
	 */
	VPU541_AXI_BRSP_CKE,
	/*
	 * Down sampled reference picture read outstanding enable.
	 * 1'h0: No outstanding
	 * 1'h1: Outstanding read, which improves data transaction efficiency,
	 * but core clock frequency should not lower than bus clock frequency.
	 */
	VPU541_DSPR_OTSD,

	/*
	 * Down sampled reference picture read outstanding enable.
	 * 1'h0: No outstanding
	 * 1'h1: Outstanding read, which improves data transaction efficiency,
	 * but core clock frequency should not lower than bus clock frequency.
	 */
	VPU540_DSPR_OTSD,
	/*
	 * AXI write response channel check enable.
	 * [7]: lpf write response check enable
	 * [6]: Reconstructed picture write response check enable.
	 * [5]: ME information write response check enable.
	 * [4]: CTU information write response check enable.
	 * [3]: Down-sampled picture write response check enable.
	 * [2]: Bit stream write response check enable.
	 * [1]: Link table mode write reponse check enable.
	 * [0]: Reserved for video preprocess.
	 */
	VPU540_AXI_BRSP_CKE,

	/*
	 * SRC_FMT
	 * Address offset: 0x0044 Access type: read and write
	 * Video source format
	 */
	/*
	 * Swap the position of alpha and RGB for ARBG8888.
	 * 1'h0: BGRA8888 or RGBA8888.
	 * 1'h1: ABGR8888 or ARGB8888.
	 */
	ALPHA_SWAP,
	/*
	 * Swap the position of R and B for BGRA8888, RGB888, RGB 656 format,
	 * Swap the position of U and V for YUV422-SP, YUV420-SP, YUYV422 and UYUV422 format.
	 * 1'h0: RGB or YUYV or UYVY.
	 * 1'h1: BGR or YVYU or VYUY.
	 */
	RBUV_SWAP,
	/*
	 * Video source color format.
	 * 4'h0: BGRA8888
	 * 4'h1: RGB888
	 * 4'h2: RGB565
	 * 4'h4: YUV422 SP
	 * 4'h5: YUV422 P
	 * 4'h6: YUV420 SP
	 * 4'h7: YUV420 P
	 * 4'h8: YUYV422
	 * 4'h9: UYVY422
	 * Others: Reserved
	 */
	SRC_CFMT,
	/*
	 * Video source clip (low active).
	 * 1'h0: [16:235] for luma and [16:240] for chroma.
	 * 1'h1: [0:255] for both luma and chroma.
	 */
	SRC_RANGE,
	/*
	 * Ourput reconstructed frame format
	 * 1'h0: yuv420
	 * 1'h1: yuv400
	 */
	OUT_FMT_CFG,

	/*
	 * SRC_UDFY
	 * Address offset: 0x0048 Access type: read and write
	 * Weight of user defined formula for RBG to Y conversion
	 */
	/* Weight of BLUE  in RBG to Y conversion formula. */
	CSC_WGT_B2Y,
	/* Weight of GREEN in RBG to Y conversion formula. */
	CSC_WGT_G2Y,
	/* Weight of RED   in RBG to Y conversion formula. */
	CSC_WGT_R2Y,

	/*
	 * SRC_UDFU
	 * Address offset: 0x004c Access type: read and write
	 * Weight of user defined formula for RBG to U conversion
	 */
	/* Weight of BLUE  in RBG to U conversion formula. */
	CSC_WGT_B2U,
	/* Weight of GREEN in RBG to U conversion formula. */
	CSC_WGT_G2U,
	/* Weight of RED   in RBG to U conversion formula. */
	CSC_WGT_R2U,

	/*
	 * SRC_UDFV
	 * Address offset: 0x0050 Access type: read and write
	 * Weight of user defined formula for RBG to V conversion
	 */
	/* Weight of BLUE  in RBG to V conversion formula. */
	CSC_WGT_B2V,
	/* Weight of GREEN in RBG to V conversion formula. */
	CSC_WGT_G2V,
	/* Weight of RED   in RBG to V conversion formula. */
	CSC_WGT_R2V,

	/*
	 * SRC_UDFO
	 * Address offset: 0x0054 Access type: read and write
	 * Offset of user defined formula for RBG to YUV conversion
	 */
	/* Offset of RBG to V conversion formula. */
	CSC_OFST_V,
	/* Offset of RBG to U conversion formula. */
	CSC_OFST_U,
	/* Offset of RBG to Y conversion formula. */
	CSC_OFST_Y,

	/*
	 * SRC_PROC
	 * Address offset: 0x0058 Access type: read and write
	 * Video source process
	 */
	/* Video source mirror mode enable. */
	SRC_MIRR,
	/*
	 * Video source rotation mode.
	 * 2'h0: 0 degree
	 * 2'h1: Clockwise 90 degree
	 * 2'h2: Clockwise 180 degree
	 * 2'h3: Clockwise 280 degree
	 */
	SRC_ROT,
	/* Video source texture analysis enable. */
	TXA_EN,
	/* AFBC decompress enable (for AFBC format video source). */
	AFBCD_EN,

	/*
	 * SLI_CFG_H264
	 * Address offset: 0x005C Access type: read and write
	 * Slice cross lines configuration, h264 only.
	 */
	/*
	 * Slice cut cross lines enable,
	 * using for breaking the resolution limit, h264 only.
	 */
	SLI_CRS_EN,

	/* reg gap 024 */
	REG_024,

	/*
	 * KLUT_OFST
	 * Address offset: 0x0064 Access type: read and write
	 * Offset of (RDO) chroma cost weight table
	 */
	/* Offset of (RDO) chroma cost weight table, values from 0 to 6. */
	CHRM_KLUT_OFST,

	/*
	 * KLUT_WGT0
	 * Address offset: 0x0068 Access type: read and write
	 * (RDO) Chroma weight table configure register0
	 */
	/* Data0 in chroma cost weight table. */
	CHRM_KLUT_WGT0,
	/* Low 9 bits of data1 in chroma cost weight table. */
	CHRM_KLUT_WGT1_L9,

	/*
	 * KLUT_WGT1
	 * Address offset: 0x006C Access type: read and write
	 * (RDO) Chroma weight table configure register1
	 */
	/* High 9 bits of data1 in chroma cost weight table. */
	CHRM_KLUT_WGT1_H9,
	/* Data2 in chroma cost weight table. */
	CHRM_KLUT_WGT2,

	/*
	 * KLUT_WGT2
	 * Address offset: 0x0070 Access type: read and write
	 * (RDO) Chroma weight table configure register2
	 */
	/* Data3 in chroma cost weight table. */
	CHRM_KLUT_WGT3,
	/* Low 9 bits of data4 in chroma cost weight table. */
	CHRM_KLUT_WGT4_L9,

	/*
	 * KLUT_WGT3
	 * Address offset: 0x0074 Access type: read and write
	 * (RDO) Chroma weight table configure register3
	 */
	/* High 9 bits of data4 in chroma cost weight table. */
	CHRM_KLUT_WGT4_H9,
	/* Data5 in chroma cost weight table. */
	CHRM_KLUT_WGT5,

	/*
	 * KLUT_WGT4
	 * Address offset: 0x0078 Access type: read and write
	 * (RDO) Chroma weight table configure register4
	 */
	/* Data6 in chroma cost weight table. */
	CHRM_KLUT_WGT6,
	/* Low 9 bits of data7 in chroma cost weight table. */
	CHRM_KLUT_WGT7_L9,

	/*
	 * KLUT_WGT5
	 * Address offset: 0x007C Access type: read and write
	 * (RDO) Chroma weight table configure register5
	 */
	/* High 9 bits of data7 in chroma cost weight table. */
	CHRM_KLUT_WGT7_H9,
	/* Data8 in chroma cost weight table. */
	CHRM_KLUT_WGT8,

	/*
	 * KLUT_WGT6
	 * Address offset: 0x0080 Access type: read and write
	 * (RDO) Chroma weight table configure register6
	 */
	/* Data9 in chroma cost weight table. */
	CHRM_KLUT_WGT9,
	/* Low 9 bits of data10 in chroma cost weight table. */
	CHRM_KLUT_WGT10_L9,

	/*
	 * KLUT_WGT7
	 * Address offset: 0x0084 Access type: read and write
	 * (RDO) Chroma weight table configure register7
	 */
	/* High 9 bits of data10 in chroma cost weight table. */
	CHRM_KLUT_WGT10_H9,
	/* Data11 in chroma cost weight table. */
	CHRM_KLUT_WGT11,

	/*
	 * KLUT_WGT8
	 * Address offset: 0x0088 Access type: read and write
	 * (RDO) Chroma weight table configure register8
	 */
	/* Data12 in chroma cost weight table. */
	CHRM_KLUT_WGT12,
	/* Low 9 bits of data13 in chroma cost weight table. */
	CHRM_KLUT_WGT13_L9,

	/*
	 * KLUT_WGT9
	 * Address offset: 0x008C Access type: read and write
	 * (RDO) Chroma weight table configure register9
	 */
	/* High 9 bits of data13 in chroma cost weight table. */
	CHRM_KLUT_WGT13_H9,
	/* Data14 in chroma cost weight table. */
	CHRM_KLUT_WGT14,

	/*
	 * KLUT_WGT10
	 * Address offset: 0x0090 Access type: read and write
	 * (RDO) Chroma weight table configure register10
	 */
	/* Data15 in chroma cost weight table. */
	CHRM_KLUT_WGT15,
	/* Low 9 bits of data16 in chroma cost weight table. */
	CHRM_KLUT_WGT16_L9,

	/*
	 * KLUT_WGT11
	 * Address offset: 0x0094 Access type: read and write
	 * (RDO) Chroma weight table configure register11
	 */
	/* High 9 bits of data16 in chroma cost weight table. */
	CHRM_KLUT_WGT16_H9,
	/* Data17 in chroma cost weight table. */
	CHRM_KLUT_WGT17,

	/*
	 * KLUT_WGT12
	 * Address offset: 0x0098 Access type: read and write
	 * (RDO) Chroma weight table configure register12
	 */
	/* Data18 in chroma cost weight table. */
	CHRM_KLUT_WGT18,
	/* Low 9 bits of data19 in chroma cost weight table. */
	CHRM_KLUT_WGT19_L9,

	/*
	 * KLUT_WGT13
	 * Address offset: 0x009C Access type: read and write
	 * (RDO) Chroma weight table configure register13
	 */
	/* High 9 bits of data19 in chroma cost weight table. */
	CHRM_KLUT_WGT19_H9,
	/* Data14 in chroma cost weight table. */
	CHRM_KLUT_WGT20,

	/*
	 * KLUT_WGT14
	 * Address offset: 0x00A0 Access type: read and write
	 * (RDO) Chroma weight table configure register14
	 */
	/* Data21 in chroma cost weight table. */
	CHRM_KLUT_WGT21,
	/* Low 9 bits of data22 in chroma cost weight table. */
	CHRM_KLUT_WGT22_L9,

	/*
	 * KLUT_WGT15
	 * Address offset: 0x00A4 Access type: read and write
	 * (RDO) Chroma weight table configure register15
	 */
	/* High 9 bits of data22 in chroma cost weight table. */
	CHRM_KLUT_WGT22_H9,
	/* Data23 in chroma cost weight table. */
	CHRM_KLUT_WGT23,

	/*
	 * KLUT_WGT16
	 * Address offset: 0x00A8 Access type: read and write
	 * (RDO) Chroma weight table configure register16
	 */
	/* Data24 in chroma cost weight table. */
	CHRM_KLUT_WGT24,
	/* Low 9 bits of data25 in chroma cost weight table. */
	CHRM_KLUT_WGT25_L9,

	/*
	 * KLUT_WGT17
	 * Address offset: 0x00AC Access type: read and write
	 * (RDO) Chroma weight table configure register17
	 */
	/* High 9 bits of data25 in chroma cost weight table. */
	CHRM_KLUT_WGT25_H9,
	/* Data26 in chroma cost weight table. */
	CHRM_KLUT_WGT26,

	/*
	 * KLUT_WGT18
	 * Address offset: 0x00B0 Access type: read and write
	 * (RDO) Chroma weight table configure register18
	 */
	/* Data27 in chroma cost weight table. */
	CHRM_KLUT_WGT27,
	/* Low 9 bits of data28 in chroma cost weight table. */
	CHRM_KLUT_WGT28_L9,

	/*
	 * KLUT_WGT19
	 * Address offset: 0x00B4 Access type: read and write
	 * (RDO) Chroma weight table configure register19
	 */
	/* High 9 bits of data28 in chroma cost weight table. */
	CHRM_KLUT_WGT28_H9,
	/* Data29 in chroma cost weight table. */
	CHRM_KLUT_WGT29,

	/*
	 * KLUT_WGT20
	 * Address offset: 0x00B8 Access type: read and write
	 * (RDO) Chroma weight table configure register20
	 */
	/* Data30 in chroma cost weight table. */
	CHRM_KLUT_WGT30,
	/* Low 9 bits of data31 in chroma cost weight table. */
	CHRM_KLUT_WGT31_L9,

	/*
	 * KLUT_WGT21
	 * Address offset: 0x00BC Access type: read and write
	 * (RDO) Chroma weight table configure register21
	 */
	/* High 9 bits of data31 in chroma cost weight table. */
	CHRM_KLUT_WGT31_H9,
	/* Data32 in chroma cost weight table. */
	CHRM_KLUT_WGT32,

	/*
	 * KLUT_WGT22
	 * Address offset: 0x00C0 Access type: read and write
	 * (RDO) Chroma weight table configure register22
	 */
	/* Data33 in chroma cost weight table. */
	CHRM_KLUT_WGT33,
	/* Low 9 bits of data34 in chroma cost weight table. */
	CHRM_KLUT_WGT34_L9,

	/*
	 * KLUT_WGT23
	 * Address offset: 0x00C4 Access type: read and write
	 * (RDO) Chroma weight table configure register23
	 */
	/* High 9 bits of data34 in chroma cost weight table. */
	CHRM_KLUT_WGT34_H9,

	/*
	 * RC_CFG
	 * Address offset: 0x00C8 Access type: read and write
	 * Rate control configuration
	 */
	/* Rate control enable. */
	RC_EN,
	/* Adaptive quantization enable. */
	AQ_EN,
	/*
	 * Mode of aq_delta calculation for CU32 and CU64.
	 * 1'b0: aq_delta of CU32/CU64 is calculated by corresponding MADI32/64,
	 * 1'b1: aq_delta of CU32/CU64 is calculated by corresponding 4/16 CU16 qp_deltas.
	 */
	AQ_MODE,
	/* RC adjustment intervals, base on CTU number. */
	RC_CTU_NUM,

	/*
	 * RC_QP
	 * Address offset: 0x00CC Access type: read and write
	 * QP configuration for rate control
	 */
	/*
	 * QP adjust range(delta_qp) in rate control.
	 * Delta_qp is constrained  between -rc_qp_range to rc_qp_range.
	 */
	RC_QP_RANGE,
	/* Max QP for rate control and AQ mode. */
	RC_MAX_QP,
	/* Min QP for rate control and AQ mode. */
	RC_MIN_QP,

	/*
	 * RC_TGT
	 * Address offset: 0x00D0 Access type: read and write
	 * The target bit rate for rate control
	 */
	/*
	 * Target bit num for one 64x64 CTU(for HEVC)
	 * or one 16x16 MB(for H.264), with 1/16 precision.
	 */
	CTU_EBIT,

	/*
	 * RC_ADJ0
	 * Address offset: 0x00D4 Access type: read and write
	 * QP adjust configuration for rate control
	 */
	/* QP adjust step0 for rate control. */
	QP_ADJ0,
	/* QP adjust step1 for rate control. */
	QP_ADJ1,
	/* QP adjust step2 for rate control. */
	QP_ADJ2,
	/* QP adjust step3 for rate control. */
	QP_ADJ3,
	/* QP adjust step4 for rate control. */
	QP_ADJ4,

	/*
	 * RC_ADJ1
	 * Address offset: 0x00D8 Access type: read and write
	 * QP adjust configuration for rate control
	 */
	/* QP adjust step5 for rate control. */
	QP_ADJ5,
	/* QP adjust step6 for rate control. */
	QP_ADJ6,
	/* QP adjust step7 for rate control. */
	QP_ADJ7,
	/* QP adjust step8 for rate control. */
	QP_ADJ8,

	/*
	 * RC_DTHD0~8
	 * Address offset: 0x00DC~0x00FC Access type: read and write
	 * Bits rate deviation threshold0~8
	 */
	/* Bits rate deviation threshold0~8. */
	RC_DTHD0,
	RC_DTHD1,
	RC_DTHD2,
	RC_DTHD3,
	RC_DTHD4,
	RC_DTHD5,
	RC_DTHD6,
	RC_DTHD7,
	RC_DTHD8,
	RC_DTHD9,

	/*
	 * ROI_QTHD0
	 * Address offset: 0x0100 Access type: read and write
	 * ROI QP threshold configuration0
	 */
	/* Min QP for 16x16 CU inside ROI area0. */
	QPMIN_AREA0,
	/* Max QP for 16x16 CU inside ROI area0. */
	QPMAX_AREA0,
	/* Min QP for 16x16 CU inside ROI area1. */
	QPMIN_AREA1,
	/* Max QP for 16x16 CU inside ROI area1. */
	QPMAX_AREA1,
	/* Min QP for 16x16 CU inside ROI area2. */
	QPMIN_AREA2,

	/*
	 * ROI_QTHD1
	 * Address offset: 0x0104 Access type: read and write
	 * ROI QP threshold configuration1
	 */
	/* Max QP for 16x16 CU inside ROI area2. */
	QPMAX_AREA2,
	/* Min QP for 16x16 CU inside ROI area3. */
	QPMIN_AREA3,
	/* Max QP for 16x16 CU inside ROI area3. */
	QPMAX_AREA3,
	/* Min QP for 16x16 CU inside ROI area4. */
	QPMIN_AREA4,
	/* Min QP for 16x16 CU inside ROI area4. */
	QPMAX_AREA4,

	/*
	 * ROI_QTHD2
	 * Address offset: 0x0108 Access type: read and write
	 * ROI QP threshold configuration2
	 */
	/* Min QP for 16x16 CU inside ROI area5. */
	QPMIN_AREA5,
	/* Max QP for 16x16 CU inside ROI area5. */
	QPMAX_AREA5,
	/* Min QP for 16x16 CU inside ROI area6. */
	QPMIN_AREA6,
	/* Max QP for 16x16 CU inside ROI area6. */
	QPMAX_AREA6,
	/* Min QP for 16x16 CU inside ROI area7. */
	QPMIN_AREA7,

	/*
	 * ROI_QTHD3
	 * Address offset: 0x010C Access type: read and write
	 * ROI QP threshold configuration3
	 */
	/* Max QP for 16x16 CU inside ROI area7. */
	QPMAX_AREA7,
	/*
	 * QP theshold generation for the CUs whose size is bigger than 16x16.
	 * 2'h0: Mean value of 16x16 CU QP thesholds
	 * 2'h1: Max value of 16x16 CU QP thesholds
	 * 2'h2: Min value of 16x16 CU QP thesholds
	 * 2'h3: Reserved
	 */
	QPMAP_MODE,

	/*
	 * PIC_OFST
	 * Address offset: 0x0110 Access type: read and write
	 * Encoding picture offset
	 */
	/* Vertical offset for encoding picture. */
	PIC_OFST_Y,
	/* Horizontal offset for encoding picture. */
	PIC_OFST_X,

	/*
	 * SRC_STRID
	 * Address offset: 0x0114 Access type: read and write
	 * Video source stride
	 */
	/*
	 * Video source stride0, based on pixel (byte).
	 * Note that if the video format is YUV, src_strd is the LUMA component
	 * stride while src_strid1 is the CHROMA component stride.
	 */
	SRC_STRD0,
	/*
	 * CHROMA stride of video source, only for YUV format.
	 * Note that U and V stride must be the same when color format is YUV
	 * planar.
	 */
	SRC_STRD1,

	/*
	 * ADR_SRC0
	 * Address offset: 0x0118 Access type: read and write
	 * Base address of the 1st storage area for video source
	 */
	/*
	 * Base address of the 1st storage area for video source.
	 * ARGB8888, BGR888, RGB565, YUYV422 and UYUV422 have only one storage
	 * area, while adr_src0 is configured as the base address of video
	 * source frame buffer.
	 * YUV422/420 semi-planar have 2 storage area, while adr_src0 is
	 * configured as the base address of Y frame buffer.
	 * YUV422/420 planar have 3 storage area, while adr_src0 is configured
	 * as the base address of Y frame buffer.
	 * Note that if the video source is compressed by AFBC, adr_src0 is
	 * configured as the base address of compressed frame buffer.
	 */
	ADR_SRC0,

	/*
	 * ADR_SRC1
	 * Address offset: 0x011C Access type: read and write
	 * Base address of the 2nd storage area for video source
	 */
	/*
	 * Base address of V frame buffer when video source is uncompress and
	 * color format is YUV422/420 planar.
	 */
	ADR_SRC1,

	/*
	 * ADR_SRC2
	 * Address offset: 0x0120 Access type: read and write
	 * Base address of the 3rd storage area for video source
	 */
	/*
	 * Base address of V frame buffer when video source is uncompress and
	 * color format is YUV422/420 planar.
	 */
	ADR_SRC2,

	/*
	 * ADR_ROI
	 * Address offset: 0x0124 Access type: read and write
	 * Base address for ROI configuration, 16 bytes aligned
	 */
	/* High 28 bits of base address for ROI configuration. */
	ROI_ADDR,

	/*
	 * ADR_RFPW_H
	 * Address offset: 0x0128 Access type: read and write
	 * Base address of header_block for compressed reference frame write,
	 * 4K bytes aligned
	 */
	/*
	 * High 20 bits of the header_block base address for compressed
	 * reference frame write.
	 */
	RFPW_H_ADDR,

	/*
	 * ADR_RFPW_B
	 * Address offset: 0x012C Access type: read and write
	 * Base address of body_block for compressed reference frame write,
	 * 4K bytes aligned
	 */
	/*
	 * High 20 bits of the body_block base address for compressed
	 * reference frame write.
	 */
	RFPW_B_ADDR,

	/*
	 * ADR_RFPR_H
	 * Address offset: 0x0130 Access type: read and write
	 * Base address of header_block for compressed reference frame read,
	 * 4K bytes aligned
	 */
	/*
	 * High 20 bits of the header_block base address for compressed
	 * reference frame read.
	 */
	RFPR_H_ADDR,

	/*
	 * ADR_RFPR_B
	 * Address offset: 0x0134 Access type: read and write
	 * Base address of body_block for compressed reference frame read,
	 * 4K bytes aligned
	 */
	/*
	 * High 20 bits of the body_block base address for compressed
	 * reference frame read.
	 */
	RFPR_B_ADDR,

	/*
	 * ADR_CMVW
	 * Address offset: 0x0138 Access type: read and write
	 * Base address for col-located Mv write, 1KB aligned, HEVC only
	 */
	/* High 22 bits of base address for col-located Mv write, HEVC only. */
	CMVW_ADDR,

	/*
	 * ADR_CMVR
	 * Address offset: 0x013C Access type: read and write
	 * Base address for col-located Mv read, 1KB aligned, HEVC only
	 */
	/* High 22 bits of base address for col-located Mv read, HEVC only. */
	CMVR_ADDR,

	/*
	 * ADR_DSPW
	 * Address offset: 0x0140 Access type: read and write
	 * Base address for down-sampled reference frame write, 1KB aligned
	 */
	/* High 22 bits of base address for down-sampled reference frame write. */
	DSPW_ADDR,

	/*
	 * ADR_DSPR
	 * Address offset: 0x0144 Access type: read and write
	 * Base address for down-sampled reference frame read, 1KB aligned
	 */
	/* High 22 bits of base address for down-sampled reference frame read. */
	DSPR_ADDR,

	/*
	 * ADR_MEIW
	 * Address offset: 0x0148 Access type: read and write
	 * Base address for ME information write, 1KB aligned
	 */
	/* High 22 bits of base address for ME information write. */
	MEIW_ADDR,

	/*
	 * ADR_BSBT
	 * Address offset: 0x014C Access type: read and write
	 * Top address of bit stream buffer, 128B aligned
	 */
	/* High 25 bits of the top address of bit stream buffer. */
	BSBT_ADDR,

	/*
	 * ADR_BSBB
	 * Address offset: 0x0150 Access type: read and write
	 * Bottom address of bit stream buffer, 128B aligned
	 */
	/* High 25 bits of the bottom address of bit stream buffer. */
	BSBB_ADDR,

	/*
	 * ADR_BSBR
	 * Address offset: 0x0154 Access type: read and write
	 * Read address of bit stream buffer, 128B aligned
	 */
	/*
	 * Read address of bit stream buffer, 128B aligned.
	 * VEPU will pause when write address meets read address and then send
	 * an interrupt. SW should move some data out from bit stream buffer
	 * and change this register accordingly.
	 * After that VEPU will continue processing automatically.
	 */
	BSBR_ADDR,

	/*
	 * ADR_BSBS
	 * Address offset: 0x0158 Access type: read and write
	 * Start address of bit stream buffer
	 */
	/*
	 * Start address of bit stream buffer.
	 * VEPU begins to write bit stream from this address and increase
	 * address automatically.
	 * Note that the VEPU's real-time write address is marked in BSB_STUS.
	 */
	ADR_BSBS,

	/*
	 * SLI_SPLT
	 * Address offset: 0x015C Access type: read and write
	 * Slice split configuration
	 */
	/* Slice split enable. */
	SLI_SPLT,
	/*
	 * Slice split mode.
	 * 1'h0: Slice splited by byte.
	 * 1'h1: Slice splited by number of MB(H.264)/CTU(HEVC).
	 */
	SLI_SPLT_MODE,
	/*
	 * Slice split compensation when slice is splited by byte.
	 * Byte distortion of current slice will be compensated in the next slice.
	 */
	SLI_SPLT_CPST,
	/* Max slice num in one frame. */
	SLI_MAX_NUM_M1,
	/* Slice flush. Flush all the bit stream after each slice finished. */
	SLI_FLSH,
	/* Number of CTU/MB for slice split. Valid when slice is splited by CTU/MB. */
	SLI_SPLT_CNUM_M1,

	/*
	 * SLI_BYTE
	 * Address offset: 0x0160 Access type: read and write
	 * Number of bytes for slice split
	 */
	/* Byte number for each slice when slice is splited by byte. */
	SLI_SPLT_BYTE,

	/*
	 * ME_RNGE
	 * Address offset: 0x0164 Access type: read and write
	 * Motion estimation range
	 */
	/* CME horizontal search range, base on 16 pixels. */
	CME_SRCH_H,
	/* CME vertical search range, base on 16 pixel. */
	CME_SRCH_V,
	/* RME horizontal search range, values from 3 to 7. */
	RME_SRCH_H,
	/* RME vertical search range, values from 4 to 5. */
	RME_SRCH_V,
	/* Frame number difference value between current and reference frame, HEVC only. */
	DLT_FRM_NUM,

	/*
	 * ME_CNST
	 * Address offset: 0x0168 Access type: read and write
	 * Motion estimation configuration
	 */
	/* Min horizontal distance for PMV selection. */
	PMV_MDST_H,
	/* Min vertical distance for PMV selection. */
	PMV_MDST_V,
	/*
	 * Motion vector limit ( by level), H.264 only.
	 * 2'h0: Mvy is limited to [-64,63].
	 * Others: Mvy is limited to [-128,127].
	 */
	MV_LIMIT,
	/* PMV number (should be constant2). */
	PMV_NUM,
	/* Store col-Mv information to external memory, HEVC only. */
	COLMV_STOR,
	/* Load co-located Mvs as predicated Mv candidates, HEVC only. */
	COLMV_LOAD,
	/*
	 * [4]: Disable 64x64 block RME.
	 * [3]: Disable 32x32 block RME.
	 * [2]: Disable 16x16 block RME.
	 * [1]: Disable 8x8   block RME.
	 * [0]: Disable 4x4   block RME.
	 */
	RME_DIS,
	/*
	 * [4]: Disable 64x64 block FME.
	 * [3]: Disable 32x32 block FME.
	 * [2]: Disable 16x16 block FME.
	 * [1]: Disable 8x8   block FME.
	 * [0]: Disable 4x4   block FME.
	 */
	FME_DIS,

	/*
	 * ME_RAM
	 * Address offset: 0x016C Access type: read and write
	 * ME cache configuration
	 */
	/* CME's max RAM address. */
	CME_RAMA_MAX,
	/* Height of CME RAMA district, base on 4 pixels. */
	CME_RAMA_H,
	/*
	 * L2 cach mapping, base on pixels.
	 * 2'h0: 32x512
	 * 2'h1: 16x1024
	 * 2'h2: 8x2048
	 * 2'h3: 4x4096
	 */
	CACH_L2_MAP,
	/* The width of CIME down-sample recon data linebuf, based on 64 pixel. */
	CME_LINEBUF_W,

	/*
	 * SYNT_LONG_REFM0
	 * Address offset: 0x0170 Access type: read and write
	 * Long term reference frame mark0 for HEVC
	 */
	/* Poc_lsb_lt[1] */
	POC_LSB_LT1,
	/* Poc_lsb_lt[2] */
	POC_LSB_LT2,

	/*
	 * SYNT_LONG_REFM1
	 * Address offset: 0x0174 Access type: read and write
	 * Long term reference frame mark1 for HEVC
	 */
	/* Delta_poc_msb_cycle_lt[1] */
	DLT_POC_MSB_CYCL1,
	/* Delta_poc_msb_cycle_lt[2] */
	DLT_POC_MSB_CYCL2,

	/*
	 * OSD_INV_CFG
	 * Address offset: 0x0178 Access type: read and write
	 * OSD color inverse  configuration
	 *
	 * Added in vepu540
	 */
	/*
	 * OSD color inverse enable of chroma component,
	 * each bit controls corresponding region.
	 */
	OSD_CH_INV_EN,
	/*
	 * OSD color inverse expression type
	 * each bit controls corresponding region.
	 * 1'h0: AND,
	 * 1'h1: OR
	 */
	OSD_ITYPE,
	/*
	 * OSD color inverse expression switch for luma component
	 * each bit controls corresponding region.
	 * 1'h0: Expression need to determine the condition,
	 * 1'h1: Expression don't need to determine the condition,
	 */
	OSD_LU_INV_MSK,
	/*
	 * OSD color inverse expression switch for chroma component
	 * each bit controls corresponding region.
	 * 1'h0: Expression need to determine the condition,
	 * 1'h1: Expression don't need to determine the condition,
	 */
	OSD_CH_INV_MSK,

	/*
	 * IPRD_CSTS
	 * Address offset: 0x0194 Access type: read and write
	 * Cost function configuration for intra prediction
	 */
	/* LUMA variance threshold to select intra prediction cost function. */
	VTHD_Y,
	/* CHROMA variance threshold to select intra prediction cost function. */
	VTHD_C,

	/*
	 * RDO_CFG_H264
	 * Address offset: 0x0198 Access type: read and write
	 * H.264 RDO configuration
	 */
	/* Limit sub_mb_rect_size for low level. */
	RECT_SIZE,
	/* 4x4 sub MB enable. */
	INTER_4X4,
	/* Reserved */
	ARB_SEL,
	/* CAVLC syntax limit. */
	VLC_LMT,
	/* Chroma special candidates enable. */
	CHRM_SPCL,
	/*
	 * [7]: Disable intra4x4.
	 * [6]: Disable intra8x8.
	 * [5]: Disable intra16x16.
	 * [4]: Disable inter8x8 with T4.
	 * [3]: Disable inter8x8 with T8.
	 * [2]: Disable inter16x16 with T4.
	 * [1]: Disable inter16x16 with T8.
	 * [0]: Disable skip mode.
	 */
	RDO_MASK,
	/* Chroma cost weight adjustment(KLUT) enable. */
	CCWA_E,
	/*
	 * Scale list selection.
	 * 1'h0: Flat scale list.
	 * 1'h1: Default scale list.
	 */
	SCL_LST_SEL,
	/* Anti-ring enable. */
	ATR_E,
	/* Edge of anti-flicker, base on MB. the MBs inside edge should not influenced. */
	ATF_EDG,
	/* Block level anti-flicker enable. */
	ATF_LVL_E,
	/* Intra mode anti-flicker enable. */
	ATF_INTRA_E,
	/*
	 * Scale list selection. (for vepu540)
	 * 2'h0: Flat scale list.
	 * 2'h1: Default scale list.
	 * 2'h2: User defined.
	 * 2'h3: Reserved.
	 */
	SCL_LST_SEL_,
	/*
	 * Rdo cost caculation expression for intra by using sad or satd.
	 * 1'h0: SATD,
	 * 1'h1: SAD,
	 */
	SATD_BYPS_FLG,

	/*
	 * SYNT_NAL_H264
	 * Address offset: 0x019C Access type: read and write
	 * NAL configuration for H.264
	 */
	/* nal_ref_idc */
	NAL_REF_IDC,
	NAL_UNIT_TYPE,
	NAL_UNIT_TYPE_HEVC,
	/* nal_unit_type */

	/*
	 * SYNT_SPS_H264
	 * Address offset: 0x01A0 Access type: read and write
	 * Sequence parameter set syntax configuration for H.264
	 */
	/* log2_max_frame_num_minus4 */
	MAX_FNUM,
	/* direct_8x8_inference_flag */
	DRCT_8X8,
	/* log2_max_pic_order_cnt_lsb_minus4 */
	MPOC_LM4,

	/*
	 * SYNT_PPS_H264
	 * Address offset: 0x01A4 Access type: read and write
	 * Picture parameter set configuration for H.264
	 */
	/* entropy_coding_mode_flag */
	ETPY_MODE,
	/* transform_8x8_mode_flag */
	TRNS_8X8,
	/* constrained_intra_pred_flag */
	CSIP_FLAG,
	/* num_ref_idx_l0_active_minus1 */
	NUM_REF0_IDX,
	/* num_ref_idx_l1_active_minus1 */
	NUM_REF1_IDX,
	/* pic_init_qp_minus26 + 26 */
	PIC_INIT_QP,
	/* chroma_qp_index_offset */
	CB_OFST,
	/* second_chroma_qp_index_offset */
	CR_OFST,
	/* weight_pred_flag */
	WGHT_PRED,
	/* deblocking_filter_control_present_flag */
	DBF_CP_FLG,

	/*
	 * SYNT_SLI0_H264
	 * Address offset: 0x01A8 Access type: read and write
	 * Slice header configuration0 for H.264
	 */
	/* slice_type: 0->P, 1->B, 2->I. */
	SLI_TYPE,
	/* pic_parameter_set_id */
	PPS_ID,
	/* direct_spatial_mv_pred_flag */
	DRCT_SMVP,
	/* num_ref_idx_active_override_flag */
	NUM_REF_OVRD,
	/* cabac_init_idc */
	CBC_INIT_IDC,
	/* frame_num */
	FRM_NUM,

	/*
	 * SYNT_SLI1_H264
	 * Address offset: 0x01AC Access type: read and write
	 * Slice header configuration1 for H.264
	 */
	/* idr_pid */
	IDR_PIC_ID,
	/* pic_order_cnt_lsb */
	POC_LSB,

	/*
	 * SYNT_SLI2_H264
	 * Address offset: 0x01B0 Access type: read and write
	 * Slice header configuration2 for H.264
	 */
	/* reordering_of_pic_nums_idc */
	RODR_PIC_IDX,
	/* ref_pic_list_reordering_flag_l0 */
	REF_LIST0_RODR,
	/* slice_beta_offset_div2 */
	SLI_BETA_OFST,
	/* slice_alpha_c0_offset_div2 */
	SLI_ALPH_OFST,
	/* disable_deblocking_filter_idc */
	DIS_DBLK_IDC,
	/* abs_diff_pic_num_minus1/long_term_pic_num */
	RODR_PIC_NUM,

	/*
	 * SYNT_REFM0_H264
	 * Address offset: 0x01B4 Access type: read and write
	 * Reference frame mark0 for H.264
	 */
	/* no_output_of_prior_pics_flag */
	NOPP_FLG,
	/* long_term_reference_flag */
	LTRF_FLG,
	/* adaptive_ref_pic_marking_mode_flag */
	ARPM_FLG,
	/* A No.4 MMCO should be executed firstly if mmo4_pre is 1 */
	MMCO4_PRE,
	/* memory_management_control_operation */
	MMCO_TYPE0,
	/*
	 * MMCO parameters which have different meanings according to different mmco_parm0 valus.
	 * difference_of_pic_nums_minus1 for mmco_parm0 equals 0 or 3.
	 * long_term_pic_num for mmco_parm0 equals 2.
	 * long_term_frame_idx for mmco_parm0 equals 6.
	 * max_long_term_frame_idx_plus1 for mmco_parm0 equals 4.
	 */
	MMCO_PARM0,
	/* memory_management_control_operation[1] */
	MMCO_TYPE1,
	/* memory_management_control_operation[2] */
	MMCO_TYPE2,

	/*
	 * SYNT_REFM1_H264
	 * Address offset: 0x01B8 Access type: read and write
	 * Reference frame mark1 for H.264
	 */
	/*
	 * MMCO parameters which have different meanings according to different mmco_parm1 valus.
	 * difference_of_pic_nums_minus1 for mmco_parm1 equals 0 or 3.
	 * long_term_pic_num for mmco_parm1 equals 2.
	 * long_term_frame_idx for mmco_parm1 equals 6.
	 * max_long_term_frame_idx_plus1 for mmco_parm1 equals 4.
	 */
	MMCO_PARM1,
	/*
	 * MMCO parameters which have different meanings according to different mmco_parm2 valus.
	 * difference_of_pic_nums_minus1 for mmco_parm2 equals 0 or 3.
	 * long_term_pic_num for mmco_parm2 equals 2.
	 * long_term_frame_idx for mmco_parm2 equals 6.
	 * max_long_term_frame_idx_plus1 for mmco_parm2 equals 4.
	 */
	MMCO_PARM2,

	/*
	 * OSD_CFG
	 * Address offset: 0x01C0 Access type: read and write
	 * OSD configuration
	 */
	/* OSD region enable, each bit controls corresponding OSD region. */
	OSD_E,
	/* OSD inverse color enable, each bit controls corresponding region. */
	OSD_INV_E,
	/*
	 * OSD palette clock selection.
	 * 1'h0: Configure bus clock domain.
	 * 1'h1: Core clock domain.
	 */
	OSD_PLT_CKS,
	/*
	 * OSD palette type.
	 * 1'h1: Default type.
	 * 1'h0: User defined type.
	 */
	OSD_PLT_TYP,

	/*
	 * OSD_INV
	 * Address offset: 0x01C4 Access type: read and write
	 * OSD color inverse configuration
	 */
	/* Color inverse theshold for OSD region0. */
	OSD_ITHD_R0,
	/* Color inverse theshold for OSD region1. */
	OSD_ITHD_R1,
	/* Color inverse theshold for OSD region2. */
	OSD_ITHD_R2,
	/* Color inverse theshold for OSD region3. */
	OSD_ITHD_R3,
	/* Color inverse theshold for OSD region4. */
	OSD_ITHD_R4,
	/* Color inverse theshold for OSD region5. */
	OSD_ITHD_R5,
	/* Color inverse theshold for OSD region6. */
	OSD_ITHD_R6,
	/* Color inverse theshold for OSD region7. */
	OSD_ITHD_R7,

	/*
	 * SYNT_REFM2_H264
	 * Address offset: 0x01C8 Access type: read and write
	 * Reference frame mark2 for H.264
	 */
	/* long_term_frame_idx[0] (when mmco equal 3) */
	LONG_TERM_FRAME_IDX0,
	/* long_term_frame_idx[1] (when mmco equal 3) */
	LONG_TERM_FRAME_IDX1,
	/* long_term_frame_idx[2] (when mmco equal 3) */
	LONG_TERM_FRAME_IDX2,

	/*
	 * SYNT_REFM3
	 * Address offset: 0x01CC Access type: read and write
	 * Reference frame mark3 for HEVC
	 */
	REG115,

	/*
	 * OSD_POS
	 * Address offset: 0x01D0~0x01EC Access type: read and write
	 * OSD region position
	 */
	OSD0_POS_LB_X,
	OSD0_POS_LB_Y,
	OSD0_POS_RB_X,
	OSD0_POS_RB_Y,

	OSD1_POS_LB_X,
	OSD1_POS_LB_Y,
	OSD1_POS_RB_X,
	OSD1_POS_RB_Y,

	OSD2_POS_LB_X,
	OSD2_POS_LB_Y,
	OSD2_POS_RB_X,
	OSD2_POS_RB_Y,

	OSD3_POS_LB_X,
	OSD3_POS_LB_Y,
	OSD3_POS_RB_X,
	OSD3_POS_RB_Y,

	OSD4_POS_LB_X,
	OSD4_POS_LB_Y,
	OSD4_POS_RB_X,
	OSD4_POS_RB_Y,

	OSD5_POS_LB_X,
	OSD5_POS_LB_Y,
	OSD5_POS_RB_X,
	OSD5_POS_RB_Y,

	OSD6_POS_LB_X,
	OSD6_POS_LB_Y,
	OSD6_POS_RB_X,
	OSD6_POS_RB_Y,

	OSD7_POS_LB_X,
	OSD7_POS_LB_Y,
	OSD7_POS_RB_X,
	OSD7_POS_RB_Y,

	/*
	 * ADR_OSD
	 * Address offset: 0x01F0~0x20C Access type: read and write
	 * Base address for OSD region, 16B aligned
	 */
	OSD_ADDR0,
	OSD_ADDR1,
	OSD_ADDR2,
	OSD_ADDR3,
	OSD_ADDR4,
	OSD_ADDR5,
	OSD_ADDR6,
	OSD_ADDR7,

	/*
	 * ST_BSL
	 * Address offset: 0x210 Access type: read only
	 * Bit stream length for current frame
	 */
	/* Bit stream length for current frame. */
	BS_LGTH,

	/*
	 * ST_SSE_L32
	 * Address offset: 0x214 Access type: read only
	 * Low 32 bits of encoding distortion (SSE)
	 */
	SSE_L32,

	/*
	 * ST_SSE_QP
	 * Address offset: 0x218 Access type: read only
	 * High 8 bits of encoding distortion (SSE) and sum of QP for the encoded frame
	 */
	/* Sum of QP for the encoded frame. */
	QP_SUM,
	/* High bits of encoding distortion(SSE). */
	SSE_H8,

	/*
	 * ST_SAO
	 * Address offset: 0x21C Access type: read only
	 * Number of CTUs which adjusted by SAO
	 */
	/* Number of CTUs whose CHROMA component are adjusted by SAO. */
	SAO_CNUM,
	/* Number of CTUs whose LUMA component are adjusted by SAO. */
	SAO_YNUM,

	/*
	 * ST_ENC
	 * Address offset: 0x228 Access type: read only
	 * VEPU working status
	 */
	/*
	 * VEPU working status.
	 * 2'h0: Idle.
	 * 2'h1: Working in register conifguration mode.
	 * 2'h2: Working in link table configuration mode.
	 */
	ST_ENC,
	/*
	 * Status of safe clear.
	 * 1'h0: Safe clear is finished or not started.
	 * 1'h1: VEPU is performing safe clear.
	 */
	ST_SCLR,

	/*
	 * ST_LKT
	 * Address offset: 0x22C Access type: read only
	 * Status of link table mode encoding
	 */
	/* Number of frames has been encoded since link table mode started. */
	FNUM_ENC,
	/* Number of frames has been configured since link table mode started. */
	FNUM_CFG,
	/*
	 * Number of frames has been encoded since link table mode started,
	 * updated only when corresponding link table node send interrupt
	 * (VEPU_ENC_PIC_node_int==1).
	 */
	FNUM_INT,

	/*
	 * ST_NADR
	 * Address offset: 0x230 Access type: read only
	 * Address of the processing link table node
	 */
	/* High 28 bits of the address for the processing linke table node. */
	NODE_ADDR,

	/*
	 * ST_BSB
	 * Address offset: 0x234 Access type: read only
	 * Status of bit stream buffer
	 */
	/* High 28 bits of bit stream buffer write address. */
	BSBW_ADDR,

	/*
	 * ST_BUS
	 * Address offset: 0x238 Access type: read only
	 * VEPU bus status
	 */
	/*
	 * AXI write response idle.
	 * [6]: Reconstructed picture channel (AXI0_WID==5)
	 * [5]: ME information channel (AXI0_WID==4)
	 * [4]: Co-located Mv channel (AXI0_WID==3)
	 * [3]: Down-sampled picture channel (AXI0_WID==2)
	 * [2]: Bit stream channel (AXI0_WID==1)
	 * [1]: Link table node channel (AXI0_WID==0)
	 * [0]: Reserved
	 */
	AXIB_IDL,
	/*
	 * AXI write response outstanding overflow.
	 * [6]: Reconstructed picture channel (AXI0_WID==5)
	 * [5]: ME information channel (AXI0_WID==4)
	 * [4]: Co-located Mv channel (AXI0_WID==3)
	 * [3]: Down-sampled picture channel (AXI0_WID==2)
	 * [2]: Bit stream channel (AXI0_WID==1)
	 * [1]: Link table node channel (AXI0_WID==0)
	 * [0]: Reserved.
	 */
	AXIB_OVFL,
	/*
	 * AXI write response error.
	 * [6]: Reconstructed picture channel (AXI0_WID==5)
	 * [5]: ME information channel (AXI0_WID==4)
	 * [4]: Co-located Mv channel (AXI0_WID==3)
	 * [3]: Down-sampled picture channel (AXI0_WID==2)
	 * [2]: Bit stream channel (AXI0_WID==1)
	 * [1]: Link table node channel (AXI0_WID==0)
	 * [0]: Reserved.
	 */
	AXIB_ERR,
	/*
	 * AXI read error.
	 * [5]: ROI configuration (AXI0_ARID==7)
	 * [4]: Down-sampled picture (AXI0_ARID==6)
	 * [3]: Co-located Mv (AXI0_ARID==5)
	 * [2]: Link table (AXI0_ARID==4)
	 * [1]: Reference picture (AXI0_ARID==1,2,3,8)
	 * [0]: Video source load (AXI1)
	 */
	AXIR_ERR,

	/*
	 * ST_SNUM
	 * Address offset: 0x23C Access type: read only
	 * Slice number status
	 */
	/* Number for slices has been encoded and not read out (by reading ST_SLEN). */
	SLI_NUM,

	/*
	 * ST_SLEN
	 * Address offset: 0x240 Access type: read only
	 * Status of slice length
	 */
	/* Byte length for the earlist encoded slice which has not been read out( by reading VEPU_ST_SLEN). */
	SLI_LEN,

	/*
	 * ST_PNUM_P64
	 * Address offset: 0x244 Access type: read only
	 * Number of 64x64 inter predicted blocks
	 */
	/* Number of 64x64 inter predicted blocks. */
	PNUM_P64,

	/*
	 * ST_PNUM_P32
	 * Address offset: 0x248 Access type: read only
	 * Number of 32x32 inter predicted blocks
	 */
	/* Number of 32x32 inter predicted blocks. */
	PNUM_P32,

	/*
	 * ST_PNUM_P16
	 * Address offset: 0x24C Access type: read only
	 * Number of 16x16 inter predicted blocks
	 */
	/* Number of 16x16 inter predicted blocks. */
	PNUM_P16,

	/*
	 * ST_PNUM_P8
	 * Address offset: 0x250 Access type: read only
	 * Number of 8x8 inter predicted blocks
	 */
	/* Number of 8x8 inter predicted blocks. */
	PNUM_P8,

	/*
	 * ST_PNUM_I32
	 * Address offset: 0x254 Access type: read only
	 * Number of 32x32 intra predicted blocks
	 */
	/* Number of 32x32 intra predicted blocks. */
	PNUM_I32,

	/*
	 * ST_PNUM_I16
	 * Address offset: 0x258 Access type: read only
	 * Number of 16x16 intra predicted blocks
	 */
	/* Number of 16x16 intra predicted blocks. */
	PNUM_I16,

	/*
	 * ST_PNUM_I8
	 * Address offset: 0x25C Access type: read only
	 * Number of 8x8 intra predicted blocks
	 */
	/* Number of 8x8 intra predicted blocks. */
	PNUM_I8,

	/*
	 * ST_PNUM_I4
	 * Address offset: 0x260 Access type: read only
	 * Number of 4x4 intra predicted blocks
	 */
	/* Number of 4x4 intra predicted blocks. */
	PNUM_I4,

	/*
	 * ST_B8_QP0~51
	 * Address offset: 0x264~0x330 Access type: read only
	 * Number of block8x8s with QP=0~51
	 */
	/*
	 * Number of block8x8s with QP value.
	 * HEVC CUs of which size are bigger that 8x8 are considered as
	 * (CU_size/8)*(CU_size/8) clock8x8s,
	 * while H.264 MB is considered as 4 block8x8s.
	 */
	//NUM_QP[52],

	/*
	 * ST_CPLX_TMP
	 * Address offset: 0x334 Access type: read only
	 * Temporal complexity(MADP) for current encoding and reference frame
	 */
	/* Mean absolute differences between current encoding and reference frame. */
	MADP,

	/*
	 * ST_BNUM_CME
	 * Address offset: 0x338 Access type: read only
	 * Number of CME blocks in frame.
	 * H.264: number CME blocks (4 MBs) in 16x64 aligned extended frame,
	 * except for the CME blocks configured as force intra.
	 * HEVC : number CME blocks (CTU) in 64x64 aligned extended frame,
	 * except for the CME blocks configured as force intra.
	 */
	/* Number of CTU (HEVC: 64x64, H.264: 64x16) for CME inter-frame prediction. */
	NUM_CTU,

	/*
	 * ST_CPLX_SPT
	 * Address offset: 0x33C Access type: read only
	 * Spatial complexity(MADI) for current encoding frame
	 */
	/* Mean absolute differences for current encoding frame. */
	MADI,

	/*
	 * ST_BNUM_B16
	 * Address offset: 0x340 Access type: read only
	 * Number of valid 16x16 blocks for one frame.
	 */
	/* Number of valid 16x16 blocks for one frame. */
	NUM_B16,
	/* sentinel */
	L1_MAX_FIELDS
};

static const struct reg_field rkvenc_vepu540_l1_fields[] = {
	[SUB_VER]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 0, 7),
	[H264_ENC]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 8, 8),
	[H265_ENC]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 9, 9),

	[PIC_SIZE]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 12, 15), /* RES_CAP in Datasheet TRM Part2 Page748 */
	[OSD_CAP]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 16, 17),
	[FILTR_CAP]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 18, 19),
	[BFRM_CAP]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 20, 20),
	[FBC_CAP]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 21, 22),
	[RKVENC_VER]	= REG_FIELD(RKVENC_VEPU540_L1_VERSION, 24, 31), /* IP_ID in Datasheet TRM Part 2 Page 747 */
	[LKT_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_STRT, 0, 7),
	[RKVENC_CMD]		= REG_FIELD(RKVENC_VEPU540_L1_STRT, 8, 9), /* VEPU_CMD in TRM */
	[CLK_GATE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_STRT, 16, 16),
	[RESETN_HW_EN]		= REG_FIELD(RKVENC_VEPU540_L1_STRT, 17, 17),
	[ENC_DONE_TMVP_EN]	= REG_FIELD(RKVENC_VEPU540_L1_STRT, 18, 18),

	[SAFE_CLR]	= REG_FIELD(RKVENC_VEPU540_L1_CLR, 0, 0),
	[FORCE_CLR]	= REG_FIELD(RKVENC_VEPU540_L1_CLR, 1, 1),

	[LKT_ADDR]	= REG_FIELD(RKVENC_VEPU540_L1_LKT_ADDR, 4, 31),

	[ENC_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 0, 0),
	[LKT_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 1, 1),
	[SCLR_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 2, 2),
	[ENC_SLICE_DONE_EN]	= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 3, 3),
	[OFLW_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 4, 4),
	[BRSP_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 5, 5),
	[BERR_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 6, 6),
	[RERR_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 7, 7),
	[WDG_DONE_EN]		= REG_FIELD(RKVENC_VEPU540_L1_INT_EN, 8, 8),

	[ENC_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 0, 0),
	[LKT_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 1, 1),
	[SCLR_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 2, 2),
	[ENC_SLICE_DONE_MSK]	= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 3, 3),
	[OFLW_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 4, 4),
	[BRSP_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 5, 5),
	[BERR_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 6, 6),
	[RERR_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 7, 7),
	[WDG_DONE_MSK]		= REG_FIELD(RKVENC_VEPU540_L1_INT_MSK, 8, 8),

	[ENC_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 0, 0),
	[LKT_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 1, 1),
	[SCLR_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 2, 2),
	[ENC_SLICE_DONE_CLR]	= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 3, 3),
	[OFLW_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 4, 4),
	[BRSP_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 5, 5),
	[BERR_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 6, 6),
	[RERR_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 7, 7),
	[WDG_DONE_CLR]		= REG_FIELD(RKVENC_VEPU540_L1_INT_CLR, 8, 8),

	[ENC_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 0, 0),
	[LKT_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 1, 1),
	[SCLR_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 2, 2),
	[ENC_SLICE_DONE_STA]	= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 3, 3),
	[OFLW_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 4, 4),
	[BRSP_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 5, 5),
	[BERR_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 6, 6),
	[RERR_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 7, 7),
	[WDG_DONE_STA]		= REG_FIELD(RKVENC_VEPU540_L1_INT_STA, 8, 8),

	[PIC_WD8_M1]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_RSL, 0, 8),
	[PIC_WFILL]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_RSL, 10, 15),
	[PIC_HD8_M1]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_RSL, 16, 24),
	[PIC_HFILL]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_RSL, 26, 31),

	[ENC_STND]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 0, 0),
	[ROI_ENC]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 1, 1),
	[CUR_FRM_REF]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 2, 2),
	[MEI_STOR]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 3, 3),
	[BS_SCP]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 4, 4),
	[LAMB_MOD_SEL]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 5, 5),
	[PIC_QP]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 8, 13),
	[TOT_POC_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 14, 18),
	[LOG2_CTU_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 19, 22),
	[ATR_THD_SEL]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 23, 23),
	[DCHS_RXID]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 24, 25),
	[DCHS_TXID]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 26, 27),
	[DCHS_RXE]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 28, 28),
	[SATD_BYPS_EN]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 29, 29),
	[SLEN_FIFO]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 30, 30),
	[NODE_INT]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_PIC, 31, 31),

	[VS_LOAD_THD]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_WDG, 0, 23),
	[RFP_LOAD_THRD]		= REG_FIELD(RKVENC_VEPU540_L1_ENC_WDG, 24, 31),

	[LPFW_BUS_ORDR]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 0, 0),
	[CMVW_BUS_ORDR]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 1, 1),
	[DSPW_BUS_ORDR]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 2, 2),
	[RFPW_BUS_ORDR]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 3, 3),
	[SRC_BUS_EDIN]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 4, 7),
	[MEIW_BUS_EDIN]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 8, 11),
	[BSW_BUS_EDIN]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 12, 14),
	[LKTR_BUS_EDIN]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 15, 18),
	[ROIR_BUS_EDIN]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 19, 22),
	[LKTW_BUS_EDIN]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 23, 26),
	[AFBC_BSIZE]		= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_MAP, 27, 27),

	[VPU541_AXI_BRSP_CKE]	= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_CFG, 0, 6),
	[VPU541_DSPR_OTSD]	= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_CFG, 7, 7),

	[VPU540_DSPR_OTSD]	= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_CFG, 7, 7),
	[VPU540_AXI_BRSP_CKE]	= REG_FIELD(RKVENC_VEPU540_L1_DTRNS_CFG, 16, 23),

	[ALPHA_SWAP]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_FMT, 0, 0),
	[RBUV_SWAP]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_FMT, 1, 1),
	[SRC_CFMT]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_FMT, 2, 5),
	[SRC_RANGE]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_FMT, 6, 6),
	[OUT_FMT_CFG]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_FMT, 7, 7),

	[CSC_WGT_B2Y]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFY, 0, 8),
	[CSC_WGT_G2Y]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFY, 9, 17),
	[CSC_WGT_R2Y]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFY, 18, 26),

	[CSC_WGT_B2U]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFU, 0, 8),
	[CSC_WGT_G2U]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFU, 9, 17),
	[CSC_WGT_R2U]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFU, 18, 26),

	[CSC_WGT_B2V]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFV, 0, 8),
	[CSC_WGT_G2V]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFV, 9, 17),
	[CSC_WGT_R2V]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFV, 18, 26),

	[CSC_OFST_V]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFO, 0, 7),
	[CSC_OFST_U]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFO, 8, 15),
	[CSC_OFST_Y]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_UDFO, 16, 20),

	[SRC_MIRR]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_PROC, 26, 26),
	[SRC_ROT]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_PROC, 27, 28),
	[TXA_EN]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_PROC, 29, 29),
	[AFBCD_EN]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_PROC, 30, 30),

	[SLI_CRS_EN]		= REG_FIELD(RKVENC_VEPU540_L1_SLI_CFG_H264, 31, 31),

	[CHRM_KLUT_OFST]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_OFST, 0, 2),

	[CHRM_KLUT_WGT0]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(0), 0, 17),
	[CHRM_KLUT_WGT1_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(0), 23, 31),

	[CHRM_KLUT_WGT1_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(1), 0, 8),
	[CHRM_KLUT_WGT2]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(1), 14, 31),

	[CHRM_KLUT_WGT3]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(2), 0, 17),
	[CHRM_KLUT_WGT4_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(2), 23, 31),

	[CHRM_KLUT_WGT4_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(3), 0, 8),
	[CHRM_KLUT_WGT5]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(3), 14, 31),

	[CHRM_KLUT_WGT6]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(4), 0, 17),
	[CHRM_KLUT_WGT7_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(4), 23, 31),

	[CHRM_KLUT_WGT7_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(5), 0, 8),
	[CHRM_KLUT_WGT8]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(5), 14, 31),

	[CHRM_KLUT_WGT9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(6), 0, 17),
	[CHRM_KLUT_WGT10_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(6), 23, 31),

	[CHRM_KLUT_WGT10_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(7), 0, 8),
	[CHRM_KLUT_WGT11]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(7), 14, 31),

	[CHRM_KLUT_WGT12]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(8), 0, 17),
	[CHRM_KLUT_WGT13_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(8), 23, 31),

	[CHRM_KLUT_WGT13_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(9), 0, 8),
	[CHRM_KLUT_WGT14]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(9), 14, 31),

	[CHRM_KLUT_WGT15]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(10), 0, 17),
	[CHRM_KLUT_WGT16_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(10), 23, 31),

	[CHRM_KLUT_WGT16_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(11), 0, 8),
	[CHRM_KLUT_WGT17]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(11), 14, 31),

	[CHRM_KLUT_WGT18]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(12), 0, 17),
	[CHRM_KLUT_WGT19_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(12), 23, 31),

	[CHRM_KLUT_WGT19_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(13), 0, 8),
	[CHRM_KLUT_WGT20]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(13), 14, 31),

	[CHRM_KLUT_WGT21]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(14), 0, 17),
	[CHRM_KLUT_WGT22_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(14), 23, 31),

	[CHRM_KLUT_WGT22_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(15), 0, 8),
	[CHRM_KLUT_WGT23]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(15), 14, 31),

	[CHRM_KLUT_WGT24]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(16), 0, 17),
	[CHRM_KLUT_WGT25_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(16), 23, 31),

	[CHRM_KLUT_WGT25_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(17), 0, 8),
	[CHRM_KLUT_WGT26]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(17), 14, 31),

	[CHRM_KLUT_WGT27]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(18), 0, 17),
	[CHRM_KLUT_WGT28_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(18), 23, 31),

	[CHRM_KLUT_WGT28_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(19), 0, 8),
	[CHRM_KLUT_WGT29]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(19), 14, 31),

	[CHRM_KLUT_WGT30]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(20), 0, 17),
	[CHRM_KLUT_WGT31_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(20), 23, 31),

	[CHRM_KLUT_WGT31_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(21), 0, 8),
	[CHRM_KLUT_WGT32]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(21), 14, 31),

	[CHRM_KLUT_WGT33]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(22), 0, 17),
	[CHRM_KLUT_WGT34_L9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(22), 23, 31),

	[CHRM_KLUT_WGT34_H9]	= REG_FIELD(RKVENC_VEPU540_L1_KLUT_WGT(23), 0, 8),

	[RC_EN]			= REG_FIELD(RKVENC_VEPU540_L1_RC_CFG, 0, 0),
	[AQ_EN]			= REG_FIELD(RKVENC_VEPU540_L1_RC_CFG, 1, 1),
	[AQ_MODE]		= REG_FIELD(RKVENC_VEPU540_L1_RC_CFG, 2, 2),
	[RC_CTU_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_RC_CFG, 16, 31),

	[RC_QP_RANGE]		= REG_FIELD(RKVENC_VEPU540_L1_RC_QP, 16, 19),
	[RC_MAX_QP]		= REG_FIELD(RKVENC_VEPU540_L1_RC_QP, 20, 25),
	[RC_MIN_QP]		= REG_FIELD(RKVENC_VEPU540_L1_RC_QP, 26, 31),

	[CTU_EBIT]		= REG_FIELD(RKVENC_VEPU540_L1_RC_TGT, 0, 19),

	[QP_ADJ0]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(0), 0, 4),
	[QP_ADJ1]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(0), 5, 9),
	[QP_ADJ2]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(0), 10, 14),
	[QP_ADJ3]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(0), 15, 19),
	[QP_ADJ4]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(0), 20, 24),

	[QP_ADJ5]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(1), 0, 4),
	[QP_ADJ6]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(1), 5, 9),
	[QP_ADJ7]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(1), 10, 14),
	[QP_ADJ8]		= REG_FIELD(RKVENC_VEPU540_L1_RC_ADJ(1), 15, 19),

	[RC_DTHD0]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(0), 0, 31),
	[RC_DTHD1]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(1), 0, 31),
	[RC_DTHD2]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(2), 0, 31),
	[RC_DTHD3]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(3), 0, 31),
	[RC_DTHD4]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(4), 0, 31),
	[RC_DTHD5]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(5), 0, 31),
	[RC_DTHD6]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(6), 0, 31),
	[RC_DTHD7]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(7), 0, 31),
	[RC_DTHD8]		= REG_FIELD(RKVENC_VEPU540_L1_RC_DTHD(8), 0, 31),

	[QPMIN_AREA0]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(0), 0, 5),
	[QPMAX_AREA0]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(0), 6, 11),
	[QPMIN_AREA1]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(0), 12, 17),
	[QPMAX_AREA1]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(0), 18, 23),
	[QPMIN_AREA2]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(0), 24, 29),

	[QPMAX_AREA2]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(1), 0, 5),
	[QPMIN_AREA3]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(1), 6, 11),
	[QPMAX_AREA3]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(1), 12, 17),
	[QPMIN_AREA4]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(1), 18, 23),
	[QPMAX_AREA4]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(1), 24, 29),

	[QPMIN_AREA5]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(2), 0, 5),
	[QPMAX_AREA5]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(2), 6, 11),
	[QPMIN_AREA6]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(2), 12, 17),
	[QPMAX_AREA6]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(2), 18, 23),
	[QPMIN_AREA7]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(2), 24, 29),

	[QPMAX_AREA7]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(3), 0, 5),
	[QPMAP_MODE]		= REG_FIELD(RKVENC_VEPU540_L1_ROI_QTHD(3), 30, 31),

	[PIC_OFST_Y]		= REG_FIELD(RKVENC_VEPU540_L1_PIC_OFST, 0, 12),
	[PIC_OFST_X]		= REG_FIELD(RKVENC_VEPU540_L1_PIC_OFST, 16, 28),

	[SRC_STRD0]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_STRD, 0, 15),
	[SRC_STRD1]		= REG_FIELD(RKVENC_VEPU540_L1_SRC_STRD, 16, 31),

	[ADR_SRC0]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_SRC0, 0, 31),

	[ADR_SRC1]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_SRC1, 0, 31),

	[ADR_SRC2]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_SRC2, 0, 31),

	[ROI_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_ROI, 4, 31),

	[RFPW_H_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_RFPW_H, 12, 31),

	[RFPW_B_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_RFPW_B, 12, 31),

	[RFPR_H_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_RFPR_H, 12, 31),

	[RFPR_B_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_RFPR_B, 12, 31),

	[CMVW_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_CMVW, 10, 31),

	[CMVR_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_CMVR, 10, 31),

	[DSPW_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_DSPW, 10, 31),

	[DSPR_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_DSPR, 10, 31),

	[MEIW_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_MEIW, 10, 31),

	[BSBT_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_BSBT, 7, 31),

	[BSBB_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_BSBB, 7, 31),

	[BSBR_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_BSBR, 7, 31),

	[ADR_BSBS]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_BSBS, 0, 31),

	[SLI_SPLT]		= REG_FIELD(RKVENC_VEPU540_L1_SLI_SPLT, 0, 0),
	[SLI_SPLT_MODE]		= REG_FIELD(RKVENC_VEPU540_L1_SLI_SPLT, 1, 1),
	[SLI_SPLT_CPST]		= REG_FIELD(RKVENC_VEPU540_L1_SLI_SPLT, 2, 2),
	[SLI_MAX_NUM_M1]	= REG_FIELD(RKVENC_VEPU540_L1_SLI_SPLT, 3, 12),
	[SLI_FLSH]		= REG_FIELD(RKVENC_VEPU540_L1_SLI_SPLT, 13, 13),
	[SLI_SPLT_CNUM_M1]	= REG_FIELD(RKVENC_VEPU540_L1_SLI_SPLT, 16, 31),

	[SLI_SPLT_BYTE]		= REG_FIELD(RKVENC_VEPU540_L1_SLI_BYTE, 0, 17),

	[CME_SRCH_H]		= REG_FIELD(RKVENC_VEPU540_L1_MR_RNGE, 0, 3),
	[CME_SRCH_V]		= REG_FIELD(RKVENC_VEPU540_L1_MR_RNGE, 4, 7),
	[RME_SRCH_H]		= REG_FIELD(RKVENC_VEPU540_L1_MR_RNGE, 8, 10),
	[RME_SRCH_V]		= REG_FIELD(RKVENC_VEPU540_L1_MR_RNGE, 11, 13),
	[DLT_FRM_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_MR_RNGE, 16, 31),

	[PMV_MDST_H]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 0, 7),
	[PMV_MDST_V]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 8, 15),
	[MV_LIMIT]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 16, 17),
	[PMV_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 18, 19),
	[COLMV_STOR]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 20, 20),
	[COLMV_LOAD]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 21, 21),
	[RME_DIS]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 22, 26),
	[FME_DIS]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 27, 31),

	[CME_RAMA_MAX]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CACH, 0, 10),
	[CME_RAMA_H]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CACH, 11, 15),
	[CACH_L2_MAP]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CACH, 16, 17),
	[CME_LINEBUF_W]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CACH, 18, 25),

	[POC_LSB_LT1]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 0, 15),
	[POC_LSB_LT2]		= REG_FIELD(RKVENC_VEPU540_L1_ME_CFG, 16, 31),

	[DLT_POC_MSB_CYCL1]	= REG_FIELD(RKVENC_VEPU540_L1_SYNT_LONG_REFM0, 0, 15),
	[DLT_POC_MSB_CYCL2]	= REG_FIELD(RKVENC_VEPU540_L1_SYNT_LONG_REFM0, 16, 31),

	[OSD_CH_INV_EN]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV_CFG, 0, 7),
	[OSD_ITYPE]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV_CFG, 8, 15),
	[OSD_LU_INV_MSK]	= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV_CFG, 16, 23),
	[OSD_CH_INV_MSK]	= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV_CFG, 24, 31),

	[VTHD_Y]		= REG_FIELD(RKVENC_VEPU540_L1_IPRD_CSTS, 0, 11),
	[VTHD_C]		= REG_FIELD(RKVENC_VEPU540_L1_IPRD_CSTS, 16, 27),

	[RECT_SIZE]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 0, 0),
	[INTER_4X4]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 1, 1),
	[ARB_SEL]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 2, 2),
	[VLC_LMT]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 3, 3),
	[CHRM_SPCL]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 4, 4),
	[RDO_MASK]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 5, 12),
	[CCWA_E]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 13, 13),
	[SCL_LST_SEL]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 14, 14),
	[ATR_E]			= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 15, 15),
	[ATF_EDG]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 16, 17),
	[ATF_LVL_E]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 18, 18),
	[ATF_INTRA_E]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 19, 19),
	[SCL_LST_SEL_]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 20, 21),
	[SATD_BYPS_FLG]		= REG_FIELD(RKVENC_VEPU540_L1_RDO_CFG_H264, 31, 31),

	[NAL_REF_IDC]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_NAL, 0, 1),
	[NAL_UNIT_TYPE]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_NAL, 2, 6),

	[MAX_FNUM]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SPS, 0, 3),
	[DRCT_8X8]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SPS, 4, 4),
	[MPOC_LM4]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SPS, 5, 8),

	[ETPY_MODE]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 0, 0),
	[TRNS_8X8]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 1, 1),
	[CSIP_FLAG]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 2, 2),
	[NUM_REF0_IDX]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 3, 4),
	[NUM_REF1_IDX]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 5, 6),
	[PIC_INIT_QP]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 7, 12),
	[CB_OFST]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 13, 17),
	[CR_OFST]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 18, 22),
	[WGHT_PRED]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 23, 23),
	[DBF_CP_FLG]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_PPS, 24, 24),

	[SLI_TYPE]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(0), 0, 1),
	[PPS_ID]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(0), 2, 9),
	[DRCT_SMVP]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(0), 10, 10),
	[NUM_REF_OVRD]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(0), 11, 11),
	[CBC_INIT_IDC]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(0), 12, 13),
	[FRM_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(0), 16, 31),

	[IDR_PIC_ID]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(1), 0, 15),
	[POC_LSB]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(1), 16, 31),

	[RODR_PIC_IDX]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(2), 0, 1),
	[REF_LIST0_RODR]	= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(2), 2, 2),
	[SLI_BETA_OFST]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(2), 3, 6),
	[SLI_ALPH_OFST]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(2), 7, 10),
	[DIS_DBLK_IDC]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(2), 11, 12),
	[RODR_PIC_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_SLI(2), 16, 31),

	[NOPP_FLG]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 0, 0),
	[LTRF_FLG]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 1, 1),
	[ARPM_FLG]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 2, 2),
	[MMCO4_PRE]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 3, 3),
	[MMCO_TYPE0]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 4, 6),
	[MMCO_PARM0]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 7, 22),
	[MMCO_TYPE1]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 23, 25),
	[MMCO_TYPE2]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM0, 26, 28),

	[MMCO_PARM1]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM1, 0, 15),
	[MMCO_PARM2]		= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM1, 16, 31),

	[OSD_E]			= REG_FIELD(RKVENC_VEPU540_L1_OSD_CFG, 0, 7),
	[OSD_INV_E]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_CFG, 8, 15),
	[OSD_PLT_CKS]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_CFG, 16, 16),
	[OSD_PLT_TYP]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_CFG, 17, 17),

	[OSD_ITHD_R0]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 0, 3),
	[OSD_ITHD_R1]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 4, 7),
	[OSD_ITHD_R2]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 8, 11),
	[OSD_ITHD_R3]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 12, 15),
	[OSD_ITHD_R4]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 16, 19),
	[OSD_ITHD_R5]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 20, 23),
	[OSD_ITHD_R6]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 24, 27),
	[OSD_ITHD_R7]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_INV, 28, 31),

	[LONG_TERM_FRAME_IDX0]	= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM2, 0, 3),
	[LONG_TERM_FRAME_IDX1]	= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM2, 4, 7),
	[LONG_TERM_FRAME_IDX2]	= REG_FIELD(RKVENC_VEPU540_L1_SYNT_REFM2, 8, 11),

	[OSD0_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(0), 0, 7),
	[OSD0_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(0), 8, 15),
	[OSD0_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(0), 16, 23),
	[OSD0_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(0), 24, 31),

	[OSD1_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(1), 0, 7),
	[OSD1_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(1), 8, 15),
	[OSD1_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(1), 16, 23),
	[OSD1_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(1), 24, 31),

	[OSD2_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(2), 0, 7),
	[OSD2_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(2), 8, 15),
	[OSD2_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(2), 16, 23),
	[OSD2_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(2), 24, 31),

	[OSD3_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(3), 0, 7),
	[OSD3_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(3), 8, 15),
	[OSD3_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(3), 16, 23),
	[OSD3_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(3), 24, 31),

	[OSD4_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(4), 0, 7),
	[OSD4_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(4), 8, 15),
	[OSD4_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(4), 16, 23),
	[OSD4_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(4), 24, 31),

	[OSD5_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(5), 0, 7),
	[OSD5_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(5), 8, 15),
	[OSD5_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(5), 16, 23),
	[OSD5_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(5), 24, 31),

	[OSD6_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(6), 0, 7),
	[OSD6_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(6), 8, 15),
	[OSD6_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(6), 16, 23),
	[OSD6_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(6), 24, 31),

	[OSD7_POS_LB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(7), 0, 7),
	[OSD7_POS_LB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(7), 8, 15),
	[OSD7_POS_RB_X]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(7), 16, 23),
	[OSD7_POS_RB_Y]		= REG_FIELD(RKVENC_VEPU540_L1_OSD_POS(7), 24, 31),

	[OSD_ADDR0]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(0), 4, 31),
	[OSD_ADDR1]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(1), 4, 31),
	[OSD_ADDR2]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(2), 4, 31),
	[OSD_ADDR3]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(3), 4, 31),
	[OSD_ADDR4]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(4), 4, 31),
	[OSD_ADDR5]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(5), 4, 31),
	[OSD_ADDR6]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(6), 4, 31),
	[OSD_ADDR7]		= REG_FIELD(RKVENC_VEPU540_L1_ADR_OSD(7), 4, 31),

	[BS_LGTH]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BSL, 0, 26),

	[SSE_L32]		= REG_FIELD(RKVENC_VEPU540_L1_ST_SSE_LE32, 0, 31),

	[QP_SUM]		= REG_FIELD(RKVENC_VEPU540_L1_ST_SSE_QP, 24, 31),
	[SSE_H8]		= REG_FIELD(RKVENC_VEPU540_L1_ST_SSE_QP, 0, 21),

	[SAO_CNUM]		= REG_FIELD(RKVENC_VEPU540_L1_ST_SAO, 0, 11),
	[SAO_YNUM]		= REG_FIELD(RKVENC_VEPU540_L1_ST_SAO, 12, 23),

	[ST_ENC]		= REG_FIELD(RKVENC_VEPU540_L1_ST_ENC, 0, 1),
	[ST_SCLR]		= REG_FIELD(RKVENC_VEPU540_L1_ST_ENC, 2, 2),

	[FNUM_ENC]		= REG_FIELD(RKVENC_VEPU540_L1_ST_LKT, 0, 7),
	[FNUM_CFG]		= REG_FIELD(RKVENC_VEPU540_L1_ST_LKT, 8, 15),
	[FNUM_INT]		= REG_FIELD(RKVENC_VEPU540_L1_ST_LKT, 16, 23),

	[NODE_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ST_NADR, 4, 31),

	[BSBW_ADDR]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BSB, 4, 31),

	[AXIB_IDL]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BUS, 0, 7),
	[AXIB_OVFL]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BUS, 8, 15),
	[AXIB_ERR]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BUS, 16, 23),
	[AXIR_ERR]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BUS, 24, 30),

	[SLI_NUM]		= REG_FIELD(RKVENC_VEPU540_L1_ST_SNUM, 0, 5),

	[SLI_LEN]		= REG_FIELD(RKVENC_VEPU540_L1_ST_SLEN, 0, 24),

	[PNUM_P64]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_P64, 0, 11),

	[PNUM_P32]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_P32, 0, 13),

	[PNUM_P16]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_P16, 0, 15),

	[PNUM_P8]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_P8, 0, 17),

	[PNUM_I32]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_I32, 0, 13),

	[PNUM_I16]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_I16, 0, 15),

	[PNUM_I8]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_I8, 0, 17),

	[PNUM_I4]		= REG_FIELD(RKVENC_VEPU540_L1_ST_PNUM_I4, 0, 19),

	//	[NUM_QP][52]		= REG_FIELD(, , ),

	[MADP]			= REG_FIELD(RKVENC_VEPU540_L1_ST_CPLX_TMP, 0, 17),

	[NUM_CTU]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BNUM_CME, 0, 15),

	[MADI]			= REG_FIELD(RKVENC_VEPU540_L1_ST_CPLX_SPT, 0, 31),

	[NUM_B16]		= REG_FIELD(RKVENC_VEPU540_L1_ST_BNUM_B16, 0, 16),
};


#define RKVENC_VEPU540_L2_IPRD_TTHDY4_(x)	(0x4 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_IPRD_TTHDC8_(x)	(0xc + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_IPRD_TTHDY8_(x)	(0x14 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_IPRD_TTHD_UL		0x1c
#define RKVENC_VEPU540_L2_IPRD_WGTY8		0x20
#define RKVENC_VEPU540_L2_IPRD_WGTY4		0x24
#define RKVENC_VEPU540_L2_IPRD_WGTY16		0x28
#define RKVENC_VEPU540_L2_IPRD_WGTC8		0x2c
#define RKVENC_VEPU540_L2_QNT_BIAS_COMB		0x30
#define RKVENC_VEPU540_L2_ATR_THD(x)		(0x34 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_ATR_WGT16		0x3c
#define RKVENC_VEPU540_L2_ATR_WGT8		0x40
#define RKVENC_VEPU540_L2_ATR_WGT4		0x44
#define RKVENC_VEPU540_L2_ATF_TTHD(x)		(0x48 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_ATF_STHD(x)		(0x50 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_ATF_WGT(x)		(0x58 + x * 0x4) // -> 0 - 2
#define RKVENC_VEPU540_L2_ATF_OFST(x)		(0x64 + x * 0x4) // -> 0 - 2
#define RKVENC_VEPU540_L2_IPRD_WGT_QP(x)	(0x70 + x * 0x4) // -> 0 - 51
#define RKVENC_VEPU540_L2_RDO_WGTA_QP(x)	(0x140 + x * 0x4) // -> 0 - 51
#define RKVENC_VEPU540_L2_RDO_WGTB_QP(x)	(0x210 + x * 0x4) // -> 0 - 51
#define RKVENC_VEPU540_L2_MADI_CFG		0x2e0
#define RKVENC_VEPU540_L2_AQ_TTHD(x)		(0x2e4 + x * 0x4) // -> 0 - 3
#define RKVENC_VEPU540_L2_AQ_STP(x)		(0x2f4 + x * 0x4) // -> 0 - 3
#define RKVENC_VEPU540_L2_RME_MVD_PNSH		0x304
#define RKVENC_VEPU540_L2_ATR1_THD(x)		(0x308 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_PREI_DIF_IDX_L(x)	(0x310 + x * 0x8) // -> 0 - 16
#define RKVENC_VEPU540_L2_PREI_DIF_IDX_H(x)	(0x314 + x * 0x8) // -> 0 - 16
#define RKVENC_VEPU540_L2_RDO_CKG		0x400
#define RKVENC_VEPU540_L2_I16_SOBEL_T		0x410
#define RKVENC_VEPU540_L2_I16_SOBEL_A(x)	(0x414 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_I16_SOBEL_B(x)	(0x41c + x * 0x4) // -> 0 - 4
#define RKVENC_VEPU540_L2_I16_SOBEL_C(x)	(0x430 + x * 0x4) // -> 0 - 1
#define RKVENC_VEPU540_L2_I16_SOBEL_D(x)	(0x438 + x * 0x4) // -> 0 - 4
#define RKVENC_VEPU540_L2_I16_SOBEL_E(x)	(0x44c + x * 0x4) // -> 0 - 16
#define RKVENC_VEPU540_L2_I32_SOBEL_T(x)	(0x494 + x * 0x4) // -> 0 - 2
#define RKVENC_VEPU540_L2_I32_SOBEL_A		0x4a0
#define RKVENC_VEPU540_L2_I32_SOBEL_B(x)	(0x4a4 + x * 0x4) // -> 0 - 2
#define RKVENC_VEPU540_L2_I32_SOBEL_C		0x4b0
#define RKVENC_VEPU540_L2_I32_SOBEL_D(x)	(0x4b4 + x * 0x4) // -> 0 - 2
#define RKVENC_VEPU540_L2_I32_SOBEL_E(x)	(0x4c0 + x * 0x4) // -> 0 - 9

enum rkvenc_vepu540_regfields_l2 {
	/*
	 * IPRD_TTHDY4_0_H264 ~ IPRD_TTHDY4_1_H264
	 * Address: 0x0004~0x0008 Access type: read and write
	 * The texture thredsholds for H.264 LUMA 4x4 intra prediction
	 */
	IPRD_TTHDY4_0,
	IPRD_TTHDY4_1,
	IPRD_TTHDY4_2,
	IPRD_TTHDY4_3,

	/*
	 * IPRD_TTHDC8_0_H264 ~ IPRD_TTHDC8_1_H264
	 * Address: 0x000C~0x0010 Access type: read and write
	 * The texture threshold for H.264 CHROMA 8x8 intra prediction.
	 */
	IPRD_TTHDC8_0,
	IPRD_TTHDC8_1,
	IPRD_TTHDC8_2,
	IPRD_TTHDC8_3,

	/*
	 * IPRD_TTHDY8_0_H264 ~ IPRD_TTHDY8_1_H264
	 * Address: 0x0014~0x0018 Access type: read and write
	 * The texture thredsholds for H.264 LUMA 8x8 intra prediction
	 */
	IPRD_TTHDY8_0,
	IPRD_TTHDY8_1,
	IPRD_TTHDY8_2,
	IPRD_TTHDY8_3,

	/*
	 * IPRD_TTHD_UL_H264
	 * Address: 0x001C Access type: read and write
	 * Texture thredsholds of up and left MB for H.264 LUMA intra prediction.
	 */
	IPRD_TTHD_UL,

	/*
	 * IPRD_WGTY8_H264
	 * Address: 0x0020 Access type: read and write
	 * Weights of the cost for H.264 LUMA 8x8 intra prediction
	 */
	IPRD_WGTY8_0,
	IPRD_WGTY8_1,
	IPRD_WGTY8_2,
	IPRD_WGTY8_3,

	/*
	 * IPRD_WGTY4_H264
	 * Address: 0x0024 Access type: read and write
	 * Weights of the cost for H.264 LUMA 4x4 intra prediction
	 */
	IPRD_WGTY4_0,
	IPRD_WGTY4_1,
	IPRD_WGTY4_2,
	IPRD_WGTY4_3,

	/*
	 * IPRD_WGTY16_H264
	 * Address: 0x0028 Access type: read and write
	 * Weights of the cost for H.264 LUMA 16x16 intra prediction
	 */
	IPRD_WGTY16_0,
	IPRD_WGTY16_1,
	IPRD_WGTY16_2,
	IPRD_WGTY16_3,

	/*
	 * IPRD_WGTC8_H264
	 * Address: 0x002C Access type: read and write
	 * Weights of the cost for H.264 CHROMA 8x8 intra prediction
	 */
	IPRD_WGTC8_0,
	IPRD_WGTC8_1,
	IPRD_WGTC8_2,
	IPRD_WGTC8_3,

	/*
	 * QNT_BIAS_COMB
	 * Address: 0x0030 Access type: read and write
	 * Quantization bias for H.264 and HEVC.
	 */
	/* Quantization bias for HEVC and H.264 I frame. */
	QNT_BIAS_I,
	/* Quantization bias for HEVC and H.264 P frame. */
	QNT_BIAS_P,

	/*
	 * ATR_THD0_H264
	 * Address: 0x0034 Access type: read and write
	 * H.264 anti ringing noise threshold configuration0.
	 */
	/* The 1st threshold for H.264 anti-ringing-noise. */
	ATR_THD0,
	/* The 2nd threshold for H.264 anti-ringing-noise. */
	ATR_THD1,

	/*
	 * ATR_THD1_H264
	 * Address: 0x0038 Access type: read and write
	 * H.264 anti ringing noise threshold configuration1.
	 */
	/* The 3rd threshold for H.264 anti-ringing-noise. */
	ATR_THD2,
	/* QP threshold of P frame for H.264 anti-ringing-nois. */
	ATR_QP,

	/*
	 * ATR_WGT16_H264
	 * Address: 0x003C Access type: read and write
	 * Weights of 16x16 cost for H.264 anti ringing noise.
	 */
	/* The 1st weight for H.264 16x16 anti-ringing-noise. */
	ATR_LV16_WGT0,
	/* The 2nd weight for H.264 16x16 anti-ringing-noise. */
	ATR_LV16_WGT1,
	/* The 3rd weight for H.264 16x16 anti-ringing-noise. */
	ATR_LV16_WGT2,

	/*
	 * ATR_WGT8_H264
	 * Address: 0x0040 Access type: read and write
	 * Weights of 8x8 cost for H.264 anti ringing noise.
	 */
	/* The 1st weight for H.264 8x8 anti-ringing-noise. */
	ATR_LV8_WGT0,
	/* The 2nd weight for H.264 8x8 anti-ringing-noise. */
	ATR_LV8_WGT1,
	/* The 3rd weight for H.264 8x8 anti-ringing-noise. */
	ATR_LV8_WGT2,

	/*
	 * ATR_WGT4_H264
	 * Address: 0x0044 Access type: read and write
	 * Weights of 4x4 cost for H.264 anti ringing noise.
	 */
	/* The 1st weight for H.264 4x4 anti-ringing-noise. */
	ATR_LV4_WGT0,
	/* The 2nd weight for H.264 4x4 anti-ringing-noise. */
	ATR_LV4_WGT1,
	/* The 3rd weight for H.264 4x4 anti-ringing-noise. */
	ATR_LV4_WGT2,

	/*
	 * ATF_TTHD0_H264 ~ ATF_TTHD1_H264
	 * Address: 0x0048~0x004C Access type: read and write
	 * Texture threshold configuration for H.264 anti-flicker
	 */
	ATF_TTHD0,
	ATF_TTHD1,
	ATF_TTHD2,
	ATF_TTHD3,

	/*
	 * ATF_STHD0_H264
	 * Address: 0x0050 Access type: read and write
	 * (CME) SAD threshold configuration1 for H.264 anti-flicker.
	 */
	/* (CME) SAD threshold0 of texture interval1 for H.264 anti-flicker. */
	ATF_STHD_10,
	/* Max (CME) SAD threshold for H.264 anti-flicker. */
	ATF_STHD_MAX,

	/*
	 * ATF_STHD1_H264
	 * Address: 0x0054 Access type: read and write
	 * (CME) SAD threshold configuration1 for H.264 anti-flicker.
	 */
	/* (CME) SAD threshold1 of texture interval1 for H.264 anti-flicker. */
	ATF_STHD_11,
	/* (CME) SAD threshold0 of texture interval2 for H.264 anti-flicker. */
	ATF_STHD_20,

	/*
	 * ATF_WGT0_H264
	 * Address: 0x0058 Access type: read and write
	 * Weight configuration0 for H.264 anti-flicker.
	 */
	/* The 1st weight in texture interval1 for H.264 anti-flicker. */
	ATF_WGT10,
	/* The 2nd weight in texture interval1 for H.264 anti-flicker. */
	ATF_WGT11,

	/*
	 * ATF_WGT1_H264
	 * Address: 0x005C Access type: read and write
	 * Weight configuration1 for H.264 anti-flicker.
	 */
	/* The 3rd weight in texture interval1 for H.264 anti-flicker. */
	ATF_WGT12,
	/* The 1st weight in texture interval2 for H.264 anti-flicker. */
	ATF_WGT20,

	/*
	 * ATF_WGT2_H264
	 * Address: 0x0060 Access type: read and write
	 * Weight configuration2 for H.264 anti-flicker.
	 */
	/* The 2nd weight in texture interval2 for H.264 anti-flicker. */
	ATF_WGT21,
	/* The weight in texture interval3 for H.264 anti-flicker. */
	ATF_WGT30,

	/*
	 * ATF_OFST0_H264
	 * Address: 0x0064 Access type: read and write
	 * Offset configuration0 for H.264 anti-flicker.
	 */
	/* The 1st offset in texture interval1 for H.264 anti-flicker. */
	ATF_OFST10,
	/* The 2nd offset in texture interval1 for H.264 anti-flicker. */
	ATF_OFST11,

	/*
	 * ATF_OFST1_H264
	 * Address: 0x0068 Access type: read and write
	 * Offset configuration1 for H.264 anti-flicker.
	 */
	/* The 3rd offset in texture interval1 for H.264 anti-flicker. */
	ATF_OFST12,
	/* The 1st offset in texture interval2 for H.264 anti-flicker. */
	ATF_OFST20,

	/*
	 * ATF_OFST2_H264
	 * Address: 0x006C Access type: read and write
	 * Offset configuration2 for H.264 anti-flicker.
	 */
	/* The 2nd offset in texture interval1 for H.264 anti-flicker. */
	ATF_OFST21,
	/* The offset in texture interval3 for H.264 anti-flicker. */
	ATF_OFST30,

	/*
	 * IPRD_WGT_QP0_HEVC ~ IPRD_WGT_QP51_HEVC
	 * Address: 0x0070 ~ 0x013C Access type: read and write
	 * Weight of SATD cost when QP is 0~51 for HEVC intra prediction.
	 */
	IPRD_WGT_QP0,
	IPRD_WGT_QP1,
	IPRD_WGT_QP2,
	IPRD_WGT_QP3,
	IPRD_WGT_QP4,
	IPRD_WGT_QP5,
	IPRD_WGT_QP6,
	IPRD_WGT_QP7,
	IPRD_WGT_QP8,
	IPRD_WGT_QP9,
	IPRD_WGT_QP10,
	IPRD_WGT_QP11,
	IPRD_WGT_QP12,
	IPRD_WGT_QP13,
	IPRD_WGT_QP14,
	IPRD_WGT_QP15,
	IPRD_WGT_QP16,
	IPRD_WGT_QP17,
	IPRD_WGT_QP18,
	IPRD_WGT_QP19,
	IPRD_WGT_QP20,
	IPRD_WGT_QP21,
	IPRD_WGT_QP22,
	IPRD_WGT_QP23,
	IPRD_WGT_QP24,
	IPRD_WGT_QP25,
	IPRD_WGT_QP26,
	IPRD_WGT_QP27,
	IPRD_WGT_QP28,
	IPRD_WGT_QP29,
	IPRD_WGT_QP30,
	IPRD_WGT_QP31,
	IPRD_WGT_QP32,
	IPRD_WGT_QP33,
	IPRD_WGT_QP34,
	IPRD_WGT_QP35,
	IPRD_WGT_QP36,
	IPRD_WGT_QP37,
	IPRD_WGT_QP38,
	IPRD_WGT_QP39,
	IPRD_WGT_QP40,
	IPRD_WGT_QP41,
	IPRD_WGT_QP42,
	IPRD_WGT_QP43,
	IPRD_WGT_QP44,
	IPRD_WGT_QP45,
	IPRD_WGT_QP46,
	IPRD_WGT_QP47,
	IPRD_WGT_QP48,
	IPRD_WGT_QP49,
	IPRD_WGT_QP50,
	IPRD_WGT_QP51,

	/*
	 * RDO_WGTA_QP0_COMB ~ RDO_WGTA_QP51_COMB
	 * Address: 0x0140 ~ 0x020C Access type: read and write
	 * Weight of group A for HEVC and H.264 RDO mode decision when QP is 0~51.
	 */
	WGT_QP_GRPA0,
	WGT_QP_GRPA1,
	WGT_QP_GRPA2,
	WGT_QP_GRPA3,
	WGT_QP_GRPA4,
	WGT_QP_GRPA5,
	WGT_QP_GRPA6,
	WGT_QP_GRPA7,
	WGT_QP_GRPA8,
	WGT_QP_GRPA9,
	WGT_QP_GRPA10,
	WGT_QP_GRPA11,
	WGT_QP_GRPA12,
	WGT_QP_GRPA13,
	WGT_QP_GRPA14,
	WGT_QP_GRPA15,
	WGT_QP_GRPA16,
	WGT_QP_GRPA17,
	WGT_QP_GRPA18,
	WGT_QP_GRPA19,
	WGT_QP_GRPA20,
	WGT_QP_GRPA21,
	WGT_QP_GRPA22,
	WGT_QP_GRPA23,
	WGT_QP_GRPA24,
	WGT_QP_GRPA25,
	WGT_QP_GRPA26,
	WGT_QP_GRPA27,
	WGT_QP_GRPA28,
	WGT_QP_GRPA29,
	WGT_QP_GRPA30,
	WGT_QP_GRPA31,
	WGT_QP_GRPA32,
	WGT_QP_GRPA33,
	WGT_QP_GRPA34,
	WGT_QP_GRPA35,
	WGT_QP_GRPA36,
	WGT_QP_GRPA37,
	WGT_QP_GRPA38,
	WGT_QP_GRPA39,
	WGT_QP_GRPA40,
	WGT_QP_GRPA41,
	WGT_QP_GRPA42,
	WGT_QP_GRPA43,
	WGT_QP_GRPA44,
	WGT_QP_GRPA45,
	WGT_QP_GRPA46,
	WGT_QP_GRPA47,
	WGT_QP_GRPA48,
	WGT_QP_GRPA49,
	WGT_QP_GRPA50,
	WGT_QP_GRPA51,

	/*
	 * RDO_WGTB_QP0_COMB ~ RDO_WGTB_QP51_COMB
	 * Address: 0x0210 ~ 0x02DC Access type: read and write
	 * Weight of group B for HEVC and H.264 RDO mode decision when QP is 0~51.
	 */
	WGT_QP_GRPB0,
	WGT_QP_GRPB1,
	WGT_QP_GRPB2,
	WGT_QP_GRPB3,
	WGT_QP_GRPB4,
	WGT_QP_GRPB5,
	WGT_QP_GRPB6,
	WGT_QP_GRPB7,
	WGT_QP_GRPB8,
	WGT_QP_GRPB9,
	WGT_QP_GRPB10,
	WGT_QP_GRPB11,
	WGT_QP_GRPB12,
	WGT_QP_GRPB13,
	WGT_QP_GRPB14,
	WGT_QP_GRPB15,
	WGT_QP_GRPB16,
	WGT_QP_GRPB17,
	WGT_QP_GRPB18,
	WGT_QP_GRPB19,
	WGT_QP_GRPB20,
	WGT_QP_GRPB21,
	WGT_QP_GRPB22,
	WGT_QP_GRPB23,
	WGT_QP_GRPB24,
	WGT_QP_GRPB25,
	WGT_QP_GRPB26,
	WGT_QP_GRPB27,
	WGT_QP_GRPB28,
	WGT_QP_GRPB29,
	WGT_QP_GRPB30,
	WGT_QP_GRPB31,
	WGT_QP_GRPB32,
	WGT_QP_GRPB33,
	WGT_QP_GRPB34,
	WGT_QP_GRPB35,
	WGT_QP_GRPB36,
	WGT_QP_GRPB37,
	WGT_QP_GRPB38,
	WGT_QP_GRPB39,
	WGT_QP_GRPB40,
	WGT_QP_GRPB41,
	WGT_QP_GRPB42,
	WGT_QP_GRPB43,
	WGT_QP_GRPB44,
	WGT_QP_GRPB45,
	WGT_QP_GRPB46,
	WGT_QP_GRPB47,
	WGT_QP_GRPB48,
	WGT_QP_GRPB49,
	WGT_QP_GRPB50,
	WGT_QP_GRPB51,

	/*
	 * MADI_CFG
	 * Address: 0x02E0 Access type: read and write
	 * MADI configuration for CU32 and CU64.
	 */
	/*
	 * MADI generation mode for CU32 and CU64.
	 * 1'h0: Follow 32x32 and 64x64 MADI functions.
	 * 1'h1: Calculated by the mean of corresponding CU16 MADIs.
	 */
	MADI_MODE,

	/*
	 * AQ_TTHD0 ~ AQ_TTHD3
	 * Address: 0x02E4 ~ 0x02F0 Access type: read and write
	 * Texture threshold configuration for adaptive QP adjustment.
	 */
	/* Texture threshold for adaptive QP adjustment. */
	AQ_TTHD0,
	AQ_TTHD1,
	AQ_TTHD2,
	AQ_TTHD3,
	AQ_TTHD4,
	AQ_TTHD5,
	AQ_TTHD6,
	AQ_TTHD7,
	AQ_TTHD8,
	AQ_TTHD9,
	AQ_TTHD10,
	AQ_TTHD11,
	AQ_TTHD12,
	AQ_TTHD13,
	AQ_TTHD14,
	AQ_TTHD15,

	/*
	 * AQ_STP0 ~ AQ_STP3
	 * Address: 0x02F4 ~ 0x300 Access type: read and write
	 * Adjustment step configuration0 for adaptive QP adjustment.
	 */
	/*
	 * MADI generation mode for CU32 and CU64.
	 * 1'h0: Follow 32x32 and 64x64 MADI functions.
	 * 1'h1: Calculated by the mean of corresponding CU16 MADIs.
	 */
	/* QP adjust step when current texture strength is between n-1 and n step. */
	AQ_STEP0,
	AQ_STEP1,
	AQ_STEP2,
	AQ_STEP3,
	AQ_STEP4,
	AQ_STEP5,
	AQ_STEP6,
	AQ_STEP7,
	AQ_STEP8,
	AQ_STEP9,
	AQ_STEP10,
	AQ_STEP11,
	AQ_STEP12,
	AQ_STEP13,
	AQ_STEP14,
	AQ_STEP15,

	/*
	 * RME_MVD_PNSH_H264
	 * Address: 0x0304 Access type: read and write
	 * RME MVD(motion vector difference) cost penalty, H.264 only.
	 */
	/* MVD cost penalty enable. */
	MVD_PNLT_E,
	/* MVD cost penalty coefficienc. */
	MVD_PNLT_COEF,
	/* MVD cost penalty constant. */
	MVD_PNLT_CNST,
	/* Low threshold of the MVs which should be punished. */
	MVD_PNLT_LTHD,
	/* High threshold of the MVs which should be punished. */
	MVD_PNLT_HTHD,

	/*
	 * ATR1_THD0_H264
	 * Address: 0x0308 Access type: read and write
	 * H.264 anti ringing noise threshold configuration0 of group1.
	 */
	/* The 1st threshold for H.264 anti-ringing-noise of group1. */
	ATR1_THD0,
	/* The 2nd threshold for H.264 anti-ringing-noise of group1. */
	ATR1_THD1,

	/*
	 * ATR1_THD0_H264
	 * Address: 0x030C Access type: read and write
	 * H.264 anti ringing noise threshold configuration1 of group1.
	 */
	/* The 3rd threshold for H.264 anti-ringing-noise of group1. */
	ATR1_THD2,

	/* sentinel */
	L2_MAX_FIELDS
};

static const struct reg_field rkvenc_vepu540_l2_fields[] = {
	[IPRD_TTHDY4_0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY4_(0), 0, 11),
	[IPRD_TTHDY4_1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY4_(0), 16, 27),
	[IPRD_TTHDY4_2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY4_(1), 0, 11),
	[IPRD_TTHDY4_3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY4_(1), 16, 27),

	[IPRD_TTHDC8_0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDC8_(0), 0, 11),
	[IPRD_TTHDC8_1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDC8_(0), 16, 27),
	[IPRD_TTHDC8_2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDC8_(1), 0, 11),
	[IPRD_TTHDC8_3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDC8_(1), 16, 27),

	[IPRD_TTHDY8_0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY8_(0), 0, 11),
	[IPRD_TTHDY8_1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY8_(0), 16, 27),
	[IPRD_TTHDY8_2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY8_(1), 0, 11),
	[IPRD_TTHDY8_3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHDY8_(1), 16, 27),

	[IPRD_TTHD_UL]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_TTHD_UL, 0, 11),

	[IPRD_WGTY8_0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY8, 0, 7),
	[IPRD_WGTY8_1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY8, 8, 15),
	[IPRD_WGTY8_2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY8, 16, 23),
	[IPRD_WGTY8_3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY8, 24, 31),

	[IPRD_WGTY4_0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY4, 0, 7),
	[IPRD_WGTY4_1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY4, 8, 15),
	[IPRD_WGTY4_2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY4, 16, 23),
	[IPRD_WGTY4_3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY4, 24, 31),

	[IPRD_WGTY16_0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY16, 0, 7),
	[IPRD_WGTY16_1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY16, 8, 15),
	[IPRD_WGTY16_2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY16, 16, 23),
	[IPRD_WGTY16_3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTY16, 24, 31),

	[IPRD_WGTC8_0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTC8, 0, 7),
	[IPRD_WGTC8_1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTC8, 8, 15),
	[IPRD_WGTC8_2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTC8, 16, 23),
	[IPRD_WGTC8_3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGTC8, 24, 31),

	[QNT_BIAS_I]	= REG_FIELD(RKVENC_VEPU540_L2_QNT_BIAS_COMB, 0, 9),
	[QNT_BIAS_P]	= REG_FIELD(RKVENC_VEPU540_L2_QNT_BIAS_COMB, 10, 19),

	[ATR_THD0]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_THD(0), 0, 11),
	[ATR_THD1]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_THD(0), 16, 27),

	[ATR_THD2]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_THD(1), 0, 11),
	[ATR_QP]		= REG_FIELD(RKVENC_VEPU540_L2_ATR_THD(1), 16, 21),

	[ATR_LV16_WGT0]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT16, 0, 7),
	[ATR_LV16_WGT1]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT16, 8, 15),
	[ATR_LV16_WGT2]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT16, 16, 23),

	[ATR_LV8_WGT0]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT8, 0, 7),
	[ATR_LV8_WGT1]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT8, 8, 15),
	[ATR_LV8_WGT2]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT8, 16, 23),

	[ATR_LV4_WGT0]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT4, 0, 7),
	[ATR_LV4_WGT1]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT4, 8, 15),
	[ATR_LV4_WGT2]	= REG_FIELD(RKVENC_VEPU540_L2_ATR_WGT4, 16, 23),

	[ATF_TTHD0]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_TTHD(0), 0, 11),
	[ATF_TTHD1]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_TTHD(0), 16, 27),
	[ATF_TTHD2]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_TTHD(1), 0, 11),
	[ATF_TTHD3]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_TTHD(1), 16, 27),

	[ATF_STHD_10]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_STHD(0), 0, 13),
	[ATF_STHD_MAX]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_STHD(0), 16, 29),

	[ATF_STHD_11]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_STHD(1), 0, 13),
	[ATF_STHD_20]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_STHD(1), 16, 29),

	[ATF_WGT10]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_WGT(0), 0, 8),
	[ATF_WGT11]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_WGT(0), 16, 24),

	[ATF_WGT12]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_WGT(1), 0, 8),
	[ATF_WGT20]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_WGT(1), 16, 24),

	[ATF_WGT21]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_WGT(2), 0, 8),
	[ATF_WGT30]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_WGT(2), 16, 24),

	[ATF_OFST10]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_OFST(0), 0, 13),
	[ATF_OFST11]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_OFST(0), 16, 29),

	[ATF_OFST12]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_OFST(1), 0, 13),
	[ATF_OFST20]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_OFST(1), 16, 29),

	[ATF_OFST21]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_OFST(2), 0, 13),
	[ATF_OFST30]	= REG_FIELD(RKVENC_VEPU540_L2_ATF_OFST(2), 16, 29),

	[IPRD_WGT_QP0]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(0), 0, 19),
	[IPRD_WGT_QP1]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(1), 0, 19),
	[IPRD_WGT_QP2]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(2), 0, 19),
	[IPRD_WGT_QP3]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(3), 0, 19),
	[IPRD_WGT_QP4]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(4), 0, 19),
	[IPRD_WGT_QP5]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(5), 0, 19),
	[IPRD_WGT_QP6]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(6), 0, 19),
	[IPRD_WGT_QP7]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(7), 0, 19),
	[IPRD_WGT_QP8]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(8), 0, 19),
	[IPRD_WGT_QP9]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(9), 0, 19),
	[IPRD_WGT_QP10]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(10), 0, 19),
	[IPRD_WGT_QP11]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(11), 0, 19),
	[IPRD_WGT_QP12]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(12), 0, 19),
	[IPRD_WGT_QP13]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(13), 0, 19),
	[IPRD_WGT_QP14]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(14), 0, 19),
	[IPRD_WGT_QP15]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(15), 0, 19),
	[IPRD_WGT_QP16]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(16), 0, 19),
	[IPRD_WGT_QP17]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(17), 0, 19),
	[IPRD_WGT_QP18]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(18), 0, 19),
	[IPRD_WGT_QP19]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(19), 0, 19),
	[IPRD_WGT_QP20]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(20), 0, 19),
	[IPRD_WGT_QP21]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(21), 0, 19),
	[IPRD_WGT_QP22]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(22), 0, 19),
	[IPRD_WGT_QP23]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(23), 0, 19),
	[IPRD_WGT_QP24]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(24), 0, 19),
	[IPRD_WGT_QP25]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(25), 0, 19),
	[IPRD_WGT_QP26]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(26), 0, 19),
	[IPRD_WGT_QP27]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(27), 0, 19),
	[IPRD_WGT_QP28]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(28), 0, 19),
	[IPRD_WGT_QP29]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(29), 0, 19),
	[IPRD_WGT_QP30]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(30), 0, 19),
	[IPRD_WGT_QP31]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(31), 0, 19),
	[IPRD_WGT_QP32]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(32), 0, 19),
	[IPRD_WGT_QP33]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(33), 0, 19),
	[IPRD_WGT_QP34]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(34), 0, 19),
	[IPRD_WGT_QP35]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(35), 0, 19),
	[IPRD_WGT_QP36]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(36), 0, 19),
	[IPRD_WGT_QP37]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(37), 0, 19),
	[IPRD_WGT_QP38]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(38), 0, 19),
	[IPRD_WGT_QP39]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(39), 0, 19),
	[IPRD_WGT_QP40]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(40), 0, 19),
	[IPRD_WGT_QP41]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(41), 0, 19),
	[IPRD_WGT_QP42]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(42), 0, 19),
	[IPRD_WGT_QP43]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(43), 0, 19),
	[IPRD_WGT_QP44]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(44), 0, 19),
	[IPRD_WGT_QP45]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(45), 0, 19),
	[IPRD_WGT_QP46]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(46), 0, 19),
	[IPRD_WGT_QP47]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(47), 0, 19),
	[IPRD_WGT_QP48]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(48), 0, 19),
	[IPRD_WGT_QP49]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(49), 0, 19),
	[IPRD_WGT_QP50]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(50), 0, 19),
	[IPRD_WGT_QP51]	= REG_FIELD(RKVENC_VEPU540_L2_IPRD_WGT_QP(51), 0, 19),

	[WGT_QP_GRPA0] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(0), 0, 23),
	[WGT_QP_GRPA1] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(1), 0, 23),
	[WGT_QP_GRPA2] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(2), 0, 23),
	[WGT_QP_GRPA3] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(3), 0, 23),
	[WGT_QP_GRPA4] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(4), 0, 23),
	[WGT_QP_GRPA5] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(5), 0, 23),
	[WGT_QP_GRPA6] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(6), 0, 23),
	[WGT_QP_GRPA7] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(7), 0, 23),
	[WGT_QP_GRPA8] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(8), 0, 23),
	[WGT_QP_GRPA9] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(9), 0, 23),
	[WGT_QP_GRPA10]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(10), 0, 23),
	[WGT_QP_GRPA11]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(11), 0, 23),
	[WGT_QP_GRPA12]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(12), 0, 23),
	[WGT_QP_GRPA13]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(13), 0, 23),
	[WGT_QP_GRPA14]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(14), 0, 23),
	[WGT_QP_GRPA15]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(15), 0, 23),
	[WGT_QP_GRPA16]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(16), 0, 23),
	[WGT_QP_GRPA17]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(17), 0, 23),
	[WGT_QP_GRPA18]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(18), 0, 23),
	[WGT_QP_GRPA19]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(19), 0, 23),
	[WGT_QP_GRPA20]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(20), 0, 23),
	[WGT_QP_GRPA21]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(21), 0, 23),
	[WGT_QP_GRPA22]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(22), 0, 23),
	[WGT_QP_GRPA23]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(23), 0, 23),
	[WGT_QP_GRPA24]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(24), 0, 23),
	[WGT_QP_GRPA25]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(25), 0, 23),
	[WGT_QP_GRPA26]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(26), 0, 23),
	[WGT_QP_GRPA27]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(27), 0, 23),
	[WGT_QP_GRPA28]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(28), 0, 23),
	[WGT_QP_GRPA29]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(29), 0, 23),
	[WGT_QP_GRPA30]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(30), 0, 23),
	[WGT_QP_GRPA31]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(31), 0, 23),
	[WGT_QP_GRPA32]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(32), 0, 23),
	[WGT_QP_GRPA33]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(33), 0, 23),
	[WGT_QP_GRPA34]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(34), 0, 23),
	[WGT_QP_GRPA35]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(35), 0, 23),
	[WGT_QP_GRPA36]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(36), 0, 23),
	[WGT_QP_GRPA37]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(37), 0, 23),
	[WGT_QP_GRPA38]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(38), 0, 23),
	[WGT_QP_GRPA39]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(39), 0, 23),
	[WGT_QP_GRPA40]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(40), 0, 23),
	[WGT_QP_GRPA41]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(41), 0, 23),
	[WGT_QP_GRPA42]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(42), 0, 23),
	[WGT_QP_GRPA43]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(43), 0, 23),
	[WGT_QP_GRPA44]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(44), 0, 23),
	[WGT_QP_GRPA45]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(45), 0, 23),
	[WGT_QP_GRPA46]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(46), 0, 23),
	[WGT_QP_GRPA47]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(47), 0, 23),
	[WGT_QP_GRPA48]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(48), 0, 23),
	[WGT_QP_GRPA49]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(49), 0, 23),
	[WGT_QP_GRPA50]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(50), 0, 23),
	[WGT_QP_GRPA51]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTA_QP(51), 0, 23),

	[WGT_QP_GRPB0] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(0), 0, 23),
	[WGT_QP_GRPB1] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(1), 0, 23),
	[WGT_QP_GRPB2] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(2), 0, 23),
	[WGT_QP_GRPB3] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(3), 0, 23),
	[WGT_QP_GRPB4] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(4), 0, 23),
	[WGT_QP_GRPB5] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(5), 0, 23),
	[WGT_QP_GRPB6] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(6), 0, 23),
	[WGT_QP_GRPB7] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(7), 0, 23),
	[WGT_QP_GRPB8] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(8), 0, 23),
	[WGT_QP_GRPB9] 	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(9), 0, 23),
	[WGT_QP_GRPB10]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(10), 0, 23),
	[WGT_QP_GRPB11]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(11), 0, 23),
	[WGT_QP_GRPB12]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(12), 0, 23),
	[WGT_QP_GRPB13]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(13), 0, 23),
	[WGT_QP_GRPB14]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(14), 0, 23),
	[WGT_QP_GRPB15]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(15), 0, 23),
	[WGT_QP_GRPB16]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(16), 0, 23),
	[WGT_QP_GRPB17]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(17), 0, 23),
	[WGT_QP_GRPB18]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(18), 0, 23),
	[WGT_QP_GRPB19]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(19), 0, 23),
	[WGT_QP_GRPB20]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(20), 0, 23),
	[WGT_QP_GRPB21]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(21), 0, 23),
	[WGT_QP_GRPB22]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(22), 0, 23),
	[WGT_QP_GRPB23]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(23), 0, 23),
	[WGT_QP_GRPB24]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(24), 0, 23),
	[WGT_QP_GRPB25]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(25), 0, 23),
	[WGT_QP_GRPB26]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(26), 0, 23),
	[WGT_QP_GRPB27]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(27), 0, 23),
	[WGT_QP_GRPB28]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(28), 0, 23),
	[WGT_QP_GRPB29]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(29), 0, 23),
	[WGT_QP_GRPB30]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(30), 0, 23),
	[WGT_QP_GRPB31]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(31), 0, 23),
	[WGT_QP_GRPB32]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(32), 0, 23),
	[WGT_QP_GRPB33]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(33), 0, 23),
	[WGT_QP_GRPB34]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(34), 0, 23),
	[WGT_QP_GRPB35]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(35), 0, 23),
	[WGT_QP_GRPB36]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(36), 0, 23),
	[WGT_QP_GRPB37]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(37), 0, 23),
	[WGT_QP_GRPB38]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(38), 0, 23),
	[WGT_QP_GRPB39]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(39), 0, 23),
	[WGT_QP_GRPB40]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(40), 0, 23),
	[WGT_QP_GRPB41]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(41), 0, 23),
	[WGT_QP_GRPB42]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(42), 0, 23),
	[WGT_QP_GRPB43]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(43), 0, 23),
	[WGT_QP_GRPB44]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(44), 0, 23),
	[WGT_QP_GRPB45]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(45), 0, 23),
	[WGT_QP_GRPB46]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(46), 0, 23),
	[WGT_QP_GRPB47]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(47), 0, 23),
	[WGT_QP_GRPB48]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(48), 0, 23),
	[WGT_QP_GRPB49]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(49), 0, 23),
	[WGT_QP_GRPB50]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(50), 0, 23),
	[WGT_QP_GRPB51]	= REG_FIELD(RKVENC_VEPU540_L2_RDO_WGTB_QP(51), 0, 23),

	[MADI_MODE]	= REG_FIELD(RKVENC_VEPU540_L2_MADI_CFG, 0, 0),

	[AQ_TTHD0]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(0), 0, 7),
	[AQ_TTHD1]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(0), 8, 15),
	[AQ_TTHD2]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(0), 16, 23),
	[AQ_TTHD3]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(0), 24, 31),
	[AQ_TTHD4]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(1), 0, 7),
	[AQ_TTHD5]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(1), 8, 15),
	[AQ_TTHD6]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(1), 16, 23),
	[AQ_TTHD7]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(1), 24, 31),
	[AQ_TTHD8]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(2), 0, 7),
	[AQ_TTHD9]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(2), 8, 15),
	[AQ_TTHD10]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(2), 16, 23),
	[AQ_TTHD11]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(2), 24, 31),
	[AQ_TTHD12]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(3), 0, 7),
	[AQ_TTHD13]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(3), 8, 15),
	[AQ_TTHD14]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(3), 16, 23),
	[AQ_TTHD15]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_TTHD(3), 24, 31),

	[AQ_STEP0]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(0), 0, 5),
	[AQ_STEP1]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(0), 8, 13),
	[AQ_STEP2]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(0), 16, 21),
	[AQ_STEP3]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(0), 24, 29),
	[AQ_STEP4]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(1), 0, 5),
	[AQ_STEP5]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(1), 8, 13),
	[AQ_STEP6]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(1), 16, 21),
	[AQ_STEP7]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(1), 24, 29),
	[AQ_STEP8]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(2), 0, 5),
	[AQ_STEP9]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(2), 8, 13),
	[AQ_STEP10]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(2), 16, 21),
	[AQ_STEP11]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(2), 24, 29),
	[AQ_STEP12]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(3), 0, 5),
	[AQ_STEP13]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(3), 8, 13),
	[AQ_STEP14]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(3), 16, 21),
	[AQ_STEP15]	= REG_FIELD(RKVENC_VEPU540_L2_AQ_STP(3), 24, 29),

	[MVD_PNLT_E]	= REG_FIELD(RKVENC_VEPU540_L2_RME_MVD_PNSH, 0, 0),
	[MVD_PNLT_COEF]	= REG_FIELD(RKVENC_VEPU540_L2_RME_MVD_PNSH, 1, 5),
	[MVD_PNLT_CNST]	= REG_FIELD(RKVENC_VEPU540_L2_RME_MVD_PNSH, 6, 19),
	[MVD_PNLT_LTHD]	= REG_FIELD(RKVENC_VEPU540_L2_RME_MVD_PNSH, 20, 23),
	[MVD_PNLT_HTHD]	= REG_FIELD(RKVENC_VEPU540_L2_RME_MVD_PNSH, 24, 27),

	[ATR1_THD0]	= REG_FIELD(RKVENC_VEPU540_L2_ATR1_THD(0), 0, 11),
	[ATR1_THD1]	= REG_FIELD(RKVENC_VEPU540_L2_ATR1_THD(0), 16, 27),

	[ATR1_THD2]	= REG_FIELD(RKVENC_VEPU540_L2_ATR1_THD(1), 0, 11),
};

#endif /* RKVENC_VEPU540_REGS_H_ */
