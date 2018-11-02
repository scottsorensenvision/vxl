//:
// \file
// \brief Estimate corner positions using Forstner/Harris approach
// \author Tim Cootes

#include "vil_corners.h"
#include <vil/vil_fill.h>
#include <cassert>
#include <vcl_compiler.h>
#ifdef _MSC_VER
#  include <vcl_msvc_warnings.h>
#endif
#include <vil/algo/vil_gauss_filter.h>
#include <vil/vil_math.h>

//: Compute Forstner/Harris corner strength function given gradient images.
//  grad_i and grad_j are assumed to be the i and j gradient images (single
//  plane), such as produced by vil_sobel_3x3().  At each pixel compute
//  the Forstner/Harris corner function: det(H)-k*sqr(trace(H)), where
//  H is the 2x2 matrix of second derivatives, generated by applying a Sobel
//  operator to the gradient images.
//
//  The local peaks of the output image correspond to corner candidates.
// \relatesalso vil_image_view
void vil_corners(const vil_image_view<float>& grad_i,
                 const vil_image_view<float>& grad_j,
                 vil_image_view<float>& dest, double k)
{
  assert(grad_i.nplanes()==1);
  assert(grad_j.nplanes()==1);
  unsigned ni = grad_i.ni(), nj = grad_i.nj();
  assert(grad_j.ni()==ni && grad_j.nj()==nj );

  dest.set_size(ni,nj);

  // Zero a two pixel border
  for (unsigned i=0;i<2;++i)
  {
    vil_fill_row(dest,i,0.0f);
    vil_fill_row(dest,nj-1-i,0.0f);
    vil_fill_col(dest,i,0.0f);
    vil_fill_col(dest,ni-1-i,0.0f);
  }

  const unsigned ni2 = ni-2;
  const unsigned nj2 = nj-2;

  // Compute relative grid positions
  //  o1 o2 o3
  //  o4    o5
  //  o6 o7 o8
  const std::ptrdiff_t oi1 = grad_i.jstep() - grad_i.istep();
  const std::ptrdiff_t oi2 = grad_i.jstep();
  const std::ptrdiff_t oi3 = grad_i.istep() + grad_i.jstep();
  const std::ptrdiff_t oi4 = -grad_i.istep();
  const std::ptrdiff_t oi5 = grad_i.istep();
  const std::ptrdiff_t oi6 = -grad_i.istep() - grad_i.jstep();
  const std::ptrdiff_t oi7 = -grad_i.jstep();
  const std::ptrdiff_t oi8 = grad_i.istep() - grad_i.jstep();

  const std::ptrdiff_t oj1 = grad_j.jstep() - grad_j.istep();
  const std::ptrdiff_t oj2 = grad_j.jstep();
  const std::ptrdiff_t oj3 = grad_j.istep() + grad_j.jstep();
  const std::ptrdiff_t oj6 = -grad_j.istep() - grad_j.jstep();
  const std::ptrdiff_t oj7 = -grad_j.jstep();
  const std::ptrdiff_t oj8 = grad_j.istep() - grad_j.jstep();

  float * d_data = &dest(2,2);
  const float * gi_data = &grad_i(2,2);
  const float * gj_data = &grad_j(2,2);

  for (unsigned j=2;j<nj2;++j)
  {
    float* d = d_data;
    const float* pgi = gi_data;
    const float* pgj = gj_data;
    for (unsigned i=2;i<ni2;++i)
    {
      // Compute gradient in i
      float dxdx = 0.125f*(pgi[oi3]+pgi[oi8] - (pgi[oi1]+pgi[oi6])) + 0.25f*(pgi[oi5]-pgi[oi4]);
      // Compute gradient in j
      float dxdy = 0.125f*(pgi[oi1]+pgi[oi3] - (pgi[oi6]+pgi[oi8])) + 0.25f*(pgi[oi2]-pgi[oi7]);
      // Compute gradient in j
      float dydy = 0.125f*(pgj[oj1]+pgj[oj3] - (pgj[oj6]+pgj[oj8])) + 0.25f*(pgj[oj2]-pgj[oj7]);

      float detH = dxdx*dydy - dxdy*dxdy;
      float traceH = dxdx+dydy;

      *d = detH - float(k) * traceH * traceH;

      pgi += grad_i.istep();
      pgj += grad_j.istep();
      d += dest.istep();
    }

    gi_data += grad_i.jstep();
    gj_data += grad_j.jstep();
    d_data  += dest.jstep();
  }
}

void vil_corners(const vil_image_view<double>& grad_i,
                 const vil_image_view<double>& grad_j,
                 vil_image_view<double>& dest, double k)
{
  assert(grad_i.nplanes()==1);
  assert(grad_j.nplanes()==1);
  unsigned ni = grad_i.ni(), nj = grad_i.nj();
  assert(grad_j.ni()==ni && grad_j.nj()==nj );

  dest.set_size(ni,nj);

  // Zero a two pixel border
  for (unsigned i=0;i<2;++i)
  {
    vil_fill_row(dest,i,0.0);
    vil_fill_row(dest,nj-1-i,0.0);
    vil_fill_col(dest,i,0.0);
    vil_fill_col(dest,ni-1-i,0.0);
  }

  const unsigned ni2 = ni-2;
  const unsigned nj2 = nj-2;

  // Compute relative grid positions
  //  o1 o2 o3
  //  o4    o5
  //  o6 o7 o8
  const std::ptrdiff_t oi1 = grad_i.jstep() - grad_i.istep();
  const std::ptrdiff_t oi2 = grad_i.jstep();
  const std::ptrdiff_t oi3 = grad_i.istep() + grad_i.jstep();
  const std::ptrdiff_t oi4 = -grad_i.istep();
  const std::ptrdiff_t oi5 = grad_i.istep();
  const std::ptrdiff_t oi6 = -grad_i.istep() - grad_i.jstep();
  const std::ptrdiff_t oi7 = -grad_i.jstep();
  const std::ptrdiff_t oi8 = grad_i.istep() - grad_i.jstep();

  const std::ptrdiff_t oj1 = grad_j.jstep() - grad_j.istep();
  const std::ptrdiff_t oj2 = grad_j.jstep();
  const std::ptrdiff_t oj3 = grad_j.istep() + grad_j.jstep();
  const std::ptrdiff_t oj6 = -grad_j.istep() - grad_j.jstep();
  const std::ptrdiff_t oj7 = -grad_j.jstep();
  const std::ptrdiff_t oj8 = grad_j.istep() - grad_j.jstep();

  double * d_data = &dest(2,2);
  const double * gi_data = &grad_i(2,2);
  const double * gj_data = &grad_j(2,2);

  for (unsigned j=2;j<nj2;++j)
  {
    double* d = d_data;
    const double* pgi = gi_data;
    const double* pgj = gj_data;
    for (unsigned i=2;i<ni2;++i)
    {
      // Compute gradient in i
      double dxdx = 0.125f*(pgi[oi3]+pgi[oi8] - (pgi[oi1]+pgi[oi6])) + 0.25f*(pgi[oi5]-pgi[oi4]);
      // Compute gradient in j
      double dxdy = 0.125f*(pgi[oi1]+pgi[oi3] - (pgi[oi6]+pgi[oi8])) + 0.25f*(pgi[oi2]-pgi[oi7]);
      // Compute gradient in j
      double dydy = 0.125f*(pgj[oj1]+pgj[oj3] - (pgj[oj6]+pgj[oj8])) + 0.25f*(pgj[oj2]-pgj[oj7]);

      double detH = dxdx*dydy - dxdy*dxdy;
      double traceH = dxdx+dydy;

      *d = detH - double(k) * traceH * traceH;

      pgi += grad_i.istep();
      pgj += grad_j.istep();
      d += dest.istep();
    }

    gi_data += grad_i.jstep();
    gj_data += grad_j.jstep();
    d_data  += dest.jstep();
  }
}

//: Compute corner strength using Rohr's recommended method
//  This computes the determinant of the matrix C=g.g'
//  after the elements of C have been smoothed.
//  g is the vector of first derivatives (gx,gy)'
//  It relies only on first derivatives.
//
//  Currently uses somewhat inefficient multi-pass method.
//  Could be improved.
void vil_corners_rohr(const vil_image_view<float>& gx,
                      const vil_image_view<float>& gy,
                      vil_image_view<float>& corner_im)
{
  vil_image_view<float> tmp_im,work_im, gx2,gy2,gxy;
  vil_gauss_filter_5tap_params smooth_params(1.0);

  // Compute smoothed products of gradients
  vil_math_image_product(gx,gx,tmp_im);
  vil_gauss_filter_5tap(tmp_im,gx2,smooth_params,work_im);

  vil_math_image_product(gy,gy,tmp_im);
  vil_gauss_filter_5tap(tmp_im,gy2,smooth_params,work_im);

  vil_math_image_product(gx,gy,tmp_im);
  vil_gauss_filter_5tap(tmp_im,gxy,smooth_params,work_im);

  // Compute gx2*gy2 - gxy*gxy at each pixel
  vil_math_image_product(gx2,gy2,corner_im);
  vil_math_image_product(gxy,gxy,tmp_im);
  vil_math_add_image_fraction(corner_im,1.0f,tmp_im,-1.0f);
}
