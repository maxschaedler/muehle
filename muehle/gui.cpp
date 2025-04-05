#include "gui.h"
#include "ui_gui.h"
#include "logic.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QDebug>

Logic logic;

QPointF offset(125,25);
int scaler = 50;
Muehle_Feld* muehle_feld = new Muehle_Feld(scaler, offset, 40);

void muehle::resetGame() {
    logic.start();
    muehle_feld->reset();
}

Muehle_Stein::Muehle_Stein()
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setCursor(Qt::OpenHandCursor);
    position = muehle_feld->eigenes_aus;
};

void Muehle_Stein::mousePressEvent(QGraphicsSceneMouseEvent *event){
    setCursor(Qt::ClosedHandCursor);
    drag_offset = event->pos();
    event->accept();
    muehle_feld->start_moving(position);
};

void Muehle_Stein::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    setCursor(Qt::OpenHandCursor);
    position.point = this->scenePos();
    Muehle_Position p = muehle_feld->stop_moving(position); // neue Position bzw. startwert bei misgluecktem snap
    bool ist_gueltig_zug = false;

    switch(muehle_feld->get_state())
    {
    case KEIN_STATE: break;
    case PLACE: ist_gueltig_zug = logic.place(p.muehle_position_kennung,color);  break;
    case MOVE: ist_gueltig_zug = logic.move(muehle_feld->start_position.muehle_position_kennung,p.muehle_position_kennung,color); break;
    case REMOVE: ist_gueltig_zug = logic.remove(muehle_feld->start_position.muehle_position_kennung,color);  if (ist_gueltig_zug) {muehle_feld->scene->removeItem(this); ist_visible = false;} break;
    }

    event->accept();

    if (ist_gueltig_zug)
    {
        setPos(p.point);
        position.set(p);
    }
    else
    {
        setPos(muehle_feld->start_position.point);
        position.set(muehle_feld->start_position);
    }

    muehle_feld->display->setText(logic.getinfo());
};

void Muehle_Stein::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    setPos(event->scenePos() - drag_offset);
    event->accept();
};


muehle::muehle(QWidget *parent) : QMainWindow(parent), ui(new Ui::muehle)
{
    ui->setupUi(this);
    this->setFixedSize(900,900);
    connect(ui->Spielstarten, &QPushButton::clicked, this, &muehle::resetGame);
    muehle_feld->display = ui->Display;
    muehle_feld->display->setText(logic.getinfo());

    // Spielfeld-Bild als QLabel
    QLabel* Feld = new QLabel(ui->Spielflaeche);
    QPixmap pix(":/images/images/Spielbrett_gezeichnet_2.png");
    Feld->move(25, 50);
    Feld->setPixmap(pix);
    Feld->setScaledContents(true);
    Feld->setFixedSize(850, 600);
    Feld->lower();
    Feld->setAttribute(Qt::WA_TransparentForMouseEvents);


    // Szene & View für Spielsteine
    QGraphicsScene* scene = new QGraphicsScene(ui->Spielflaeche);
    muehle_feld->scene = scene;

    QGraphicsView* view = new QGraphicsView(scene, ui->Spielflaeche);
    view->setStyleSheet("background: transparent;");
    view->setFrameShape(QFrame::NoFrame);
    view->setSceneRect(0, 0, 900, 800);
    view->move(0, 0);

    for (uint8_t stein_index = 0; stein_index < 18; stein_index += 1) {
        muehle_feld->steine[stein_index] = new Muehle_Stein();
        muehle_feld->steine[stein_index]->setRect(0, 0, 50, 50);
        muehle_feld->steine[stein_index]->setFlag(QGraphicsItem::ItemIsMovable);
    }
    muehle_feld->reset();
}

muehle::~muehle()
{
    delete ui;
}

void Muehle_Position::set_Position_from_Kennung(enum Muehle_Position_Kennung kennung)
{
    muehle_position_kennung = kennung;
    switch (kennung) {
    case KEINE_KENNUNG: point.setX(-1); point.setY(-1); break;
    case    W_AUS: point.setX(-2);point.setY(6);break;
    case    O_AUS: point.setX(14);point.setY(6);break;
    case    N1: point.setX(6);point.setY(0);break;
    case    O1: point.setX(12);point.setY(6);break;
    case    S1: point.setX(6);point.setY(12);break;
    case    W1: point.setX(0);point.setY(6);break;
    case    N2: point.setX(6);point.setY(2);break;
    case    O2: point.setX(10);point.setY(6);break;
    case    S2: point.setX(6);point.setY(10);break;
    case    W2: point.setX(2);point.setY(6);break;
    case    N3: point.setX(6);point.setY(4);break;
    case    O3: point.setX(8);point.setY(6);break;
    case    S3: point.setX(6);point.setY(8);break;
    case    W3: point.setX(4);point.setY(6);break;
    case    NO1:point.setX(12);point.setY(0);break;
    case    NW1:point.setX(0);point.setY(0);break;
    case    SO1:point.setX(12);point.setY(12);break;
    case    SW1:point.setX(0);point.setY(12);break;
    case    NO2:point.setX(10);point.setY(2);break;
    case    NW2:point.setX(2);point.setY(2);break;
    case    SO2:point.setX(10);point.setY(10);break;
    case    SW2:point.setX(2);point.setY(10);break;
    case    NO3:point.setX(8);point.setY(4);break;
    case    NW3:point.setX(4);point.setY(4);break;
    case    SO3:point.setX(8);point.setY(8);break;
    case    SW3:point.setX(4);point.setY(8);break;
    }
};

qreal Muehle_Position::get_distance(Muehle_Position position)
{
    return QLineF(point, position.point).length();
};

void Muehle_Position::set_real_position_from_kennung(enum Muehle_Position_Kennung kennung, int8_t skalierwert, QPointF offset)
{
    set_Position_from_Kennung(kennung);
    point = point * skalierwert + offset;
};

bool Muehle_Position::ist_position_gleich(Muehle_Position position)
{
    return muehle_position_kennung == position.muehle_position_kennung;
};

void Muehle_Position::set(Muehle_Position position){
    point = position.point;
    muehle_position_kennung = position.muehle_position_kennung;
};

Muehle_Feld::Muehle_Feld(int8_t neuer_skalierwert, QPointF neuer_offset, qreal neue_min_snap_distance)
{
    feld_offset = neuer_offset;
    skalierwert = neuer_skalierwert;
    min_snap_distance = neue_min_snap_distance;
    invalid.set_real_position_from_kennung(KEINE_KENNUNG,skalierwert,feld_offset);
    eigenes_aus.set_real_position_from_kennung(W_AUS,skalierwert,feld_offset);
    gegner_aus.set_real_position_from_kennung(O_AUS,skalierwert,feld_offset);
    uint8_t index = 0;
    muehle_positionen[index++].set_real_position_from_kennung(N1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(NO1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(O1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(SO1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(S1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(SW1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(W1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(NW1, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(N2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(NO2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(O2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(SO2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(S2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(SW2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(W2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(NW2, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(N3, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(NO3, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(O3, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(SO3, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(S3, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(SW3, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(W3, skalierwert, feld_offset);
    muehle_positionen[index++].set_real_position_from_kennung(NW3, skalierwert, feld_offset);
};

void Muehle_Feld::reset() {
    display->setText(logic.getinfo());
    for (uint8_t stein_index = 0; stein_index < 18; stein_index += 1) {
        if (steine[stein_index]->ist_visible) {
            scene->removeItem(steine[stein_index]);
        }
        if (stein_index%2==0) {
            steine[stein_index]->setPos(eigenes_aus.point);
            steine[stein_index]->setBrush(Qt::black);
            steine[stein_index]->color = Chip::BLACK;
            steine[stein_index]->position.set(eigenes_aus);
        }
        else {
            steine[stein_index]->setPos(gegner_aus.point);
            steine[stein_index]->setBrush(Qt::white);
            steine[stein_index]->color = Chip::WHITE;
            steine[stein_index]->position.set(gegner_aus);
        }
        scene->addItem(steine[stein_index]);
        steine[stein_index]->ist_visible = true;
    }
};

void Muehle_Feld::start_moving(Muehle_Position position)
{
    start_position.set(position);
};

Muehle_Position Muehle_Feld::stop_moving(Muehle_Position free_position)
{

    if (free_position.get_distance(eigenes_aus) < min_snap_distance)
    {
        state = REMOVE;
        return eigenes_aus;
    }


    if (free_position.get_distance(gegner_aus) < min_snap_distance)
    {
        state = REMOVE;
        return gegner_aus;
    }


    for (uint8_t index = 0; index < 24; index += 1)
    {
        if (free_position.get_distance(muehle_positionen[index]) < min_snap_distance)
        {
            if (start_position.ist_position_gleich(eigenes_aus)||start_position.ist_position_gleich(gegner_aus))
            {
                state = PLACE;
            }
            else
            {
                state = MOVE;
            }
            return muehle_positionen[index];
        }
    }
    state = KEIN_STATE;
    return start_position;
};

Muehle_State Muehle_Feld::get_state()
{
    return state;
};
