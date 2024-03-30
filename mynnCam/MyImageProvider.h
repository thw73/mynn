#ifndef MYIMAGEPROVIDER_H
#define MYIMAGEPROVIDER_H

#include <QQuickImageProvider>


class MyImageProvider: public QObject, public QQuickImageProvider
{

 Q_OBJECT

public:

    MyImageProvider(QObject *parent = nullptr);
    ~MyImageProvider();


    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize );

    QImage m_image;

public slots:
     void setImage(QImage);

signals:
    void imageChanged();

};

#endif // MYIMAGEPROVIDER_H



