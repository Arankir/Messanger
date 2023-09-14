#ifndef FORMSETTINGS_H
#define FORMSETTINGS_H

#include <QMainWindow>

namespace Ui {
class FormSettings;
}

class FormSettings : public QMainWindow
{
    Q_OBJECT

public:
    explicit FormSettings(QWidget *parent = nullptr);
    ~FormSettings();

private:
    Ui::FormSettings *ui;
};

#endif // FORMSETTINGS_H
