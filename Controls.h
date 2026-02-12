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

    void setFilmstripImage(const juce::Image& image, int totalFrames)
    {
        knobImage = image;
        numFrames = totalFrames;
        mode = Mode::Filmstrip;
        repaint();
    }

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
                0, numFrames - 1,
                (int)juce::jmap((float)getValue(),
                    (float)getMinimum(),
                    (float)getMaximum(),
                    0.0f,
                    (float)(numFrames - 1)));

            g.drawImage(knobImage,
                bounds.getX(), bounds.getY(),
                bounds.getWidth(), bounds.getHeight(),
                0, frameIndex * frameHeight,
                knobImage.getWidth(), frameHeight);
        }
        else
        {
            g.saveState();

            const float normValue = (float)juce::jmap(
                (double)getValue(), getMinimum(), getMaximum(), 0.0, 1.0);

            const float angle = juce::jmap(normValue, 0.0f, 1.0f,
                rotationStart, rotationEnd);

            const auto centre = bounds.getCentre();

            const float scale = juce::jmin(bounds.getWidth() / knobImage.getWidth(),
                bounds.getHeight() / knobImage.getHeight());

            juce::AffineTransform transform =
                juce::AffineTransform::translation(-knobImage.getWidth() * 0.5f,
                    -knobImage.getHeight() * 0.5f)
                .scaled(scale)
                .rotated(angle)
                .translated(centre.x, centre.y);

            g.drawImageTransformed(knobImage, transform);
            g.restoreState();
        }
    }

private:
    juce::Image knobImage;
    int   numFrames = 0;
    float rotationStart = 0.0f;
    float rotationEnd = 0.0f;
    Mode  mode = Mode::Filmstrip;
};

//==============================================================================
// AREA SLIDER
class AreaSlider
{
public:
    enum class Direction { Horizontal, Vertical, Both };

    void setDirection(Direction d) { direction = d; }
    void setHandleImage(const juce::Image& img) { handleImage = img; }
    void setValue(juce::Point<float> v)
    {
        value.x = juce::jlimit(0.0f, 1.0f, v.x);
        value.y = juce::jlimit(0.0f, 1.0f, v.y);
    }

    juce::Point<float> getValue() const { return value; }

    juce::Point<float> getCentre(juce::Rectangle<float> bounds) const
    {
        return {
            juce::jmap(value.x, 0.0f, 1.0f, bounds.getX(), bounds.getRight()),
            juce::jmap(value.y, 1.0f, 0.0f, bounds.getY(), bounds.getBottom())
        };
    }

    void paint(juce::Graphics& g, juce::Rectangle<float> bounds)
    {
        if (!handleImage.isValid())
            return;

        auto p = getCentre(bounds);
        g.drawImageAt(handleImage,
            int(p.x - handleImage.getWidth() * 0.5f),
            int(p.y - handleImage.getHeight() * 0.5f));
    }

private:
    Direction direction = Direction::Both;
    juce::Image handleImage;
    juce::Point<float> value{ 0.5f, 0.5f };
};



//==============================================================================
// MULTI AREA SLIDER (shared full area)
class MultiAreaSlider : public juce::Component
{
public:
    std::function<void(int, juce::Point<float>)> onChange;

    void setup(int numPoints,
        AreaSlider::Direction dir,
        const juce::Image& handleImg,
        const juce::Array<juce::Point<float>>& defaults)
    {
        jassert(defaults.size() == numPoints);

        sliders.clear();
        direction = dir;

        for (int i = 0; i < numPoints; ++i)
        {
            AreaSlider s;
            s.setDirection(dir);
            s.setHandleImage(handleImg);
            s.setValue(defaults[i]);
            sliders.add(s);
        }

        repaint();
    }

    juce::Point<float> getValue(int index) const
    {
        return sliders[index].getValue();
    }

    void setValue(int index, juce::Point<float> v)
    {
        sliders.getReference(index).setValue(v);
        repaint();
    }


    void paint(juce::Graphics& g) override
    {
        auto b = getLocalBounds().toFloat();
        for (auto& s : sliders)
            s.paint(g, b);
    }

    //================ MOUSE =================
    void mouseDown(const juce::MouseEvent& e) override
    {
        activeIndex = findClosest(e.position);
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (activeIndex < 0)
            return;

        auto b = getLocalBounds().toFloat();
        auto& s = sliders.getReference(activeIndex);

        juce::Point<float> v = s.getValue();

        if (direction != AreaSlider::Direction::Vertical)
            v.x = juce::jmap(e.position.x, b.getX(), b.getRight(), 0.0f, 1.0f);

        if (direction != AreaSlider::Direction::Horizontal)
            v.y = juce::jmap(e.position.y, b.getBottom(), b.getY(), 0.0f, 1.0f);

        s.setValue(v);
        pushNeighbors(activeIndex);

        repaint();

        if (onChange)
            onChange(activeIndex, s.getValue());
    }

private:
    juce::Array<AreaSlider> sliders;
    AreaSlider::Direction direction = AreaSlider::Direction::Both;
    int activeIndex = -1;

    //================ HIT TEST =================
    int findClosest(juce::Point<float> p)
    {
        auto b = getLocalBounds().toFloat();

        float best = std::numeric_limits<float>::max();
        int index = -1;

        for (int i = 0; i < sliders.size(); ++i)
        {
            auto c = sliders[i].getCentre(b);
            float d = c.getDistanceFrom(p);
            if (d < best)
            {
                best = d;
                index = i;
            }
        }
        return index;
    }

    //================ OVERLAP PUSH =================
    void pushNeighbors(int moved)
    {
        constexpr float minX = 0.02f;

        auto v = sliders[moved].getValue();

        for (int i = 0; i < sliders.size(); ++i)
        {
            if (i == moved)
                continue;

            auto o = sliders[i].getValue();

            if (std::abs(o.x - v.x) < minX)
            {
                o.x = (o.x < v.x) ? v.x - minX : v.x + minX;
                sliders[i].setValue(o);
            }
        }
    }
};
