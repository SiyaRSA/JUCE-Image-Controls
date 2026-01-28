# JUCE Image Controls

*Skinnable image-based knobs and buttons for JUCE.*

**Tagline:**  
Create visually rich JUCE interfaces with customizable image knobs and buttons.

---

## Description

`JUCE Image Controls` is a lightweight collection of customizable UI components for JUCE. It provides:

- **ImageKnob** – a rotary knob with two modes:
  - **Filmstrip mode:** Uses a multi-frame image to animate the knob’s position.
  - **Rotating mode:** Rotates a single image between configurable angles.
- **ImageButton** – a button with customizable images for **normal**, **hover**, and **pressed** states.

These controls are perfect for audio plugins, apps, or any JUCE project where a polished, visually rich interface is needed.

---

## Features

- Filmstrip and rotating image knobs  
- Skinnable buttons with multiple states  
- Lightweight, header-only classes  
- Easy integration into existing JUCE projects  
- Fully customizable appearance  

---

## Installation

1. Clone or download this repository.  
2. Add the `Source/` folder to your JUCE project.  
3. Include the headers in your code:

```cpp
#include "ImageKnob.h"
#include "ImageButton.h"
