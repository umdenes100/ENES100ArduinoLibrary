#include "Enes100.h"

/* Create a new Enes100 object
 * Parameters:
 *  string teamName
 *  int teamType
 *  int markerId
 *  int rxPin
 *  int txPin
 */
Enes100 enes("Team Name Here", BLACK_BOX, 3, 8, 9);

void setup() {
}

void loop() {
    // Update the OSV's current location
    if (enes.updateLocation()) {
        enes.println("Huzzah! Location updated!");
        enes.print("My x coordinate is ");
        enes.println(enes.location.x);
        enes.print("My y coordinate is ");
        enes.println(enes.location.y);
        enes.print("My theta is ");
        enes.println(enes.location.theta);
    } else {
        enes.println("Sad trombone... I couldn't update my location");
    }

    enes.navigated();

    // Transmit the coordinate of the black box
    Coordinate blackBox(2.9, 1.4);
    enes.baseObjective(blackBox);
}
