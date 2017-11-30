#include "mainwindow.h"
#include <QtWidgets>
#include <cassert>
#include <QLoggingCategory>

void clearLayout(QLayout * layout, bool deleleWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleleWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleleWidgets);
        delete item;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    fieldWidth = 0;
    fieldHeight = 0;

    gameData = new GameData();
    field_layout = new QGridLayout();
    QWidget * central = new QWidget();

    central->setLayout(field_layout);
    this->setCentralWidget(central);

    readLeaderboard();
    setupMenus();
    setupStatusBar();

    animTimer = new QTimer(this);
    animTimer->setInterval(33);
    connect(animTimer, SIGNAL(timeout()), this, SLOT(updateAnimation()));

    setWindowTitle(tr("BlockQuiz"));
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupMenus()
{
    QMenuBar * root = this->menuBar();
    QMenu * gameMenu = root->addMenu("Game");
    gameMenu->addAction("New", this, SLOT(showRestartGameDialog()));
    gameMenu->addAction("Cancel turn", this, SLOT(cancelTurn()));
    gameMenu->addAction("Leaderboard", this, SLOT(showLeaderboard()));
    gameMenu->addAction("Exit", this, SLOT(exitGame()));
}

void MainWindow::setupStatusBar()
{
    QStatusBar * bar = this->statusBar();

    this->labelCurrentTurn = new QLabel();
    this->labelRetries = new QLabel();
    bar->addPermanentWidget(labelCurrentTurn, 50);
    bar->addPermanentWidget(labelRetries, 50);
}


void MainWindow::readLeaderboard()
{
    leaderboard.clear();
    QString path = getSettingsPath();
    QSettings settings(path, QSettings::IniFormat);
    this->lastName = settings.value("lastPlayer").toString();
    int size = settings.beginReadArray("leaderboard");
    for (int i = 0; i < size; ++i) {
        LeaderPtr rec(new LeaderboardRecord());
        settings.setArrayIndex(i);
        rec->name = settings.value("name").toString();
        rec->turns = settings.value("turns").toInt();
        rec->retries = settings.value("retries").toInt();
        rec->rank = i;
        leaderboard.push_back(std::move(rec));
    }
    settings.endArray();
}

void MainWindow::writeLeaderboard()
{
    QString path = getSettingsPath();
    QSettings settings(path, QSettings::IniFormat);
    settings.setValue("lastPlayer", lastName);
    settings.beginWriteArray("leaderboard", leaderboard.size());
    for(size_t i = 0; i < leaderboard.size(); i++)
    {
        const LeaderboardRecord & rec = *leaderboard[i];
        settings.setArrayIndex(i);
        settings.setValue("name", rec.name);
        settings.setValue("turns", rec.turns);
        settings.setValue("retries", rec.retries);
        settings.setValue("rank", rec.rank);
    }
    settings.endArray();
}

void MainWindow::generateFieldWidgets(GameField & field)
{
    if(field.getWidth() != fieldWidth || field.getHeight() != fieldHeight)
    {
        field_widgets.clear();
        clearLayout(field_layout, true);
    }

    for(int row = 0; row < field.getHeight(); row++)
    {
        for(int col = 0; col < field.getWidth(); col++)
        {
            BlockWidget * block = new BlockWidget(gameData, col, row);
            connect(block, SIGNAL(onClickBlock(int, int)), this, SLOT(onBlockChanged(int,int)));
            field_widgets.push_back(block);
            field_layout->addWidget(block, row, col, 1, 1);
        }
    }

    fieldWidth = field.getWidth();
    fieldHeight = field.getHeight();
    field_layout->update();

    this->syncUI();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Cheat code to make a victory here
    if(event->key() == Qt::Key_V)
    {
        for(int y = 0; y < field.getHeight(); y++)
        {
            for(int x = 0; x < field.getWidth(); x++)
            {
                field.setBlock(x, y, BlockState::BlockHorizontal);
            }
        }
        syncUI();
    }
}

void MainWindow::onBlockChanged(int x, int y)
{
    if(this->isLocked())
        return;

    this->queuedBlockX = x;
    this->queuedBlockY = y;
    // Try to run next command
    this->runNextCommand();
}

void MainWindow::cancelTurn()
{
    this->field.cancelTurn();
    syncUI();
}

void MainWindow::syncUI()
{
    for(BlockWidget * widget: field_widgets)
    {
        int x = widget->getX();
        int y = widget->getY();
        BlockState state = field.getState(x,y);
        widget->setState(state);
        widget->update();
    }

    this->labelCurrentTurn->setText(QString::asprintf("Turn: %d", field.getCurrentTurn()));
    this->labelRetries->setText(QString::asprintf("Retries: %d", field.getRetries()));
    this->statusBar()->updateGeometry();

    if(field.checkVictory())
    {
        // 1. We add half-complete leaderboard record, and fill in its name in victory dialog
        LeaderPtr rec = this->addLeaderboardRecord(field.getCurrentTurn(), field.getRetries());
        VictoryDialog * dialog = new VictoryDialog(this, rec);

        connect(dialog, SIGNAL(onComplete(LeaderPtr)), this, SLOT(onCompleteGame(LeaderPtr)));
        // TODO: should add another event?
        startDialog(dialog);
    }
}

void MainWindow::releaseDialogLock()
{
    if(this->dialogLocked > 0)
        dialogLocked--;
}

bool MainWindow::isLocked() const
{
    return dialogLocked > 0;
}

void MainWindow::updateAnimation()
{
    if( animationTick == animationDuration)
        return;

    for(BlockWidget * widget: field_widgets)
    {
        widget->updateAnimation(animationTick);
    }

    animationTick++;

    if(animationTick == animationDuration && animationDuration > 0)
    {
        animTimer->stop();
        animationTick = 0;
        animationDuration = 0;
        if(this->hasEnqueuedCommand())
            this->runNextCommand();

        qInfo().nospace() << QString::asprintf("Animation is complete");
    }
}

bool MainWindow::hasEnqueuedCommand() const
{
    return this->queuedBlockX >= 0 && queuedBlockY >= 0;
}

void MainWindow::runNextCommand()
{
    if(!hasEnqueuedCommand())
    {
        qInfo().nospace() << QString::asprintf("Has no command to execute");
        return;
    }

    if(animationTick != animationDuration)
    {
        qInfo().nospace() << QString::asprintf("Can not execute next command right now");
        return;
    }

    int x = this->queuedBlockX;
    int y = this->queuedBlockY;

    qInfo().nospace() << QString::asprintf("Starting animation for (%d;%d)", x, y);

    queuedBlockX = -1;
    queuedBlockY = -1;
    // 1. Generate new field state
    field.newTurn();
    int max_offset = 0;
    int offset_per_tile = 2;
    // 2. Update columns and rows
    for(int col = 0; col < field.getWidth(); col++)
    {
        BlockState state = field.switchBlock(col, y);
        int index = col + y * field.getWidth();
        int offset = abs(x - col) * offset_per_tile;
        max_offset = std::max<int>(offset, max_offset);
        this->field_widgets[index]->setAnimationState(state, offset);
    }

    for(int row = 0; row < field.getWidth(); row++)
    {
        if(row != y)    // skipping central block.
        {
            BlockState state = field.switchBlock(x, row);
            int index = x + row * field.getWidth();
            int offset = abs(y - row) * offset_per_tile;
            max_offset = std::max<int>(offset, max_offset);
            this->field_widgets[index]->setAnimationState(state, offset);
        }
    }
    // 3. Sync UI according to new field state
    this->syncUI();
    // 4. Run animation
    this->animationTick = 0;
    this->animationDuration = max_offset + gameData->animFrames-1;
    this->animTimer->start();
}

void MainWindow::startDialog(QDialog * dialog)
{
    connect(dialog, SIGNAL(close), this, SLOT(releaseDialogLock()));
    dialog->show();
}

void MainWindow::startNewGame(int field_size)
{
    this->field.init(field_size, field_size);
    this->generateFieldWidgets(field);
    this->show();
}

void MainWindow::onCompleteGame(LeaderPtr rec)
{
    if(rec != nullptr)
        this->lastName = rec->name;
    this->writeLeaderboard();

    LeaderboardDialog * dialog = new LeaderboardDialog(this, leaderboard, true);
    connect(dialog, SIGNAL(onReturn()), this, SLOT(showNewGameDialog()));
    connect(dialog, SIGNAL(onExit()), this, SLOT(hide()));
    dialog->show();
}

LeaderPtr MainWindow::addLeaderboardRecord(int turns, int retries)
{
    LeaderPtr result(new LeaderboardRecord());
    result->name = this->lastName;
    result->turns = turns;
    result->retries = retries;
    result->rank = -1;
    leaderboard.push_back(result);

    // Sort leaderboard
    std::sort(leaderboard.begin(), leaderboard.end(), LeaderComparator());

    // Fixing ranks for all records
    for(size_t i = 0; i < leaderboard.size(); i++)
    {
        leaderboard[i]->rank = i;
    }
    return result;
}

void MainWindow::showNewGameDialog()
{
    NewGameDialog * dialog = new NewGameDialog(this, true);
    connect(dialog, SIGNAL(onExit()), this, SLOT(exitGame()));
    connect(dialog, SIGNAL(onStartGame(int)), this, SLOT(startNewGame(int)));
    dialog->show();
}

void MainWindow::showRestartGameDialog()
{
    NewGameDialog * dialog = new NewGameDialog(this, false);
    connect(dialog, SIGNAL(onStartGame(int)), this, SLOT(startNewGame(int)));
    dialog->show();
}

void MainWindow::showLeaderboard()
{
    LeaderboardDialog * dialog = new LeaderboardDialog(this, leaderboard, false);
    //connect(dialog, SIGNAL(onReturn()), this, SLOT(showNewGameDialog()));
    //connect(dialog, SIGNAL(onExit()), this, SLOT(hide()));
    dialog->show();
}

void MainWindow::exitGame()
{
    this->close();
}

QString MainWindow::getSettingsPath()
{
    QString appPath = QApplication::applicationDirPath();
    if(appPath.length() > 0)
        appPath += "/";
    appPath += "settings.ini";
    return appPath;
}
