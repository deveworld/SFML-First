#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace sf;
using namespace std;

int WINDOW_X = 1500;
int WINDOW_Y = 1000;

void GameThread(RenderWindow *windowRef)
{
    windowRef->setVerticalSyncEnabled(true);

    Font font;
    font.loadFromFile("../resources/tnr.ttf");

    Text text("SFML-FIRST", font, 100);
    text.setFillColor(Color::White);
    text.setStyle(Text::Regular);

    FloatRect textRect(text.getLocalBounds());
    text.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);
    text.setPosition(WINDOW_X/2, WINDOW_Y/2);

    RenderTexture renderTexture;
    renderTexture.create(WINDOW_X, WINDOW_Y, ContextSettings(0, 0, 8));

    Sprite sprite;
    sprite.setTexture(renderTexture.getTexture());

    Clock clock;
    bool boom = false;

	while (windowRef->isOpen())
	{
        if (clock.getElapsedTime().asMilliseconds() >= 1)
        {
            int size(text.getCharacterSize());
            if (boom)
            {
                size += 3;
                if (size >= 150)
                {
                    boom = !boom;
                }
            }
            else {
                size -= 1;
                if (size <= 30)
                {
                    boom = !boom;
                }
            }
            text.setCharacterSize(size);
            clock.restart();
        }

        FloatRect textRect(text.getLocalBounds());
        text.setOrigin(textRect.left + textRect.width/2, textRect.top + textRect.height/2);

        renderTexture.create(WINDOW_X, WINDOW_Y, ContextSettings(0, 0, 8));
        renderTexture.setSmooth(true);
        renderTexture.clear(Color::Black);
        renderTexture.draw(text);
        renderTexture.display();
        
        windowRef->clear(Color::Black);
        windowRef->draw(sprite);
        windowRef->display();
	}
}

int main()
{
    RenderWindow window;
    Thread gameThread(&GameThread, &window);
    Event event;

    ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    window.create(VideoMode(WINDOW_X, WINDOW_Y), "SFML First", Style::Default, settings);
    window.setActive(false);

    gameThread.launch();

    while (window.waitEvent(event))
    {
        
        if (event.type == Event::Closed)
        {
            window.close();
        }
        if (event.type == Event::Resized)
        {
            window.setView(View(window.getView().getCenter(), Vector2f((float)event.size.width, (float)event.size.height)));
        }
    }

    gameThread.wait();

    return 0;
}