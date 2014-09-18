namespace NumericalSolver
{

   double 
   computeGradientAndHesseMatrix( double * const       f_state_p,
                                  double * const       f_gradient_p,
                                  double * const       f_hesseMatrix_p,
                                  const double * const f_steps_p,
                                  const int            f_n_i,
                                  const int            f_m_i,
                                  double              (*compMahalanobis_p)( const double *, int ) );
   
   double 
   solveByNewton ( double * const       f_state_p,
                   double * const       f_gradient_p,
                   double * const       f_hesseMatrix_p,
                   double * const       f_steps_p,
                   const int            f_n_i,
                   const int            f_m_i,
                   const int            f_maxIters_i,
                   const double         f_resTolerance_d,
                   double              (*compMahalanobis_p)( const double *, int ) );
}

