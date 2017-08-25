#include "Enes100.h"

/* Create a new Enes100 object
 * Parameters:
 *  string teamName
 *  int teamType
 *  int markerId
 *  int rxPin
 *  int txPin
 */
Enes100 enes("Team Name Here", WATER, 3, 8, 9);

void setup() {
    // Retrieve the destination
    while (!enes.retrieveDestination()) {
        enes.println("Unable to retrieve location");
    }

    enes.print("My destination is at ");
    enes.print(enes.destination.x);
    enes.print(",");
    enes.println(enes.destination.y);
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

    // Transmit the water quality
    enes.baseObjective(POLLUTED);

    // Transmit the depth of the pool
    enes.bonusObjective(44.2);
}
