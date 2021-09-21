## Zapper Mod ATtiny 85

Use an ATTiny 85 microcontroller to fix the rapid trigger issue when using a NES Zapper on a Playchoice 10.

See [ATtiny85 Playchoice Zapper Mod blog post](https://nes4life.wordpress.com/2021/09/18/attiny85-playchoice-zapper-mod/) for full details:

  - Program an ATTiny 85 using this Arduino sketch
  - Remove the main transistor in the Zapper itself
  - Bypass the transistor by connecting points B & C with a jumper wire
  - Remove the debounce capacitor (connects to the Red and Black wires coming from the trigger microswitch)
  - Connect ATtiny 85
    - pin 4 to Ground
    - pin 5 to Trigger Signal
    - pin 8 to Vcc (5v)

### Playchoice PCB Jumpers

  - Jumper SL7 (Hit NOT bypass) on the Playchoice PCB cab be used if you do not want to remove and bypass the main transistor in the Zapper PCB itself
  - Jumper SL6 (Trigger NOT bypass) on the Playchoice PCB will not work for Zappers or original PC-10 Lightguns
