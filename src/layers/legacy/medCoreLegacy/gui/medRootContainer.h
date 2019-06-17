#pragma once
#include <QGridLayout>
#include <medViewContainerSplitter.h>
#include <medTabbedViewContainers.h>

class medRootContainer : public QWidget
{
    Q_OBJECT
private:
    medViewContainerSplitter *m_poSplitter;
    QGridLayout m_oInternalLayout;
    medTabbedViewContainers *m_poParent;

public:
    medRootContainer(medTabbedViewContainers* parent);
    ~medRootContainer() override;

    void replaceSplitter(medViewContainerSplitter *pi_poSplitter);
    inline medViewContainerSplitter * getSplitter() const { return m_poSplitter; }
};

