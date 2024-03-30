#ifndef MYFILTERRUNABLE_H
#define MYFILTERRUNABLE_H

#include <QAbstractVideoFilter>

#include "MyFilter.h"



class MyFilterRunnable : public QVideoFilterRunnable {


public:
    MyFilterRunnable( MyFilter* filter );
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

    QImage QVideoFrameToQImage( const QVideoFrame& videoFrame );

protected:
    MyFilter* m_Filter;


};


#endif // MYFILTERRUNABLE_H
