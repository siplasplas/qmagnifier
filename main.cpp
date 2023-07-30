#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QSplitter>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <cmath>
#include <algorithm>

class Magnifier : public QLabel {
Q_OBJECT
public:
    explicit Magnifier(QWidget* parent = nullptr)
            : QLabel(parent)
    {
        setMouseTracking(true);
    }
    void mouseMoveEvent(QMouseEvent* event) override;
};

class MainWindow : public QMainWindow {
Q_OBJECT
    Magnifier *magnifier;
    float scale = 8;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
public:
    explicit MainWindow(QWidget* parent = nullptr)
            : QMainWindow(parent)
    {
        magnifier = new Magnifier(this);
        setCentralWidget(magnifier);
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
        QAction *exitAction = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
        exitAction->setShortcuts(QKeySequence::Quit);
        statusBar()->showMessage(tr("Ready"));
    }
};

void Magnifier::mouseMoveEvent(QMouseEvent* event) {
    QPixmap pixmap = grab();
    QImage image = pixmap.toImage();
    QColor color = image.pixelColor(event->x(), event->y());
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    QMainWindow* mwin = dynamic_cast<QMainWindow*>(parent());
    QString s = QString("%1, %2, %3 ").arg(red).arg(green).arg(blue);
    mwin->statusBar()->showMessage(s);
    QLabel::mouseMoveEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    QScreen *screen = QApplication::screenAt(event->globalPos());
    if (screen) {
        int mw = magnifier->width();
        int mh = magnifier->height();
        int scrW = round(mw/scale);
        int scrH = round(mh/scale);
        int cx = event->globalX();
        int cy = event->globalY();
        int startX = cx-scrW/2;
        int startY = cy-scrH/2;
        startX = std::max(startX, 0);
        startY = std::max(startY, 0);
        if (startX+scrW > screen->geometry().right())
            startX = screen->geometry().right() - scrW;
        if (startY+scrH > screen->geometry().bottom())
            startY = screen->geometry().bottom() - scrH;
        const QPixmap screenshot = screen->grabWindow(0, startX, startY, scrW, scrH);
        magnifier->setPixmap(screenshot.scaled(mw, mh));
        magnifier->show();
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
