#pragma once
#include "medVtkImageInfo.h"

#include <vtkAlgorithmOutput.h>
#include <vtkImageAlgorithm.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkSetGet.h>


class vtkImage3DDisplay : public vtkObject
{
public:
    static vtkImage3DDisplay*New();

    vtkTypeMacro (vtkImage3DDisplay, vtkObject);

    virtual void SetInputData(vtkImageData *pi_poVtkImage);
    void SetInputProducer(vtkAlgorithmOutput *pi_poAlgorithmOutput);
    virtual medVtkImageInfo* GetMedVtkImageInfo();

    virtual bool isInputSet();

    vtkSetMacro(Opacity, double);
    vtkGetMacro(Opacity, double);

    vtkSetMacro(Visibility, int);
    vtkGetMacro(Visibility, int);

    vtkSetMacro(UseLookupTable, bool);
    vtkGetMacro(UseLookupTable, bool);

    vtkSetObjectMacro(LookupTable, vtkLookupTable);
    virtual vtkLookupTable*GetLookupTable();

    vtkSetMacro(ColorWindow, double);
    vtkGetMacro(ColorWindow, double);

    vtkSetMacro(ColorLevel, double);
    vtkGetMacro(ColorLevel, double);

    virtual vtkImageAlgorithm* GetInputProducer() const { return this->InputProducer; }

protected:
    vtkImage3DDisplay();
    ~vtkImage3DDisplay();

private:
    vtkSmartPointer<vtkImageAlgorithm> InputProducer;
    double Opacity;
    int Visibility;
    double ColorWindow;
    double ColorLevel;
    bool UseLookupTable;
    vtkSmartPointer<vtkLookupTable>             LookupTable;
    medVtkImageInfo                             m_sVtkImageInfo;

    vtkImage3DDisplay(const vtkImage3DDisplay&);
    void operator=(const vtkImage3DDisplay&);
};
