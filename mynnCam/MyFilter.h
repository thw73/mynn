#ifndef MYFILTER_H
#define MYFILTER_H

#include <QAbstractVideoFilter>
#include <QObject>
#include <QDebug>

#include <opencv2/core/core.hpp>


class MyFilter : public QAbstractVideoFilter {

    Q_OBJECT


    Q_PROPERTY( bool m_Capturing READ capturing NOTIFY capturingChanged )
    Q_PROPERTY( int videoOutputOrientation MEMBER m_VideoOutputOrientation )

public:

    MyFilter(QObject* parent = nullptr);
    ~MyFilter();



    QVideoFilterRunnable *createFilterRunnable();

    bool capturing() const { return m_Capturing; }
    int videoOutputOrientation() const {
                                           return m_VideoOutputOrientation;
                                        }

    void setCapturing( bool capturing );


    Q_INVOKABLE void capture();

    void imageProcessing(QImage);

protected:

    bool m_Capturing;
    int m_VideoOutputOrientation;


signals:
    void capturingChanged();
    void sendImage(QImage);
    void finished(QObject *result);

};

#endif // MYFILTER_H
