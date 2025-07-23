#ifndef GUIDEWINDOW_H
#define GUIDEWINDOW_H

#include <QDialog>

namespace Ui {
class GuideWindow;
}

class GuideWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GuideWindow(QWidget *parent = nullptr);
    ~GuideWindow();

private:
    Ui::GuideWindow *ui;
};

#endif // GUIDEWINDOW_H
