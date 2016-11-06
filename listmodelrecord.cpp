#include "listmodelrecord.h"

ListModelRecord::ListModelRecord(QString fileToLoad, QObject *parent) :
    QAbstractListModel(parent)
{
    fichierRecord = fileToLoad;
    this->records_ = new std::vector<Record*>;
    QFile file(fileToLoad);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        this->records_->push_back(new Record(fields.at(0), (fields.at(1)).toFloat(), fields.at(2).toInt(), fields.at(3)));

    }
    qSort(records_->begin(), records_->end(), Record::cmpRec);

}

void ListModelRecord::addRecord(Record * r)
{
    beginInsertRows(QModelIndex(),0,0);
    this->records_->push_back(r);
    qDebug()<<records_->size();
    qSort(records_->begin(), records_->end(), Record::cmpRec);
    endInsertRows();


    QFile file( fichierRecord);
    qDebug()<<fichierRecord;
    if ( file.open(QIODevice::ReadWrite | QIODevice::Append) )
    {
        QTextStream stream( &file );
        stream << r->name + "," + QString::number(r->time) + "," + QString::number(r->level) + "," + r->fileName<< endl;
    }

}

int ListModelRecord::rowCount(const QModelIndex& parent) const {
    return records_->size();
}

QVariant ListModelRecord::data(const QModelIndex& index, int role) const {
    // Check that the index is valid and within the correct range first:
    if (!index.isValid()) return QVariant();
    if (index.row() >= records_->size()) return QVariant();

    if (role == Qt::DisplayRole) {
        // Only returns something for the roles you support (DisplayRole is a minimum)
        return QVariant(records_->at(index.row())->displayInList());
    } else {
        return QVariant();
    }


}

Record* ListModelRecord::atInd(int i)
{
    return this->records_->at(i);
}
