// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef ITKPROCESSILOGDEMONREGISTRATION_H
#define ITKPROCESSILOGDEMONREGISTRATION_H

#include <dtkCore/dtkAbstractProcess.h>

#include "itkProcessiLogDemonRegistrationPluginExport.h"

class itkProcessiLogDemonRegistrationPrivate;

class ITKPROCESSILOGDEMONREGISTRATIONPLUGIN_EXPORT itkProcessiLogDemonRegistration : public dtkAbstractProcess
{
    Q_OBJECT

public:
             itkProcessiLogDemonRegistration(void);
    virtual ~itkProcessiLogDemonRegistration(void);

    virtual QString description(void) const;

    static bool registered(void);

private:
    itkProcessiLogDemonRegistrationPrivate *d;
};

dtkAbstractProcess *createItkProcessiLogDemonRegistration(void);

#endif
