#ifndef GUI_H
#define GUI_H

#include "logic.h"

#include <QMainWindow>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QPixmap>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class muehle;
}
QT_END_NAMESPACE

class Muehle_Stein;

/**
 * @brief muehle: Die GUI.
 */
class muehle : public QMainWindow
{
    Q_OBJECT

public:
    muehle(QWidget *parent = nullptr);
    ~muehle();
public slots:

    /**
     * @brief resetGame: Die Chips und die logic wird zurückgesetzt.
     */
    void resetGame();

private:
    Ui::muehle *ui;
};

/**
 * @brief Muehle_Position_Kennung: Die Positionen des Spielfelds werden als enum definieret und mit den Werten von der Klasse Board von logic versehen.
 */
enum Muehle_Position_Kennung {
    KEINE_KENNUNG=-3, W_AUS=-2, O_AUS=-1,
    N1=0, O1=2, S1=4, W1=6,
    N2=8, O2=10, S2=12, W2=14,
    N3=16, O3=18, S3=20, W3=22,
    NO1=1, NW1=7, SO1=3, SW1=5,
    NO2=9, NW2=15, SO2=11, SW2=13,
    NO3=17, NW3=23, SO3=19, SW3=21
};

/**
 * @brief Muehle_State: Die Spielphasen:
 *                              KEIN_STATE für kein snapping,
 *                              PLACE für Bewegung des Chips von Außen auf das Mühlebrett,
 *                              MOVE für Zug und Springen des Chips,
 *                              REMOVE für Entfernung des Chips.
 */
enum Muehle_State {KEIN_STATE, PLACE, MOVE, REMOVE};

/**
 * @brief Muehle_Position: Die Klasse Muehle_Position beschreibt die Positionen des innerhalb und außerhalb des Spielfelds als Koordinaten QPointF für die GUI und die Muehle_Position_Kennung.
 */
class Muehle_Position
{
public:
    /**
     * @brief point: Koordinaten QPointF der GUI.
     */
    QPointF point;

    /**
     * @brief muehle_position_kennung: Variable, um die aktuelle Muehle_Position_Kennung zu speichern.
     */
    enum Muehle_Position_Kennung muehle_position_kennung = KEINE_KENNUNG;

    /**
     * @brief set_Position_from_Kennung: Setzt die Koordinaten QPointF auf das kleinst mögliche Spielbrett (12*12 Pixel).
     * @param kennung: Je nach Kennung wird eine andere Koordinate QPointF übergeben.
     */
    void set_Position_from_Kennung(enum Muehle_Position_Kennung kennung);

    /**
     * @brief get_distance: Berechnet die Entfernung von dieser zur anderen Muehle_Position basierend auf dessen Koordinaten QPointF.
     * @param position: Die andere Muehle_Position.
     * @return Entfernung zur anderen Muehle_Position.
     */
    qreal get_distance(Muehle_Position position);

    /**
     * @brief ist_position_gleich: Überprüft,ob die Muehle_Position_Kennung gleich sind.
     * @param position: Die andere Muehle_Position.
     * @return true, wenn die Muehle_Position_Kennung gleich sind.
     */
    bool ist_position_gleich(Muehle_Position position);

    /**
     * @brief set: Setzt die Muehle_Positions_Kennung und die Koordinaten auf die der anderen Muehle_Position.
     * @param position: die andere Muehle_Position.
     */
    void set(Muehle_Position position);

    /**
     * @brief set_real_position_from_kennung die Muehle_Position_Kennung und die Koordinaten QPointF für das kleinst mögliche Spielfeld werden gesetzt, danach werden die Koordinaten QPointF.
     * skaliert und um den offset verschoben.
     * @param kennung: neue Muehle_Position_Kennung.
     * @param skalierwert: Koordinaten QPointF werden um diesen Faktor skaliert.
     * @param offset: Koordinaten QPointF werden nach dem skalieren um diesen Offset verschoben.
     */
    void set_real_position_from_kennung(enum Muehle_Position_Kennung kennung, int8_t skalierwert, QPointF offset);
};

/**
 * @brief Muehle_Feld: Das Muehle_Feld berechnet zuerst alle möglichen Koordinaten QPointF, wo ein Stein abgelegt werden kann,
 * basierend auf dem skalierwert und dem offset.
 * Das Muehle Feld wird außerdem verwendet, um die Muehle_Steine zu organisieren, sie also auf dem Spielfeld richtig zu positionieren und die Spielzustände
 * Platzieren, Bewegen, und Entfernen aus dem Verschieben der Muehle_Steine zu ermitteln.
 * Das Muehle_Feld enthält wichtige GUI-Objekte wie die Scene und das Label, um die Muehle_Steine auf dem Spielfeld erscheinen und verschwinden zu lassen sowie den
 * Infotext aus der Logik auszugeben.
 */
class Muehle_Feld : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief steine: Die Muehle_Stein Pointer für 18 Spielsteine.
     */
    Muehle_Stein* steine[18];

    /**
     * @brief invalid: Die Muehle_Position für Muehle_Position_Kennung KEINE_KENNUNG;
     */
    Muehle_Position invalid;

    /**
     * @brief eigenes_aus: Die Muehle_Position der Ablage für die schwarzen Spielsteine.
     */
    Muehle_Position eigenes_aus;

    /**
     * @brief gegner_aus: Die Muehle_Position der Ablage für die weißen Spielsteine.
     */
    Muehle_Position gegner_aus;

    /**
     * @brief muehle_positionen: Die 24 Muehle_Positionen, die das Mühlespielfeld hat.
     */
    Muehle_Position muehle_positionen[24];

    /**
     * @brief start_position: In dieser Muehle_Position wird die Ausgangsposition jedes Steins gespeichert, wenn er ergriffen wird.
     */
    Muehle_Position start_position;

    /**
     * @brief state: Der Muehle_State wird hier gespeichert. Der Muehle_State ist nur gültig nachdem  start_moving() gefolgt von stop_moving() aufgerufen wird.
     */
    enum Muehle_State state = KEIN_STATE;

    /**
     * @brief skalierwert: Der skalierwert wird verwendet, um alle Muehle_Positionen entsprechend zu skalieren.
     */
    int8_t skalierwert = 1;

    /**
     * @brief feld_offset: Der QPointF Feld_offset wird verwendet, um alle Muehle_Positionen an die vorgesehenen Koordinaten zu verschieben.
     */
    QPointF feld_offset;

    /**
     * @brief min_snap_distance: Die min_snap_distance ist die maximale Entfernung die ein abgelegter Muehle_Stein zu einer vorgegeben Muehle_Position, wie z.B. eigenes_aus, haben kann, um an diese Position gesetzt werden zu können.
     */
    qreal min_snap_distance = 40;

    /**
     * @brief scene: Die QGraphicsScene wird verwendet, um die Steine auf der GUI zu erscheinen und verschwinden zu lassen.
     */
    QGraphicsScene* scene;

    /**
     * @brief Display: Das QLabel zeigt den Infotext der logik an.
     */
    QLabel* display;

    /**
     * @brief Muehle_Feld: Der Konstruktor für Muehle_Feld berechnet alle möglichen Muehle_Positionen anhand des skalierwerts und Offsets.
     * @param neuer_skalierwert: Der neue skalierwert wird gesetzt.
     * @param neuer_offset: Der neue Offset wird gesetzt.
     * @param neue_min_snap_distance die neue min_snap_distance wird gesetzt.
     */
    Muehle_Feld(int8_t neuer_skalierwert, QPointF neuer_offset,qreal neue_min_snap_distance);

    /**
     * @brief reset: Die reset-Funktion lässt alle Spielsteine auf dem Brett verschwinden, setzt die Spielsteine zurück in die jeweiligen Ablagen für schwarze und weiße Steine und stellt die Steine wieder sichtbar.
     */
    void reset();

    /**
     * @brief start_moving: Die Funktion wird aufgerufen, wenn ein Spielstein ergriffen wurde, um seine Ausgangsposition zu erhalten.
     * @param position: Die Muehle_Position, die der Spielstein hat, wenn er gegriffen wird.
     */
    void start_moving(Muehle_Position position);

    /**
     * @brief stop_moving: Die Funktion wird aufgerufen, wenn ein Spielstein losgelassen wurde, ist die neue Muehle_Position des Steines nahe genug an einer möglichen Muehle_Position, so wird der Stein dort abgelegt. Je nachdem wo der Stein ergriffen und losgelassen wurde, stellt sich der Muehle_State ein.
     * Ist keine mögliche Muehle_Position nah genug, wird start_position zurückgegeben und der Muehle_Sate auf KEIN_STATE gesetzt.
     * @param free_position: Die Muehle_Position free_position sind die tatsächlichen Koordinaten QPointF des bewegten Spielsteins. Die Entfernung der Muehle_Position free_position wird auf alle möglichen Muehle_Position berechnet, wenn eine Distanz kleiner ist als min_snap_distance, wird der Stein dort abgelegt.
     * @return Ist der Abstand zu einer möglichen Muehle_Position klein genug, wird die entsprechende mögliche Muehle_Position zurückgegeben, sonst wird die start_position zurückgegeben und der Muehle_State ist KEIN_STATE.
     */
    Muehle_Position stop_moving(Muehle_Position free_position);

    /**
     * @brief get_state: Der Muehle_State wird zurückgegeben.
     * @return: Der Muehle_State wird zurückgegeben.
     */
    Muehle_State get_state();
};


/**
 * @brief Muehle_Stein: Die Klasse Muehle_Stein repräsentiert die Spielsteine in der GUI, also QGraphicsEllipseItem. Der Muehle_Stein kann per Drag and Drop bewegt werden. Die Klasse nutzt die logik, um herauszufinden, ob ein Zug gültig ist.
 */
class Muehle_Stein : public QGraphicsEllipseItem
{
public:

    /**
     * @brief ist_visible: Die Variable ist_visible gibt, an ob der Muehle_Stein gerade sichtbar ist, ist_visible ist auch dann wahr, wenn der Stein unter einem anderen liegt.
     */
    bool ist_visible = false;

    /**
     * @brief color: Die Farbe des Chips wird entweder Schwarz oder Weiß sein.
     */
    Chip color;

    /**
     * @brief position: Jeder Muehle_Stein besitzt eine Muehle_Position, die Muehle_Position wird während des Verschiebens nicht akkurat gesetzt, nur beim Loslassen des Spielsteins ändert sich der Wert entsprechend.
     */
    Muehle_Position position;

    /**
     * @brief Muehle_Stein: Der Konstruktor für Muehle_Stein setzt jeden Stein als greifbares Objekt. Größe und Farbe werden in der Klasse muehle gesetzt.
     */
    Muehle_Stein();

protected:

    /**
     * @brief mousePressEvent: Wird einmal aufgerufen, wenn die Maus gedrückt wird. Hier wird die start_moving Funktion des globalen muehle_felds aufgerufen.
     * @param event: Das event wird benutzt um die Cursorposition und Tasteneingabe der Maus zu verarbeiten.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief mouseReleaseEvent: Beim Loslassen des Muehle_Stein wird mit stop_moving versucht, den Spielstein in eine mögliche Muehle_Position zu setzen, wenn der Abstand es zulässt, wird diese mögliche Muehle_Position ggf. zur neuen Muehle_Position des Muehle_Stein.
     * Danach erfolgt die Abfrage des Muehle_State: Für den Muehle_State Place, Move und Remove werden entsprechend die logik-Funktionen place, move und remove mit den entsprechenden Parametern der Muehle_Position_Kennung und Chip color aufgerufen. Wenn die logik-Funktionen true zurückgeben,
     * handelt es sich um einen gültigen Zug und der Stein wird tatsächlich an der neuen Muehle_Position abgelegt, andernfalls würde er in die start_position zurückgesetzt. Beim Loslassen des Spielsteins wird auch der Infotext aus der logik an das QLabel übergeben.
     * @param event: Das event wird benutzt um die Cursorposition und Tasteneingabe der Maus zu verarbeiten.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    /**
     * @brief mouseMoveEvent: Dieser Code verschiebt den Spielstein akkurat zur Mausposition.
     * @param event: Das event wird benutzt um die Cursorposition und Tasteneingabe der Maus zu verarbeiten.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
private:

    /**
     * @brief drag_offset: Da das QGraphicsEllipseItem seine Position durch die linke obere Ecke erhällt, wird die Mausposition um den drag_offset verschoben, um die Wirkung zu haben, dass der Muehle_Stein beliebig gegriffen werden kann.
     */
    QPointF drag_offset;

};

#endif // GUI_H
