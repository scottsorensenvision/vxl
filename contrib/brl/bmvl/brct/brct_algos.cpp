// brct_algos.cpp: implementation of the brct_algos class.
//
//////////////////////////////////////////////////////////////////////

#include "brct_algos.h"
#include <vnl/algo/vnl_qr.h>
#include <vnl/algo/vnl_svd.h>
#include <vnl/vnl_double_4.h>
#include <vnl/vnl_double_4x4.h>
#include <vgl/vgl_homg_point_3d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vsol/vsol_box_3d.h>
#include <vcl_cassert.h>

// Construction/Destruction

brct_algos::brct_algos()
{
}

brct_algos::~brct_algos()
{
}

vgl_point_3d<double> brct_algos::triangulate_3d_point(const vgl_point_2d<double>& x1, const vnl_double_3x4& P1, \
                                                      const vgl_point_2d<double>& x2, const vnl_double_3x4& P2)
{
  vnl_double_4x4 A;

  for (int i=0; i<4; i++){
    A[0][i] = x1.x()*P1[2][i] - P1[0][i];
    A[1][i] = x1.y()*P1[2][i] - P1[1][i];
    A[2][i] = x2.x()*P2[2][i] - P2[0][i];
    A[3][i] = x2.y()*P2[2][i] - P2[1][i];
  }

  vnl_svd<double> svd_solver(A);
  vnl_double_4 p = svd_solver.nullvector();
  vgl_homg_point_3d<double> X(p[0], p[1], p[2], p[3]);

  return X;
}

#if 0 // unused static function
static vgl_point_2d<double> projection_3d_point(const vgl_point_3d<double> & x, const vnl_double_3x4& P)
{
  vnl_double_4 X;
  X[0] = x.x();
  X[1] = x.y();
  X[2] = x.z();
  X[3] = 1;

  vnl_double_3 t = P*X;
  vgl_homg_point_2d<double> u(t[0], t[1], t[2]);

  return u;
}
#endif // 0

vnl_double_3 brct_algos::bundle_reconstruct_3d_point(vcl_vector<vnl_double_2> &pts,
                                                     vcl_vector<vnl_double_3x4> &Ps)
{
  int nviews = pts.size();
  assert(pts.size() == Ps.size());

  vnl_double_3 X;

  vnl_matrix<double> A(3*nviews, 4, 0.0);
  vnl_vector<double> b(3*nviews, 0.0);

  for (int i=0; i<nviews; i++){
    int pos = 3*i;
    vnl_double_3x4 &P = Ps[i];
    vnl_double_2 &pt = pts[i];
    for (int j=0; j<3; j++){
      A[pos][j] = P[0][j];
      A[pos+1][j] = P[1][j];
      A[pos+2][j] = P[2][j];

      b[pos] = P[0][3];
      b[pos+1] = P[1][3];
      b[pos+2] = P[2][3];
    }

    A[pos][i+3] = -pt[0];
    A[pos+1][i+3] = -pt[1];
    A[pos+2][i+3] = -1.0;
  }

  vnl_qr<double> QR(A);
  vnl_vector<double> solution = QR.solve(b);

  for (int i=0; i<3; i++)
    X[i] = solution[i];

  return X;
}

vsol_box_3d_sptr brct_algos::get_bounding_box(vcl_vector<vgl_point_3d<double> > &pts_3d)
{
  vsol_box_3d_sptr box = new vsol_box_3d;

  int size = pts_3d.size();

  for (int i=0; i<size; i++){
    vgl_point_3d<double> &pt = pts_3d[i];
    box->add_point(pt.x(), pt.y(), pt.z());
  }

  return box;
}

void brct_algos::add_box_vrml(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax)
{
}
