# Setting up the development board
<sub> NOTE: These instructions are for connecting to the BMS but can be adapted for other use cases </sub>

<details>
    <summary>Completed Wiring Example</summary>
    <img src="./images/Wiring_Example.jpg" alt-text="The Wiring" width="60%"></img>
</details>

## Preparing the STM32 Dev board and CAN Transceiver
##### The dev board will be powered by the USB port for testing purposes

<details>
    <summary>CAN Transceiver Reference Image</summary>
    <img src="./images/CAN_Transceiver.jpg" alt-text="The CAN Transciever" width="30%"></img>
</details>

1. Connect the CAN transceiver to the stm32
    
    * The TX pin on the transveiver goes to the PD_1 pin of the Nucleo
    * The RX pin on the transceiver goes to the PD_0 pin of the Nucleo
    <details>
        <summary>Nucleo Pinout Reference Image</summary>
        <img src="./images/stm32_pinout.jpg" alt-text="The Nucleo pinout" width="70%"></img>
    </details>


2. Connect the CAN transciever to Power using the power board

    * Ensure that the yellow jumper are set to output 5v on the board
    <details>
        <summary>Power Board Reference Image</summary>
        <img src="./images/Power_Board.jpg" alt-text="Arduino power breakout board">
    </details>


## Preparing the BMS

1. Connect the Solid Red and Solid Green wires from the BMS Wire Harness to the Red wire of the voltage supply 
2. Connect the Solid White Wire from the BMS to the Black wire of the voltage supply
    <details>
        <summary>Image of the connection </summary>

    * The wires from the BMS Wire Harness should be stripped already to allow for easy connections to be made

        <img src="./images/Powering_BMS.jpg" alt-text="Powering the BMS" width="40%"></img>
    </details>
    <details>
        <summary>Power Supply Example Settings</summary>

    * The voltage supply should be set around 12 Volts and 0.1-0.25 Amps
        <img src="./images/Power_Supply.jpg" alt-text="set Voltage supply" width="40%">

    </details>

3. Ensure that the Wire Harness is connected to thie BMS
    
    * After Successful connection with the voltage supply properly set a red light should light up on the BMS
        <details>
            <summary>BMS Red light running</summary>
            <img src="./images/BMS_Light.jpg" alt-text="Arduino power breakout board">
        </details>

## Connecting the BMS to the CAN Transceiver
1. Locate the Serial Port that is soldered to the BMS Wiring Harness
    
    * This is the CAN connector

2. Use a wire to connect the CANH Pin on the transceiver to the hole that the Red wire on the back of the CAN connector is soldered to

3. Use another wire to connect the CANL Pin on the transceiver to the hole that the Black wire on the back of the CAN connector is soldered to