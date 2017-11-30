#ifndef LEADERBOARDDIALOG_H
#define LEADERBOARDDIALOG_H

#include <QDialog>
#include <vector>
#include <memory>

struct LeaderboardRecord
{
    QString name;
    int turns;
    int retries;
    int rank;       // Current rank
};

typedef std::shared_ptr<LeaderboardRecord> LeaderPtr;

// Comparator for leaderboard sorting
struct LeaderComparator
{
  bool operator() (const LeaderPtr & a, const LeaderPtr & b) const
  {
      return (a->turns == b->turns) ? (a->retries < b->retries) : (a->turns < b->turns);
  }
};


typedef std::vector<LeaderPtr> Leaderboard;

class LeaderboardDialog : public QDialog
{
    Q_OBJECT
public:
    LeaderboardDialog(QWidget *parent, const Leaderboard & leaderboard, bool makeNewGame);

signals:
    void onReturn();
    void onExit();
protected:
    // Should we start a new game after we exit dialog
    bool newGame;
};

#endif // LEADERBOARDDIALOG_H
