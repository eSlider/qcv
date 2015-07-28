/*
 * Copyright (C) 2012 Hernan Badino <hernan.badino@gmail.com>
 *
 * This file is part of QCV
 *
 * QCV is under the terms of the GNU Lesser General Public License
 * version 2.1. See the GNU LGPL version 2.1 for details.
 * QCV is distributed "AS IS" without ANY WARRANTY, without even the
 * implied warranty of merchantability or fitness for a particular
 * purpose. 
 *
 * In no event shall the authors or contributors be liable
 * for any direct, indirect, incidental, special, exemplary, or
 * consequential damages arising in any way out of the use of this
 * software.
 *
 * By downloading, copying, installing or using the software you agree
 * to this license. Do not download, install, copy or use the
 * software, if you do not agree to this license.
 */

#include <opencv2/core/core.hpp>

namespace NumericalSolver
{
   template <typename TCostFunction>
   double 
   computeGradientAndHesseMatrix( double * const       f_state_p,
                                  double * const       f_gradient_p,
                                  double * const       f_hesseMatrix_p,
                                  const double * const f_steps_p,
                                  const int            f_n_i,
                                  const int            f_m_i,
                                  TCostFunction       &f_compMahalanobis )
   {
      double valAtMin_d = f_compMahalanobis(f_state_p, f_n_i);

      memset(f_gradient_p,    0, sizeof(double)*f_m_i);
      memset(f_hesseMatrix_p, 0, sizeof(double)*f_m_i*f_m_i);

      for (int i = 0; i < f_m_i; ++i)
         f_hesseMatrix_p[f_m_i*i+i] = 1;


#if defined ( _OPENMP )
      const unsigned int numThreads_ui = omp_get_max_threads();
#else
      const unsigned int numThreads_ui = 1;
#endif

      double *paramsData_p = new double[f_m_i*numThreads_ui];

#if defined ( _OPENMP )
#pragma omp parallel for num_threads(numThreads_ui) schedule(dynamic)
#endif
      for (int i = 0; i < f_n_i; i++)
      { 
#if defined ( _OPENMP )
         int threadNum_i = omp_get_thread_num();
         double * const params_p = paramsData_p + f_m_i * threadNum_i;
#else
         double * const params_p = paramsData_p;
#endif
         double fval[3];
         double tempg1, tempg2;

         memcpy(params_p, f_state_p, f_m_i*sizeof(double));

         params_p[i] = f_state_p[i] - f_steps_p[i];   fval[0]  = f_compMahalanobis(params_p, f_n_i);
         params_p[i] = f_state_p[i] + f_steps_p[i];   fval[2]  = f_compMahalanobis(params_p, f_n_i);

         // Gradient 
         f_gradient_p[i] = (fval[2] - fval[0]) / (2*f_steps_p[i]);

         // Hessian 
         f_hesseMatrix_p[i*f_m_i+i] = (fval[2] - 2 * valAtMin_d + fval[0]) / (f_steps_p[i] * f_steps_p[i]);

         params_p[i] = f_state_p[i];

         // Off-diagonal elements
         for (int j = i + 1; j < f_n_i; j++)
         {
            params_p[j] = f_state_p[j] + f_steps_p[j];
            params_p[i] = f_state_p[i] + f_steps_p[i];   fval[1]  = f_compMahalanobis(params_p, f_n_i);
            params_p[i] = f_state_p[i] - f_steps_p[i];   fval[0]  = f_compMahalanobis(params_p, f_n_i);

            tempg1 = (fval[1] - fval[0]) / (2*f_steps_p[i]);

            params_p[j] = f_state_p[j] - f_steps_p[j];
            fval[0]  = f_compMahalanobis(params_p, f_n_i);
            params_p[i] = f_state_p[i] + f_steps_p[i];   fval[1]  = f_compMahalanobis(params_p, f_n_i);

            tempg2 = (fval[1] - fval[0]) / (2*f_steps_p[i]);

            f_hesseMatrix_p[i*f_m_i+j] = f_hesseMatrix_p[j*f_m_i+i] =((tempg1-tempg2)) / (2*f_steps_p[j]);

            params_p[j] = f_state_p[j];
            params_p[i] = f_state_p[i];
         }
      }

      delete [] paramsData_p;
      
      return valAtMin_d;
   }

#define invertNxNMatrix(inpptr, outptr, n, m) {                         \
   cv::Mat matInp ( cv::Size((m),(m)), CV_64FC1, (void *) (inpptr) );   \
   cv::Mat matOut ( cv::Size((m),(m)), CV_64FC1, (void *) (outptr) );   \
   matOut(cv::Rect(0,0,(n),(n)))=matInp(cv::Rect(0,0,(n),(n))).inv(); }

   template <typename TCostFunction>
   double solveByNewton ( double * const       f_state_p,
                          double * const       f_gradient_p,
                          double * const       f_hesseMatrix_p,
                          double * const       f_steps_p,
                          const int            f_n_i,
                          const int            f_m_i,
                          const int            f_maxIters_i,
                          const double         f_resTolerance_d,
                          TCostFunction       &f_compMahalanobis )
   {
      if (f_n_i < 1 || f_m_i < f_n_i || f_maxIters_i < 1) return -1.;

      double *dtemp_p     = new double[f_m_i*f_m_i];
      double *prevState_p = new double[f_m_i];

      double residual_d      = std::numeric_limits<float>::max();
      double prevResidual_d  = std::numeric_limits<float>::max();
      double pprevResidual_d = std::numeric_limits<float>::max();

      memcpy(prevState_p, f_state_p, sizeof(double) * f_m_i);

      int it;

      for (it = 0; it < f_maxIters_i; ++it)
      {       
         residual_d = computeGradientAndHesseMatrix( f_state_p, 
                                                     f_gradient_p, 
                                                     f_hesseMatrix_p, 
                                                     f_steps_p, 
                                                     f_n_i,
                                                     f_m_i,
                                                     f_compMahalanobis );
        
         if ( fabs(prevResidual_d - residual_d) < f_resTolerance_d )
         {
            break;
         }
         else
            if ( prevResidual_d - residual_d < 0 )
            {
               prevResidual_d = pprevResidual_d;
               memcpy(f_state_p, prevState_p, sizeof(double) * f_m_i);
               for (int i = 0; i < f_n_i; ++i) f_steps_p[i]/=2;
               continue;
            }
         
         invertNxNMatrix(f_hesseMatrix_p, dtemp_p, f_n_i, f_m_i);         

         memcpy(prevState_p, f_state_p, sizeof(double) * f_m_i);
         for (int i = 0; i < f_n_i; ++i)
            for (int j = 0; j < f_n_i; ++j)
               f_state_p[i] -= dtemp_p[i*f_m_i+j] * f_gradient_p[j];
      
         pprevResidual_d = prevResidual_d;
         prevResidual_d = residual_d;
      }

      if (prevResidual_d - residual_d < 0)
         memcpy(f_state_p, prevState_p, sizeof(double) * f_m_i);
   
      residual_d = computeGradientAndHesseMatrix( f_state_p, 
                                                  f_gradient_p, 
                                                  f_hesseMatrix_p, 
                                                  f_steps_p, 
                                                  f_n_i,
                                                  f_m_i,
                                                  f_compMahalanobis );
      delete [] dtemp_p;
      delete [] prevState_p;
   
      return residual_d;
   }
}
