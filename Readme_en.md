# Arduino stepper potentiometer guide

  This sketch allows you to control the rotation direction of a 28BYJ-48 stepper motor using a potentiometer. 
  When the upper limit is exceeded, the motor rotates clockwise.
  When the lower limit is reached, the motor rotates counterclockwise.  
  In the middle position, the motor does not rotate.

# Operating Principle

The sketch continuously reads the potentiometer value (0-1023) and compares it with threshold values.

- **If value >= upper limit** → motor makes 8 microsteps (1 full step) **clockwise**
- **If value < upper limit AND value > lower limit** → motor does not rotate (middle position)
- **If value <= lower limit** → motor makes 8 microsteps **counter-clockwise**

Thus, by turning the potentiometer knob, you make the motor repeat the movement in the corresponding direction.

# Components

**Arduino**: Uno (any compatible board will work)

**Stepper motor**: 5V 28BYJ-48

**Stepper motor driver**: ULN2003

**Potentiometer**: B5K (or any other compatible)

# Connection Diagram

## Driver Connection
| Driver Input | Arduino Digital Pin |
|--------------|---------------------|
| IN1          | PIN_8               |
| IN2          | PIN_9               |
| IN3          | PIN_10              |
| IN4          | PIN_11              |

"+" Driver -> 5V Arduino
"-" Driver -> GND Arduino
*!When connecting higher voltage motors, external power must be supplied to the driver + and - terminals!*

The motor connects to the driver socket via connector.

## Potentiometer Connection

Left leg   -> 5V Arduino
Middle leg -> A0 Arduino Analog Pin (pin can be changed in the sketch)
Right leg  -> GND Arduino

*!It is recommended to use a breadboard for easier assembly!*

# Quick Start

1. Run the sketch
2. Calibrate in debug mode if necessary

# Project Variables and Constants

## Configuration Constants (DEFINE)

### `SENSOR_PIN`
- **Type:** `#define` (numeric constant)
- **Value:** `A0`
- **Description:** Defines the Arduino analog pin connected to the potentiometer middle leg.
- **Range:** A0-A5 (on Arduino Uno)

### `PIN_1`, `PIN_2`, `PIN_3`, `PIN_4`
- **Type:** `#define` (numeric constants)
- **Values:** `8`, `9`, `10`, `11`
- **Description:** Digital Arduino pins connected to the ULN2003 driver inputs (IN1, IN2, IN3, IN4 respectively).
- **Range:** 0-13 (digital pins)

### `DELAY_TIME`
- **Type:** `#define` (numeric constant)
- **Value:** `2`
- **Description:** Delay time between microsteps in milliseconds. Determines motor rotation speed.
- **Recommendations:**
  - `1-2 ms` - fast rotation (possible step skipping)
  - `3-5 ms` - optimal speed for 28BYJ-48

### `UPPER_LIMIT`
- **Type:** `#define` (numeric constant)
- **Value:** `600`
- **Description:** Upper limit of potentiometer values. When exceeded, motor rotates clockwise.
- **Range:** 0-1023
- **Recommendation:** set higher than the lower limit.

### `LOWER_LIMIT`
- **Type:** `#define` (numeric constant)
- **Value:** `500`
- **Description:** Lower limit of potentiometer values. When dropped below, motor rotates counter-clockwise.
- **Range:** 0-1023
- **Recommendation:** set lower than the upper limit.

## Global Variables

### `sensorValue`
- **Type:** `int` (integer)
- **Initial value:** `0`
- **Description:** Stores the current voltage value read from the analog pin `SENSOR_PIN` (potentiometer). Updated in each `loop()` cycle.
- **Range:** 0-1023
  - `0` - minimum voltage (0V)
  - `1023` - maximum voltage (5V)

## Tables and Arrays

### `seq[8][4]`
- **Type:** two-dimensional `int` array
- **Size:** 8 rows × 4 columns
- **Description:** State sequence table for half-step control mode of 28BYJ-48 stepper motor. Each row represents one microstep and contains values for four pins (PIN_1, PIN_2, PIN_3, PIN_4).
- **Table values:**
  - `1` - HIGH (on)
  - `0` - LOW (off)
- **Table rows (microsteps):**
  - Row 0: `{1, 0, 0, 0}` - only PIN_1 active
  - Row 1: `{1, 1, 0, 0}` - PIN_1 and PIN_2 active
  - Row 2: `{0, 1, 0, 0}` - only PIN_2 active
  - Row 3: `{0, 1, 1, 0}` - PIN_2 and PIN_3 active
  - Row 4: `{0, 0, 1, 0}` - only PIN_3 active
  - Row 5: `{0, 0, 1, 1}` - PIN_3 and PIN_4 active
  - Row 6: `{0, 0, 0, 1}` - only PIN_4 active
  - Row 7: `{1, 0, 0, 1}` - PIN_1 and PIN_4 active
- **Note:** 8 microsteps (full table) make 1 full motor step. A full rotation of the 28BYJ-48 motor requires 4096 microsteps (512 full steps).

## Debug Conditional Constants

### `DEBUG`
- **Type:** macro (`#define`)
- **Description:** Uncommenting the line `//#define DEBUG` enables debug mode, outputting `sensorValue` to the serial monitor.
- **Usage:**
  - `//#define DEBUG` - debug disabled (default mode)
  - `#define DEBUG` - debug enabled

# Possible Issues

| Problem | Solution |
|---------|----------|
| Motor doesn't rotate | Check connection of pins 8,9,10,11 and driver power supply |
| Motor skips steps | Increase `DELAY_TIME`, check power supply |
| Values in serial monitor don't change | Check potentiometer connection (middle pin must be on A0) |
| When connecting Arduino Tools -> port is inactive | Reconnect Arduino or try another cable |
| avrdude: stk500_recv(): programmer is not responding | Check COM port visibility, reconnect Arduino, restart IDE |