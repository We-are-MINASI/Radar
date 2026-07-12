QT += core gui widgets testlib
TEMPLATE = app
CONFIG += c++17 console
CONFIG -= app_bundle

TARGET = run_tests

# Прописываем пути к исходникам приложения, чтобы тесты их видели
INCLUDEPATH += ../src \
               ../src/Handler \
               ../src/Imitator \
               ../src/UI

# Подключаем файлы логики, которые хотим протестировать (БЕЗ main.cpp приложения!)
# Для примера протестируем модуль времени или АЦП
SOURCES += Imitator.cpp \
           ../src/Handler/ADC.c \
           ../src/Imitator/UnifedTime.c

HEADERS += ../src/Handler/ADC.h \
           ../src/Imitator/UnifedTime.h