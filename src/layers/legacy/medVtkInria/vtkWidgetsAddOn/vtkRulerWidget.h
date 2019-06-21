/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2018. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medVtkInriaExport.h>
#include <vtkInteractorObserver.h>


class vtkRulerWidgetObserver;
class vtkPolyData;
class vtkActor2D;
class vtkRenderer;

class MEDVTKINRIA_EXPORT vtkRulerWidget : public vtkInteractorObserver
{

 public:
  static vtkRulerWidget *New();
  vtkTypeMacro(vtkRulerWidget, vtkInteractorObserver);
  void PrintSelf(ostream& os, vtkIndent indent)  override {};
  vtkRulerWidget(const vtkRulerWidget&) = delete;
  void operator=(const vtkRulerWidget&) = delete;

  void SetEnabled(int enabling) override;

  void ExecuteCameraUpdateEvent(vtkObject *o, unsigned long event, void *calldata);

 protected:
  vtkRulerWidget();
  ~vtkRulerWidget() override;


 private:

  vtkRulerWidgetObserver *Observer;
  unsigned long StartEventObserverId;

  vtkRenderer *RendererY;
  vtkPolyData *OutlineY;
  vtkActor2D  *OutlineYActor;

  vtkRenderer *RendererX;
  vtkPolyData *OutlineX;
  vtkActor2D  *OutlineXActor;
};



