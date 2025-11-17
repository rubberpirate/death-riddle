#include "MainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUI();
    createMenus();
    
    setWindowTitle("Death Riddle - Escape from the Game Master");
    resize(1000, 700);
}

void MainWindow::setupUI() {
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // Create main menu
    m_mainMenuWidget = createMainMenuWidget();
    m_stackedWidget->addWidget(m_mainMenuWidget);
    
    // Create game widget
    m_gameWidget = new GameWidget(this);
    m_stackedWidget->addWidget(m_gameWidget);
    
    // Connect game widget signals
    connect(m_gameWidget, &GameWidget::returnToMainMenu, this, &MainWindow::showMainMenu);
    
    // Show main menu first
    m_stackedWidget->setCurrentWidget(m_mainMenuWidget);
}

QWidget* MainWindow::createMainMenuWidget() {
    QWidget* widget = new QWidget(this);
    widget->setStyleSheet("QWidget { background-color: #0f0f1e; }");
    
    QVBoxLayout* layout = new QVBoxLayout(widget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);
    
    // Title
    QLabel* titleLabel = new QLabel("DEATH RIDDLE", widget);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(48);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    color: #8B0000;"
        "    padding: 20px;"
        "}"
    );
    layout->addWidget(titleLabel);
    
    // Subtitle
    QLabel* subtitleLabel = new QLabel("Escape from the Game Master", widget);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont;
    subtitleFont.setPointSize(20);
    subtitleFont.setItalic(true);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("QLabel { color: #FFD700; padding-bottom: 40px; }");
    layout->addWidget(subtitleLabel);
    
    // Story intro
    QLabel* storyLabel = new QLabel(
        "You awaken in an endless void...\n\n"
        "A mysterious Game Master has trapped you in their realm.\n"
        "Only by solving deadly riddles and making crucial choices\n"
        "can you hope to escape.\n\n"
        "Will you achieve true freedom, or become another lost soul?",
        widget
    );
    storyLabel->setAlignment(Qt::AlignCenter);
    storyLabel->setWordWrap(true);
    storyLabel->setMaximumWidth(600);
    QFont storyFont;
    storyFont.setPointSize(14);
    storyLabel->setFont(storyFont);
    storyLabel->setStyleSheet(
        "QLabel {"
        "    color: #CCCCCC;"
        "    background-color: rgba(20, 20, 40, 150);"
        "    padding: 30px;"
        "    border-radius: 10px;"
        "    border: 2px solid #4169E1;"
        "}"
    );
    layout->addWidget(storyLabel);
    
    layout->addSpacing(30);
    
    // Buttons
    QString buttonStyle = 
        "QPushButton {"
        "    background-color: #8B0000;"
        "    color: white;"
        "    border: 2px solid #FF0000;"
        "    border-radius: 10px;"
        "    padding: 15px 50px;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    min-width: 250px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #A00000;"
        "    border-color: #FF3333;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #700000;"
        "}";
    
    QPushButton* newGameButton = new QPushButton("Start New Game", widget);
    newGameButton->setStyleSheet(buttonStyle);
    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::startNewGame);
    layout->addWidget(newGameButton, 0, Qt::AlignCenter);
    
    QPushButton* aboutButton = new QPushButton("About", widget);
    aboutButton->setStyleSheet(buttonStyle);
    connect(aboutButton, &QPushButton::clicked, this, &MainWindow::showAbout);
    layout->addWidget(aboutButton, 0, Qt::AlignCenter);
    
    QPushButton* quitButton = new QPushButton("Quit", widget);
    quitButton->setStyleSheet(buttonStyle);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::quitGame);
    layout->addWidget(quitButton, 0, Qt::AlignCenter);
    
    layout->addStretch();
    
    return widget;
}

void MainWindow::createMenus() {
    QMenu* gameMenu = menuBar()->addMenu("&Game");
    
    QAction* newGameAction = gameMenu->addAction("&New Game");
    connect(newGameAction, &QAction::triggered, this, &MainWindow::startNewGame);
    
    QAction* mainMenuAction = gameMenu->addAction("&Main Menu");
    connect(mainMenuAction, &QAction::triggered, this, &MainWindow::showMainMenu);
    
    gameMenu->addSeparator();
    
    QAction* quitAction = gameMenu->addAction("&Quit");
    connect(quitAction, &QAction::triggered, this, &MainWindow::quitGame);
    
    QMenu* helpMenu = menuBar()->addMenu("&Help");
    
    QAction* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::showMainMenu() {
    m_stackedWidget->setCurrentWidget(m_mainMenuWidget);
}

void MainWindow::startNewGame() {
    m_stackedWidget->setCurrentWidget(m_gameWidget);
    m_gameWidget->startGame();
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "About Death Riddle",
        "<h2>Death Riddle</h2>"
        "<p><b>Version 1.0.0</b></p>"
        "<p>An Interactive CTF-style GUI game built with C++ and Qt6</p>"
        "<br>"
        "<p><b>Game Design:</b></p>"
        "<ul>"
        "<li>Multiple branching storylines with 5 unique endings</li>"
        "<li>CTF-style riddles testing logic, ciphers, and problem-solving</li>"
        "<li>Health system with consequences for wrong answers</li>"
        "<li>Animated characters and atmospheric effects</li>"
        "</ul>"
        "<br>"
        "<p><b>OOP Principles:</b></p>"
        "<p>Built following Object-Oriented Programming principles including "
        "encapsulation, inheritance, polymorphism, and abstraction.</p>"
        "<br>"
        "<p><b>Technologies:</b></p>"
        "<p>C++17, Qt6 (Core, Widgets, Gui, Multimedia), CMake</p>"
        "<br>"
        "<p>© 2024 Death Riddle. MIT License.</p>"
    );
}

void MainWindow::quitGame() {
    QApplication::quit();
}
