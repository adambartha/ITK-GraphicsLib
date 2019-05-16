#ifndef ABUNO_HPP
#define ABUNO_HPP

#include "ABGFX.hpp"
#include <algorithm>

enum card_type
{
    normal, skip, taketwo, reverse, takefour, pickcolor
};

enum card_color
{
    black, red, green, blue, yellow
};

enum game_index
{
    menu, settings, help, game
};

enum text_align
{
    left, right, center
};

class card
{
    size_t x, y, color, type, number;
    color3 color_out;
public:
    card(size_t set_type, size_t set_color = card_color::black, size_t set_number = 0)
    {
        x = 0;
        y = 0;
        color = set_color;
        if(set_color == card_color::black)
        {
            color_out = COLOR_BLACK;
        }
        else if(set_color == card_color::red)
        {
            color_out = COLOR_RUBY;
        }
        else if(set_color == card_color::green)
        {
            color_out = COLOR_BRIGHTGREEN;
        }
        else if(set_color == card_color::blue)
        {
            color_out = COLOR_OCEAN;
        }
        else if(set_color == card_color::yellow)
        {
            color_out = COLOR_YELLOW;
        }
        type = set_type;
        number = set_number;
    }
    void show()
    {
        if(x == 0 && y == 0)
        {
            return;
        }
        set_font(font_type, 96);
        if(in_range())
        {
            gout << move_to(x - 5, y - 5) << COLOR_GREEN.to_gout() << box(210, 310);
        }
        gout << move_to(x, y) << COLOR_BLACK.to_gout() << box(200, 300)
        << move_to(x + 1, y + 1) << COLOR_WHITE.to_gout() << box(198, 298)
        << move_to(x + 10, y + 10) << color_out.to_gout() << box(180, 280)
        << move_to(x + 50, y + 75) << COLOR_WHITE.to_gout() << box(100, 150);
        for(size_t i = 1; i <= 25; i++)
        {
            gout << move_to(x + 10, y + 9 + i) << line(i, -i)
            << move_to(x + 189, y + 290 - i) << line(-i, i);
        }
        if(color == card_color::black)
        {
            if(type == card_type::pickcolor)
            {
                gout << move_to(x + 52, y + 77) << COLOR_BRIGHTGREEN.to_gout() << box(48, 73)
                << move_to(x + 100, y + 77) << COLOR_YELLOW.to_gout() << box(48, 73)
                << move_to(x + 52, y + 150) << COLOR_OCEAN.to_gout() << box(48, 73)
                << move_to(x + 100, y + 150) << COLOR_RUBY.to_gout() << box(48, 73);
            }
            else if(type == card_type::takefour)
            {
                gout << move_to(x + 65, y + 80) << COLOR_BLACK.to_gout() << box(50, 75)
                << move_to(x + 66, y + 81) << COLOR_WHITE.to_gout() << box(48, 73)
                << move_to(x + 68, y + 83) << COLOR_OCEAN.to_gout() << box(44, 69)
                << move_to(x + 95, y + 100) << COLOR_BLACK.to_gout() << box(50, 75)
                << move_to(x + 96, y + 101) << COLOR_WHITE.to_gout() << box(48, 73)
                << move_to(x + 98, y + 103) << COLOR_BRIGHTGREEN.to_gout() << box(44, 69)
                << move_to(x + 85, y + 145) << COLOR_BLACK.to_gout() << box(50, 75)
                << move_to(x + 86, y + 146) << COLOR_WHITE.to_gout() << box(48, 73)
                << move_to(x + 88, y + 148) << COLOR_YELLOW.to_gout() << box(44, 69)
                << move_to(x + 55, y + 125) << COLOR_BLACK.to_gout() << box(50, 75)
                << move_to(x + 56, y + 126) << COLOR_WHITE.to_gout() << box(48, 73)
                << move_to(x + 58, y + 128) << COLOR_RUBY.to_gout() << box(44, 69)
                << move_to(x + 97 - (gout.twidth(tostring("+4")) / 2), y + 137 - (gout.cascent() / 2))
                << COLOR_DARKGREY.to_gout() << text(tostring("+4"))
                << move_to(x + 95 - (gout.twidth(tostring("+4")) / 2), y + 135 - (gout.cascent() / 2))
                << COLOR_WHITE.to_gout() << text(tostring("+4"));
            }
        }
        else
        {
            if(type == card_type::normal)
            {
                gout << move_to(x + 102 - (gout.twidth(tostring(number)) / 2), y + 137 - (gout.cascent() / 2))
                << COLOR_DARKGREY.to_gout() << text(tostring(number))
                << move_to(x + 100 - (gout.twidth(tostring(number)) / 2), y + 135 - (gout.cascent() / 2))
                << color_out.to_gout() << text(tostring(number));
            }
            else if(type == card_type::taketwo)
            {
                gout << move_to(x + 95, y + 100) << COLOR_BLACK.to_gout() << box(50, 75)
                << move_to(x + 96, y + 101) << COLOR_WHITE.to_gout() << box(48, 73)
                << move_to(x + 98, y + 103) << color_out.to_gout() << box(44, 69)
                << move_to(x + 55, y + 125) << COLOR_BLACK.to_gout() << box(50, 75)
                << move_to(x + 56, y + 126) << COLOR_WHITE.to_gout() << box(48, 73)
                << move_to(x + 58, y + 128) << color_out.to_gout() << box(44, 69)
                << move_to(x + 97 - (gout.twidth(tostring("+2")) / 2), y + 137 - (gout.cascent() / 2))
                << COLOR_DARKGREY.to_gout() << text(tostring("+2"))
                << move_to(x + 95 - (gout.twidth(tostring("+2")) / 2), y + 135 - (gout.cascent() / 2))
                << COLOR_WHITE.to_gout() << text(tostring("+2"));
            }
            else if(type == card_type::skip)
            {
                gout << COLOR_WHITE.to_gout();
                for(int i = -65; i < 65; i++)
                {
                    for(int j = -65; j < 65; j++)
                    {
                        if((pow(i, 2) + pow(j, 2)) < pow(65, 2))
                        {
                            gout << move_to(x + 100 + i, y + 150 + j) << dot;
                        }
                    }
                }
                gout << color_out.to_gout();
                for(int i = -60; i < 60; i++)
                {
                    for(int j = -60; j < 60; j++)
                    {
                        if((pow(i, 2) + pow(j, 2)) > pow(40, 2) && (pow(i, 2) + pow(j, 2)) < pow(60, 2))
                        {
                            gout << move_to(x + 100 + i, y + 150 + j) << dot;
                        }
                    }
                }
                for(int i = 1; i < 28; i++)
                {
                    gout << move_to(x + 50 + i, y + 185) << line(70, -70);
                }
            }
            else if(type == card_type::reverse)
            {
                gout << move_to(x + 55, y + 110) << color_out.to_gout() << box(60, 30)
                << move_to(x + 85, y + 160) << box(60, 30);
                for(size_t i = 1; i <= 30; i++)
                {
                    gout << move_to(x + 114 + i, y + 94 + i) << line(1, ((31 - i) * 2))
                    << move_to(x + 54 + i, y + 175 - i) << line(1, (i * 2));
                }
            }
        }
    }
    void place(size_t set_x, size_t set_y)
    {
        x = set_x;
        y = set_y;
    }
    bool in_range()
    {
        return (evx >= x && evx < (x + 200) && evy >= y && evy < (y + 300));
    }
    bool match(card* other)
    {
        return (color == other->get_color() || number == other->get_color());
    }
    size_t get_color()
    {
        return color;
    }
    size_t get_type()
    {
        return type;
    }
    size_t get_number()
    {
        return number;
    }
};

class player
{
    vector<card*> hand;
    bool cpu, out;
public:
    player(bool set_cpu = true)
    {
        cpu = set_cpu;
        out = false;
    }
    void draw_card(vector<card*> &deck, size_t to_x = 0, size_t to_y = 0)
    {
        hand.push_back(deck.back());
        hand.back()->place(to_x, to_y);
        deck.pop_back();
    }
    void place_card(vector<card*> &game_deck, size_t index)
    {
        game_deck.push_back(hand[index]);
        hand.erase(hand.begin() + index);
    }
    void show_hand()
    {
        for(auto *i:hand)
        {
            i->show();
        }
    }
    bool is_cpu()
    {
        return cpu;
    }
    vector<card*>& get_hand()
    {
        return hand;
    }
};

void shuffle_deck(vector<card*> &deck)
{
    random_shuffle(deck.begin(), deck.end());
}

void load_deck(vector<card*> &deck)
{
    deck.clear();
    for(size_t i = 1; i <= 9; i++) // NUMBERS 1-9 x2 (72)
    {
        for(size_t j = 0; j < 2; j++)
        {
            for(size_t k = 1; k <= 4; k++)
            {
                deck.push_back(new card(card_type::normal, k, i));
            }
        }
    }
    for(size_t i = 1; i <= 4; i++) // ZEROS (4)
    {
        deck.push_back(new card(card_type::normal, i));
    }
    for(size_t i = 1; i <= 3; i++) // ACTION x2 (24)
    {
        for(size_t j = 0; j < 2; j++)
        {
            for(size_t k = 1; k <= 4; k++)
            {
                deck.push_back(new card(i, k));
            }
        }
    }
    for(size_t i = 4; i <= 5; i++) // WILD x4 (8)
    {
        for(size_t j = 0; j < 4; j++)
        {
            deck.push_back(new card(i));
        }
    }
    shuffle_deck(deck);
}

void discard(vector<card*> &deck_from, vector<card*> &deck_to)
{
    deck_to.push_back(deck_from.back());
    deck_from.pop_back();
    deck_to.back()->place(500, 50);
}

void display_text(size_t to_x, size_t to_y, const string set_text, size_t set_align = text_align::left)
{
    if(set_align == text_align::right)
    {
        to_x -= gout.twidth(set_text);
    }
    else if(set_align == text_align::center)
    {
        to_x -= (gout.twidth(set_text) / 2);
    }
    gout << move_to(to_x + 2, to_y + 2) << COLOR_TAN.blend(COLOR_BLACK).to_gout() << text(set_text)
    << move_to(to_x, to_y) << COLOR_WHITE.to_gout() << text(set_text);
}

void turn_next(unsigned short &turn, const unsigned short num_players, bool reverse = false)
{
    if(reverse)
    {
        if(turn == 0)
        {
            turn = (num_players - 1);
        }
        else
        {
            turn--;
        }
    }
    else
    {
        if(turn == (num_players - 1))
        {
            turn = 0;
        }
        else
        {
            turn++;
        }
    }
}

#endif // ABUNO_HPP
