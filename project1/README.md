# Smart Parking Lot Monitoring System

An Arduino monitoring system for a small parking area. It watch four parking spaces and show whether each space is occupied or available. Push buttons simulate vehicle arrival and departure, and LEDs light up when a space is taken. Status print to the Serial Monitor on boot and only when a real button click change the lot.

## Hardware

- `4 buttons` on pins `2, 4, 6, 8` (one per space, pull-down)
- `4 LEDs` on pins `3, 5, 7, 9` (on = occupied, off = available)
- Debounce delay of `50ms` so noisy clicks do not flip the state twice

## How it works

1. On boot all spaces start `available`, LEDs off, and a status dump print to Serial
2. Each button click (LOW >> HIGH) toggle that space: empty > occupied, or occupied > empty
3. Occupied spaces turn their LED on; freeing a space turn it off
4. Serial status reprint only when the lot actually change
5. Extra clicks are denied if the lot is already full or empty in a bad state

## How to run

Quick and easy way is to tinker it using Tinkercard via [this link](https://www.tinkercad.com/things/aT6Cx3EPYnH-smartparkinglot?sharecode=EI5gP_dK_82Zpx9ltZII1BAtFqRRTELIyIPilZzfiLE)

Or
1. Wire the buttons and LEDs to the pins above
2. Copy and paste codes in `park_lot.cpp` it into Tinkercad's code section and start simulation
4. Press a button to park or leave a space and watch the LED and Serial output

---
<div align="center">
<p> Build with ❤️</p>
</div>
