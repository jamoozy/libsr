/*!
 * \addtogroup pal
 * \{
 *
 * \file thresh.h
 * Defines the thresholds needed for PaleoSketch recognition.  For more info,
 * see the paper: \cite PaleoSketch.
 */

#ifndef __paleo_thresh_h__
#define __paleo_thresh_h__

#define PAL_THRESH_A   0.5      //!< Tail-removing thresh.
#define PAL_THRESH_B   5.0      //!< Min points for tail removal.
#define PAL_THRESH_C  70.0      //!< Min px_length for tail removal.
#define PAL_THRESH_D   1.31     //!< Overtraced revolution percentage.
#define PAL_THRESH_E   0.16     //!< Closedness dist/len ratio.
#define PAL_THRESH_F   0.75     //!< Closedness min revolutions.
#define PAL_THRESH_G   2.0      //!< Line seg straightness.
#define PAL_THRESH_H  10.25     //!< Line FA/len max ratio.
#define PAL_THRESH_I   0.0036   //!< Pline LSE max.
#define PAL_THRESH_J   6.0      //!< Min DCR for Pline/Arc/Curve.
#define PAL_THRESH_K   0.8      //!< Ellipse/Arc/Spiral NDDE min.
#define PAL_THRESH_L  30.0      //!< Ellipse maj-axis len req.
#define PAL_THRESH_M   0.33     //!< Max FA error for ellipse.
#define PAL_THRESH_N  16.0      //!< Circle/Arc radius len req.
#define PAL_THRESH_O   0.425    //!< Ellipse/Circle tie-breaker.
#define PAL_THRESH_P   0.35     //!< Max FAE for Circle.
#define PAL_THRESH_Q   0.4      //!< Max FAE for Arc.
#define PAL_THRESH_R   0.37     //!< Max LSE for Bézier curves.
#define PAL_THRESH_S   0.9      //!< Spiral avg / bbox r max.
#define PAL_THRESH_T   0.25     //!< Spiral sub-center max difference.
#define PAL_THRESH_U   0.2      //!< Spiral max ep_dist / px_len.
#define PAL_THRESH_V   0.1
#define PAL_THRESH_W   9.0      //!< Min DCR val for pline add at H#3.
#define PAL_THRESH_X  10.0
#define PAL_THRESH_Y   0.99     //!< Corner detection.
#define PAL_THRESH_Z   0.06     //!< Corner merge percentage.

#endif  // __paleo_thresh_h__

/*! \} */
