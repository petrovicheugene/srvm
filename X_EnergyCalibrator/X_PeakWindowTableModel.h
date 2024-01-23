//======================================================
#ifndef X_PEAKWINDOWTABLEMODEL_H
#define X_PEAKWINDOWTABLEMODEL_H
//======================================================
#include <QAbstractTableModel>
#include <QList>
//======================================================
class X_ControlAction;
class X_PeakWindow;
//======================================================
class X_PeakWindowTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit X_PeakWindowTableModel(QObject *parent = 0);
    ~X_PeakWindowTableModel();
    // override FUNCS
    virtual Qt::ItemFlags	flags(const QModelIndex & index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
    virtual int	rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant	data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    virtual QVariant	headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // custom FUNCS
    QList<X_ControlAction*> xp_buttonActions() const;
    QList<X_ControlAction*> xp_contextActions() const;

signals:

    void xg_inquiryCurrentVisibleSceneRect(QRectF& sceneRect) const;
    void xg_inquiryCurrentIndex(QModelIndex& index);

public slots:


protected slots:

    void xh_onAddPeakWindowAction();
    void xh_onRemovePeakWindowAction();

protected:

    // VARS
    X_ControlAction* xv_addPeakWindowAction;
    X_ControlAction* xv_removePeakWindowAction;
    QList<X_PeakWindow*> xv_peakWindowList;

    // FUNCS
    void xh_createActions();
    void xh_createConnections();

};
//======================================================
#endif // X_PEAKWINDOWTABLEMODEL_H
