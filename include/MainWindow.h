#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameWidget.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() = default;

private slots:
    void showMainMenu();
    void startNewGame();
    void showAbout();
    void quitGame();

private:
    void setupUI();
    void createMenus();
    QWidget* createMainMenuWidget();

    QStackedWidget* m_stackedWidget;
    GameWidget* m_gameWidget;
    QWidget* m_mainMenuWidget;
};

#endif // MAINWINDOW_H
