// SPDX-License-Identifier: GPL-2.0
/*
 * Rockchip Video Decoder driver
 *
 * Copyright (C) 2019 Collabora, Ltd.
 *
 * Based on rkvpu driver by Google LLC. (Tomasz Figa <tfiga@chromium.org>)
 * Based on s5p-mfc driver by Samsung Electronics Co., Ltd.
 * Copyright (C) 2011 Samsung Electronics Co., Ltd.
 */

#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/videodev2.h>
#include <linux/workqueue.h>
#include <media/v4l2-event.h>
#include <media/v4l2-mem2mem.h>
#include <media/videobuf2-core.h>
#include <media/videobuf2-vmalloc.h>

#include "rkvpu.h"

static int rkvpu_try_ctrl(struct v4l2_ctrl *ctrl)
{
	struct rkvpu_ctx *ctx = container_of(ctrl->handler, struct rkvpu_ctx, ctrl_hdl);
	const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;

	if (desc->ops->try_ctrl)
		return desc->ops->try_ctrl(ctx, ctrl);

	return 0;
}

static const struct v4l2_ctrl_ops rkvpu_ctrl_ops = {
	.try_ctrl = rkvpu_try_ctrl,
};

static const struct rkvpu_ctrl controls[] = {
	{
		.codec = RKVPU_H264_ENCODER,
		.cfg.id = V4L2_CID_STATELESS_H264_ENCODE_PARAMS,
	},
	{
		.codec = RKVPU_H264_ENCODER,
		.cfg.id = V4L2_CID_STATELESS_H264_ENCODE_RC,
	},
	{
		.codec = RKVPU_H264_ENCODER,
		.cfg.id = V4L2_CID_STATELESS_H264_SPS,
		.cfg.ops = &rkvpu_ctrl_ops,
	},
	{
		.codec = RKVPU_H264_ENCODER,
		.cfg.id = V4L2_CID_STATELESS_H264_PPS,
	},
	{
		.codec = RKVPU_H264_ENCODER,
		.cfg.id = V4L2_CID_STATELESS_H264_ENCODE_FEEDBACK,
			/* XXX: Maybe put the flag back and see. */
//			.flags = V4L2_CTRL_FLAG_READ_ONLY,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_STATELESS_H264_DECODE_PARAMS,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_STATELESS_H264_SPS,
		.cfg.ops = &rkvpu_ctrl_ops,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_STATELESS_H264_PPS,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_STATELESS_H264_SCALING_MATRIX,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_STATELESS_H264_DECODE_MODE,
		.cfg.min = V4L2_STATELESS_H264_DECODE_MODE_FRAME_BASED,
		.cfg.max = V4L2_STATELESS_H264_DECODE_MODE_FRAME_BASED,
		.cfg.def = V4L2_STATELESS_H264_DECODE_MODE_FRAME_BASED,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_STATELESS_H264_START_CODE,
		.cfg.min = V4L2_STATELESS_H264_START_CODE_ANNEX_B,
		.cfg.def = V4L2_STATELESS_H264_START_CODE_ANNEX_B,
		.cfg.max = V4L2_STATELESS_H264_START_CODE_ANNEX_B,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_MPEG_VIDEO_H264_PROFILE,
		.cfg.min = V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE,
		.cfg.max = V4L2_MPEG_VIDEO_H264_PROFILE_HIGH,
		.cfg.menu_skip_mask =
			BIT(V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED),
		.cfg.def = V4L2_MPEG_VIDEO_H264_PROFILE_MAIN,
	},
	{
		.codec = RKVPU_H264_DECODER,
		.cfg.id = V4L2_CID_MPEG_VIDEO_H264_LEVEL,
		.cfg.min = V4L2_MPEG_VIDEO_H264_LEVEL_1_0,
		.cfg.max = V4L2_MPEG_VIDEO_H264_LEVEL_5_1,
	},
	{
		.codec = RKVPU_VP9_DECODER,
		.cfg.id = V4L2_CID_STATELESS_VP9_FRAME,
	},
	{
		.codec = RKVPU_VP9_DECODER,
		.cfg.id = V4L2_CID_STATELESS_VP9_COMPRESSED_HDR,
	},
	{
		.codec = RKVPU_VP9_DECODER,
		.cfg.id = V4L2_CID_MPEG_VIDEO_VP9_PROFILE,
		.cfg.min = V4L2_MPEG_VIDEO_VP9_PROFILE_0,
		.cfg.max = V4L2_MPEG_VIDEO_VP9_PROFILE_0,
		.cfg.def = V4L2_MPEG_VIDEO_VP9_PROFILE_0,
	},
};

static const struct rkvpu_hw_fmt rkvpu_h264_vp9_hw_dst_fmts[] = {
	{
		.fourcc = V4L2_PIX_FMT_NV12,
	},
};

static const struct rkvpu_fmt_desc rkvpu_dec_fmts[] = {
	{
		.fourcc = V4L2_PIX_FMT_H264_SLICE,
		.frmsize = {
			.min_width = 48,
			.max_width = 4096,
			.step_width = 16,
			.min_height = 48,
			.max_height = 2304,
			.step_height = 16,
		},
		.ops = &rkvdec_h264_fmt_ops,
		.num_peer_fmts = ARRAY_SIZE(rkvpu_h264_vp9_hw_dst_fmts),
		.peer_fmts = rkvpu_h264_vp9_hw_dst_fmts,
		.subsystem_flags = VB2_V4L2_FL_SUPPORTS_M2M_HOLD_CAPTURE_BUF,
		.codec = RKVPU_H264_DECODER,
	},
	{
		.fourcc = V4L2_PIX_FMT_VP9_FRAME,
		.frmsize = {
			.min_width = 64,
			.max_width = 4096,
			.step_width = 64,
			.min_height = 64,
			.max_height = 2304,
			.step_height = 64,
		},
		.ops = &rkvdec_vp9_fmt_ops,
		.num_peer_fmts = ARRAY_SIZE(rkvpu_h264_vp9_hw_dst_fmts),
		.peer_fmts = rkvpu_h264_vp9_hw_dst_fmts,
		.codec = RKVPU_VP9_DECODER,
	}
};

static const struct rkvpu_hw_fmt rkvpu_hw_src_fmts[] = {
	{
		.fourcc = V4L2_PIX_FMT_YUV420M,
		.enc_fmt = RKVPU_ENC_FMT_YUV420P,
	},
	{
		.fourcc = V4L2_PIX_FMT_NV12M,
		.enc_fmt = RKVPU_ENC_FMT_YUV420SP,
	},
	{
		.fourcc = V4L2_PIX_FMT_YUYV,
		.enc_fmt = RKVPU_ENC_FMT_YUYV422,
	},
	{
		.fourcc = V4L2_PIX_FMT_UYVY,
		.enc_fmt = RKVPU_ENC_FMT_UYVY422,
	},
	{
		.fourcc = V4L2_PIX_FMT_RGB565,
		.enc_fmt = RKVPU_ENC_FMT_RGB565,
	},
	{
		.fourcc = V4L2_PIX_FMT_RGB444,
		.enc_fmt = RKVPU_ENC_FMT_RGB444,
	},
	{
		.fourcc = V4L2_PIX_FMT_RGB24,
		.enc_fmt = RKVPU_ENC_FMT_RGB888,
	},
};

static const struct rkvpu_fmt_desc rkvpu_enc_fmts[] = {
	{
		.fourcc = V4L2_PIX_FMT_H264_SLICE,
		.max_depth = 2,
		.frmsize = {
			.min_width = 96,
			.max_width = 4096,
			.step_width = MB_DIM,
			.min_height = 32,
			.max_height = 4096,
			.step_height = MB_DIM,
		},
		.ops = &rkvenc_h264_fmt_ops,
		.subsystem_flags = VB2_V4L2_FL_SUPPORTS_M2M_HOLD_CAPTURE_BUF,
		.num_peer_fmts = ARRAY_SIZE(rkvpu_hw_src_fmts),
		.peer_fmts = rkvpu_hw_src_fmts,
		.codec = RKVPU_H264_ENCODER,
	},
};

static const struct rkvpu_fmt_desc *
rkvpu_find_fmt_desc_supports_peer(const struct rkvpu_ctx *ctx, u32 fourcc)
{
	struct rkvpu_dev *rkvpu = ctx->dev;
	const struct rkvpu_variant *variant = rkvpu->variant;
	int i, j;

	for (i = 0; i < variant->num_fmts; i++) {
		for (j = 0; j < variant->fmts[i].num_peer_fmts; j++)
			if (variant->fmts[i].peer_fmts[j].fourcc == fourcc)
				return &variant->fmts[i];
	}

	return NULL;
}

static const struct rkvpu_fmt_desc *
rkvpu_find_fmt_desc(const struct rkvpu_ctx *ctx, u32 fourcc)
{
	struct rkvpu_dev *rkvpu = ctx->dev;
	const struct rkvpu_variant *variant = rkvpu->variant;
	int i;

	for (i = 0; i < variant->num_fmts; i++)
		if (variant->fmts[i].fourcc == fourcc)
			return &variant->fmts[i];

	return NULL;
}

static void rkvpu_reset_fmt(struct rkvpu_ctx *ctx, struct v4l2_format *f,
			     u32 fourcc)
{
	memset(f, 0, sizeof(*f));
	f->fmt.pix_mp.pixelformat = fourcc;
	f->fmt.pix_mp.field = V4L2_FIELD_NONE;
	f->fmt.pix_mp.colorspace = V4L2_COLORSPACE_REC709;
	f->fmt.pix_mp.ycbcr_enc = V4L2_YCBCR_ENC_DEFAULT;
	f->fmt.pix_mp.quantization = V4L2_QUANTIZATION_DEFAULT;
	f->fmt.pix_mp.xfer_func = V4L2_XFER_FUNC_DEFAULT;
}

static void rkvpu_reset_src_fmt(struct rkvpu_ctx *ctx)
{
	struct v4l2_format *f = &ctx->src_fmt;
	struct rkvpu_dev *rkvpu = ctx->dev;
	const struct rkvpu_variant *variant = rkvpu->variant;
	const struct rkvpu_fmt_desc *desc;

	desc = ctx->fmt_desc = &variant->fmts[0];
	rkvpu_reset_fmt(ctx, f, desc->fourcc);

	f->type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	f->fmt.pix_mp.width = ctx->fmt_desc->frmsize.min_width;
	f->fmt.pix_mp.height = ctx->fmt_desc->frmsize.min_height;

	if (desc->ops->adjust_fmt)
		desc->ops->adjust_fmt(ctx, f);
}

static void rkvpu_reset_dst_fmt(struct rkvpu_ctx *ctx)
{
	struct v4l2_format *f = &ctx->dst_fmt;
	const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;

	rkvpu_reset_fmt(ctx, f, desc->peer_fmts[0].fourcc);
	f->type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	v4l2_fill_pixfmt_mp(&f->fmt.pix_mp,
			    ctx->fmt_desc->fourcc,
			    ctx->src_fmt.fmt.pix_mp.width,
			    ctx->src_fmt.fmt.pix_mp.height);
	f->fmt.pix_mp.plane_fmt[0].sizeimage += 128 *
		DIV_ROUND_UP(f->fmt.pix_mp.width, 16) *
		DIV_ROUND_UP(f->fmt.pix_mp.height, 16);
}

static int rkvpu_enum_framesizes(struct file *file, void *priv,
				  struct v4l2_frmsizeenum *fsize)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	const struct rkvpu_fmt_desc *fmt;

	if (fsize->index != 0)
		return -EINVAL;

	fmt = rkvpu_find_fmt_desc(ctx, fsize->pixel_format);
	if (!fmt)
		return -EINVAL;

	fsize->type = V4L2_FRMSIZE_TYPE_STEPWISE;
	fsize->stepwise = fmt->frmsize;
	return 0;
}

static int rkvpu_querycap(struct file *file, void *priv,
			   struct v4l2_capability *cap)
{
	struct rkvpu_dev *rkvpu = video_drvdata(file);
	struct video_device *vdev = video_devdata(file);

	strscpy(cap->driver, rkvpu->dev->driver->name,
		sizeof(cap->driver));
	strscpy(cap->card, vdev->name, sizeof(cap->card));
	snprintf(cap->bus_info, sizeof(cap->bus_info), "platform:%s",
		 rkvpu->dev->driver->name);
	return 0;
}

static int rkvpu_try_capture_fmt(struct file *file, void *priv,
				  struct v4l2_format *f)
{
	struct v4l2_pix_format_mplane *pix_mp = &f->fmt.pix_mp;
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	struct rkvpu_dev *rkvpu = ctx->dev;
	const struct rkvpu_variant *variant = rkvpu->variant;
	const struct rkvpu_fmt_desc *desc;
	unsigned int i;

	/*
	 * The codec context should point to a src format desc, if the format
	 * on the src end has not been set yet, it should point to the
	 * default value.
	 */
	desc = ctx->fmt_desc;
	if (!ctx->is_encoder) {
		if (WARN_ON(!desc))
			return -EINVAL;

		for (i = 0; i < desc->num_peer_fmts; i++) {
			if (desc->peer_fmts[i].fourcc == pix_mp->pixelformat)
				break;
		}

		if (i == variant->num_fmts)
			pix_mp->pixelformat = variant->fmts[0].fourcc;

		v4l2_info(&rkvpu->v4l2_dev, "setting in %s found %c%c%c%c\n",
			  __func__,
			  (pix_mp->pixelformat & 0x7f),
			  (pix_mp->pixelformat >> 8) & 0x7f,
			  (pix_mp->pixelformat >> 16) & 0x7f,
			  (pix_mp->pixelformat >> 24) & 0x7f);

		/* Always apply the frmsize constraint of the src end. */
		pix_mp->width = max(pix_mp->width, ctx->src_fmt.fmt.pix_mp.width);
		pix_mp->height = max(pix_mp->height, ctx->src_fmt.fmt.pix_mp.height);
	} else if(!desc) {
		pix_mp->pixelformat = variant->fmts[0].fourcc;
		desc = &variant->fmts[0];
	}

	if (ctx->is_encoder)
		pix_mp->num_planes = 1;

	v4l2_apply_frmsize_constraints(&pix_mp->width,
				       &pix_mp->height,
				       &desc->frmsize);

	if (!ctx->is_encoder) {
		v4l2_fill_pixfmt_mp(pix_mp, pix_mp->pixelformat,
				    pix_mp->width, pix_mp->height);
		pix_mp->plane_fmt[0].sizeimage +=
			128 *
			DIV_ROUND_UP(pix_mp->width, 16) *
			DIV_ROUND_UP(pix_mp->height, 16);
	} else if (!pix_mp->plane_fmt[0].sizeimage) {
		/*
		 * For coded formats the application can specify
		 * sizeimage. If the application passes a zero sizeimage,
		 * let's default to the maximum frame size.
		 */
		pix_mp->plane_fmt[0].sizeimage = desc->header_size +
			pix_mp->width * pix_mp->height * desc->max_depth;
	}
	pix_mp->field = V4L2_FIELD_NONE;

	return 0;
}

static int rkvpu_try_output_fmt(struct file *file, void *priv,
				 struct v4l2_format *f)
{
	struct v4l2_pix_format_mplane *pix_mp = &f->fmt.pix_mp;
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	struct rkvpu_dev *rkvpu = ctx->dev;
	const struct rkvpu_fmt_desc *desc;
	const struct rkvpu_variant *variant = rkvpu->variant;
	unsigned int i;

	/*
	 * The codec context should point to a src format desc, if the format
	 * on the src end has not been set yet, it should point to the
	 * default value.
	 */
	desc = ctx->fmt_desc;

	if (ctx->is_encoder) {
		bool found = false;
		if (WARN_ON(!desc))
			return -EINVAL;

		v4l2_info(&rkvpu->v4l2_dev, "trying in %s found %c%c%c%c\n",
			  __func__,
			  (pix_mp->pixelformat & 0x7f),
			  (pix_mp->pixelformat >> 8) & 0x7f,
			  (pix_mp->pixelformat >> 16) & 0x7f,
			  (pix_mp->pixelformat >> 24) & 0x7f);

		for (i = 0; i < desc->num_peer_fmts; i++) {
			if (desc->peer_fmts[i].fourcc == pix_mp->pixelformat) {
				found = true;
				break;
			}
		}

		if (!found && i == variant->num_fmts)
			pix_mp->pixelformat = variant->fmts[0].fourcc;

		v4l2_info(&rkvpu->v4l2_dev, "setting in %s %d %c%c%c%c\n",
			  __func__, i,
			  (pix_mp->pixelformat & 0x7f),
			  (pix_mp->pixelformat >> 8) & 0x7f,
			  (pix_mp->pixelformat >> 16) & 0x7f,
			  (pix_mp->pixelformat >> 24) & 0x7f);

	} else if (!desc) {
		pix_mp->pixelformat = variant->fmts[0].fourcc;
		desc = &variant->fmts[0];
	}

	v4l2_apply_frmsize_constraints(&pix_mp->width,
				       &pix_mp->height,
				       &desc->frmsize);

	pix_mp->field = V4L2_FIELD_NONE;
	/* All decoder src formats are considered single planar for now. */
	if (!ctx->is_encoder)
		pix_mp->num_planes = 1;

	v4l2_fill_pixfmt_mp(pix_mp, pix_mp->pixelformat,
			    pix_mp->width, pix_mp->height);

	if (ctx->fmt_desc->ops->adjust_fmt) {
		int ret;

		ret = ctx->fmt_desc->ops->adjust_fmt(ctx, f);
		if (ret)
			return ret;
	}

	return 0;
}

static int rkvpu_s_capture_fmt(struct file *file, void *priv,
				struct v4l2_format *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct v4l2_m2m_ctx *m2m_ctx = ctx->fh.m2m_ctx;
	struct vb2_queue *vq;
	struct v4l2_pix_format_mplane *pix_mp;
	struct v4l2_format *cap_fmt;
	int ret;

	/* Change not allowed if queue is busy */
	vq = v4l2_m2m_get_vq(m2m_ctx, V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE);
	if (vb2_is_busy(vq))
		return -EBUSY;

	if (ctx->is_encoder) {
		struct vb2_queue *peer_vq;

		/*
		 * Since format change on the CAPTURE queue will reset
		 * the OUTPUT queue, we can't allow doing so
		 * when the OUTPUT queue has buffers allocated.
		 */
		peer_vq = v4l2_m2m_get_vq(m2m_ctx, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
		if (vb2_is_busy(peer_vq) &&
		    (pix_mp->pixelformat != ctx->dst_fmt.fmt.pix_mp.pixelformat ||
		     pix_mp->height != ctx->dst_fmt.fmt.pix_mp.height ||
		     pix_mp->width != ctx->dst_fmt.fmt.pix_mp.width))
			return -EBUSY;
	}

	ret = rkvpu_try_capture_fmt(file, priv, f);
	if (ret)
		return ret;

	ctx->dst_fmt = *f;

	pix_mp = &f->fmt.pix_mp;

	v4l2_info(&rkvpu->v4l2_dev, "setting in %s dst_fmt %c%c%c%c\n",
		  __func__,
		  (pix_mp->pixelformat & 0x7f),
		  (pix_mp->pixelformat >> 8) & 0x7f,
		  (pix_mp->pixelformat >> 16) & 0x7f,
		  (pix_mp->pixelformat >> 24) & 0x7f);

	/* Colorimetry information are always propagated. */
	cap_fmt = &ctx->src_fmt;
	cap_fmt->fmt.pix_mp.colorspace = f->fmt.pix_mp.colorspace;
	cap_fmt->fmt.pix_mp.xfer_func = f->fmt.pix_mp.xfer_func;
	cap_fmt->fmt.pix_mp.ycbcr_enc = f->fmt.pix_mp.ycbcr_enc;
	cap_fmt->fmt.pix_mp.quantization = f->fmt.pix_mp.quantization;

	return 0;
}

static int rkvpu_s_output_fmt(struct file *file, void *priv,
			       struct v4l2_format *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct v4l2_m2m_ctx *m2m_ctx = ctx->fh.m2m_ctx;
	const struct rkvpu_fmt_desc *desc;
	struct v4l2_format *cap_fmt;
	struct vb2_queue *vq;
	struct v4l2_pix_format_mplane *pix_mp;
	int ret;

	vq = v4l2_m2m_get_vq(m2m_ctx, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
	ret = rkvpu_try_output_fmt(file, priv, f);
	if (ret)
		return ret;

	if (!ctx->is_encoder) {
		struct vb2_queue *peer_vq;
		/*
		 * In order to support dynamic resolution change, the decoder admits
		 * a resolution change, as long as the pixelformat remains. Can't be
		 * done if streaming.
		 */
		if (vb2_is_streaming(vq) ||
		    (vb2_is_busy(vq) &&
		     f->fmt.pix_mp.pixelformat != ctx->src_fmt.fmt.pix_mp.pixelformat))
			return -EBUSY;

		/*
		 * Since format change on the OUTPUT queue will reset the CAPTURE
		 * queue, we can't allow doing so when the CAPTURE queue has buffers
		 * allocated.
		 */
		peer_vq = v4l2_m2m_get_vq(m2m_ctx, V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE);
		if (vb2_is_busy(peer_vq))
			return -EBUSY;
	} else {
		/*
		 * The encoder doesn't admit a format change if
		 * there are OUTPUT buffers allocated.
		 */
		if (vb2_is_busy(vq))
			return -EBUSY;
	}

	desc = rkvpu_find_fmt_desc_supports_peer(ctx, f->fmt.pix_mp.pixelformat);
	desc = ctx->fmt_desc;
	if (!desc)
		return -EINVAL;
	ctx->fmt_desc = desc;
	ctx->src_fmt = *f;

	pix_mp = &f->fmt.pix_mp;

	v4l2_info(&rkvpu->v4l2_dev, "setting in %s src_fmt %c%c%c%c\n",
		  __func__,
		  (pix_mp->pixelformat & 0x7f),
		  (pix_mp->pixelformat >> 8) & 0x7f,
		  (pix_mp->pixelformat >> 16) & 0x7f,
		  (pix_mp->pixelformat >> 24) & 0x7f);

	/*
	 * Current src format might have become invalid with newly
	 * selected codec, so reset it to default just to be safe and
	 * keep internal driver state sane. User is mandated to set
	 * the desrc format again after we return, so we don't need
	 * anything smarter.
	 *
	 * Note that this will propagates any size changes to the desrc format.
	 */
	if (!ctx->is_encoder)
		rkvpu_reset_dst_fmt(ctx);

	/* Propagate colorspace information to capture. */
	cap_fmt = &ctx->dst_fmt;
	cap_fmt->fmt.pix_mp.colorspace = f->fmt.pix_mp.colorspace;
	cap_fmt->fmt.pix_mp.xfer_func = f->fmt.pix_mp.xfer_func;
	cap_fmt->fmt.pix_mp.ycbcr_enc = f->fmt.pix_mp.ycbcr_enc;
	cap_fmt->fmt.pix_mp.quantization = f->fmt.pix_mp.quantization;

	/* Enable format specific queue features */
	vq->subsystem_flags |= desc->subsystem_flags;

	return 0;
}

static int rkvpu_g_output_fmt(struct file *file, void *priv,
			       struct v4l2_format *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);

	*f = ctx->src_fmt;
	return 0;
}

static int rkvpu_g_capture_fmt(struct file *file, void *priv,
				struct v4l2_format *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);

	*f = ctx->dst_fmt;
	return 0;
}

static int rkvpu_enum_fmt(struct file *file, void *priv,
				  struct v4l2_fmtdesc *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	struct rkvpu_dev *rkvpu = ctx->dev;
	bool capture = V4L2_TYPE_IS_CAPTURE(f->type);
	bool coded = capture == ctx->is_encoder;

	if (coded) {
		const struct rkvpu_variant *variant = rkvpu->variant;

		if (f->index >= variant->num_fmts)
			return -EINVAL;

		f->pixelformat = variant->fmts[f->index].fourcc;
	} else {
		const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;

		if (WARN_ON(!desc))
			return -EINVAL;

		if (f->index >= desc->num_peer_fmts)
			return -EINVAL;

		f->pixelformat = desc->peer_fmts[f->index].fourcc;
	}

	return 0;
}

static const struct v4l2_ioctl_ops rkvpu_ioctl_ops = {
	.vidioc_querycap = rkvpu_querycap,
	.vidioc_enum_framesizes = rkvpu_enum_framesizes,

	.vidioc_try_fmt_vid_cap_mplane = rkvpu_try_capture_fmt,
	.vidioc_try_fmt_vid_out_mplane = rkvpu_try_output_fmt,
	.vidioc_s_fmt_vid_out_mplane = rkvpu_s_output_fmt,
	.vidioc_s_fmt_vid_cap_mplane = rkvpu_s_capture_fmt,
	.vidioc_g_fmt_vid_out_mplane = rkvpu_g_output_fmt,
	.vidioc_g_fmt_vid_cap_mplane = rkvpu_g_capture_fmt,
	.vidioc_enum_fmt_vid_out = rkvpu_enum_fmt,
	.vidioc_enum_fmt_vid_cap = rkvpu_enum_fmt,

	.vidioc_reqbufs = v4l2_m2m_ioctl_reqbufs,
	.vidioc_querybuf = v4l2_m2m_ioctl_querybuf,
	.vidioc_qbuf = v4l2_m2m_ioctl_qbuf,
	.vidioc_dqbuf = v4l2_m2m_ioctl_dqbuf,
	.vidioc_prepare_buf = v4l2_m2m_ioctl_prepare_buf,
	.vidioc_create_bufs = v4l2_m2m_ioctl_create_bufs,
	.vidioc_expbuf = v4l2_m2m_ioctl_expbuf,

	.vidioc_subscribe_event = v4l2_ctrl_subscribe_event,
	.vidioc_unsubscribe_event = v4l2_event_unsubscribe,

	.vidioc_streamon = v4l2_m2m_ioctl_streamon,
	.vidioc_streamoff = v4l2_m2m_ioctl_streamoff,
};

static int rkvpu_queue_setup(struct vb2_queue *vq, unsigned int *num_buffers,
			      unsigned int *num_planes, unsigned int sizes[],
			      struct device *alloc_devs[])
{
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vq);
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct v4l2_format *f;
	struct v4l2_pix_format_mplane *pix_mp;
	unsigned int i;

	if (V4L2_TYPE_IS_OUTPUT(vq->type))
		f = &ctx->src_fmt;
	else
		f = &ctx->dst_fmt;

	pix_mp = &f->fmt.pix_mp;

	v4l2_info(&rkvpu->v4l2_dev, "trying format %c%c%c%c\n",
		  (pix_mp->pixelformat & 0x7f),
		  (pix_mp->pixelformat >> 8) & 0x7f,
		  (pix_mp->pixelformat >> 16) & 0x7f,
		  (pix_mp->pixelformat >> 24) & 0x7f);

	if (*num_planes) {
		if (*num_planes != f->fmt.pix_mp.num_planes) {
			return -EINVAL;
		}

		for (i = 0; i < f->fmt.pix_mp.num_planes; i++) {
			if (sizes[i] < f->fmt.pix_mp.plane_fmt[i].sizeimage) {
				return -EINVAL;
			}
		}
	} else {
		*num_planes = f->fmt.pix_mp.num_planes;
		for (i = 0; i < f->fmt.pix_mp.num_planes; i++)
			sizes[i] = f->fmt.pix_mp.plane_fmt[i].sizeimage;
	}

	return 0;
}

unsigned int rkvpu_h264_enc_rec_luma_size(unsigned int width,
					   unsigned int height)
{
	return round_up(width, MB_DIM) * round_up(height, MB_DIM);
}

unsigned int rkvpu_h264_enc_rec_image_size(unsigned int width,
					    unsigned int height)
{
	/* Reconstructed image is YUV 4:2:0 with 1.5 bpp. */
	return rkvpu_h264_enc_rec_luma_size(width, height) * 3 / 2;
}

static int rkvpu_buf_init(struct vb2_buffer *vb)
{
	struct vb2_queue *vq = vb->vb2_queue;
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vq);
	struct rkvpu_dev *vpu = ctx->dev;
	struct rkvpu_enc_buf *enc_buf;
	struct rkvpu_aux_buf *rec_buf;
	struct rkvpu_aux_buf *ds_buf;
	struct rkvpu_aux_buf *me_buf;

	if (!ctx->is_encoder || V4L2_TYPE_IS_OUTPUT(vq->type))
		return 0;

	enc_buf = rkvpu_get_enc_buf(vbuf);
	rec_buf = &enc_buf->rec_buf;
	ds_buf = &enc_buf->ds_buf;
	me_buf = &enc_buf->me_buf;

	rec_buf->size = rkvpu_h264_enc_rec_image_size(ctx->src_fmt.fmt.pix_mp.width,
						       ctx->src_fmt.fmt.pix_mp.height);
	rec_buf->cpu = dma_alloc_coherent(vpu->dev, rec_buf->size,
					  &rec_buf->dma, GFP_KERNEL);
	if (!rec_buf->cpu)
		return -ENOMEM;

	ds_buf->size = rkvpu_h264_enc_rec_image_size(ctx->src_fmt.fmt.pix_mp.width,
						       ctx->src_fmt.fmt.pix_mp.height);
	ds_buf->cpu = dma_alloc_coherent(vpu->dev, ds_buf->size,
					  &ds_buf->dma, GFP_KERNEL);
	if (!ds_buf->cpu)
		return -ENOMEM;

	me_buf->size = rkvpu_h264_enc_rec_image_size(ctx->src_fmt.fmt.pix_mp.width,
						       ctx->src_fmt.fmt.pix_mp.height);
	me_buf->cpu = dma_alloc_coherent(vpu->dev, me_buf->size,
					  &me_buf->dma, GFP_KERNEL);
	if (!me_buf->cpu)
		return -ENOMEM;

	return 0;
}

static int rkvpu_buf_prepare(struct vb2_buffer *vb)
{
	struct vb2_queue *vq = vb->vb2_queue;
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vq);
	struct rkvpu_dev *rkvpu = ctx->dev;
	struct v4l2_format *f;
	struct v4l2_pix_format_mplane *pix_mp;
	unsigned int i;

	if (V4L2_TYPE_IS_OUTPUT(vq->type))
		f = &ctx->src_fmt;
	else
		f = &ctx->dst_fmt;

	pix_mp = &f->fmt.pix_mp;

	v4l2_info(&rkvpu->v4l2_dev, "trying format %c%c%c%c\n",
		  (pix_mp->pixelformat & 0x7f),
		  (pix_mp->pixelformat >> 8) & 0x7f,
		  (pix_mp->pixelformat >> 16) & 0x7f,
		  (pix_mp->pixelformat >> 24) & 0x7f);

	for (i = 0; i < f->fmt.pix_mp.num_planes; ++i) {
		u32 sizeimage = f->fmt.pix_mp.plane_fmt[i].sizeimage;

		if (vb2_plane_size(vb, i) < sizeimage)
			return -EINVAL;
	}

	/*
	 * Buffer's bytesused must be written by driver for CAPTURE buffers.
	 * (for OUTPUT buffers, if userspace passes 0 bytesused, v4l2-core sets
	 * it to buffer length).
	 */
	if (V4L2_TYPE_IS_CAPTURE(vq->type)) {
		if (ctx->is_encoder)
			vb2_set_plane_payload(vb, 0, 0);
		else
			vb2_set_plane_payload(vb, 0, f->fmt.pix_mp.plane_fmt[0].sizeimage);
	}

	return 0;
}

static void rkvpu_buf_queue(struct vb2_buffer *vb)
{
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vb->vb2_queue);
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);

	v4l2_m2m_buf_queue(ctx->fh.m2m_ctx, vbuf);
}

static void rkvpu_buf_cleanup(struct vb2_buffer *vb)
{
	struct vb2_queue *vq = vb->vb2_queue;
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vq);
	struct rkvpu_dev *vpu = ctx->dev;
	struct rkvpu_enc_buf *enc_buf;
	struct rkvpu_aux_buf *rec_buf;
	struct rkvpu_aux_buf *ds_buf;
	struct rkvpu_aux_buf *me_buf;

	if (!ctx->is_encoder || V4L2_TYPE_IS_OUTPUT(vq->type))
		return;

	enc_buf = rkvpu_get_enc_buf(vbuf);
	rec_buf = &enc_buf->rec_buf;
	ds_buf = &enc_buf->ds_buf;
	me_buf = &enc_buf->me_buf;

	dma_free_coherent(vpu->dev, rec_buf->size, rec_buf->cpu, rec_buf->dma);
	dma_free_coherent(vpu->dev, ds_buf->size, ds_buf->cpu, ds_buf->dma);
	dma_free_coherent(vpu->dev, me_buf->size, me_buf->cpu, me_buf->dma);
}

static int rkvpu_buf_out_validate(struct vb2_buffer *vb)
{
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);

	vbuf->field = V4L2_FIELD_NONE;
	return 0;
}

static void rkvpu_buf_request_complete(struct vb2_buffer *vb)
{
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vb->vb2_queue);

	v4l2_ctrl_request_complete(vb->req_obj.req, &ctx->ctrl_hdl);
}

static int rkvpu_start_streaming(struct vb2_queue *q, unsigned int count)
{
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(q);
	const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;

	int ret;

	if (V4L2_TYPE_IS_CAPTURE(q->type))
		return 0;

	if (WARN_ON(!desc))
		return -EINVAL;

	if (desc->ops->start) {
		ret = desc->ops->start(ctx);
		if (ret)
			return ret;
	}

	return 0;
}

static void rkvpu_queue_cleanup(struct vb2_queue *vq, u32 state)
{
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vq);

	while (true) {
		struct vb2_v4l2_buffer *vbuf;

		if (V4L2_TYPE_IS_OUTPUT(vq->type))
			vbuf = v4l2_m2m_src_buf_remove(ctx->fh.m2m_ctx);
		else
			vbuf = v4l2_m2m_dst_buf_remove(ctx->fh.m2m_ctx);

		if (!vbuf)
			break;

		v4l2_ctrl_request_complete(vbuf->vb2_buf.req_obj.req,
					   &ctx->ctrl_hdl);
		v4l2_m2m_buf_done(vbuf, state);
	}
}

static void rkvpu_stop_streaming(struct vb2_queue *q)
{
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(q);

	if (V4L2_TYPE_IS_OUTPUT(q->type)) {
		const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;

		if (WARN_ON(!desc))
			return;

		if (desc->ops->stop)
			desc->ops->stop(ctx);
	}

	rkvpu_queue_cleanup(q, VB2_BUF_STATE_ERROR);
}

static const struct vb2_ops rkvpu_queue_ops = {
	.queue_setup = rkvpu_queue_setup,
	.buf_init = rkvpu_buf_init,
	.buf_prepare = rkvpu_buf_prepare,
	.buf_queue = rkvpu_buf_queue,
	.buf_out_validate = rkvpu_buf_out_validate,
	.buf_request_complete = rkvpu_buf_request_complete,
	.buf_cleanup = rkvpu_buf_cleanup,
	.start_streaming = rkvpu_start_streaming,
	.stop_streaming = rkvpu_stop_streaming,
	.wait_prepare = vb2_ops_wait_prepare,
	.wait_finish = vb2_ops_wait_finish,
};

static int rkvpu_request_validate(struct media_request *req)
{
	unsigned int count;

	count = vb2_request_buffer_cnt(req);
	if (!count)
		return -ENOENT;
	else if (count > 1)
		return -EINVAL;

	return vb2_request_validate(req);
}

static const struct media_device_ops rkvpu_media_ops = {
	.req_validate = rkvpu_request_validate,
	.req_queue = v4l2_m2m_request_queue,
};

static int rkvpu_register_entity(struct media_device *mdev,
				  struct media_entity *entity,
				  const char *entity_name,
				  struct media_pad *pads, int num_pads,
				  int function, struct video_device *vdev)
{
	char *name;
	int ret;

	entity->obj_type = MEDIA_ENTITY_TYPE_BASE;
	if (function == MEDIA_ENT_F_IO_V4L) {
		entity->info.dev.major = VIDEO_MAJOR;
		entity->info.dev.minor = vdev->minor;
	}

	name = devm_kasprintf(mdev->dev, GFP_KERNEL, "%s-%s", vdev->name,
			      entity_name);
	if (!name)
		return -ENOMEM;

	entity->name = name;
	entity->function = function;

	ret = media_entity_pads_init(entity, num_pads, pads);
	if (ret)
		return ret;

	ret = media_device_register_entity(mdev, entity);
	if (ret)
		return ret;

	return 0;
}

static int rkvpu_attach_func(struct rkvpu_dev *vpu,
			      struct rkvpu_func *func)
{
	struct media_device *mdev = &vpu->mdev;
	struct media_link *link;
	int ret;

	/* Create the three encoder entities with their pads */
	func->source_pad.flags = MEDIA_PAD_FL_SOURCE;
	ret = rkvpu_register_entity(mdev, &func->vdev.entity, "source",
				     &func->source_pad, 1, MEDIA_ENT_F_IO_V4L,
				     &func->vdev);
	if (ret)
		return ret;

	func->proc_pads[0].flags = MEDIA_PAD_FL_SINK;
	func->proc_pads[1].flags = MEDIA_PAD_FL_SOURCE;
	ret = rkvpu_register_entity(mdev, &func->proc, "proc",
				     func->proc_pads, 2, func->id,
				     &func->vdev);
	if (ret)
		goto err_rel_entity0;

	func->sink_pad.flags = MEDIA_PAD_FL_SINK;
	ret = rkvpu_register_entity(mdev, &func->sink, "sink",
				     &func->sink_pad, 1, MEDIA_ENT_F_IO_V4L,
				     &func->vdev);
	if (ret)
		goto err_rel_entity1;

	/* Connect the three entities */
	ret = media_create_pad_link(&func->vdev.entity, 0, &func->proc, 0,
				    MEDIA_LNK_FL_IMMUTABLE |
				    MEDIA_LNK_FL_ENABLED);
	if (ret)
		goto err_rel_entity2;

	ret = media_create_pad_link(&func->proc, 1, &func->sink, 0,
				    MEDIA_LNK_FL_IMMUTABLE |
				    MEDIA_LNK_FL_ENABLED);
	if (ret)
		goto err_rm_links0;

	/* Create video interface */
	func->intf_devnode = media_devnode_create(mdev, MEDIA_INTF_T_V4L_VIDEO,
						  0, VIDEO_MAJOR,
						  func->vdev.minor);
	if (!func->intf_devnode) {
		ret = -ENOMEM;
		goto err_rm_links1;
	}

	/* Connect the two DMA engines to the interface */
	link = media_create_intf_link(&func->vdev.entity,
				      &func->intf_devnode->intf,
				      MEDIA_LNK_FL_IMMUTABLE |
				      MEDIA_LNK_FL_ENABLED);
	if (!link) {
		ret = -ENOMEM;
		goto err_rm_devnode;
	}

	link = media_create_intf_link(&func->sink, &func->intf_devnode->intf,
				      MEDIA_LNK_FL_IMMUTABLE |
				      MEDIA_LNK_FL_ENABLED);
	if (!link) {
		ret = -ENOMEM;
		goto err_rm_devnode;
	}
	return 0;

err_rm_devnode:
	media_devnode_remove(func->intf_devnode);

err_rm_links1:
	media_entity_remove_links(&func->sink);

err_rm_links0:
	media_entity_remove_links(&func->proc);
	media_entity_remove_links(&func->vdev.entity);

err_rel_entity2:
	media_device_unregister_entity(&func->sink);

err_rel_entity1:
	media_device_unregister_entity(&func->proc);

err_rel_entity0:
	media_device_unregister_entity(&func->vdev.entity);
	return ret;
}

static void rkvpu_detach_func(struct rkvpu_func *func)
{
	media_devnode_remove(func->intf_devnode);
	media_entity_remove_links(&func->sink);
	media_entity_remove_links(&func->proc);
	media_entity_remove_links(&func->vdev.entity);
	media_device_unregister_entity(&func->sink);
	media_device_unregister_entity(&func->proc);
	media_device_unregister_entity(&func->vdev.entity);
}

static const struct v4l2_file_operations rkvpu_fops;
static const struct of_device_id of_rkvpu_match[];

static int rkvpu_add_func(struct rkvpu_dev *rkvpu, unsigned int funcid)
{
	const struct of_device_id *match;
	struct rkvpu_func *func;
	struct video_device *vfd;
	int ret;

	match = of_match_node(of_rkvpu_match, rkvpu->dev->of_node);
	func = devm_kzalloc(rkvpu->dev, sizeof(*func), GFP_KERNEL);
	if (!func) {
		v4l2_err(&rkvpu->v4l2_dev, "Failed to allocate video device\n");
		return -ENOMEM;
	}

	func->id = funcid;

	vfd = &func->vdev;
	vfd->fops = &rkvpu_fops;
	vfd->release = video_device_release_empty;
	vfd->lock = &rkvpu->vdev_lock;
	vfd->v4l2_dev = &rkvpu->v4l2_dev;
	vfd->vfl_dir = VFL_DIR_M2M;
	vfd->device_caps = V4L2_CAP_STREAMING |
			   V4L2_CAP_VIDEO_M2M_MPLANE;
	vfd->ioctl_ops = &rkvpu_ioctl_ops;
	snprintf(vfd->name, sizeof(vfd->name), "%s-%s", match->compatible,
		 funcid == MEDIA_ENT_F_PROC_VIDEO_ENCODER ? "enc" : "dec");

	if (funcid == MEDIA_ENT_F_PROC_VIDEO_ENCODER) {
		rkvpu->encoder = func;
	} else {
		rkvpu->decoder = func;
		v4l2_disable_ioctl(vfd, VIDIOC_TRY_ENCODER_CMD);
		v4l2_disable_ioctl(vfd, VIDIOC_ENCODER_CMD);
	}

	video_set_drvdata(vfd, rkvpu);

	ret = video_register_device(vfd, VFL_TYPE_VIDEO, -1);
	if (ret) {
		v4l2_err(&rkvpu->v4l2_dev, "Failed to register video device\n");
		return ret;
	}

	ret = rkvpu_attach_func(rkvpu, func);
	if (ret) {
		v4l2_err(&rkvpu->v4l2_dev,
			 "Failed to attach functionality to the media device\n");
		goto err_unreg_dev;
	}

	v4l2_info(&rkvpu->v4l2_dev, "registered %s as /dev/video%d\n",
		  vfd->name, vfd->num);

	return 0;

err_unreg_dev:
	video_unregister_device(vfd);
	return ret;
}

static int rkvpu_add_enc_func(struct rkvpu_dev *rkvpu)
{
	if (!(rkvpu->variant->codec & RKVPU_ENCODERS))
		return 0;

	return rkvpu_add_func(rkvpu, MEDIA_ENT_F_PROC_VIDEO_ENCODER);
}

static int rkvpu_add_dec_func(struct rkvpu_dev *rkvpu)
{
	if (!(rkvpu->variant->codec & RKVPU_DECODERS))
		return 0;

	return rkvpu_add_func(rkvpu, MEDIA_ENT_F_PROC_VIDEO_DECODER);
}

static void rkvpu_remove_func(struct rkvpu_dev *rkvpu,
			       unsigned int funcid)
{
	struct rkvpu_func *func;

	if (funcid == MEDIA_ENT_F_PROC_VIDEO_ENCODER)
		func = rkvpu->encoder;
	else
		func = rkvpu->decoder;

	if (!func)
		return;

	rkvpu_detach_func(func);
	video_unregister_device(&func->vdev);
}

static void rkvpu_remove_enc_func(struct rkvpu_dev *rkvpu)
{
	rkvpu_remove_func(rkvpu, MEDIA_ENT_F_PROC_VIDEO_ENCODER);
}

static void rkvpu_remove_dec_func(struct rkvpu_dev *rkvpu)
{
	rkvpu_remove_func(rkvpu, MEDIA_ENT_F_PROC_VIDEO_DECODER);
}

static void rkvpu_job_finish_no_pm(struct rkvpu_ctx *ctx,
				    enum vb2_buffer_state result)
{
	const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;

	if (desc->ops->done) {
		struct vb2_v4l2_buffer *src_buf, *dst_buf;

		src_buf = v4l2_m2m_next_src_buf(ctx->fh.m2m_ctx);
		dst_buf = v4l2_m2m_next_dst_buf(ctx->fh.m2m_ctx);
		desc->ops->done(ctx, src_buf, dst_buf, result);
	}

	v4l2_m2m_buf_done_and_job_finish(ctx->dev->m2m_dev, ctx->fh.m2m_ctx,
					 result);
}

static void rkvpu_job_finish(struct rkvpu_ctx *ctx,
			      enum vb2_buffer_state result)
{
	struct rkvpu_dev *rkvpu = ctx->dev;

	pm_runtime_mark_last_busy(rkvpu->dev);
	pm_runtime_put_autosuspend(rkvpu->dev);
	rkvpu_job_finish_no_pm(ctx, result);
}

void rkvpu_run_preamble(struct rkvpu_ctx *ctx, struct rkvpu_run *run)
{
	struct media_request *src_req;

	memset(run, 0, sizeof(*run));

	run->bufs.src = v4l2_m2m_next_src_buf(ctx->fh.m2m_ctx);
	run->bufs.dst = v4l2_m2m_next_dst_buf(ctx->fh.m2m_ctx);

	/* Apply request(s) controls if needed. */
	src_req = run->bufs.src->vb2_buf.req_obj.req;
	if (src_req)
		v4l2_ctrl_request_setup(src_req, &ctx->ctrl_hdl);

	v4l2_m2m_buf_copy_metadata(run->bufs.src, run->bufs.dst, true);
}

void rkvpu_run_postamble(struct rkvpu_ctx *ctx, struct rkvpu_run *run)
{
	struct media_request *src_req = run->bufs.src->vb2_buf.req_obj.req;

	if (src_req)
		v4l2_ctrl_request_complete(src_req, &ctx->ctrl_hdl);
}

static void rkvpu_device_run(void *priv)
{
	struct rkvpu_ctx *ctx = priv;
	struct rkvpu_dev *rkvpu = ctx->dev;
	const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;
	int ret;

	if (WARN_ON(!desc))
		return;

	ret = pm_runtime_resume_and_get(rkvpu->dev);
	if (ret < 0) {
		rkvpu_job_finish_no_pm(ctx, VB2_BUF_STATE_ERROR);
		return;
	}

	ret = desc->ops->run(ctx);
	if (ret)
		rkvpu_job_finish(ctx, VB2_BUF_STATE_ERROR);
}

static const struct v4l2_m2m_ops rkvpu_m2m_ops = {
	.device_run = rkvpu_device_run,
};

static int rkvpu_queue_init(void *priv,
			     struct vb2_queue *src_vq,
			     struct vb2_queue *dst_vq)
{
	struct rkvpu_ctx *ctx = priv;
	struct rkvpu_dev *rkvpu = ctx->dev;
	int ret;

	src_vq->type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	src_vq->io_modes = VB2_MMAP | VB2_DMABUF;
	src_vq->drv_priv = ctx;
	src_vq->ops = &rkvpu_queue_ops;
	src_vq->mem_ops = &vb2_dma_contig_memops;

	if (ctx->is_encoder)
		src_vq->buf_struct_size = sizeof(struct rkvpu_src_buffer);
	/*
	 * Driver does mostly sequential access, so sacrifice TLB efficiency
	 * for faster allocation. Also, no CPU access on the source queue,
	 * so no kernel mapping needed.
	 */
	src_vq->dma_attrs = DMA_ATTR_ALLOC_SINGLE_PAGES |
			    DMA_ATTR_NO_KERNEL_MAPPING;
	src_vq->buf_struct_size = sizeof(struct v4l2_m2m_buffer);
	src_vq->timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_COPY;
	src_vq->lock = &rkvpu->vdev_lock;
	src_vq->dev = rkvpu->v4l2_dev.dev;
	src_vq->supports_requests = true;
	src_vq->requires_requests = true;

	if (ctx->is_encoder)
		src_vq->buf_struct_size = sizeof(struct rkvpu_enc_buf);

	ret = vb2_queue_init(src_vq);
	if (ret)
		return ret;

	dst_vq->bidirectional = true;
	dst_vq->mem_ops = &vb2_dma_contig_memops;
	dst_vq->dma_attrs = DMA_ATTR_ALLOC_SINGLE_PAGES;
	if (!ctx->is_encoder)
		dst_vq->dma_attrs |= DMA_ATTR_NO_KERNEL_MAPPING;
	dst_vq->type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	dst_vq->io_modes = VB2_MMAP | VB2_DMABUF;
	dst_vq->drv_priv = ctx;
	dst_vq->ops = &rkvpu_queue_ops;
	dst_vq->buf_struct_size = sizeof(struct rkvpu_src_buffer);
	dst_vq->timestamp_flags = V4L2_BUF_FLAG_TIMESTAMP_COPY;
	dst_vq->lock = &rkvpu->vdev_lock;
	dst_vq->dev = rkvpu->v4l2_dev.dev;

	return vb2_queue_init(dst_vq);
}

static int rkvpu_init_ctrls(struct rkvpu_ctx *ctx, int allowed_codecs)
{
	unsigned int i, nctrls = ARRAY_SIZE(controls);
	int ret;

	v4l2_ctrl_handler_init(&ctx->ctrl_hdl, nctrls);

	for (i = 0; i < nctrls; i++) {
		if (!(allowed_codecs & controls[i].codec))
			continue;

		v4l2_ctrl_new_custom(&ctx->ctrl_hdl,
				     &controls[i].cfg, ctx);
			if (ctx->ctrl_hdl.error)
				return ctx->ctrl_hdl.error;

	}

	ret = v4l2_ctrl_handler_setup(&ctx->ctrl_hdl);
	if (ret)
		goto err_free_handler;

	ctx->fh.ctrl_handler = &ctx->ctrl_hdl;
	return 0;

err_free_handler:
	v4l2_ctrl_handler_free(&ctx->ctrl_hdl);
	return ret;
}

static int rkvpu_open(struct file *filp)
{
	struct rkvpu_dev *rkvpu = video_drvdata(filp);
	struct video_device *vdev = video_devdata(filp);
	struct rkvpu_func *func = rkvpu_vdev_to_func(vdev);
	struct rkvpu_ctx *ctx;
	int allowed_codecs, ret;

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->dev = rkvpu;
	if (func->id == MEDIA_ENT_F_PROC_VIDEO_ENCODER) {
		allowed_codecs = rkvpu->variant->codec & RKVPU_ENCODERS;
		ctx->is_encoder = true;
	} else if (func->id == MEDIA_ENT_F_PROC_VIDEO_DECODER) {
		allowed_codecs = rkvpu->variant->codec & RKVPU_DECODERS;
		ctx->is_encoder = false;
	} else {
		ret = -ENODEV;
		goto err_free_ctx;
	}

	rkvpu_reset_src_fmt(ctx);
	rkvpu_reset_dst_fmt(ctx);
	v4l2_fh_init(&ctx->fh, video_devdata(filp));

	ret = rkvpu_init_ctrls(ctx, allowed_codecs);
	if (ret)
		goto err_free_fh;

	ctx->fh.m2m_ctx = v4l2_m2m_ctx_init(rkvpu->m2m_dev, ctx,
					    rkvpu_queue_init);
	if (IS_ERR(ctx->fh.m2m_ctx)) {
		ret = PTR_ERR(ctx->fh.m2m_ctx);
		goto err_cleanup_ctrls;
	}

	filp->private_data = &ctx->fh;
	v4l2_fh_add(&ctx->fh);

	return 0;

err_cleanup_ctrls:
	v4l2_ctrl_handler_free(&ctx->ctrl_hdl);

err_free_fh:
	v4l2_fh_exit(&ctx->fh);

err_free_ctx:
	kfree(ctx);
	return ret;
}

static int rkvpu_release(struct file *filp)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(filp->private_data);

	v4l2_fh_del(&ctx->fh);
	v4l2_m2m_ctx_release(ctx->fh.m2m_ctx);
	v4l2_ctrl_handler_free(&ctx->ctrl_hdl);
	v4l2_fh_exit(&ctx->fh);
	kfree(ctx);

	return 0;
}

static const struct v4l2_file_operations rkvpu_fops = {
	.owner = THIS_MODULE,
	.open = rkvpu_open,
	.release = rkvpu_release,
	.poll = v4l2_m2m_fop_poll,
	.unlocked_ioctl = video_ioctl2,
	.mmap = v4l2_m2m_fop_mmap,
};

static int rkvpu_v4l2_init(struct rkvpu_dev *rkvpu)
{
	int ret;

	ret = v4l2_device_register(rkvpu->dev, &rkvpu->v4l2_dev);
	if (ret) {
		dev_err(rkvpu->dev, "Failed to register V4L2 device\n");
		return ret;
	}

	rkvpu->m2m_dev = v4l2_m2m_init(&rkvpu_m2m_ops);
	if (IS_ERR(rkvpu->m2m_dev)) {
		v4l2_err(&rkvpu->v4l2_dev, "Failed to init mem2mem device\n");
		ret = PTR_ERR(rkvpu->m2m_dev);
		goto err_unregister_v4l2;
	}

	rkvpu->mdev.dev = rkvpu->dev;
	strscpy(rkvpu->mdev.model, "rkvpu", sizeof(rkvpu->mdev.model));
	strscpy(rkvpu->mdev.bus_info, "platform:rkvpu",
		sizeof(rkvpu->mdev.bus_info));
	media_device_init(&rkvpu->mdev);
	rkvpu->mdev.ops = &rkvpu_media_ops;
	rkvpu->v4l2_dev.mdev = &rkvpu->mdev;

	ret = rkvpu_add_enc_func(rkvpu);
	if (ret) {
		dev_err(rkvpu->dev, "Failed to register encoder\n");
		goto err_cleanup_mc;
	}

	ret = rkvpu_add_dec_func(rkvpu);
	if (ret) {
		dev_err(rkvpu->dev, "Failed to register decoder\n");
		goto err_rm_enc_func;
	}

	ret = media_device_register(&rkvpu->mdev);
	if (ret) {
		v4l2_err(&rkvpu->v4l2_dev, "Failed to register media device\n");
		goto err_rm_dec_func;
	}

	return 0;


err_rm_dec_func:
	rkvpu_remove_dec_func(rkvpu);

err_rm_enc_func:
	rkvpu_remove_enc_func(rkvpu);

err_cleanup_mc:
	media_device_cleanup(&rkvpu->mdev);
	v4l2_m2m_release(rkvpu->m2m_dev);

err_unregister_v4l2:
	v4l2_device_unregister(&rkvpu->v4l2_dev);
	return ret;
}

static void rkvpu_v4l2_cleanup(struct rkvpu_dev *rkvpu)
{
	media_device_unregister(&rkvpu->mdev);
	rkvpu_remove_dec_func(rkvpu);
	rkvpu_remove_enc_func(rkvpu);
	media_device_cleanup(&rkvpu->mdev);
	v4l2_m2m_release(rkvpu->m2m_dev);
	v4l2_device_unregister(&rkvpu->v4l2_dev);
}

static irqreturn_t rkvpu_irq_handler(int irq, void *priv)
{
	struct rkvpu_dev *rkvpu = priv;
	struct rkvpu_ctx *ctx = v4l2_m2m_get_curr_priv(rkvpu->m2m_dev);
	const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;
	enum vb2_buffer_state state = VB2_BUF_STATE_ERROR;

	if (WARN_ON(!desc))
		return IRQ_HANDLED;

	if (desc->ops->irq)
		state = desc->ops->irq(ctx);

	if (cancel_delayed_work(&rkvpu->watchdog_work)) {
		struct rkvpu_ctx *ctx;

		ctx = v4l2_m2m_get_curr_priv(rkvpu->m2m_dev);
		rkvpu_job_finish(ctx, state);
	}

	return IRQ_HANDLED;
}

static void rkvpu_watchdog_func(struct work_struct *work)
{
	struct rkvpu_dev *rkvpu = container_of(to_delayed_work(work),
				struct rkvpu_dev, watchdog_work);
	struct rkvpu_ctx *ctx = v4l2_m2m_get_curr_priv(rkvpu->m2m_dev);
	const struct rkvpu_fmt_desc *desc = ctx->fmt_desc;

	dev_err(rkvpu->dev, "Frame processing timed out!\n");

	if (WARN_ON(!desc))
		return;

	if (desc->ops->watchdog)
		desc->ops->watchdog(ctx);

	rkvpu_job_finish(ctx, VB2_BUF_STATE_ERROR);
}

const struct rkvpu_variant rkvdpu_vpu_variant = {
	.fmts = rkvpu_dec_fmts,
	.num_fmts = ARRAY_SIZE(rkvpu_dec_fmts),
	.codec = RKVPU_H264_DECODER | RKVPU_VP9_DECODER,
};

const struct rkvpu_variant rkvepu540_vpu_variant = {
	.fmts = rkvpu_enc_fmts,
	.num_fmts = ARRAY_SIZE(rkvpu_enc_fmts),
	.codec = RKVPU_H264_ENCODER,
};

static const struct of_device_id of_rkvpu_match[] = {
	{ .compatible = "rockchip,rk3399-vdec", .data = &rkvdpu_vpu_variant },
	{ .compatible = "rockchip,rk356x-venc", .data = &rkvepu540_vpu_variant },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_rkvpu_match);

static const char * const rkvpu_clk_names[] = {
	"axi", "ahb", "cabac", "core"
};

static int rkvpu_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	struct rkvpu_dev *rkvpu;
	unsigned int i;
	int ret, irq;

	rkvpu = devm_kzalloc(&pdev->dev, sizeof(*rkvpu), GFP_KERNEL);
	if (!rkvpu)
		return -ENOMEM;

	platform_set_drvdata(pdev, rkvpu);
	rkvpu->dev = &pdev->dev;
	mutex_init(&rkvpu->vdev_lock);
	INIT_DELAYED_WORK(&rkvpu->watchdog_work, rkvpu_watchdog_func);

	match = of_match_node(of_rkvpu_match, pdev->dev.of_node);
	rkvpu->variant = match->data;

	rkvpu->clocks = devm_kcalloc(&pdev->dev, ARRAY_SIZE(rkvpu_clk_names),
				      sizeof(*rkvpu->clocks), GFP_KERNEL);
	if (!rkvpu->clocks)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(rkvpu_clk_names); i++)
		rkvpu->clocks[i].id = rkvpu_clk_names[i];

	ret = devm_clk_bulk_get_optional(&pdev->dev,
				ARRAY_SIZE(rkvpu_clk_names),
				rkvpu->clocks);
	if (ret)
		return ret;

	rkvpu->regs = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(rkvpu->regs))
		return PTR_ERR(rkvpu->regs);

	ret = dma_set_coherent_mask(&pdev->dev, DMA_BIT_MASK(32));
	if (ret) {
		dev_err(&pdev->dev, "Could not set DMA coherent mask.\n");
		return ret;
	}

	vb2_dma_contig_set_max_seg_size(&pdev->dev, DMA_BIT_MASK(32));

	irq = platform_get_irq(pdev, 0);
	if (irq <= 0)
		return -ENXIO;

	ret = devm_request_threaded_irq(&pdev->dev, irq, NULL,
					rkvpu_irq_handler, IRQF_ONESHOT,
					dev_name(&pdev->dev), rkvpu);
	if (ret) {
		dev_err(&pdev->dev, "Could not request vdec IRQ\n");
		return ret;
	}

	pm_runtime_set_autosuspend_delay(&pdev->dev, 100);
	pm_runtime_use_autosuspend(&pdev->dev);
	pm_runtime_enable(&pdev->dev);

	ret = rkvpu_v4l2_init(rkvpu);
	if (ret)
		goto err_disable_runtime_pm;

	return 0;

err_disable_runtime_pm:
	pm_runtime_dont_use_autosuspend(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
	return ret;
}

static int rkvpu_remove(struct platform_device *pdev)
{
	struct rkvpu_dev *rkvpu = platform_get_drvdata(pdev);

	rkvpu_v4l2_cleanup(rkvpu);
	pm_runtime_disable(&pdev->dev);
	pm_runtime_dont_use_autosuspend(&pdev->dev);
	return 0;
}

#ifdef CONFIG_PM
static int rkvpu_runtime_resume(struct device *dev)
{
	struct rkvpu_dev *rkvpu = dev_get_drvdata(dev);

	return clk_bulk_prepare_enable(ARRAY_SIZE(rkvpu_clk_names),
				       rkvpu->clocks);
}

static int rkvpu_runtime_suspend(struct device *dev)
{
	struct rkvpu_dev *rkvpu = dev_get_drvdata(dev);

	clk_bulk_disable_unprepare(ARRAY_SIZE(rkvpu_clk_names),
				   rkvpu->clocks);
	return 0;
}
#endif

static const struct dev_pm_ops rkvpu_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				pm_runtime_force_resume)
	SET_RUNTIME_PM_OPS(rkvpu_runtime_suspend, rkvpu_runtime_resume, NULL)
};

static struct platform_driver rkvpu_driver = {
	.probe = rkvpu_probe,
	.remove = rkvpu_remove,
	.driver = {
		   .name = "rkvpu",
		   .of_match_table = of_rkvpu_match,
		   .pm = &rkvpu_pm_ops,
	},
};
module_platform_driver(rkvpu_driver);

MODULE_AUTHOR("Boris Brezillon <boris.brezillon@collabora.com>");
MODULE_DESCRIPTION("Rockchip Video Decoder driver");
MODULE_LICENSE("GPL v2");
