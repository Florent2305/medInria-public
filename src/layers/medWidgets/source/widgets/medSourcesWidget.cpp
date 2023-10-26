#include "medSourcesWidget.h"

#include <medDataHub.h>
#include <medSourceModel.h>
#include <medSourceModelItem.h>
#include <medSourceModelPresenter.h>

#include <medDataInfoWidget.h>

#include <medDataExporter.h>

#include <QPushButton>
#include <QTreeView>
#include <QAction>

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

#include <QComboBox>
#include <QLabel>
//#include <>

class medSortFilterProxyModel : public QSortFilterProxyModel
{
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override
    {
        return source_column == 0;
    }

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        bool bRes = false;
        for (int col = 0; col < sourceModel()->columnCount(source_parent); col++)
        {
            QModelIndex index1 = sourceModel()->index(source_row, col, source_parent);
            if (sourceModel()->data(index1).toString().contains(filterRegExp()))
            {
                bRes = true;
            }
        }
        return bRes;
    }
};

medSourcesWidget::medSourcesWidget()
{
     this->setLayout(&m_layout);
     m_layout.setAlignment(Qt::AlignTop);
     connect(medSourcesLoader::instance(), &medSourcesLoader::sourceHidden, this, &medSourcesWidget::hideSource);
}

medSourcesWidget::~medSourcesWidget()
{
}

void medSourcesWidget::addSources(medDataHub *dataHub)
{
    auto  sourceModels = dataHub->models();
    for (auto sourceModel : sourceModels)
    {
        addSource(dataHub, sourceModel->getSourceIntanceId());
    }
}

void medSourcesWidget::addSource(medDataHub *dataHub, QString sourceInstanceId)
{
    auto                        *sourceModel = dataHub->getModel(sourceInstanceId);
    QString                      instanceName = medSourceHandler::instance()->getInstanceName(sourceInstanceId);
    bool isOnline, isLocal, isWritable, isCached;
    medSourceHandler::instance()->sourceGlobalInfo(sourceInstanceId,isOnline, isLocal, isWritable, isCached);
    medSourceModelPresenter *sourcePresenter = new medSourceModelPresenter(sourceModel);

    QPushButton *sourceTreeTitle = new QPushButton(instanceName);
    QPushButton *plusButton = new QPushButton(QIcon(":/pixmaps/plus.png"), "");
    plusButton->setToolTip("Expand All");
    plusButton->setMaximumSize(QSize(20,20));
    plusButton->setCheckable(true);
    plusButton->setFlat(true);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(sourceTreeTitle);
    hLayout->addWidget(plusButton, 0, Qt::AlignRight);


    QTreeView   *sourceTreeView  = sourcePresenter->buildTree(new medSortFilterProxyModel());
    connect(sourceTreeView, &QTreeView::doubleClicked, this, &medSourcesWidget::onDoubleClick);
    connect(sourceTreeView, &QTreeView::clicked, [=](QModelIndex index) 
    {
        bool isValid = index.isValid();
        isValid != isValid;
    });

    connect(plusButton, &QPushButton::toggled, [=](bool checked) {
        if (checked)
        {
            plusButton->setIcon(QIcon(":/pixmaps/minus.png"));
            dataHub->expandAll(sourceInstanceId);
            sourceTreeView->expandAll();
        }
        else
        {
            plusButton->setIcon(QIcon(":/pixmaps/plus.png"));
            sourceTreeView->collapseAll();
        }
    });

    sourceTreeView->setDragEnabled(true);
    sourceTreeView->viewport()->setAcceptDrops(true);
    sourceTreeView->setAcceptDrops(true);
    //sourceTreeView->setDropIndicatorShown(true);
    sourceTreeView->setDragDropMode(QAbstractItemView::DragDrop);

    //context menu code
    auto pMenu = new medSourceContextMenu(sourceTreeView);

    sourceTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_TreeviewByMenuMap[pMenu] = sourceTreeView;
    QAction *pushAction    = new QAction(tr("Push"),          pMenu);
    QAction *refreshAction = new QAction(tr("Refresh"),       pMenu);
    QAction *saveAction    = new QAction(tr("Save on disk"),  pMenu);
    QAction *removeAction  = new QAction(tr("Remove"),        pMenu);
    QAction *fetchAction   = new QAction(tr("Fetch"),         pMenu);
//    QAction *preloadAction = new QAction(tr("Pre-Load"),      pMenu);
    QAction *readerAction  = new QAction(tr("Change reader"), pMenu);
    QAction *unloadAction  = new QAction(tr("Unload"),        pMenu);
    QAction *infoAction    = new QAction(tr("Information"),   pMenu);
    pMenu->addAction(pushAction);
    pMenu->addAction(refreshAction);
    pMenu->addAction(saveAction);
    pMenu->addAction(removeAction);
    pMenu->addAction(fetchAction);
    if (isLocal)
    {
        fetchAction->setDisabled(true);
    }
//    pMenu->addAction(preloadAction);
    pMenu->addAction(readerAction);
    pMenu->addAction(unloadAction);
    pMenu->addAction(infoAction);








    //connect(pushAction,    &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(refreshAction, &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    connect(saveAction,    &QAction::triggered, [=]() {
        QModelIndexList sourceItemList;
        QModelIndex indexClicked;
        medDataIndex dataIndexCliked;
        QList<medDataIndex> dataIndexList;

        clickedOrSelectedItems(pMenu, sourceItemList, indexClicked);

        auto model = static_cast<const medSourceModel*>(indexClicked.model());
        for (auto aSourceItem : sourceItemList)
        {
            dataIndexList << model->dataIndexFromModelIndex(aSourceItem);
        }
        dataIndexCliked = model->dataIndexFromModelIndex(indexClicked);

        if (indexClicked.isValid())
        {
            exportData(dataHub, dataIndexCliked, dataIndexList);
        }
    
    });
    //connect(removeAction,  &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(fetchAction,   &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    //connect(preloadAction, &QAction::triggered, [=]() {
    //    QModelIndex index = this->indexFromMenu(pMenu);
    //    if (index.isValid())
    //    {
    //        const_cast<medSourceModel*>(static_cast<const medSourceModel*>(index.model()))->fetchData(index);
    //    }
    //});
    connect(fetchAction, &QAction::triggered, [=]() {
        QModelIndex index = this->indexFromMenu(pMenu);
        if (index.isValid())
        {
            const_cast<medSourceModel*>(static_cast<const medSourceModel*>(index.model()))->fetchData(index);
        }
    });

    //connect(readerAction,  &QAction::triggered, [=]() {  emit infoActionSignal(this->itemFromMenu(pMenu)); });
    connect(unloadAction,  &QAction::triggered, [=]() {

        QModelIndex index = this->indexFromMenu(pMenu);
        const medSourceModel * model = static_cast<const medSourceModel*>(index.model());

        medDataIndex medIndex = model->dataIndexFromModelIndex(index);
        dataHub->dataUnloaded(medIndex);
    });
    connect(infoAction,    &QAction::triggered, [=]() {
        medSourceModel::datasetAttributes mandatoriesAttributes;

        QModelIndex index = this->indexFromMenu(pMenu);
        if (index.isValid())
        {
            mandatoriesAttributes = sourceModel->getMendatoriesMetaData(index);
            QMap<QString, QString> dataAttributes;
            for (auto & key : mandatoriesAttributes.values.keys())
            {
                dataAttributes[key] = mandatoriesAttributes.values[key].toString();
            }
            auto popupDataInfo = new medDataInfoWidget(dataAttributes);
            popupDataInfo->show();
        }

    });



    connect(sourceTreeView, &QTreeView::customContextMenuRequested, [=](QPoint const& point) { onCustomContextMenu(point, pMenu); });
    
    QWidget * srcWidget = new QWidget();
    QVBoxLayout * srcLayout = new QVBoxLayout();
    srcWidget->setLayout(srcLayout);
    srcLayout->addLayout(hLayout);
    srcLayout->addWidget(sourceTreeView);
    m_layout.addWidget(srcWidget);
    
    srcWidget->setHidden(medSourcesLoader::instance()->isInvisible(sourceInstanceId));

    connect(sourceTreeView, &QTreeView::clicked, [=]() {
        if (!sourceTreeView->isEnabled())
        {
            dataHub->sourceIsOnline(sourceInstanceId);
        }
    });
    connect(sourceTreeTitle, &QPushButton::clicked, [=]() {
        if (!sourceTreeView->isEnabled())
        {
            dataHub->sourceIsOnline(sourceInstanceId);
        }
    });

    connect(sourceTreeTitle, &QPushButton::clicked, [=]() {sourceTreeView->setVisible(!sourceTreeView->isVisible()); });
    connect(sourceModel, &medSourceModel::online, sourceTreeView, &QTreeView::setEnabled);
    //connect(sourceModel, &medSourceModel::columnCountChange, [&](int iColumnCount) {
    //    for (int i = 1; i < iColumnCount; ++i)
    //    {
    //        sourceTreeView->setColumnHidden(i, true);
    //    }
    //});

    m_treeMap  [sourceInstanceId] = sourceTreeView;
    m_titleMap [sourceInstanceId] = hLayout;
    delete sourcePresenter;
}

void medSourcesWidget::removeSource(QString sourceInstanceId)
{
    if (m_treeMap.contains(sourceInstanceId))
    {
        auto widget = m_treeMap.take(sourceInstanceId);
        auto titleLayout = m_titleMap.take(sourceInstanceId);
        
        QLayoutItem *child;
        while ((child = titleLayout->takeAt(0)) != nullptr)
        {
            delete child->widget(); // delete the widget
            delete child;   // delete the layout item
        }

        //m_layout.removeWidget(widget);
        //m_layout.removeWidget(label);

        //TODO made disconnection here
        
        delete widget;
        delete titleLayout;
    }
}

void medSourcesWidget::filter(const QString &text)
{
    for ( auto sourceTreeView : m_treeMap.values())
    {
        auto tree = dynamic_cast<QTreeView*>(sourceTreeView);
        auto proxy = dynamic_cast<medSortFilterProxyModel *>(tree->model());
        QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::Wildcard);
        proxy->setFilterRegExp(regExp);
        proxy->setRecursiveFilteringEnabled(true);
    }
}

void medSourcesWidget::onDoubleClick(QModelIndex const & index)
{
    auto proxy = static_cast<const medSortFilterProxyModel*>(index.model());
    auto sourceIndex = proxy->mapToSource(index);
    medDataIndex medIndex = static_cast<const medSourceModel*>(sourceIndex.model())->dataIndexFromModelIndex(sourceIndex);
    emit openOnDoubleClick(medIndex);
}

void medSourcesWidget::hideSource(QString sourceInstanceId, bool hide)
{
    auto * pTreeView = m_treeMap.value(sourceInstanceId);
    if (pTreeView)
    {
        pTreeView->parentWidget()->setHidden(hide);
    }
}

void medSourcesWidget::onCustomContextMenu(QPoint const &point, QMenu *pi_pMenu)
{
    auto pTreeView = m_TreeviewByMenuMap[pi_pMenu];
    QModelIndex index = pTreeView->indexAt(point);
    auto proxy = static_cast<medSortFilterProxyModel*>(pTreeView->model());

    if (index.isValid())
    {
        m_rightClikedIndex = proxy->mapToSource(index);
        QPoint pos = pTreeView->viewport()->mapToGlobal(point);
        pi_pMenu->exec(pos);
    }
    else
    {
        m_rightClikedIndex = QModelIndex();
    }
}

QModelIndex medSourcesWidget::indexFromMenu(QMenu * pi_pMenu)
{
    //QModelIndex indexRes;
    //
    //auto sourceTreeView = m_TreeviewByMenuMap[pi_pMenu];    
    //auto proxy = static_cast<medSortFilterProxyModel*>(sourceTreeView->model());
    //auto pos = sourceTreeView->viewport()->mapFromGlobal(pi_pMenu->pos());
    //indexRes = proxy->mapToSource(sourceTreeView->indexAt(pos));
    //
    //return indexRes;

    return m_rightClikedIndex;
}


void medSourcesWidget::exportData(medDataHub *hub, medDataIndex index, QList<medDataIndex> selectedIndexList) 
{
    if (selectedIndexList.count() > 1)
    {
        auto writersCapabilitiesMap = medDataExporter::getWriterInfoList();
        //auto possibleWriters = hub->getPossibleWriters(index);

        QFileDialog * exportDialog = new QFileDialog(0, tr("Exporting many data: please choose a directory"));

        exportDialog->setOption(QFileDialog::DontUseNativeDialog);
        exportDialog->setAcceptMode(QFileDialog::AcceptSave);
        exportDialog->setFileMode(QFileDialog::DirectoryOnly);
        exportDialog->setOption(QFileDialog::ShowDirsOnly, false);


        QComboBox* typesHandled = new QComboBox(exportDialog);
        // we use allWriters as the list of keys to make sure we traverse possibleWriters
        // in the order specified by the writers priorities.
        for (auto writerInfo : writersCapabilitiesMap)
        {
            QString label = writerInfo.descr + " (" + writerInfo.File.join(", ") + ")";
            QString extension = (writerInfo.File.isEmpty()) ? QString() : writerInfo.File.first();
            
            typesHandled->addItem(label, writerInfo.Id);
            typesHandled->setItemData(typesHandled->count() - 1, extension, Qt::UserRole + 1);
            typesHandled->setItemData(typesHandled->count() - 1, QVariant::fromValue<QObject*>(exportDialog), Qt::UserRole + 2);
        }
        
        int *pTypesHandledIndex = new int;
        *pTypesHandledIndex = 0;
        
        connect(typesHandled, qOverload<int>(&QComboBox::currentIndexChanged), [=](int index) {*pTypesHandledIndex = index; });

        QLayout* layout = exportDialog->layout();
        QGridLayout* gridbox = qobject_cast<QGridLayout*>(layout);

        // nasty hack to hide the filter list
        QWidget * filtersLabel = gridbox->itemAtPosition(gridbox->rowCount() - 1, 0)->widget();
        QWidget * filtersList = gridbox->itemAtPosition(gridbox->rowCount() - 1, 1)->widget();
        filtersLabel->hide(); filtersList->hide();

        if (gridbox) 
        {
            gridbox->addWidget(new QLabel("Export format:", exportDialog), gridbox->rowCount() - 1, 0);
            gridbox->addWidget(typesHandled, gridbox->rowCount() - 1, 1);
        }

        exportDialog->setLayout(gridbox);


        connect(exportDialog, &QFileDialog::accepted, [=]() {

            QDir dir = exportDialog->directory();
            QString dirPath = dir.absolutePath();            

            for (medDataIndex anIndex : selectedIndexList)
            {                
                auto data = hub->getData(anIndex);
                QString dataPath = dirPath + "/" + anIndex.uri().last();
                medDataExporter::convertSingleDataOnfly(data, dataPath, writersCapabilitiesMap[*pTypesHandledIndex].File);
            }

            delete pTypesHandledIndex;

        }
        );

        if (exportDialog->exec())
        {

        }

        delete exportDialog;
    }
    else if (selectedIndexList.count() == 1)
    {
        auto possibleWriters = hub->getWriterInfoList(index);

        QStringList filterList;

        for (auto possibleWriter : possibleWriters)
        {
            filterList << possibleWriter.descr + " (" + possibleWriter.File.join(", ") + ")";
        }

        QString filter = filterList.join(";;");

        QFileDialog * exportDialog = new QFileDialog(0, tr("Exporting: please choose a file name and directory"), "", filter);
        exportDialog->setOption(QFileDialog::DontUseNativeDialog);
        exportDialog->setAcceptMode(QFileDialog::AcceptSave);
        exportDialog->setOption(QFileDialog::ShowDirsOnly, false);

        //exportDialog->selectFile(defaultName);
        connect(exportDialog, &QFileDialog::accepted, [=]() {

            auto fileName = exportDialog->selectedFiles()[0];
            QDir dir = exportDialog->directory();
            QString dirPath = dir.absolutePath();

            QStringList namesFilters = exportDialog->selectedNameFilter().split('(')[1].split(')')[0].replace(" ", "").split(',');


            QString nameSuffix = QFileInfo(fileName).completeSuffix();


            QStringList filters;

            if (nameSuffix.isEmpty() || !namesFilters.contains('.' + nameSuffix))
            {
                filters = namesFilters;
            }
            else
            {
                filters << '.' + nameSuffix;
            }


            QString path = fileName;
            medDataExporter::convertSingleDataOnfly(hub->getData(index), path, filters);

        }
        );

        if (exportDialog->exec())
        {

        }

        delete exportDialog;
    }
}

void medSourcesWidget::clickedOrSelectedItems(medSourceContextMenu * pi_pMenu, QModelIndexList & po_sourceItemList, QModelIndex &po_indexClicked)
{

    auto sourceTreeView = m_TreeviewByMenuMap[pi_pMenu];
    auto proxy = static_cast<medSortFilterProxyModel*>(sourceTreeView->model());

    //auto pos = sourceTreeView->viewport()->mapFromGlobal(pi_pMenu->pos());
    //auto indexClickedOnProxy = sourceTreeView->indexAt(pos);
    po_indexClicked = m_rightClikedIndex; //proxy->mapToSource(indexClickedOnProxy);

    QModelIndexList proxyItemList = sourceTreeView->selectionModel()->selectedIndexes();
    for (auto item : proxyItemList)
    {
        po_sourceItemList << proxy->mapToSource(item);
    }


    //auto sourceTreeView = m_TreeviewByMenuMap[pi_pMenu];
    //auto proxy = static_cast<medSortFilterProxyModel*>(sourceTreeView->model());
    //auto pos = sourceTreeView->viewport()->mapFromGlobal(pi_pMenu->pos());
    //indexRes = proxy->mapToSource(sourceTreeView->indexAt(pos));


    po_sourceItemList = po_sourceItemList.toSet().toList();
    if (!po_sourceItemList.contains(po_indexClicked))
    {
        po_sourceItemList << po_indexClicked;
    }
}
