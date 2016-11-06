#include "listrecords.h"
#include <QStringListModel>
ListRecords::ListRecords(QWidget *parent) :
    QListView(parent)
{
    // Create model

    ListModelRecord *monModel = new ListModelRecord("records/records.txt", this);
    this->setModel(monModel);

}
