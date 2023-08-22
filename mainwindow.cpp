#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), is_right_(false), is_down_(false)
{
    ui->setupUi(this);

    // We need a graphics scene in which to draw rectangles.
    scene_ = new QGraphicsScene(this);

    // The graphicsView object is placed on the window
    // at the following coordinates, but if you want
    // different placement, you can change their values.
    int left_margin = 100; // x coordinate
    int top_margin = 150; // y coordinate

    // The width of the graphicsView is BORDER_RIGHT added by 2,
    // since the borders take one pixel on each side
    // (1 on the left, and 1 on the right).
    // Similarly, the height of the graphicsView is BORDER_DOWN added by 2.
    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  BORDER_RIGHT + 2, BORDER_DOWN + 2);
    ui->graphicsView->setScene(scene_);

    // The width of the scene_ is BORDER_RIGHT decreased by 1 and
    // the height of it is BORDER_DOWN decreased by 1, because
    // each square of a tetromino is considered to be inside the sceneRect,
    // if its upper left corner is inside the sceneRect.
    scene_->setSceneRect(0, 0, BORDER_RIGHT - 1, BORDER_DOWN - 1);


    QPixmap grid_image(":/grid.png");

    scene_->addPixmap(grid_image);
    ui->graphicsView->viewport()->update();


    ui->player_name_line->setMaxLength(10);




    // Setting random engine ready for the first real call.
    int seed = time(0); // You can change seed value for testing purposes
    randomEng.seed(seed);
    distr = std::uniform_int_distribution<int>(0, NUMBER_OF_TETROMINOS - 1);
    distr(randomEng); // Wiping out the first random number (which is almost always 0)
    // After the above settings, you can use randomEng to draw the next falling
    // tetromino by calling: distr(randomEng) in a suitable method.

    // Add more initial settings and connect calls, when needed.



    timer_.setSingleShot(false);
    //    timer_.start(500);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::square_move);

    //second timer
    ui->lcd_seconds->setPalette(Qt::green);
    ui->lcd_minutes->setPalette(Qt::green);
    ui->point_display->setPalette(Qt::green);
    //    second_timer_.start(1000);
    connect(&second_timer_, &QTimer::timeout, this, &MainWindow::tic_toc);


    ui->resume_button->setDisabled(true);
    ui->pause_button->setDisabled(true);
    ui->clear_button->setDisabled(true);
    ui->submit_button->setDisabled(true);
    ui->player_name_line->setText("Player 1");

    clear_grid();
    //    generate_tetromino();



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent* event) {

    if(event->key() == Qt::Key_A) {
        direction_ = LEFT;
        return;
    }

    if(event->key() == Qt::Key_D) {
        direction_ = RIGHT;

        return;
    }


    if(event->key() == Qt::Key_S or event->key() == Qt::Key_Space) {
        int distance_to_bottom = shortest_distance_to_down();

        for(int i = 1;i<distance_to_bottom/STEP;++i){
            square_move();
        }


        return;
    }
}

void MainWindow::tic_toc()
{
    ++secs_gone_;
    int seconds = secs_gone_%60;
    minutes_gone_ = secs_gone_/60;
    ui->lcd_minutes->display(minutes_gone_);
    ui->lcd_seconds->display(seconds);

}

void MainWindow::update_points()
{
    ui->point_display->display(total_tetrominos_created_*100);
}

void MainWindow::update_next_tetromino()
{
    switch(next_tetromino_){

    case HORIZONTAL:{
        QPixmap tetromino(":/tetrominos/0_horizontal.png");
        ui->next_tetromino_label->setPixmap(tetromino);
        break;
    }

    case LEFT_CORNER:{
        QPixmap tetromino(":/tetrominos/1_left_corner.png");
        ui->next_tetromino_label->setPixmap(tetromino);
        break;
    }
    case RIGHT_CORNER:{
        QPixmap tetromino(":/tetrominos/2_right_corner.png");
        ui->next_tetromino_label->setPixmap(tetromino);
        break;
    }
    case SQUARE:{
        QPixmap tetromino(":/tetrominos/3_square.png");
        ui->next_tetromino_label->setPixmap(tetromino);
        break;
    }
    case STEP_UP_RIGHT:{
        QPixmap tetromino(":/tetrominos/4_right_step_up.png");
        ui->next_tetromino_label->setPixmap(tetromino);
        break;
    }
    case PYRAMID:{
        QPixmap tetromino(":/tetrominos/5_pyramid.png");
        ui->next_tetromino_label->setPixmap(tetromino);
        break;
    }
    case STEP_UP_LEFT:{
        QPixmap tetromino(":/tetrominos/6_left_step_up.png");
        ui->next_tetromino_label->setPixmap(tetromino);
        break;
    }
    }

}
void MainWindow::generate_tetromino()
{


    QPen blackPen(Qt::black);
    blackPen.setWidth(2);

    total_tetrominos_created_ +=1;
    qDebug()<<"Tetromino : "<<total_tetrominos_created_;
    if(difficulty_ > 200){
        difficulty_-=ACCELERATION;
    }

    timer_.stop();
    timer_.start(difficulty_);



    squares_.clear();


    //   next_tetromino_ = 3;


    switch(next_tetromino_){

    case HORIZONTAL:{
        qDebug()<<"horizontal";
        QBrush redBrush(QColor(255,102,102));
        QGraphicsRectItem* square1 = scene_->addRect(0,0,STEP,STEP,blackPen,redBrush);
        QGraphicsRectItem* square2 = scene_->addRect(0,0,STEP,STEP,blackPen,redBrush);
        QGraphicsRectItem* square3 = scene_->addRect(0,0,STEP,STEP,blackPen,redBrush);
        QGraphicsRectItem* square4 = scene_->addRect(0,0,STEP,STEP,blackPen,redBrush);

        square1->setPos(80,0);
        square2->setPos(100,0);
        square3->setPos(120,0);
        square4->setPos(140,0);

        squares_.push_back(square1);
        squares_.push_back(square2);
        squares_.push_back(square3);
        squares_.push_back(square4);
        all_squares_.push_back(square1);
        all_squares_.push_back(square2);
        all_squares_.push_back(square3);
        all_squares_.push_back(square4);


        break;
    }

    case LEFT_CORNER:{
        qDebug()<<"Leftcorner";
        QBrush cyanBrush(QColor(102,255,255));

        QGraphicsRectItem* square1 = scene_->addRect(0,0,STEP,STEP,blackPen,cyanBrush);
        QGraphicsRectItem* square2 = scene_->addRect(0,0,STEP,STEP,blackPen,cyanBrush);
        QGraphicsRectItem* square3 = scene_->addRect(0,0,STEP,STEP,blackPen,cyanBrush);
        QGraphicsRectItem* square4 = scene_->addRect(0,0,STEP,STEP,blackPen,cyanBrush);

        square1->setPos(80,0);
        square2->setPos(80,20);
        square3->setPos(100,20);
        square4->setPos(120,20);

        squares_.push_back(square1);
        squares_.push_back(square2);
        squares_.push_back(square3);
        squares_.push_back(square4);
        all_squares_.push_back(square1);
        all_squares_.push_back(square2);
        all_squares_.push_back(square3);
        all_squares_.push_back(square4);

        break;
    }
    case RIGHT_CORNER:{
        qDebug()<<"Rightcorner";
        QBrush blueBrush(QColor(51,153,255));

        QGraphicsRectItem* square1 = scene_->addRect(0,0,STEP,STEP,blackPen,blueBrush);
        QGraphicsRectItem* square2 = scene_->addRect(0,0,STEP,STEP,blackPen,blueBrush);
        QGraphicsRectItem* square3 = scene_->addRect(0,0,STEP,STEP,blackPen,blueBrush);
        QGraphicsRectItem* square4 = scene_->addRect(0,0,STEP,STEP,blackPen,blueBrush);

        square1->setPos(120,0);
        square2->setPos(120,20);
        square3->setPos(100,20);
        square4->setPos(80,20);

        squares_.push_back(square1);
        squares_.push_back(square2);
        squares_.push_back(square3);
        squares_.push_back(square4);
        all_squares_.push_back(square1);
        all_squares_.push_back(square2);
        all_squares_.push_back(square3);
        all_squares_.push_back(square4);

        break;
    }
    case SQUARE:{
        qDebug()<<"Square";
        QBrush magentaBrush(QColor(178,102,255));

        QGraphicsRectItem* square1 = scene_->addRect(0,0,STEP,STEP,blackPen,magentaBrush);
        QGraphicsRectItem* square2 = scene_->addRect(0,0,STEP,STEP,blackPen,magentaBrush);
        QGraphicsRectItem* square3 = scene_->addRect(0,0,STEP,STEP,blackPen,magentaBrush);
        QGraphicsRectItem* square4 = scene_->addRect(0,0,STEP,STEP,blackPen,magentaBrush);

        square1->setPos(100,0);
        square2->setPos(120,0);
        square3->setPos(100,20);
        square4->setPos(120,20);

        squares_.push_back(square1);
        squares_.push_back(square2);
        squares_.push_back(square3);
        squares_.push_back(square4);
        all_squares_.push_back(square1);
        all_squares_.push_back(square2);
        all_squares_.push_back(square3);
        all_squares_.push_back(square4);
        break;
    }
    case STEP_UP_RIGHT:{
        qDebug()<<"StepUpRight";
        QBrush yellowBrush(QColor(255,255,102));

        QGraphicsRectItem* square1 = scene_->addRect(0,0,STEP,STEP,blackPen,yellowBrush);
        QGraphicsRectItem* square2 = scene_->addRect(0,0,STEP,STEP,blackPen,yellowBrush);
        QGraphicsRectItem* square3 = scene_->addRect(0,0,STEP,STEP,blackPen,yellowBrush);
        QGraphicsRectItem* square4 = scene_->addRect(0,0,STEP,STEP,blackPen,yellowBrush);

        square1->setPos(120,0);
        square2->setPos(140,0);
        square3->setPos(120,20);
        square4->setPos(100,20);

        squares_.push_back(square1);
        squares_.push_back(square2);
        squares_.push_back(square3);
        squares_.push_back(square4);
        all_squares_.push_back(square1);
        all_squares_.push_back(square2);
        all_squares_.push_back(square3);
        all_squares_.push_back(square4);
        break;
    }
    case PYRAMID:{
        qDebug()<<"Pyramid";
        QBrush greenBrush(QColor(102,255,178));

        QGraphicsRectItem* square1 = scene_->addRect(0,0,STEP,STEP,blackPen,greenBrush);
        QGraphicsRectItem* square2 = scene_->addRect(0,0,STEP,STEP,blackPen,greenBrush);
        QGraphicsRectItem* square3 = scene_->addRect(0,0,STEP,STEP,blackPen,greenBrush);
        QGraphicsRectItem* square4 = scene_->addRect(0,0,STEP,STEP,blackPen,greenBrush);

        square1->setPos(100,0);
        square2->setPos(80,20);
        square3->setPos(100,20);
        square4->setPos(120,20);

        squares_.push_back(square1);
        squares_.push_back(square2);
        squares_.push_back(square3);
        squares_.push_back(square4);
        all_squares_.push_back(square1);
        all_squares_.push_back(square2);
        all_squares_.push_back(square3);
        all_squares_.push_back(square4);
        break;
    }
    case STEP_UP_LEFT:{
        qDebug()<<"StepUpLeft";
        QBrush darkGreenBrush(QColor(102,255,102));

        QGraphicsRectItem* square1 = scene_->addRect(0,0,STEP,STEP,blackPen,darkGreenBrush);
        QGraphicsRectItem* square2 = scene_->addRect(0,0,STEP,STEP,blackPen,darkGreenBrush);
        QGraphicsRectItem* square3 = scene_->addRect(0,0,STEP,STEP,blackPen,darkGreenBrush);
        QGraphicsRectItem* square4 = scene_->addRect(0,0,STEP,STEP,blackPen,darkGreenBrush);

        square1->setPos(80,0);
        square2->setPos(100,0);
        square3->setPos(100,20);
        square4->setPos(120,20);

        squares_.push_back(square1);
        squares_.push_back(square2);
        squares_.push_back(square3);
        squares_.push_back(square4);
        all_squares_.push_back(square1);
        all_squares_.push_back(square2);
        all_squares_.push_back(square3);
        all_squares_.push_back(square4);
        break;
    }
    }

    next_tetromino_ = distr(randomEng);
    update_next_tetromino();
}



void MainWindow::square_move()
{



    int delta_y = STEP;
    int delta_x = 0;

    if(direction_ == RIGHT) {
        delta_x = static_cast<int>(STEP);  // right (positive step)
    } else if(direction_ == LEFT){
        delta_x = static_cast<int>(-STEP); // left (negative step)
    }



    if(delta_x != 0){
        // Checking on each square if its possible to move on the sides, if not delta_x is 0 for every next check and movement of squares
        for(auto &square : squares_){
            if(!can_move_to_side(square,delta_x)){
                delta_x = 0;
            }
        }
    }

    for(auto &square : squares_){

        if(!can_move_down(square,delta_x)){
            delta_y = 0;
        }
    }


    squares_.at(0)->moveBy(delta_x,delta_y);
    squares_.at(1)->moveBy(delta_x,delta_y);
    squares_.at(2)->moveBy(delta_x,delta_y);
    squares_.at(3)->moveBy(delta_x,delta_y);
    direction_ = NO_DIRECTION;

    if(delta_y == 0){
        occupy_tile(squares_[0]);
        occupy_tile(squares_[1]);
        occupy_tile(squares_[2]);
        occupy_tile(squares_[3]);
        update_points();



        if(game_over()){
            end_game();
            return;
        }
        generate_tetromino();

        return;
    }
    return;


}

bool MainWindow::game_over()
{
    for(auto v : grid_){
        if(v[0] == true){
            return true;
        }
    }
    return false;
}

void MainWindow::end_game()
{
    int points = ui->point_display->intValue();
    hiscores_.insert({points,current_player_});
    current_player_ = "";
    timer_.stop();
    second_timer_.stop();
    ui->submit_button->setDisabled(false);
    ui->clear_button->setDisabled(false);
    ui->pause_button->setDisabled(true);
    return;
}

int MainWindow::shortest_distance_to_down()
{

    int closest_occupied_cell = BORDER_DOWN-STEP;
    int min_y = BORDER_DOWN;

    for(auto square : squares_){
        int x = static_cast<int>(square->x());
        int y = static_cast<int>(square->y());
        if(y < min_y){
            min_y = y;
        }

        unsigned int column = static_cast<unsigned int>(x/20); // sen vektorin indeksi, joka käydään läpi ja  etsitään true

        int i = 0;
        for(auto e : grid_.at(column)){
            if (e == true){
                if(i*STEP < closest_occupied_cell){
                    closest_occupied_cell = i * STEP;
                }
                break;
            }
            ++i;
        }


    }
    if(closest_occupied_cell == BORDER_DOWN-STEP){

        return closest_occupied_cell - min_y;
    }
    return closest_occupied_cell;
}



void MainWindow::occupy_tile(QGraphicsRectItem *square)
{
    int x = static_cast<int>(square->x());
    int y = static_cast<int>(square->y());

    unsigned int column = static_cast<unsigned int>(x/20);
    unsigned int row = static_cast<unsigned int>(y/20);

    grid_.at(column).at(row) = true;
}



bool MainWindow::is_tile_occupied(int x, int y)
{
    unsigned int column = static_cast<unsigned int>(x/20);
    unsigned int row = static_cast<unsigned int>(y/20);

    if(grid_.at(column).at(row) == false){
        return false;
    }else{
        return true;
    }
}

bool MainWindow::can_move_to_side(QGraphicsRectItem* square, int delta_x)
{

    int delta_y = STEP;
    int current_x = static_cast<int>(square->x());
    int current_y = static_cast<int>(square->y());

    if(current_y == BORDER_DOWN-STEP){
        return false;
    }


    if(current_x == 0 and delta_x == LEFT*STEP){
        return false;
    }
    if(current_x == BORDER_RIGHT-STEP and delta_x == RIGHT*STEP){
        return false;
    }


    // Future position of a single square
    int new_x = current_x + delta_x;
    int new_y = current_y + delta_y;


    if(is_tile_occupied(new_x,new_y) == true){ // if cant move on sides, setting delta x = 0
        new_x = current_x;
        delta_x = 0;
        return false;
    }else{
        return true;
    }


}

bool MainWindow::can_move_down(QGraphicsRectItem *square, int delta_x)
{
    int delta_y = STEP;
    int current_x = static_cast<int>(square->x());
    int current_y = static_cast<int>(square->y());


    // Future position of a single square
    int new_x = current_x + delta_x;
    int new_y = current_y + delta_y;


    QRectF rect = scene_->sceneRect();
    if(rect.contains(new_x, new_y + STEP/2) and //checking if scene had the coordinates
            is_tile_occupied(new_x,new_y) == false) { // Checking if the future coordinates are occupied
        return true;
    }else{
        return false;
    }
}


void MainWindow::clear_grid()
{
    unsigned int total_rows = 24;
    unsigned int total_columns = 12;

    grid_.resize(total_columns,std::vector<bool>(total_rows,false));
    for(auto &v : grid_){
        for(auto e : v){
            e = false;
        }
    }

}

void MainWindow::clear_squares()
{
    for(auto &square : all_squares_){
        scene_->removeItem(square);
    }
    all_squares_.clear();
}

void MainWindow::on_start_button_clicked()
{

    current_player_ = ui->player_name_line->text().toStdString();
    if (current_player_ == ""){
        current_player_ = "Player 1";
    }


    difficulty_ = ui->difficulty_slider->value();
    ui->player_name_line->setDisabled(true);
    ui->start_button->setDisabled(true);
    ui->pause_button->setDisabled(false);
    ui->difficulty_slider->setDisabled(true);
    ui->clear_button->setDisabled(true);
    ui->submit_button->setDisabled(true);

    switch (difficulty_) {
    case 0: {difficulty_ = 900; break;} //EASY
    case 1: {difficulty_ = 600; break;} //MEDIUM
    case 2: {difficulty_ = 300; break;} //HARD
    }

    second_timer_.start(1000);
    timer_.start(difficulty_);

    //first tetromino
    next_tetromino_ = distr(randomEng);

    generate_tetromino();

}

void MainWindow::on_pause_button_clicked()
{
    ui->pause_button->setDisabled(true);
    ui->resume_button->setDisabled(false);
    ui->clear_button->setDisabled(false);
    timer_.stop();
    second_timer_.stop();
}

void MainWindow::on_resume_button_clicked()
{
    ui->clear_button->setDisabled(true);
    ui->resume_button->setDisabled(true);
    timer_.start(difficulty_);
    ui->pause_button->setDisabled(false);
    second_timer_.start(1000);
}

void MainWindow::on_quit_button_clicked()
{
    close();
}

void MainWindow::on_clear_button_clicked()
{

    total_tetrominos_created_ = 0;
    second_timer_.stop();
    secs_gone_ = 0;
    ui->lcd_minutes->display(0);
    ui->lcd_seconds->display(0);
    clear_grid();
    clear_squares();

    ui->player_name_line->setDisabled(false);
    ui->clear_button->setDisabled(true);
    ui->start_button->setDisabled(false);
    ui->resume_button->setDisabled(true);
    ui->difficulty_slider->setDisabled(false);
    ui->next_tetromino_label->clear();
    ui->submit_button->setDisabled(true);
}

void MainWindow::on_submit_button_clicked()
{
    ui->hiscores_browser->clear();

    for (auto player = hiscores_.rbegin(); player != hiscores_.rend(); ++player) {
        ui->hiscores_browser->append(QString::fromStdString(player->second)+" : "+
                                 QString::number(player->first));
        }

}
