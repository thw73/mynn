#ifndef MYNNWRAPPER_H
#define MYNNWRAPPER_H

#include <QObject>
#include <QImage>

#include "mynn.h"

class mynnwrapper:public QObject
{
    Q_OBJECT
public:
    mynnwrapper();
    ~mynnwrapper();

    neuralNetwork mynn;

public slots:
    void calcmynn(QImage);

signals:
    void textChanged(QString text1);
};

#endif // MYNNWRAPPER_H
