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
#include "rkvpu-regs.h"

static int rkvpu_try_ctrl(struct v4l2_ctrl *ctrl)
{
	struct rkvpu_ctx *ctx = container_of(ctrl->handler, struct rkvpu_ctx, ctrl_hdl);
	const struct rkvpu_fmt_desc *desc = ctx->src_fmt_desc;

	if (desc->ops->try_ctrl)
		return desc->ops->try_ctrl(ctx, ctrl);

	return 0;
}

static const struct v4l2_ctrl_ops rkvpu_ctrl_ops = {
	.try_ctrl = rkvpu_try_ctrl,
};

static const struct rkvpu_ctrl_desc rkvdec_h264_ctrl_descs[] = {
	{
		.cfg.id = V4L2_CID_STATELESS_H264_DECODE_PARAMS,
	},
	{
		.cfg.id = V4L2_CID_STATELESS_H264_SPS,
		.cfg.ops = &rkvpu_ctrl_ops,
	},
	{
		.cfg.id = V4L2_CID_STATELESS_H264_PPS,
	},
	{
		.cfg.id = V4L2_CID_STATELESS_H264_SCALING_MATRIX,
	},
	{
		.cfg.id = V4L2_CID_STATELESS_H264_DECODE_MODE,
		.cfg.min = V4L2_STATELESS_H264_DECODE_MODE_FRAME_BASED,
		.cfg.max = V4L2_STATELESS_H264_DECODE_MODE_FRAME_BASED,
		.cfg.def = V4L2_STATELESS_H264_DECODE_MODE_FRAME_BASED,
	},
	{
		.cfg.id = V4L2_CID_STATELESS_H264_START_CODE,
		.cfg.min = V4L2_STATELESS_H264_START_CODE_ANNEX_B,
		.cfg.def = V4L2_STATELESS_H264_START_CODE_ANNEX_B,
		.cfg.max = V4L2_STATELESS_H264_START_CODE_ANNEX_B,
	},
	{
		.cfg.id = V4L2_CID_MPEG_VIDEO_H264_PROFILE,
		.cfg.min = V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE,
		.cfg.max = V4L2_MPEG_VIDEO_H264_PROFILE_HIGH,
		.cfg.menu_skip_mask =
			BIT(V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED),
		.cfg.def = V4L2_MPEG_VIDEO_H264_PROFILE_MAIN,
	},
	{
		.cfg.id = V4L2_CID_MPEG_VIDEO_H264_LEVEL,
		.cfg.min = V4L2_MPEG_VIDEO_H264_LEVEL_1_0,
		.cfg.max = V4L2_MPEG_VIDEO_H264_LEVEL_5_1,
	},
};

static const struct rkvpu_ctrls rkvdec_h264_ctrls = {
	.ctrls = rkvdec_h264_ctrl_descs,
	.num_ctrls = ARRAY_SIZE(rkvdec_h264_ctrl_descs),
};

static const u32 rkvpu_h264_vp9_dst_fmts[] = {
	V4L2_PIX_FMT_NV12,
};

static const struct rkvpu_ctrl_desc rkvdec_vp9_ctrl_descs[] = {
	{
		.cfg.id = V4L2_CID_STATELESS_VP9_FRAME,
	},
	{
		.cfg.id = V4L2_CID_STATELESS_VP9_COMPRESSED_HDR,
	},
	{
		.cfg.id = V4L2_CID_MPEG_VIDEO_VP9_PROFILE,
		.cfg.min = V4L2_MPEG_VIDEO_VP9_PROFILE_0,
		.cfg.max = V4L2_MPEG_VIDEO_VP9_PROFILE_0,
		.cfg.def = V4L2_MPEG_VIDEO_VP9_PROFILE_0,
	},
};

static const struct rkvpu_ctrls rkvdec_vp9_ctrls = {
	.ctrls = rkvdec_vp9_ctrl_descs,
	.num_ctrls = ARRAY_SIZE(rkvdec_vp9_ctrl_descs),
};

static const struct rkvpu_fmt_desc rkvpu_src_fmts[] = {
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
		.ctrls = &rkvdec_h264_ctrls,
		.ops = &rkvdec_h264_fmt_ops,
		.num_dst_fmts = ARRAY_SIZE(rkvpu_h264_vp9_dst_fmts),
		.dst_fmts = rkvpu_h264_vp9_dst_fmts,
		.subsystem_flags = VB2_V4L2_FL_SUPPORTS_M2M_HOLD_CAPTURE_BUF,
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
		.ctrls = &rkvdec_vp9_ctrls,
		.ops = &rkvdec_vp9_fmt_ops,
		.num_dst_fmts = ARRAY_SIZE(rkvpu_h264_vp9_dst_fmts),
		.dst_fmts = rkvpu_h264_vp9_dst_fmts,
	}
};

static const struct rkvpu_fmt_desc *
rkvpu_find_src_fmt_desc(u32 fourcc)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(rkvpu_src_fmts); i++) {
		if (rkvpu_src_fmts[i].fourcc == fourcc)
			return &rkvpu_src_fmts[i];
	}

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

	ctx->src_fmt_desc = &rkvpu_src_fmts[0];
	rkvpu_reset_fmt(ctx, f, ctx->src_fmt_desc->fourcc);

	f->type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	f->fmt.pix_mp.width = ctx->src_fmt_desc->frmsize.min_width;
	f->fmt.pix_mp.height = ctx->src_fmt_desc->frmsize.min_height;

	if (ctx->src_fmt_desc->ops->adjust_fmt)
		ctx->src_fmt_desc->ops->adjust_fmt(ctx, f);
}

static void rkvpu_reset_dst_fmt(struct rkvpu_ctx *ctx)
{
	struct v4l2_format *f = &ctx->dst_fmt;

	rkvpu_reset_fmt(ctx, f, ctx->src_fmt_desc->dst_fmts[0]);
	f->type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	v4l2_fill_pixfmt_mp(&f->fmt.pix_mp,
			    ctx->src_fmt_desc->dst_fmts[0],
			    ctx->src_fmt.fmt.pix_mp.width,
			    ctx->src_fmt.fmt.pix_mp.height);
	f->fmt.pix_mp.plane_fmt[0].sizeimage += 128 *
		DIV_ROUND_UP(f->fmt.pix_mp.width, 16) *
		DIV_ROUND_UP(f->fmt.pix_mp.height, 16);
}

static int rkvpu_enum_framesizes(struct file *file, void *priv,
				  struct v4l2_frmsizeenum *fsize)
{
	const struct rkvpu_fmt_desc *fmt;

	if (fsize->index != 0)
		return -EINVAL;

	fmt = rkvpu_find_src_fmt_desc(fsize->pixel_format);
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
	const struct rkvpu_fmt_desc *src_desc;
	unsigned int i;

	/*
	 * The codec context should point to a src format desc, if the format
	 * on the src end has not been set yet, it should point to the
	 * default value.
	 */
	src_desc = ctx->src_fmt_desc;
	if (WARN_ON(!src_desc))
		return -EINVAL;

	for (i = 0; i < src_desc->num_dst_fmts; i++) {
		if (src_desc->dst_fmts[i] == pix_mp->pixelformat)
			break;
	}

	if (i == src_desc->num_dst_fmts)
		pix_mp->pixelformat = src_desc->dst_fmts[0];

	/* Always apply the frmsize constraint of the src end. */
	pix_mp->width = max(pix_mp->width, ctx->src_fmt.fmt.pix_mp.width);
	pix_mp->height = max(pix_mp->height, ctx->src_fmt.fmt.pix_mp.height);
	v4l2_apply_frmsize_constraints(&pix_mp->width,
				       &pix_mp->height,
				       &src_desc->frmsize);

	v4l2_fill_pixfmt_mp(pix_mp, pix_mp->pixelformat,
			    pix_mp->width, pix_mp->height);
	pix_mp->plane_fmt[0].sizeimage +=
		128 *
		DIV_ROUND_UP(pix_mp->width, 16) *
		DIV_ROUND_UP(pix_mp->height, 16);
	pix_mp->field = V4L2_FIELD_NONE;

	return 0;
}

static int rkvpu_try_output_fmt(struct file *file, void *priv,
				 struct v4l2_format *f)
{
	struct v4l2_pix_format_mplane *pix_mp = &f->fmt.pix_mp;
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	const struct rkvpu_fmt_desc *desc;

	desc = rkvpu_find_src_fmt_desc(pix_mp->pixelformat);
	if (!desc) {
		pix_mp->pixelformat = rkvpu_src_fmts[0].fourcc;
		desc = &rkvpu_src_fmts[0];
	}

	v4l2_apply_frmsize_constraints(&pix_mp->width,
				       &pix_mp->height,
				       &desc->frmsize);

	pix_mp->field = V4L2_FIELD_NONE;
	/* All src formats are considered single planar for now. */
	pix_mp->num_planes = 1;

	if (desc->ops->adjust_fmt) {
		int ret;

		ret = desc->ops->adjust_fmt(ctx, f);
		if (ret)
			return ret;
	}

	return 0;
}

static int rkvpu_s_capture_fmt(struct file *file, void *priv,
				struct v4l2_format *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	struct vb2_queue *vq;
	int ret;

	/* Change not allowed if queue is busy */
	vq = v4l2_m2m_get_vq(ctx->fh.m2m_ctx,
			     V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE);
	if (vb2_is_busy(vq))
		return -EBUSY;

	ret = rkvpu_try_capture_fmt(file, priv, f);
	if (ret)
		return ret;

	ctx->dst_fmt = *f;
	return 0;
}

static int rkvpu_s_output_fmt(struct file *file, void *priv,
			       struct v4l2_format *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);
	struct v4l2_m2m_ctx *m2m_ctx = ctx->fh.m2m_ctx;
	const struct rkvpu_fmt_desc *desc;
	struct v4l2_format *cap_fmt;
	struct vb2_queue *peer_vq, *vq;
	int ret;

	/*
	 * In order to support dynamic resolution change, the decoder admits
	 * a resolution change, as long as the pixelformat remains. Can't be
	 * done if streaming.
	 */
	vq = v4l2_m2m_get_vq(m2m_ctx, V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
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

	ret = rkvpu_try_output_fmt(file, priv, f);
	if (ret)
		return ret;

	desc = rkvpu_find_src_fmt_desc(f->fmt.pix_mp.pixelformat);
	if (!desc)
		return -EINVAL;
	ctx->src_fmt_desc = desc;
	ctx->src_fmt = *f;

	/*
	 * Current desrc format might have become invalid with newly
	 * selected codec, so reset it to default just to be safe and
	 * keep internal driver state sane. User is mandated to set
	 * the desrc format again after we return, so we don't need
	 * anything smarter.
	 *
	 * Note that this will propagates any size changes to the desrc format.
	 */
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

static int rkvpu_enum_output_fmt(struct file *file, void *priv,
				  struct v4l2_fmtdesc *f)
{
	if (f->index >= ARRAY_SIZE(rkvpu_src_fmts))
		return -EINVAL;

	f->pixelformat = rkvpu_src_fmts[f->index].fourcc;
	return 0;
}

static int rkvpu_enum_capture_fmt(struct file *file, void *priv,
				   struct v4l2_fmtdesc *f)
{
	struct rkvpu_ctx *ctx = fh_to_rkvpu_ctx(priv);

	if (WARN_ON(!ctx->src_fmt_desc))
		return -EINVAL;

	if (f->index >= ctx->src_fmt_desc->num_dst_fmts)
		return -EINVAL;

	f->pixelformat = ctx->src_fmt_desc->dst_fmts[f->index];
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
	.vidioc_enum_fmt_vid_out = rkvpu_enum_output_fmt,
	.vidioc_enum_fmt_vid_cap = rkvpu_enum_capture_fmt,

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
	struct v4l2_format *f;
	unsigned int i;

	if (V4L2_TYPE_IS_OUTPUT(vq->type))
		f = &ctx->src_fmt;
	else
		f = &ctx->dst_fmt;

	if (*num_planes) {
		if (*num_planes != f->fmt.pix_mp.num_planes)
			return -EINVAL;

		for (i = 0; i < f->fmt.pix_mp.num_planes; i++) {
			if (sizes[i] < f->fmt.pix_mp.plane_fmt[i].sizeimage)
				return -EINVAL;
		}
	} else {
		*num_planes = f->fmt.pix_mp.num_planes;
		for (i = 0; i < f->fmt.pix_mp.num_planes; i++)
			sizes[i] = f->fmt.pix_mp.plane_fmt[i].sizeimage;
	}

	return 0;
}

static int rkvpu_buf_prepare(struct vb2_buffer *vb)
{
	struct vb2_queue *vq = vb->vb2_queue;
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vq);
	struct v4l2_format *f;
	unsigned int i;

	if (V4L2_TYPE_IS_OUTPUT(vq->type))
		f = &ctx->src_fmt;
	else
		f = &ctx->dst_fmt;

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
	if (V4L2_TYPE_IS_CAPTURE(vq->type))
		vb2_set_plane_payload(vb, 0, f->fmt.pix_mp.plane_fmt[0].sizeimage);

	return 0;
}

static void rkvpu_buf_queue(struct vb2_buffer *vb)
{
	struct rkvpu_ctx *ctx = vb2_get_drv_priv(vb->vb2_queue);
	struct vb2_v4l2_buffer *vbuf = to_vb2_v4l2_buffer(vb);

	v4l2_m2m_buf_queue(ctx->fh.m2m_ctx, vbuf);
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
	const struct rkvpu_fmt_desc *desc;
	int ret;

	if (V4L2_TYPE_IS_CAPTURE(q->type))
		return 0;

	desc = ctx->src_fmt_desc;
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
		const struct rkvpu_fmt_desc *desc = ctx->src_fmt_desc;

		if (WARN_ON(!desc))
			return;

		if (desc->ops->stop)
			desc->ops->stop(ctx);
	}

	rkvpu_queue_cleanup(q, VB2_BUF_STATE_ERROR);
}

static const struct vb2_ops rkvpu_queue_ops = {
	.queue_setup = rkvpu_queue_setup,
	.buf_prepare = rkvpu_buf_prepare,
	.buf_queue = rkvpu_buf_queue,
	.buf_out_validate = rkvpu_buf_out_validate,
	.buf_request_complete = rkvpu_buf_request_complete,
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

static void rkvpu_job_finish_no_pm(struct rkvpu_ctx *ctx,
				    enum vb2_buffer_state result)
{
	if (ctx->src_fmt_desc->ops->done) {
		struct vb2_v4l2_buffer *src_buf, *dst_buf;

		src_buf = v4l2_m2m_next_src_buf(ctx->fh.m2m_ctx);
		dst_buf = v4l2_m2m_next_dst_buf(ctx->fh.m2m_ctx);
		ctx->src_fmt_desc->ops->done(ctx, src_buf, dst_buf, result);
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
	const struct rkvpu_fmt_desc *desc = ctx->src_fmt_desc;
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

	ret = vb2_queue_init(src_vq);
	if (ret)
		return ret;

	dst_vq->bidirectional = true;
	dst_vq->mem_ops = &vb2_dma_contig_memops;
	dst_vq->dma_attrs = DMA_ATTR_ALLOC_SINGLE_PAGES |
			    DMA_ATTR_NO_KERNEL_MAPPING;
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

static int rkvpu_add_ctrls(struct rkvpu_ctx *ctx,
			    const struct rkvpu_ctrls *ctrls)
{
	unsigned int i;

	for (i = 0; i < ctrls->num_ctrls; i++) {
		const struct v4l2_ctrl_config *cfg = &ctrls->ctrls[i].cfg;

		v4l2_ctrl_new_custom(&ctx->ctrl_hdl, cfg, ctx);
		if (ctx->ctrl_hdl.error)
			return ctx->ctrl_hdl.error;
	}

	return 0;
}

static int rkvpu_init_ctrls(struct rkvpu_ctx *ctx)
{
	unsigned int i, nctrls = 0;
	int ret;

	for (i = 0; i < ARRAY_SIZE(rkvpu_src_fmts); i++)
		nctrls += rkvpu_src_fmts[i].ctrls->num_ctrls;

	v4l2_ctrl_handler_init(&ctx->ctrl_hdl, nctrls);

	for (i = 0; i < ARRAY_SIZE(rkvpu_src_fmts); i++) {
		ret = rkvpu_add_ctrls(ctx, rkvpu_src_fmts[i].ctrls);
		if (ret)
			goto err_free_handler;
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
	struct rkvpu_ctx *ctx;
	int ret;

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->dev = rkvpu;
	rkvpu_reset_src_fmt(ctx);
	rkvpu_reset_dst_fmt(ctx);
	v4l2_fh_init(&ctx->fh, video_devdata(filp));

	ret = rkvpu_init_ctrls(ctx);
	if (ret)
		goto err_free_ctx;

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

	rkvpu->vdev.lock = &rkvpu->vdev_lock;
	rkvpu->vdev.v4l2_dev = &rkvpu->v4l2_dev;
	rkvpu->vdev.fops = &rkvpu_fops;
	rkvpu->vdev.release = video_device_release_empty;
	rkvpu->vdev.vfl_dir = VFL_DIR_M2M;
	rkvpu->vdev.device_caps = V4L2_CAP_STREAMING |
				   V4L2_CAP_VIDEO_M2M_MPLANE;
	rkvpu->vdev.ioctl_ops = &rkvpu_ioctl_ops;
	video_set_drvdata(&rkvpu->vdev, rkvpu);
	strscpy(rkvpu->vdev.name, "rkvpu", sizeof(rkvpu->vdev.name));

	ret = video_register_device(&rkvpu->vdev, VFL_TYPE_VIDEO, -1);
	if (ret) {
		v4l2_err(&rkvpu->v4l2_dev, "Failed to register video device\n");
		goto err_cleanup_mc;
	}

	ret = v4l2_m2m_register_media_controller(rkvpu->m2m_dev, &rkvpu->vdev,
						 MEDIA_ENT_F_PROC_VIDEO_DECODER);
	if (ret) {
		v4l2_err(&rkvpu->v4l2_dev,
			 "Failed to initialize V4L2 M2M media controller\n");
		goto err_unregister_vdev;
	}

	ret = media_device_register(&rkvpu->mdev);
	if (ret) {
		v4l2_err(&rkvpu->v4l2_dev, "Failed to register media device\n");
		goto err_unregister_mc;
	}

	return 0;

err_unregister_mc:
	v4l2_m2m_unregister_media_controller(rkvpu->m2m_dev);

err_unregister_vdev:
	video_unregister_device(&rkvpu->vdev);

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
	v4l2_m2m_unregister_media_controller(rkvpu->m2m_dev);
	video_unregister_device(&rkvpu->vdev);
	media_device_cleanup(&rkvpu->mdev);
	v4l2_m2m_release(rkvpu->m2m_dev);
	v4l2_device_unregister(&rkvpu->v4l2_dev);
}

static irqreturn_t rkvpu_irq_handler(int irq, void *priv)
{
	struct rkvpu_dev *rkvpu = priv;
	enum vb2_buffer_state state;
	u32 status;

	status = readl(rkvpu->regs + RKVDEC_REG_INTERRUPT);
	state = (status & RKVDEC_RDY_STA) ?
		VB2_BUF_STATE_DONE : VB2_BUF_STATE_ERROR;

	writel(0, rkvpu->regs + RKVDEC_REG_INTERRUPT);
	if (cancel_delayed_work(&rkvpu->watchdog_work)) {
		struct rkvpu_ctx *ctx;

		ctx = v4l2_m2m_get_curr_priv(rkvpu->m2m_dev);
		rkvpu_job_finish(ctx, state);
	}

	return IRQ_HANDLED;
}

static void rkvpu_watchdog_func(struct work_struct *work)
{
	struct rkvpu_dev *rkvpu;
	struct rkvpu_ctx *ctx;

	rkvpu = container_of(to_delayed_work(work), struct rkvpu_dev,
			      watchdog_work);
	ctx = v4l2_m2m_get_curr_priv(rkvpu->m2m_dev);
	if (ctx) {
		dev_err(rkvpu->dev, "Frame processing timed out!\n");
		writel(RKVDEC_IRQ_DIS, rkvpu->regs + RKVDEC_REG_INTERRUPT);
		writel(0, rkvpu->regs + RKVDEC_REG_SYSCTRL);
		rkvpu_job_finish(ctx, VB2_BUF_STATE_ERROR);
	}
}

static const struct of_device_id of_rkvpu_match[] = {
	{ .compatible = "rockchip,rk3399-vdec" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_rkvpu_match);

static const char * const rkvpu_clk_names[] = {
	"axi", "ahb", "cabac", "core"
};

static int rkvpu_probe(struct platform_device *pdev)
{
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

	rkvpu->clocks = devm_kcalloc(&pdev->dev, ARRAY_SIZE(rkvpu_clk_names),
				      sizeof(*rkvpu->clocks), GFP_KERNEL);
	if (!rkvpu->clocks)
		return -ENOMEM;

	for (i = 0; i < ARRAY_SIZE(rkvpu_clk_names); i++)
		rkvpu->clocks[i].id = rkvpu_clk_names[i];

	ret = devm_clk_bulk_get(&pdev->dev, ARRAY_SIZE(rkvpu_clk_names),
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
