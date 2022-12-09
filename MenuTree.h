#ifndef MENUTREE_H
#define MENUTREE_H

#include <QObject>

class MenuTree : public QObject
{
    Q_OBJECT
public:
    explicit MenuTree(QObject *parent = nullptr);

signals:

};

#endif // MENUTREE_H
