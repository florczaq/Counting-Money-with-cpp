#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#define w_w 900
#define w_h w_w

class Background : public sf::Drawable
{
public:
	sf::Image img;
	sf::Texture tr;
	sf::Sprite sprite;

	Background()
	{
		img.loadFromFile("bck.png");
		tr.loadFromImage(img);
		sprite.setTexture(tr);
		sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
		sprite.setPosition(w_w/2, w_h/2);
		sprite.scale(0.65, 0.65);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite);
	}

};

class Border : public sf::Drawable
{
public:
	sf::RectangleShape rectangle;

	int height = w_h, width = w_w / 8;

	Border()
	{
		rectangle.setSize(sf::Vector2f(width, height));
		rectangle.setPosition(w_w / 2, w_h / 2);
		rectangle.setOrigin(width / 2, height / 2);
		rectangle.setFillColor(sf::Color(2, 71, 93));
	}

	void set_rotation(int num)
	{
		if (num % 2 == 0)
			rectangle.rotate(90);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rectangle);
	}
};

class Borders : public sf::Drawable
{
public:
	Border border[4];

	Borders()
	{
		for (int i = 0; i < 4; i++)
		{
			border[i].set_rotation(i + 1);

			switch (i)
			{
			case 0:
				border[i].rectangle.setPosition(border->width / 2, w_h / 2);
				break;

			case 1:
				border[i].rectangle.setPosition(w_w / 2, border->width / 2);
				break;

			case 2:
				border[i].rectangle.setPosition(w_w - border->width / 2, w_h / 2);
				break;

			case 3:
				border[i].rectangle.setPosition(w_w / 2, w_h - border->width / 2);
				break;
			}

		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (int i = 0; i < 4; i++)
			target.draw(border[i]);
	}






};

class Cell : public sf::Drawable
{
public:

	Border border;

	sf::RectangleShape rectangle;
	sf::Font font;
	sf::Text text;

	int width = (w_w - 2 * border.width) / 2, height = (w_h - 2 * border.width) / 14;
	float posx = border.width + width / 2, posy = border.width + height / 2;

	Cell()
	{

		rectangle.setSize(sf::Vector2f(width, height));
		rectangle.setOrigin(width / 2, height / 2);
		rectangle.setPosition(w_w / 2, w_h / 2);
		rectangle.setFillColor(sf::Color(100, 100, 100));
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(2);


	}

	void set_pos(int change, int y_padding)
	{
		rectangle.setPosition(posx + change, posy + y_padding);
		font.loadFromFile("bn.ttf");
		text.setFont(font);
		text.setString("#NOTHING");
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setFillColor(sf::Color::White);
		text.setOutlineThickness(1.7);
		text.setPosition(rectangle.getPosition());
	}

	void set_string(int id, int& id2, std::string tab[12])
	{
		if (id % 2 == 0)
		{
			switch (id)
			{
			case 0:
				text.setString("Value");
				text.setCharacterSize(35);
				break;

			case 2: text.setString("0.10"); break;
			case 4: text.setString("0.20"); break;
			case 6: text.setString("0.50"); break;
			case 8: text.setString("1"); break;
			case 10: text.setString("2"); break;
			case 12: text.setString("5"); break;
			case 14: text.setString("10"); break;
			case 16: text.setString("20"); break;
			case 18: text.setString("50"); break;
			case 20: text.setString("100"); break;
			case 22: text.setString("200"); break;
			case 24: text.setString("500"); break;
			}
		}
		else
		{
			if (id == 1)
			{
				text.setString("Amount");
				text.setCharacterSize(35);
			}
			else
			{
				text.setString(tab[id2]);
				id2++;

			}
		}
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 1.2);

	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rectangle);
		target.draw(text);
	}


};

class Cells : public sf::Drawable
{
public:
	Border border;
	Cell cell[26];
	int y_padding = 0, id_a = 0, num_tab[12];
	std::string amount[12];

	Cells()
	{
		read();
		for (int i = 0; i < 26; i++)
		{
			if (i > 1 && i % 2 == 0)
				y_padding += cell->height;

			i % 2 == 0 ? cell[i].set_pos(0, y_padding) : cell[i].set_pos(cell->width, y_padding);

			cell[i].set_string(i, id_a, amount);
		}
	}

	void set_strings()
	{
		id_a = 0;
		read();
		for (int i = 1; i < 26; i += 2)
			cell[i].set_string(i, id_a, amount);
	}

	void read()
	{
		bool exist = false;
		std::fstream plik;
		std::string line;
		plik.open("Data.txt", std::ios::in);

		if (!plik.good())
		{
			exist = false;
		}
		else
		{
			exist = true;
			for (int i = 0; i < 12; i++)
			{
				getline(plik, line);
				amount[i] = line;
				num_tab[i] = atoi(amount[i].c_str());
			}
		}
		plik.close();

		if (!exist)
		{
			plik.open("Data.txt", std::ios::out);

			for (int i = 0; i < 12; i++)
				plik << "0" << std::endl;
			plik.close();

		}
	}

	void increase(int id)
	{
		num_tab[id]++;
	}

	void decrease (int id)
	{
		if(num_tab[id]>0)
			num_tab[id]--;
	}

	void save()
	{
		std::fstream plik;
		plik.open("Data.txt", std::ios::out);

		for (int i = 0; i < 12; i++)
			plik << num_tab[i] << std::endl;

		plik.close();
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (int i = 0; i < 26; i++)
			target.draw(cell[i]);
	}
};

class SumCell : public sf::Drawable
{
public:

	Border border;
	Cell cell;
	Cells cells;

	float result = 0;

	std::string str= std::to_string((int)result) + "." + std::to_string((int)((result - (int)result) * 100 + 0.5f));

	sf::Text text;
	sf::Font font;

	SumCell()
	{
		cell.rectangle.setSize(sf::Vector2f(cell.width * 2, cell.height));
		cell.rectangle.setOrigin(cell.rectangle.getLocalBounds().width / 2, cell.rectangle.getLocalBounds().height / 2);
		cell.rectangle.setPosition(w_w / 1.99, w_h - (border.width + cell.height / 2));

		font.loadFromFile("bn.ttf");
		text.setFont(font);
		text.setFillColor(sf::Color::White);
		text.setString("0.0");
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
		text.setPosition(cell.rectangle.getPosition());
		text.setOutlineThickness(2);
	}
	void set_string()
	{
		str = std::to_string((int)result) + "." + std::to_string((int)((result - (int)result) * 100 + 0.5f));
		text.setString("Together:   " + str );
		text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 1.5);
	}

	void calculations(int tab[12])
	{
		result = 0;

		for (int i = 0; i < 12; i++)
		{
			switch (i)
			{
			case 0: result += 0.10 * tab[i]; break;
			case 1: result += 0.20 * tab[i]; break;
			case 2: result += 0.50 * tab[i]; break;

			case 3: result += 1 * tab[i]; break;
			case 4: result += 2 * tab[i]; break;
			case 5: result += 5 * tab[i]; break;
			case 6: result += 10 * tab[i]; break;
			case 7: result += 20 * tab[i]; break;
			case 8: result += 50 * tab[i]; break;
			case 9: result += 100 * tab[i]; break;
			case 10: result += 200 * tab[i]; break;
			case 11: result += 500 * tab[i]; break;
			}

		}
	}


	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(cell);
		target.draw(text);
	}

};

class Button : public sf::Drawable
{
public:
	sf::RectangleShape rectangle;

	int wall = 30;

	sf::Text text;
	sf::Font font;

	Button()
	{
		rectangle.setSize(sf::Vector2f(wall, wall));
		rectangle.setOrigin(wall / 2, wall / 2);
		rectangle.setOutlineColor(sf::Color::Black);
		rectangle.setOutlineThickness(2);
		rectangle.setPosition(w_w / 2, w_h / 2);

		font.loadFromFile("arialb.ttf");
		text.setFont(font);
		text.setPosition(rectangle.getPosition());
	}

	void set_string_plus()
	{
		text.setString("+");
		text.setOrigin(text.getLocalBounds().width / 1.7, text.getLocalBounds().height / 0.9);
		text.setFillColor(sf::Color::Black);
	}

	void set_string_minus()
	{
		text.setString("-");
		text.setOrigin(text.getLocalBounds().width / 1.7, text.getLocalBounds().height / 0.25);
		text.setFillColor(sf::Color::Black);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(rectangle);
		target.draw(text);
	}
};

class Increase : public sf::Drawable
{
public:
	Button button[12];
	Cells cell;

	float pos_x = 0, pos_y = 0;

	Increase()
	{
		for (int i = 0, j = 3; i < 12; i++, j += 2)
		{
			pos_x = cell.cell[j].rectangle.getPosition().x * 1.32;
			pos_y = cell.cell[j].rectangle.getPosition().y;

			button[i].set_string_plus();
			button[i].rectangle.setPosition(pos_x, pos_y);
			button[i].text.setPosition(pos_x, pos_y);
		}
	}		

	void if_touch(sf::FloatRect rect)
	{
		for (int i = 0; i < 12; i++)
		{
			if (rect.intersects(button[i].rectangle.getGlobalBounds()));
			{
				cell.increase(i);
				std::cout << cell.num_tab[i] << std::endl;

			}
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (int i = 0; i < 12; i++)
			target.draw(button[i]);
	}
};

class Decrease : public sf::Drawable
{
public:
	Button button[12];
	Increase inc;
	Cells cell;
	int padding = w_h / 20;

	Decrease()
	{
		for (int i = 0; i < 12; i++)
		{
			button[i].set_string_minus();
			button[i].rectangle.setPosition(inc.button[i].rectangle.getPosition().x + padding, inc.button[i].rectangle.getPosition().y);
			button[i].text.setPosition(inc.button[i].rectangle.getPosition().x + padding, inc.button[i].rectangle.getPosition().y);
		}
	}

	void if_touch(sf::FloatRect rect)
	{
		for (int i = 0; i < 12; i++)
		{
			if (rect.intersects(button[i].rectangle.getGlobalBounds()));
			{
				cell.num_tab[i]--;
				std::cout << cell.num_tab[i] << std::endl;
			}
		}
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (int i = 0; i < 12; i++)
			target.draw(button[i]);
	}
};

class Circle_mouse : public sf::Drawable
{
public:

	sf::CircleShape circle;

	float r = 0.1;

	Circle_mouse()
	{
		circle.setRadius(r);
		circle.setOrigin(r, r);
	}

	void set_pos(sf::Vector2f pos)
	{
		circle.setPosition(pos);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(circle);
	}

};

int main()
{
	sf::RenderWindow window(sf::VideoMode(w_w, w_h), "MCM" , sf::Style::Close);

	window.setFramerateLimit(30);
	
	Borders border;
	Cells cell;
	SumCell scell;
	Increase plus;
	Decrease minus;
	Circle_mouse mouse_c;
	Background back;

	sf::FloatRect mp;

	bool if_changed = false;;

	while (window.isOpen())
	{
		mp = mouse_c.circle.getGlobalBounds();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (int i = 0, j = 3; i < 12; i++, j += 2)
				{
					if (plus.button[i].rectangle.getGlobalBounds().intersects(mp))
						cell.increase(i);
					else if (minus.button[i].rectangle.getGlobalBounds().intersects(mp))
						cell.decrease(i);
				}
				cell.save();
				
			}
		}

		mouse_c.set_pos(sf::Vector2f(sf::Mouse::getPosition(window)));

		cell.set_strings();

		scell.calculations(cell.num_tab);
		scell.set_string();

		window.clear(sf::Color::White);
		window.draw(border);
		window.draw(back);
		window.draw(cell);
		window.draw(scell);
		window.draw(plus);
		window.draw(minus);
		window.draw(mouse_c);
		window.setTitle("Total value:  " + scell.str);
		window.display();
	}

	return 0;
}
