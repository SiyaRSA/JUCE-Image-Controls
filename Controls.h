#pragma once
#include <JuceHeader.h>

//==============================================================================
// IMAGE ROTARY KNOB (filmstrip OR rotating image)
class ImageKnob : public juce::Slider
{
public:
    enum class Mode
    {
        Filmstrip,
        Rotating
    };

    ImageKnob()
    {
        setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    }

    //==============================================================================
    // FILMSTRIP MODE
    void setFilmstripImage(const juce::Image& image, int totalFrames)
    {
        knobImage = image;
        numFrames = totalFrames;
        mode = Mode::Filmstrip;
        repaint();
    }

    //==============================================================================
    // ROTATING IMAGE MODE
    void setRotatingImage(const juce::Image& image,
        float startAngleRadians = -juce::MathConstants<float>::pi * 0.75f,
        float endAngleRadians = juce::MathConstants<float>::pi * 0.75f)
    {
        knobImage = image;
        rotationStart = startAngleRadians;
        rotationEnd = endAngleRadians;
        mode = Mode::Rotating;
        repaint();
    }

    //==============================================================================
    void paint(juce::Graphics& g) override
    {
        if (!knobImage.isValid())
            return;

        const auto bounds = getLocalBounds().toFloat();

        if (mode == Mode::Filmstrip)
        {
            if (numFrames <= 0)
                return;

            const int frameHeight = knobImage.getHeight() / numFrames;

            const int frameIndex = juce::jlimit(
                0,
                numFrames - 1,
                (int)juce::jmap(
                    (float)getValue(),
                    (float)getMinimum(),
                    (float)getMaximum(),
                    0.0f,
                    (float)(numFrames - 1)));

            g.drawImage(
                knobImage,
                bounds.getX(),
                bounds.getY(),
                bounds.getWidth(),
                bounds.getHeight(),
                0,
                frameIndex * frameHeight,
                knobImage.getWidth(),
                frameHeight);
        }
        else // Rotating image
        {
            g.saveState();
            g.setOpacity(1.0f);
            
            const float normValue = (float)juce::jmap(
                (double)getValue(),
                getMinimum(),
                getMaximum(),
                0.0,
                1.0);

            const float angle = juce::jmap(
                normValue,
                0.0f,
                1.0f,
                rotationStart,
                rotationEnd);

            const auto bounds = getLocalBounds().toFloat();
            const auto centre = bounds.getCentre();

            // Scale image to fit bounds
            const float scale = juce::jmin(
                bounds.getWidth() / knobImage.getWidth(),
                bounds.getHeight() / knobImage.getHeight());

            juce::AffineTransform transform =
                juce::AffineTransform::translation(
                    -knobImage.getWidth() * 0.5f,
                    -knobImage.getHeight() * 0.5f)
                .scaled(scale)
                .rotated(angle)
                .translated(centre.x, centre.y);

            g.drawImageTransformed(knobImage, transform, false);
            g.restoreState();
        }

    }

private:
    juce::Image knobImage;

    // Filmstrip
    int numFrames = 0;

    // Rotating
    float rotationStart = 0.0f;
    float rotationEnd = 0.0f;

    Mode mode = Mode::Filmstrip;
};

//==============================================================================
// IMAGE BUTTON
class ImageButton : public juce::Button
{
public:
    ImageButton(const juce::String& name)
        : juce::Button(name) {}

    void setImages(const juce::Image& normal,
        const juce::Image& hover,
        const juce::Image& down)
    {
        normalImage = normal;
        hoverImage = hover;
        downImage = down;
    }

    void paintButton(juce::Graphics& g,
        bool isMouseOverButton,
        bool isButtonDown) override
    {
        const juce::Image& img =
            isButtonDown ? downImage :
            isMouseOverButton ? hoverImage :
            normalImage;

        if (!img.isValid())
            return;

        const auto bounds = getLocalBounds();

        g.drawImage(
            img,
            bounds.getX(),
            bounds.getY(),
            bounds.getWidth(),
            bounds.getHeight(),
            0,
            0,
            img.getWidth(),
            img.getHeight()
        );
    }

private:
    juce::Image normalImage, hoverImage, downImage;
};