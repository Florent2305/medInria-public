/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

// This class behaves like vtkInteractorStyleTrackballCamera except that the right click
// is canceled (no effect). This is particularly usefull when one wants a menu to popup
// when right clikcking on the  window. The wheel has the same effect as the right click.

#include <vtkInteractorStyleTrackballCamera.h>

#include <medVtkInriaExport.h>

class vtkTransform;

class MEDVTKINRIA_EXPORT vtkInteractorStyleTrackballCamera2 : public vtkInteractorStyleTrackballCamera
{
 public:
  static vtkInteractorStyleTrackballCamera2 *New();
  vtkTypeMacro(vtkInteractorStyleTrackballCamera2,vtkInteractorStyleTrackballCamera);

  void OnLeftButtonDown()     override;
  void OnMouseWheelForward()  override;
  void OnMouseWheelBackward() override;
  void Rotate()               override;

  void Azimuth(double angle);
  void Elevation(double angle);

  vtkInteractorStyleTrackballCamera2(const vtkInteractorStyleTrackballCamera2&) = delete;
  void operator=(const vtkInteractorStyleTrackballCamera2&) = delete;

 protected:
  vtkInteractorStyleTrackballCamera2();
  ~vtkInteractorStyleTrackballCamera2() override;

  double UpAxis[3];
  double RightAxis[3];
  vtkTransform *Transform;

 private:
};




