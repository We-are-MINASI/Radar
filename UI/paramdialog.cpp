#include "paramdialog.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFrame>
#include <QMessageBox>

ParamDialog::ParamDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Редактор параметров имитатора");
    resize(600, 450);

    // Делаем окно модальным (блокирует основное окно, пока открыто)
    setWindowModality(Qt::WindowModal);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(10);
    horizontalLayout->setContentsMargins(10, 10, 10, 10);

    // === Левая панель параметров ===
    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(8);
    leftWidget->setFixedWidth(200);

    QLabel *label1 = new QLabel("Основные параметры:", leftWidget);
    label1->setStyleSheet("font-weight: bold; color: #2c3e50;");
    comboBox = new QComboBox(leftWidget);
    comboBox->addItem("uTime");
    comboBox->addItem("imitator");
    comboBox->addItem("frequencyConverter");
    comboBox->addItem("azimuth");
    connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParamDialog::onComboBoxChanged);

    QLabel *label2 = new QLabel("Параметры целей:", leftWidget);
    label2->setStyleSheet("font-weight: bold; color: #2c3e50;");
    comboBox_2 = new QComboBox(leftWidget);
    comboBox_2->addItem("targetResponse");
    comboBox_2->addItem("targetPosition");
    comboBox_2->addItem("targetFormation");
    connect(comboBox_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParamDialog::onComboBox2Changed);

    QLabel *label3 = new QLabel("Параметры помех:", leftWidget);
    label3->setStyleSheet("font-weight: bold; color: #2c3e50;");
    comboBox_3 = new QComboBox(leftWidget);
    comboBox_3->addItem("noise");
    comboBox_3->addItem("ppPosition");
    comboBox_3->addItem("nipPosition");
    comboBox_3->addItem("nipLevel");
    comboBox_3->addItem("nipFormation");
    comboBox_3->addItem("summator");
    comboBox_3->addItem("clutterResponse");
    comboBox_3->addItem("clutterFormation");
    connect(comboBox_3, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParamDialog::onComboBox3Changed);

    leftLayout->addWidget(label1); leftLayout->addWidget(comboBox);
    leftLayout->addWidget(label2); leftLayout->addWidget(comboBox_2);
    leftLayout->addWidget(label3); leftLayout->addWidget(comboBox_3);
    leftLayout->addStretch(); // Смещаем всё наверх

    horizontalLayout->addWidget(leftWidget);

    // === Правая панель с прокруткой ===
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);

    rightContentWidget = new QWidget();
    rightLayout = new QVBoxLayout(rightContentWidget);
    rightLayout->setSpacing(10);
    rightLayout->setContentsMargins(10, 10, 10, 10);
    rightLayout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(rightContentWidget);
    horizontalLayout->addWidget(scrollArea);

    createStructFields();

    // Сразу отображаем первую структуру по умолчанию
    displayStructFields("uTime");
}

ParamDialog::~ParamDialog() {}

void ParamDialog::createStructFields()
{
    structFields["uTime"] = {
        {"pulse_time", "long long", nullptr},
        {"probing_time", "long long", nullptr},
        {"sampling_rate", "long long", nullptr},
        {"max_sampling_cnt", "long long", nullptr}
    };
    structFields["targetResponse"] = {{"enable", "bool", nullptr}};
    structFields["targetPosition"] = {
        {"enable", "bool", nullptr},
        {"cntTarget", "int", nullptr},
        {"Targetamplitude", "float", nullptr}
    };
    structFields["targetFormation"] = {{"enable", "bool", nullptr}};
    structFields["ppPosition"] = {
        {"enable", "bool", nullptr},
        {"cntPP", "int", nullptr},
        {"PPamplitude", "float", nullptr}
    };
    structFields["noise"] = {
        {"enable", "bool", nullptr},
        {"mean", "double", nullptr},
        {"sigma", "double", nullptr}
    };
    structFields["nipPosition"] = {
        {"enable", "bool", nullptr},
        {"cntNIP", "int", nullptr},
        {"NIPamplitude", "float", nullptr},
        {"MaxNIP_probingTime", "long long", nullptr}
    };
    structFields["nipLevel"] = {
        {"enable", "bool", nullptr},
        {"amplitudeDecrease", "float", nullptr}
    };
    structFields["nipFormation"] = {{"enable", "bool", nullptr}};
    structFields["summator"] = {{"enable", "bool", nullptr}};
    structFields["imitator"] = {{"maxDistance", "float", nullptr}};
    structFields["frequencyConverter"] = {{"enable", "bool", nullptr}};
    structFields["clutterResponse"] = {{"enable", "bool", nullptr}};
    structFields["clutterFormation"] = {{"enable", "bool", nullptr}};
    structFields["azimuth"] = {
        {"enable", "bool", nullptr},
        {"startAngle", "float", nullptr},
        {"angularVelocity", "float", nullptr}
    };
}

void ParamDialog::clearRightPanel()
{
    QLayoutItem *item;
    while ((item = rightLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void ParamDialog::displayStructFields(const QString &structName)
{
    clearRightPanel();
    currentStructName = structName;

    if (!structFields.contains(structName)) {
        QLabel *label = new QLabel("Структура не найдена");
        label->setStyleSheet("color: red;");
        rightLayout->addWidget(label);
        return;
    }

    auto &fields = structFields[structName];

    QLabel *titleLabel = new QLabel(QString("<b>%1</b>").arg(structName));
    titleLabel->setStyleSheet("font-size: 14px; color: #2c3e50;");
    rightLayout->addWidget(titleLabel);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    rightLayout->addWidget(line);

    for (auto &field : fields) {
        QHBoxLayout *fieldLayout = new QHBoxLayout();
        QLabel *label = new QLabel(field.fieldName + ":");
        label->setMinimumWidth(120);
        label->setStyleSheet("font-weight: bold;");
        fieldLayout->addWidget(label);

        if (field.fieldType == "bool") {
            QCheckBox *checkBox = new QCheckBox();
            checkBox->setObjectName(field.fieldName);
            field.widget = checkBox;
            fieldLayout->addWidget(checkBox);
        } else {
            QLineEdit *lineEdit = new QLineEdit();
            lineEdit->setObjectName(field.fieldName);
            lineEdit->setPlaceholderText(field.fieldType == "int" || field.fieldType == "long long" ? "Введите целое число" : "Введите число");
            field.widget = lineEdit;
            fieldLayout->addWidget(lineEdit);
        }

        fieldLayout->addStretch();
        QWidget *fieldWidget = new QWidget();
        fieldWidget->setLayout(fieldLayout);
        rightLayout->addWidget(fieldWidget);
    }

    QPushButton *applyButton = new QPushButton("Применить параметры");
    applyButton->setStyleSheet("QPushButton { "
                               "background-color: #3498db; "
                               "color: white; padding: 8px; "
                               "border-radius: 4px; }"
                               "QPushButton:hover { background-color: #2980b9; }");
    connect(applyButton, &QPushButton::clicked, this, &ParamDialog::onApplyButtonClicked);
    rightLayout->addWidget(applyButton);
    rightLayout->addStretch();
}

void ParamDialog::collectFieldValues()
{
    if (!structFields.contains(currentStructName)) return;
    auto &fields = structFields[currentStructName];
    QString message = QString("Параметры структуры %1:\n\n").arg(currentStructName);

    for (const auto &field : fields) {
        if (field.widget) {
            if (field.fieldType == "bool") {
                QCheckBox *checkBox = qobject_cast<QCheckBox*>(field.widget);
                if (checkBox) {
                    message += QString("%1: %2\n").arg(field.fieldName).arg(checkBox->isChecked() ? "Включен" : "Выключен");
                }
            } else {
                QLineEdit *lineEdit = qobject_cast<QLineEdit*>(field.widget);
                if (lineEdit) {
                    QString value = lineEdit->text().isEmpty() ? "(не задано)" : lineEdit->text();
                    // ИСПРАВЛЕНО: цепочка вызовов .arg() вместо запятой
                    message += QString("%1: %2\n").arg(field.fieldName, value);
                }
            }
        }
    }
    QMessageBox::information(this, "Параметры", message);
}

void ParamDialog::onApplyButtonClicked() { collectFieldValues(); }

// ИСПРАВЛЕНО: При выборе в одном ComboBox сбрасываем визуальный выбор в других, чтобы не путать пользователя
void ParamDialog::onComboBoxChanged(int index) {
    if (index >= 0) {
        comboBox_2->blockSignals(true); comboBox_2->setCurrentIndex(-1); comboBox_2->blockSignals(false);
        comboBox_3->blockSignals(true); comboBox_3->setCurrentIndex(-1); comboBox_3->blockSignals(false);
        displayStructFields(comboBox->currentText());
    }
}
void ParamDialog::onComboBox2Changed(int index) {
    if (index >= 0) {
        comboBox->blockSignals(true); comboBox->setCurrentIndex(-1); comboBox->blockSignals(false);
        comboBox_3->blockSignals(true); comboBox_3->setCurrentIndex(-1); comboBox_3->blockSignals(false);
        displayStructFields(comboBox_2->currentText());
    }
}
void ParamDialog::onComboBox3Changed(int index) {
    if (index >= 0) {
        comboBox->blockSignals(true); comboBox->setCurrentIndex(-1); comboBox->blockSignals(false);
        comboBox_2->blockSignals(true); comboBox_2->setCurrentIndex(-1); comboBox_2->blockSignals(false);
        displayStructFields(comboBox_3->currentText());
    }
}