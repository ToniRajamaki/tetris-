#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QGraphicsScene>
#include <random>
#include <QTimer>
#include <QGraphicsRectItem>
#include <vector>
#include <iostream>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event) override;

private slots:

    void tic_toc();

    void update_points();

    void update_next_tetromino();


    void on_start_button_clicked();

    void on_pause_button_clicked();

    void on_resume_button_clicked();

    void on_quit_button_clicked();

    void on_clear_button_clicked();

    void on_submit_button_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene* scene_;
//    QGraphicsRectItem* square_;
    std::vector<QGraphicsRectItem*> squares_;
    std::vector<QGraphicsRectItem*> all_squares_;
    int next_tetromino_;

    // Constants describing scene coordinates
    // Copied from moving circle example and modified a bit
    const int STEP = 20;
    const int BORDER_UP = 0;
    const int BORDER_DOWN = 480; // 260; (in moving circle)
    const int BORDER_LEFT = 0;
    const int BORDER_RIGHT = 240; // 680; (in moving circle)
    const int LEFT = -1;
    const int RIGHT = 1;
    const int NO_DIRECTION = 3;
    const int ACCELERATION = 10;





    // Size of a tetromino component
    const int SQUARE_SIDE = 20;
    // Number of horizontal cells (places for tetromino components)
    const int COLUMNS = BORDER_RIGHT / SQUARE_SIDE;
    // Number of vertical cells (places for tetromino components)
    const int ROWS = BORDER_DOWN / SQUARE_SIDE;

    // Constants for different tetrominos and the number of them
    enum Tetromino_kind {HORIZONTAL,
                         LEFT_CORNER,
                         RIGHT_CORNER,
                         SQUARE,
                         STEP_UP_RIGHT,
                         PYRAMID,
                         STEP_UP_LEFT,
                         NUMBER_OF_TETROMINOS};

    /*
    // From the enum values above, only the last one is needed in this template.
    // Recall from enum type that the value of the first enumerated value is 0,
    // the second is 1, and so on.
    // Therefore the value of the last one is 7 at the moment.
    // Remove those tetromino kinds above that you do not implement,
    // whereupon the value of NUMBER_OF_TETROMINOS changes, too.
    // You can also remove all the other values, if you do not need them,
    // but most probably you need a constant value for NUMBER_OF_TETROMINOS.
     */

    // For randomly selecting the next dropping tetromino
    std::default_random_engine randomEng;
    std::uniform_int_distribution<int> distr;

    // More constants, attibutes, and methods
    int direction_ = NO_DIRECTION;


    std::vector<std::vector<bool>> grid_;
    int total_tetrominos_created_ = 0;
    QTimer second_timer_;
    QTimer timer_;          // for continuous moving
    bool is_right_;    // is_right // is_horizontal
    bool is_down_;      // is_down // is_vertical

    int secs_gone_ = 0;
    int minutes_gone_ = 0;
    int difficulty_ = 1;

    std::string current_player_;
    std::multimap<int,std::string> hiscores_; //  <score,player>

    void generate_tetromino();

    void square_move();

    bool game_over();

    void end_game();

    int shortest_distance_to_down();



    // Checks if moving is possible and turns the move button
    // either enabled or disabled
    void update_move_button();


    void occupy_tile(QGraphicsRectItem* square);

    /*!
     * \brief is_tile_occupied, This function is used to determina whats the
     * occupancy situation is in the grid_vector with given coordinates
     * \param x coordinate in graphics scene ( 0-220 ) 0 + total_columns * 20
     * \param y coordinate in graphics scene ( 0 - 460 ) 0 + total rows * 20
     * \return True, if the given coordinates has no tile already
     */
    bool is_tile_occupied(int x,  int y);


    bool can_move_to_side(QGraphicsRectItem* square, int delta_x);

    bool can_move_down(QGraphicsRectItem* square, int delta_x);

    /*!
     * \brief Creates 24x12 matrix, and sets everyvalue to false;
     */
    void clear_grid();
    void clear_squares();


};

#endif // MAINWINDOW_HH
