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
#include <linux/clk.h>

#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ioctl.h>
#include <media/videobuf2-core.h>
#include <media/videobuf2-dma-contig.h>

struct rkvpu_ctx;

struct rkvpu_ctrl_desc {
	struct v4l2_ctrl_config cfg;
};

struct rkvpu_ctrls {
	const struct rkvpu_ctrl_desc *ctrls;
	unsigned int num_ctrls;
};

struct rkvpu_run {
	struct {
		struct vb2_v4l2_buffer *src;
		struct vb2_v4l2_buffer *dst;
	} bufs;
};

struct rkvpu_vp9_decoded_buffer_info {
	/* Info needed when the decoded frame serves as a reference frame. */
	unsigned short width;
	unsigned short height;
	unsigned int bit_depth : 4;
};

struct rkvpu_decoded_buffer {
	/* Must be the first field in this struct. */
	struct v4l2_m2m_buffer base;

	union {
		struct rkvpu_vp9_decoded_buffer_info vp9;
	};
};

static inline struct rkvpu_decoded_buffer *
vb2_to_rkvpu_decoded_buf(struct vb2_buffer *buf)
{
	return container_of(buf, struct rkvpu_decoded_buffer,
			    base.vb.vb2_buf);
}

struct rkvpu_coded_fmt_ops {
	int (*adjust_fmt)(struct rkvpu_ctx *ctx,
			  struct v4l2_format *f);
	int (*start)(struct rkvpu_ctx *ctx);
	void (*stop)(struct rkvpu_ctx *ctx);
	int (*run)(struct rkvpu_ctx *ctx);
	void (*done)(struct rkvpu_ctx *ctx, struct vb2_v4l2_buffer *src_buf,
		     struct vb2_v4l2_buffer *dst_buf,
		     enum vb2_buffer_state result);
	int (*try_ctrl)(struct rkvpu_ctx *ctx, struct v4l2_ctrl *ctrl);
};

struct rkvpu_coded_fmt_desc {
	u32 fourcc;
	struct v4l2_frmsize_stepwise frmsize;
	const struct rkvpu_ctrls *ctrls;
	const struct rkvpu_coded_fmt_ops *ops;
	unsigned int num_decoded_fmts;
	const u32 *decoded_fmts;
	u32 subsystem_flags;
};

struct rkvpu_dev {
	struct v4l2_device v4l2_dev;
	struct media_device mdev;
	struct video_device vdev;
	struct v4l2_m2m_dev *m2m_dev;
	struct device *dev;
	struct clk_bulk_data *clocks;
	void __iomem *regs;
	struct mutex vdev_lock; /* serializes ioctls */
	struct delayed_work watchdog_work;
};

struct rkvpu_ctx {
	struct v4l2_fh fh;
	struct v4l2_format coded_fmt;
	struct v4l2_format decoded_fmt;
	const struct rkvpu_coded_fmt_desc *coded_fmt_desc;
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
};

void rkvpu_run_preamble(struct rkvpu_ctx *ctx, struct rkvpu_run *run);
void rkvpu_run_postamble(struct rkvpu_ctx *ctx, struct rkvpu_run *run);

extern const struct rkvpu_coded_fmt_ops rkvdec_h264_fmt_ops;
extern const struct rkvpu_coded_fmt_ops rkvdec_vp9_fmt_ops;

#endif /* RKVDEC_H_ */
