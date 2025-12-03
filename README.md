# Traffic Light Control System - Finite State Machine
<img width="1071" height="688" alt="image" src="https://github.com/user-attachments/assets/849cbbb4-6240-4aa2-8cd5-141e02ce52d2" />


## System Description

The Traffic Light Control FSM, depicted in the figure above, manages the operational modes and the timing configuration logic of the traffic light system. The system operation revolves around a cyclic transition between four primary modes, controlled by **Button 0**, while parameter adjustments are handled within specific states using **Button 2** and **Button 3**.

Its operation is defined as follows:

* **MODE 1: NORMAL**: The system operates in its default state, executing the standard traffic light sequence based on currently saved timings.
    * The system monitors input from **Button 0**. When `Button 0 = 1` is detected, the FSM transitions to **MODE 2**, initiating the configuration phase.

* **MODE 2: SET RED**: This state allows the user to configure the duration of the Red LED.
    * **Navigation:** If **Button 0** is pressed, the system saves the current configuration and transitions to **MODE 3** (SET AMBER).
    * **Adjustment:**
        * The state maintains a self-loop when `Button 2 = 1` is detected (typically serving as a value selector or toggle).
        * If `Button 3 = 1`, the system executes the **Modify red led time** action to update the duration variable, then returns to the current state.

* **MODE 3: SET AMBER**: This state manages the configuration for the Amber (Yellow) LED duration.
    * **Navigation:** Upon detecting `Button 0 = 1`, the FSM transitions to **MODE 4** (SET GREEN).
    * **Adjustment:**
        * Similar to the previous mode, `Button 2 = 1` triggers a self-loop.
        * Triggering `Button 3 = 1` initiates the **Modify amber led time** action to adjust the specific timing parameter for the Amber light.

* **MODE 4: SET GREEN**: This is the final configuration state, allowing adjustments to the Green LED duration.
    * **Navigation:** Pressing **Button 0** (`Button 0 = 1`) completes the configuration cycle and transitions the FSM back to **MODE 1: NORMAL**.
    * **Adjustment:**
        * The system remains in this state or performs selection logic if `Button 2 = 1`.
        * The **Modify green led time** action is executed when `Button 3 = 1` is detected, updating the Green LED timing before returning to the state wait loop.

## Video Demonstration
For a visual demonstration of the system in operation, please refer to the video linked below:
[**Watch Project Video (Google Drive)**](https://drive.google.com/drive/folders/1WiqQAwh8Ci2PWZe7i7Og9IEMaFVuzDOf?usp=sharing)
