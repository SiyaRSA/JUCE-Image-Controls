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
```
---

## Usage

```cpp
// Create knob
auto knob = std::make_unique<ImageKnob>();

// Filmstrip mode
juce::Image filmstrip = juce::ImageFileFormat::loadFrom(BinaryData::knob_png, BinaryData::knob_pngSize);
knob->setFilmstripImage(filmstrip, 32); // 32 frames

// OR Rotating mode
juce::Image singleImage = juce::ImageFileFormat::loadFrom(BinaryData::rotatingKnob_png, BinaryData::rotatingKnob_pngSize);
knob->setRotatingImage(singleImage,
                       -juce::MathConstants<float>::pi * 0.75f,
                       juce::MathConstants<float>::pi * 0.75f);
```
---

```cpp
auto button = std::make_unique<ImageButton>("MyButton");

// Load images for states
juce::Image normal = juce::ImageFileFormat::loadFrom(BinaryData::buttonNormal_png, BinaryData::buttonNormal_pngSize);
juce::Image hover  = juce::ImageFileFormat::loadFrom(BinaryData::buttonHover_png, BinaryData::buttonHover_pngSize);
juce::Image down   = juce::ImageFileFormat::loadFrom(BinaryData::buttonDown_png, BinaryData::buttonDown_pngSize);

// Set images
button->setImages(normal, hover, down);
```
---

## License

This project is licensed under **GPLv3**, compatible with the JUCE GPL license.

**Note:** This project uses JUCE, which is available under the GPL or a commercial license. By using these controls, your project must comply with JUCE’s GPL licensing if you do not have a commercial JUCE license.  

You can freely use and modify this code in open-source JUCE projects. For commercial closed-source projects, a commercial JUCE license is required.

---
