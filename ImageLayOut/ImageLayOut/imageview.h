#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QGraphicsView>

class QPinchGesture;
class QGestureEvent;

class ImageView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    bool event(QEvent* event) override;

private:
    void pinchTriggered(QPinchGesture *gesture);
    bool gestureEvent(QGestureEvent *event);

    QGraphicsScene *graphicsScene;

    bool _pan;
    int _panStartX, _panStartY;
    int _numScheduledScalings;
    int _currentStepScaleFactor;
    int _scaleFactor, _rotationAngle;

private slots:
    void scalingTime(qreal);
    void animFinished();
};

#endif // IMAGEVIEW_H
