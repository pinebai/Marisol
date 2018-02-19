/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "LaplacianSplitAnisotropic.h"

template <>
InputParameters
validParams<LaplacianSplitAnisotropic>()
{
  InputParameters params = validParams<KernelGrad>();
  params.addClassDescription(
      "Split with a variable that holds the Laplacian of a phase field variable."
      "anisotropic Laplacian for anisotropic damage model");
  params.addRequiredCoupledVar("c", "Field variable to take the Laplacian of");
  return params;
}

LaplacianSplitAnisotropic::LaplacianSplitAnisotropic(const InputParameters & parameters)
  : KernelGrad(parameters), _var_c(coupled("c")), _grad_c(coupledGradient("c"))
{
}

RealGradient
LaplacianSplitAnisotropic::precomputeQpResidual()
{
  RealGradient anisotropic_grad_c;

  anisotropic_grad_c(0) = _grad_c[_qp](0);
  anisotropic_grad_c(1) = _grad_c[_qp](1);
  anisotropic_grad_c(2) = 0.0;

  return anisotropic_grad_c; // * _grad_test[_i][_qp]
}

RealGradient
LaplacianSplitAnisotropic::precomputeQpJacobian()
{
  return 0.0;
}

Real
LaplacianSplitAnisotropic::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _var_c)
    return _grad_phi[_j][_qp] * _grad_test[_i][_qp];

  return 0.0;
}