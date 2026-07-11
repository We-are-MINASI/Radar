#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H
//#ifdef C

#include <QApplication>
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QVector>
#include <QString>
#include <QMap>
#include <QLabel>
#include <QPushButton>
#include <cmath>

class ParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamDialog(QWidget *parent = nullptr);
    ~ParamDialog();

private slots:
    void onComboBoxChanged(int index);
    void onComboBox2Changed(int index);
    void onComboBox3Changed(int index);
    void onApplyButtonClicked();

private:
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QComboBox *comboBox_3;

    QVBoxLayout *rightLayout;
    QWidget *rightContentWidget;
    QString currentStructName;

    struct FieldInfo {
        QString fieldName;
        QString fieldType;
        QWidget *widget;
    };

    QMap<QString, QVector<FieldInfo>> structFields;
    QMap<QString, QVariant> structValues;

    void createStructFields();
    void displayStructFields(const QString &structName);
    void clearRightPanel();
    void collectFieldValues();
};

#endif // PARAMDIALOG_H
