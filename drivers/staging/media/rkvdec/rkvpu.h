/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Rockchip Video Decoder driver
 *
 * Copyright (C) 2019 Collabora, Ltd.
 *
 * Based on rkvpu driver by Google LLC. (Tomasz Figa <tfiga@chromium.org>)
 * Based on s5p-mfc driver by Samsung Electronics Co., Ltd.
 * Copyright (C) 2011 Samsung Electronics Co., Ltd.
 */
#ifndef RKVDEC_H_
#define RKVDEC_H_

#include <linux/platform_device.h>
#include <linux/videodev2.h>
#include <linux/wait.h>
#include <linux/regmap.h>
#include <linux/clk.h>

#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-core.h>
#include <media/videobuf2-dma-contig.h>

#define MB_DIM			16
#define TILE_MB_DIM		4
#define MB_WIDTH(w)		DIV_ROUND_UP(w, MB_DIM)
#define MB_HEIGHT(h)		DIV_ROUND_UP(h, MB_DIM)

struct rkvpu_ctx;

#define RKVPU_H264_ENCODER	BIT(1)
#define RKVPU_ENCODERS		0x0000ffff
#define RKVPU_H264_DECODER	BIT(18)
#define RKVPU_VP9_DECODER	BIT(20)
#define RKVPU_DECODERS		0xffff0000

struct rkvpu_ctrl {
	unsigned int codec;
	struct v4l2_ctrl_config cfg;
};

struct rkvpu_run {
	struct {
		struct vb2_v4l2_buffer *src;
		struct vb2_v4l2_buffer *dst;
	} bufs;
};

struct rkvpu_vp9_src_buffer_info {
	/* Info needed when the src frame serves as a reference frame. */
	unsigned short width;
	unsigned short height;
	unsigned int bit_depth : 4;
};

struct rkvpu_src_buffer {
	/* Must be the first field in this struct. */
	struct v4l2_m2m_buffer base;

	union {
		struct rkvpu_vp9_src_buffer_info vp9;
	};
};

static inline struct rkvpu_src_buffer *
vb2_to_rkvpu_src_buf(struct vb2_buffer *buf)
{
	return container_of(buf, struct rkvpu_src_buffer,
			    base.vb.vb2_buf);
}

struct rkvpu_ops {
	int (*adjust_fmt)(struct rkvpu_ctx *ctx,
			  struct v4l2_format *f);
	int (*start)(struct rkvpu_ctx *ctx);
	void (*stop)(struct rkvpu_ctx *ctx);
	int (*run)(struct rkvpu_ctx *ctx);
	void (*done)(struct rkvpu_ctx *ctx, struct vb2_v4l2_buffer *src_buf,
		     struct vb2_v4l2_buffer *dst_buf,
		     enum vb2_buffer_state result);
	int (*irq)(struct rkvpu_ctx *ctx);
	int (*watchdog)(struct rkvpu_ctx *ctx);
	int (*try_ctrl)(struct rkvpu_ctx *ctx, struct v4l2_ctrl *ctrl);
};

/**
 * enum rkvpu_enc_fmt - source format ID for hardware registers.
 *
 * @RKVPU_ENC_FMT_YUV420P: Y/CbCr 4:2:0 planar format
 * @RKVPU_ENC_FMT_YUV420SP: Y/CbCr 4:2:0 semi-planar format
 * @RKVPU_ENC_FMT_YUYV422: YUV 4:2:2 packed format (YUYV)
 * @RKVPU_ENC_FMT_UYVY422: YUV 4:2:2 packed format (UYVY)
 */
enum rkvpu_enc_fmt {
	RKVPU_ENC_FMT_YUV420P = 0,
	RKVPU_ENC_FMT_YUV420SP = 1,
	RKVPU_ENC_FMT_YUYV422 = 2,
	RKVPU_ENC_FMT_UYVY422 = 3,
	RKVPU_ENC_FMT_RGB565 = 4,
	RKVPU_ENC_FMT_RGB444 = 5,
	RKVPU_ENC_FMT_RGB888 = 6,
	RKVPU_ENC_FMT_RGB101010 = 7,
};

struct rkvpu_hw_fmt {
	enum rkvpu_enc_fmt enc_fmt;
	struct v4l2_frmsize_stepwise frmsize;
	u32 fourcc;
};

struct rkvpu_fmt_desc {
	char *name;
	unsigned int codec;
	int header_size;
	u32 fourcc;
	int max_depth;
	const struct rkvpu_ops *ops;
	unsigned int num_peer_fmts;
	const struct rkvpu_hw_fmt *peer_fmts;
	struct v4l2_frmsize_stepwise frmsize;
	u32 subsystem_flags;
};

struct rkvpu_variant {
	unsigned int codec;
	const struct rkvpu_ctrls *ctrls;
	unsigned int num_fmts;
	const struct rkvpu_fmt_desc *fmts;
};

struct rkvpu_func {
	unsigned int id;
	struct video_device vdev;
	struct media_pad source_pad;
	struct media_entity sink;
	struct media_pad sink_pad;
	struct media_entity proc;
	struct media_pad proc_pads[2];
	struct media_intf_devnode *intf_devnode;
};

static inline struct rkvpu_func *
rkvpu_vdev_to_func(struct video_device *vdev)
{
	return container_of(vdev, struct rkvpu_func, vdev);
}

struct rkvpu_dev {
	struct v4l2_device v4l2_dev;
	struct media_device mdev;
	struct rkvpu_func *encoder;
	struct rkvpu_func *decoder;
	struct v4l2_m2m_dev *m2m_dev;
	struct device *dev;
	struct clk_bulk_data *clocks;
	void __iomem *regs;
	struct mutex vdev_lock; /* serializes ioctls */
	struct delayed_work watchdog_work;

	const struct rkvpu_ops *ops;

	const struct rkvpu_variant *variant;
};

struct rkvpu_ctx {
	struct v4l2_fh fh;
	bool is_encoder;
	struct v4l2_format src_fmt;
	struct v4l2_format dst_fmt;
	const struct rkvpu_fmt_desc *fmt_desc;
	struct v4l2_ctrl_handler ctrl_hdl;
	struct rkvpu_dev *dev;
	void *priv;
};

static inline struct rkvpu_ctx *fh_to_rkvpu_ctx(struct v4l2_fh *fh)
{
	return container_of(fh, struct rkvpu_ctx, fh);
}

struct rkvpu_aux_buf {
	void *cpu;
	dma_addr_t dma;
	size_t size;
	unsigned long attrs;
};

struct rkvpu_enc_buf {
	struct v4l2_m2m_buffer m2m_buf;
	struct rkvpu_aux_buf rec_buf;
	struct rkvpu_aux_buf ds_buf;
	struct rkvpu_aux_buf me_buf;
};

void rkvpu_run_preamble(struct rkvpu_ctx *ctx, struct rkvpu_run *run);
void rkvpu_run_postamble(struct rkvpu_ctx *ctx, struct rkvpu_run *run);

static inline struct rkvpu_enc_buf *
rkvpu_get_enc_buf(struct vb2_v4l2_buffer *v4l2_buf)
{
	struct v4l2_m2m_buffer *m2m_buf;
	struct rkvpu_enc_buf *enc_buf;

	m2m_buf = container_of(v4l2_buf, struct v4l2_m2m_buffer, vb);
	enc_buf = container_of(m2m_buf, struct rkvpu_enc_buf, m2m_buf);

	return enc_buf;
}

extern const struct rkvpu_ops rkvenc_h264_fmt_ops;
extern const struct rkvpu_ops rkvdec_h264_fmt_ops;
extern const struct rkvpu_ops rkvdec_vp9_fmt_ops;

#endif /* RKVDEC_H_ */
