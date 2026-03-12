#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <unistd.h>

class GameWidget : public QWidget {
  Q_OBJECT

public:
  GameWidget(QWidget *parent = nullptr) : QWidget(parent) {
    setFixedSize(800, 600);
    playerX = 50;
    playerY = 250;
    enemyActive = false;
    targetActive = false;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWidget::gameLoop);
    timer->start(16); // ~60 FPS
  }

protected:
  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);
    painter.fillRect(rect(), Qt::darkGray); // Hintergrund

    // Object
    painter.setBrush(Qt::green);
    painter.drawRect(playerX, playerY, 20, 20);

    // Target
    if(targetActive){
      QColor orange(255, 165, 0);
      painter.setBrush(orange);
      painter.drawEllipse(targetX, targetY, 20, 20);
    }

    // Enemy
    if(enemyActive) {
      painter.setBrush(Qt::red);
      painter.drawRect(enemyX, enemyY, 30, 30);
      int abstandX, abstandY;
      if(enemyX > playerX){
        abstandX = enemyX - playerX;
      }else{
        abstandX = playerX - enemyX;
      }
      if(enemyY > playerY){
        abstandY = enemyY - playerY;
      }else{
        abstandY = playerY - enemyY;
      }
      if(abstandX < 20 && abstandY < 20){
        printf("Game finished\n");
        printf("Points: %d\n", score);
        sleep(3);
        exit(0);
      }
    }
  }

  void keyPressEvent(QKeyEvent *event) override {
    switch (event->key()) {
      case Qt::Key_Up:
        playerY -= 5;
        if(playerY < 0) playerY = 0;
        break;
      case Qt::Key_Down:
        playerY += 5;
        if(playerY > height()-20) playerY = height()-20;
        break;
      case Qt::Key_Left:
        playerX -= 5;
        if(playerX < 0) playerX = 0;
        break;
      case Qt::Key_Right:
        playerX += 5;
        if(playerX > width()-20) playerX = width()-20;
        break;
      default:
        QWidget::keyPressEvent(event);
      }
      int abstandX, abstandY;
      if(targetX > playerX){
        abstandX = targetX - playerX;
      }else{
        abstandX = playerX - targetX;
      }
      if(targetY > playerY){
        abstandY = targetY - playerY;
      }else{
        abstandY = playerY - targetY;
      }
      if(abstandX < 20 && abstandY < 20){
        score++;
        printf("Hit target! Add 1 point to score, N(points)=%d\n", score);
        targetActive = false;
      }
      update();
    }

private slots:
  void gameLoop(){
    if(enemyActive){ 
      enemyY += 10;
      usleep(500);
      if(enemyY > height()){
        enemyActive = false;
      }
    }
    else{
      enemyActive = true;
      enemyX = rand()%799;
      enemyY = 0;
    }
    if(!targetActive){
      targetX = rand()%(width()-1);
      targetY = rand()%(height()-1);
      targetActive = true;
    }
    update();
  }

private:
  int playerX, playerY;
  int enemyX, enemyY;
  int targetX, targetY;
  int score = 0;
  bool enemyActive, targetActive;
  QTimer *timer;
};

int main(int argc, char *argv[]) {
  srand(time(NULL));
  QApplication app(argc, argv);
  GameWidget w;
  w.show();
  return app.exec();
}

#include "main.moc"
