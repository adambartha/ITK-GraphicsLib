#include "ABUNO.hpp"

size_t wait_reset()
{
    return (300 + (rand() % 700));
}

int is_win(vector<player*> players)
{
    size_t index = 0;
    for(auto *i:players)
    {
        if(i->get_hand_num() == 0)
        {
            return index;
        }
        else
        {
            index++;
        }
    }
    return -1;
}

int main()
{
    init(1200, 800, "Segoe UI Bold Italic", 32);
    event ev;
    gout.set_title("UNO!");

    srand(time(0));

    vector<card*> pile_draw;
    vector<card*> pile_discard;
    vector<player*> players;

    size_t master_index = game_index::menu, num_cpu = 3, num_start = 5, turn, turn_player, to_draw , color_request;
    bool dir_rev = false;

    button* button_start = new button(100, center_y - 130, 300, 80, 5, "Start");
    button* button_settings = new button(100, center_y - 40, 300, 80, 5, "Settings");
    button* button_help = new button(100, center_y + 50, 300, 80, 5, "Help");
    button* button_return = new button(center_x - 150, res_y - 130, 300, 80, 5, "Return");

    numeric* numeric_players = new numeric(center_x + 50, 300, 100, 50, 2, 3, interval(1, 7));
    numeric* numeric_starter = new numeric(center_x + 50, 360, 100, 50, 2, 5, interval(5, 9), 2);

    numeric_players->set_colors
    (
        COLOR_WHITE,
        COLOR_BLACK,
        COLOR_GOLD,
        COLOR_BRIGHTGREEN
    );
    numeric_players->set_state(0);
    numeric_starter->set_colors
    (
        COLOR_WHITE,
        COLOR_BLACK,
        COLOR_GOLD,
        COLOR_OCEAN
    );
    numeric_starter->set_state(0);

    button_return->set_task([&]()
    {
        button_start->set_state(2);
        button_settings->set_state(2);
        button_help->set_state(2);
        button_return->set_state(0);
        numeric_players->set_state(0);
        numeric_starter->set_state(0);
        num_cpu = numeric_players->get_value();
        num_start = numeric_starter->get_value();
        master_index = game_index::menu;
    });
    button_return->set_colors
    (
        COLOR_RUBY,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_RUBY,
        COLOR_GOLD
    );
    button_return->set_state(0);

    button_start->set_task([&]()
    {
        button_start->set_state(0);
        button_settings->set_state(0);
        button_help->set_state(0);
        pile_discard.clear();
        players.clear();
        load_deck(pile_draw);
        turn = 0;
        turn_player = (rand() % (num_cpu + 1));
        to_draw = 0;
        color_request = 0;
        for(size_t i = 0; i <= num_cpu; i++)
        {
            if(i == turn_player)
            {
                players.push_back(new player(false));
            }
            else
            {
                players.push_back(new player());
            }
        }
        for(size_t i = 0; i < num_start; i++)
        {
            for(auto *p:players)
            {
                if(p->is_cpu())
                {
                    p->draw_card(pile_draw);
                }
                else
                {
                    p->draw_card(pile_draw, (50 + (i * 225)), 450);
                }
            }
        }
        while(pile_draw.back()->get_type() == card_type::takefour)
        {
            shuffle_deck(pile_draw);
        }
        pile_discard.push_back(pile_draw.back());
        pile_draw.pop_back();
        pile_discard.back()->place(500, 50);
        if(pile_discard.back()->get_type() == card_type::skip)
        {
            turn = 1;
        }
        else if(pile_discard.back()->get_type() == card_type::taketwo)
        {
            to_draw = 2;
        }
        else if(pile_discard.back()->get_type() == card_type::reverse)
        {
            turn = (players.size() - 1);
            dir_rev = true;
        }
        else if(pile_discard.back()->get_type() == card_type::pickcolor)
        {
            color_request = (1 + (rand() % 4));
        }
        master_index = game_index::game;
    });
    button_settings->set_task([&]()
    {
        button_start->set_state(0);
        button_settings->set_state(0);
        button_help->set_state(0);
        button_return->set_state(2);
        numeric_players->set_state(2);
        numeric_starter->set_state(2);
        master_index = game_index::settings;
    });
    button_help->set_task([&]()
    {
        button_start->set_state(0);
        button_settings->set_state(0);
        button_help->set_state(0);
        button_return->set_state(2);
        master_index = game_index::help;
    });

    button_start->set_colors
    (
        COLOR_RUBY,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_RUBY,
        COLOR_GOLD
    );
    button_settings->set_colors
    (
        COLOR_BRIGHTGREEN,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_BRIGHTGREEN,
        COLOR_GOLD
    );
    button_help->set_colors
    (
        COLOR_OCEAN,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_OCEAN,
        COLOR_GOLD
    );

    button* button_color_red = new button(center_x - 210, center_y, 200, 50, 2, "Red");
    button* button_color_green = new button(center_x + 10, center_y, 200, 50, 2, "Green");
    button* button_color_blue = new button(center_x - 210, center_y + 60, 200, 50, 2, "Blue");
    button* button_color_yellow = new button(center_x + 10, center_y + 60, 200, 50, 2, "Yellow");

    button_color_red->set_colors
    (
        COLOR_RUBY,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_RUBY,
        COLOR_GOLD
    );
    button_color_red->set_state(0);

    button_color_green->set_colors
    (
        COLOR_BRIGHTGREEN,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_BRIGHTGREEN,
        COLOR_GOLD
    );
    button_color_green->set_state(0);

    button_color_blue->set_colors
    (
        COLOR_OCEAN,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_OCEAN,
        COLOR_GOLD
    );
    button_color_blue->set_state(0);

    button_color_yellow->set_colors
    (
        COLOR_YELLOW,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_YELLOW,
        COLOR_GOLD
    );
    button_color_yellow->set_state(0);

    button* button_uno = new button(center_x - 100, center_y - 25, 200, 50, 2, "UNO!");

    button_uno->set_colors
    (
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_WHITE,
        COLOR_GOLD,
        COLOR_GOLD
    );
    button_uno->set_state(0);

    while(gin >> ev && ev.keycode != key_escape)
    {
        trim(COLOR_TAN);
        run_events(ev);
        if(master_index == game_index::menu)
        {
            set_font(font_type, 224);
            gout << move_to(150, 0) << COLOR_GOLD.blend(COLOR_TAN).to_gout() << box(200, res_y)
            << move_to(center_x - 98, center_y - ((2 * gout.cascent()) / 3) + 2) << COLOR_TAN.blend(COLOR_BLACK).to_gout() << text("UNO!")
            << move_to(center_x - 100, center_y - ((2 * gout.cascent()) / 3)) << COLOR_WHITE.to_gout() << text("UNO!");
            set_font(font_type, 32);
        }
        else if(master_index == game_index::settings)
        {
            display_text(center_x + 30, 300, "CPU Players:", text_align::right);
            display_text(center_x + 30, 360, "Starter Cards:", text_align::right);
        }
        else if(master_index == game_index::help)
        {
            display_text(center_x, center_y - gout.cascent() - 5, "RIGHT CLICK: Draw card(s)", text_align::center);
            display_text(center_x, center_y + 5, "LEFT/RIGHT ARROWS: Scroll between cards", text_align::center);
        }
        else if(master_index == game_index::game)
        {
            set_font(font_type, 64);
            display_text(center_x, center_y - gout.cascent(), "Your turn: " + tostring(turn_player + 1) + "/" + tostring(players.size()), text_align::center);
            set_font(font_type, 32);
            display_text(center_x, center_y + gout.cascent(), "Press ANY KEY to begin!", text_align::center);
            gout << refresh;
            while(gin >> ev && ev.keycode == 0) {};
            size_t cpu_wait = wait_reset(), turn_total = 1;
            player* local_player = players[turn_player];
            while(gin >> ev && ev.keycode != key_escape && is_win(players) == -1)
            {
                trim(COLOR_TAN);
                run_events(ev);
                gout << move_to(25, 425) << COLOR_STEEL.to_gout() << box(1150, 350);
                for(size_t i = local_player->get_hand_num(); i < 5; i++)
                {
                    gout << move_to(50 + (i * 225), 450) << COLOR_STEEL.blend(COLOR_BLACK).to_gout() << box(200, 300);
                }
                if(local_player->get_hand_num() > 5)
                {
                    if(ev.keycode == key_left)
                    {
                        local_player->scroll(-1);
                    }
                    else if(ev.keycode == key_right)
                    {
                        local_player->scroll(1);
                    }
                    double offset = local_player->get_scroll_index(), wscroll = (5500 / (double)local_player->get_hand_num());
                    gout << move_to(50, 760) << COLOR_LIME.blend(COLOR_STEEL).to_gout() << box(1100, 5)
                    << move_to(50 + (offset * (wscroll / 5)), 760) << COLOR_LIME.to_gout() << box(ceil(wscroll), 5);
                }
                local_player->show_hand(pile_discard.back(), color_request);
                if(pile_draw.size() == 0)
                {
                    reshuffle(pile_draw, pile_discard);
                }
                pile_discard.back()->show();
                set_font(font_type, 12);
                display_text(center_x + 110, center_y - 50 - gout.cascent(), tostring(pile_discard.size()) + "/" + tostring(pile_draw.size()));
                set_font(font_type, 16);
                gout << move_to(50, 50) << COLOR_TAN.blend(COLOR_GREY).to_gout() << box(100, (players.size() * (gout.cascent() + 5))) << move_to(50, 50 + (turn * (gout.cascent() + 5)));
                if(turn == turn_player)
                {
                    gout << COLOR_OCEAN.to_gout();
                }
                else
                {
                    gout << COLOR_RED.to_gout();
                }
                gout << box(100, gout.cascent() + 5);
                size_t n = 1, to_y;
                for(size_t i = 0; i < players.size(); i++)
                {
                    to_y = (49 + (i * (gout.cascent() + 5)));
                    if(i == turn_player)
                    {
                        display_text(55, to_y, tostring(i + 1) + ". You");
                    }
                    else
                    {
                        if(num_cpu > 1)
                        {
                            display_text(55, to_y, tostring(i + 1) + ". CPU " + tostring(n));
                            n++;
                        }
                        else
                        {
                            display_text(55, to_y, tostring(i + 1) + ". CPU");
                        }
                    }
                    size_t hand_num = players[i]->get_hand_num();
                    for(size_t j = 0; j < hand_num; j++)
                    {
                        gout << move_to(155 + (j * 15), to_y + 2) << COLOR_BLACK.to_gout() << box(12, gout.cascent() + 1);
                    }
                    gout << move_to(156 + (hand_num * 15), to_y - 1) << text(tostring(hand_num));
                }
                if(num_cpu > 1)
                {
                    to_y += (gout.cascent() + 15);
                    gout << COLOR_BLUE.to_gout();
                    if(dir_rev)
                    {
                        for(size_t i = 1; i <= 20; i++)
                        {
                            gout << move_to(99 - i, to_y + i) << line(i * 2, 1);
                        }
                    }
                    else
                    {
                        for(size_t i = 1; i <= 20; i++)
                        {
                            gout << move_to(79 + i, to_y + i) << line((21 - i) * 2, 1);
                        }
                    }
                }
                string turn_no = ("Turn No. " + tostring(turn_total));
                gout << move_to(res_x - 60 - gout.twidth(turn_no), 50) << COLOR_TAN.blend(COLOR_GREY).to_gout() << box(gout.twidth(turn_no) + 10, gout.cascent() + 10);
                display_text(res_x - 55, 52, turn_no, text_align::right);
                set_font(font_type, 32);
                bool is_skip = false;
                if(turn == turn_player)
                {
                    gin.timer(0);
                    if(to_draw > 0 && color_request > 0)
                    {
                        display_text(center_x, center_y - gout.cascent(), "Your turn - Draw " + tostring(to_draw) + " cards and play " + color_to_text(color_request) + "!", text_align::center);
                        if(ev.button == btn_right)
                        {
                            if(to_draw > pile_draw.size())
                            {
                                reshuffle(pile_draw, pile_discard);
                            }
                            while(to_draw > 0)
                            {
                                local_player->draw_card(pile_draw, (50 + ((local_player->get_hand_num() - 1) * 225)), 450);
                                to_draw--;
                            }
                        }
                    }
                    else if(to_draw > 0 || color_request > 0)
                    {
                        if(to_draw > 0)
                        {
                            display_text(center_x, center_y - gout.cascent(), "Your turn - Draw " + tostring(to_draw) + " cards!", text_align::center);
                            if(ev.button == btn_right)
                            {
                                if(to_draw > pile_draw.size())
                                {
                                    reshuffle(pile_draw, pile_discard);
                                }
                                while(to_draw > 0)
                                {
                                    local_player->draw_card(pile_draw, (50 + ((local_player->get_hand_num() - 1) * 225)), 450);
                                    to_draw--;
                                }
                                turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                cpu_wait = wait_reset();
                            }
                        }
                        else if(color_request > 0)
                        {
                            display_text(center_x, center_y - gout.cascent(), "Your turn - Play " + color_to_text(color_request) + "!", text_align::center);
                            if(ev.button == btn_left)
                            {
                                size_t index = 0;
                                for(auto *i:local_player->get_hand())
                                {
                                    if(i->in_range() && i->get_color() == color_request)
                                    {
                                        local_player->play_card(pile_discard, index);
                                        set_modifiers(pile_discard, dir_rev, is_skip, color_request, to_draw);
                                        turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                        cpu_wait = wait_reset();
                                        color_request = 0;
                                    }
                                    else
                                    {
                                        index++;
                                    }
                                }
                            }
                            else if(ev.button == btn_right)
                            {
                                local_player->draw_card(pile_draw, (50 + ((local_player->get_hand_num() - 1) * 225)), 450);
                                turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                cpu_wait = wait_reset();
                            }
                        }
                    }
                    else
                    {
                        display_text(center_x, center_y - gout.cascent(), "Your turn!", text_align::center);
                        if(ev.button == btn_left)
                        {
                            size_t index = 0;
                            for(auto *i:local_player->get_hand())
                            {
                                if(i->in_range() && i->match(pile_discard.back()))
                                {
                                    local_player->play_card(pile_discard, index);
                                    bool uno_draw = false;
                                    if(local_player->get_hand_num() == 1)
                                    {
                                        bool uno = false;
                                        size_t uno_timer = 100;
                                        button_uno->set_state(2);
                                        gin.timer(1);
                                        while(gin >> ev && !uno && uno_timer > 0)
                                        {
                                            trim(COLOR_TAN);
                                            run_events(ev);
                                            display_text(center_x, center_y - 100, "Quick! Press UNO!", text_align::center);
                                            gout << move_to(98, 598) << COLOR_DARKGREY.to_gout() << box(1004, 14)
                                            << move_to(100, 600) << COLOR_LIME.blend(COLOR_BLACK).to_gout() << box(1000, 10)
                                            << move_to(100, 600) << COLOR_LIME.to_gout() << box((uno_timer * 10), 10);
                                            if(ev.type == ev_timer && uno_timer > 0)
                                            {
                                                uno_timer--;
                                            }
                                            if(ev.button == btn_left && button_uno->in_range())
                                            {
                                                uno = true;
                                            }
                                            update();
                                        }
                                        gin.timer(0);
                                        button_uno->set_state(0);
                                        if(!uno)
                                        {
                                            uno_draw = true;
                                        }
                                    }
                                    if(uno_draw)
                                    {
                                        to_draw = 2;
                                    }
                                    else
                                    {
                                        if(pile_discard.back()->get_type() == card_type::takefour || pile_discard.back()->get_type() == card_type::pickcolor)
                                        {
                                            button_color_red->set_state(2);
                                            button_color_green->set_state(2);
                                            button_color_blue->set_state(2);
                                            button_color_yellow->set_state(2);
                                            gin.timer(1);
                                            while(gin >> ev && color_request == 0)
                                            {
                                                trim(COLOR_TAN);
                                                run_events(ev);
                                                display_text(center_x, center_y - 100, "Choose a color!", text_align::center);
                                                if(ev.button == btn_left)
                                                {
                                                    if(button_color_red->in_range())
                                                    {
                                                        color_request = card_color::red;
                                                    }
                                                    else if(button_color_green->in_range())
                                                    {
                                                        color_request = card_color::green;
                                                    }
                                                    else if(button_color_blue->in_range())
                                                    {
                                                        color_request = card_color::blue;
                                                    }
                                                    else if(button_color_yellow->in_range())
                                                    {
                                                        color_request = card_color::yellow;
                                                    }
                                                }
                                                update();
                                            }
                                            gin.timer(0);
                                            button_color_red->set_state(0);
                                            button_color_green->set_state(0);
                                            button_color_blue->set_state(0);
                                            button_color_yellow->set_state(0);
                                        }
                                        set_modifiers(pile_discard, dir_rev, is_skip, color_request, to_draw);
                                        turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                        cpu_wait = wait_reset();
                                    }
                                }
                                else
                                {
                                    index++;
                                }
                            }
                        }
                        else if(ev.button == btn_right)
                        {
                            local_player->draw_card(pile_draw, (50 + ((local_player->get_hand_num() - 1) * 225)), 450);
                            turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                            cpu_wait = wait_reset();
                        }
                    }
                }
                else
                {
                    gin.timer(1);
                    if(to_draw > 0 && color_request > 0)
                    {
                        if(num_cpu > 1)
                        {
                            if(turn > turn_player)
                            {
                                display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn) + "'s turn - CPU " + tostring(turn) + " has to draw " + tostring(to_draw) + " cards and play " + color_to_text(color_request) + "!", text_align::center);
                            }
                            else
                            {
                                display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn + 1) + "'s turn - CPU " + tostring(turn + 1) + " has to draw " + tostring(to_draw) + " cards and play " + color_to_text(color_request) + "!", text_align::center);
                            }
                        }
                        else
                        {
                            display_text(center_x, center_y - gout.cascent(), "CPU's turn - CPU has to draw " + tostring(to_draw) + " cards and play " + color_to_text(color_request) + "!", text_align::center);
                        }
                    }
                    else if(to_draw > 0 || color_request > 0)
                    {
                        if(to_draw > 0)
                        {
                            if(num_cpu > 1)
                            {
                                if(turn > turn_player)
                                {
                                    display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn) + "'s turn - CPU " + tostring(turn) + " has to draw " + tostring(to_draw) + " cards!", text_align::center);
                                }
                                else
                                {
                                    display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn + 1) + "'s turn - CPU " + tostring(turn + 1) + " has to draw " + tostring(to_draw) + " cards!", text_align::center);
                                }
                            }
                            else
                            {
                                display_text(center_x, center_y - gout.cascent(), "CPU's turn - CPU has to draw " + tostring(to_draw) + " cards!", text_align::center);
                            }
                        }
                        else if(color_request > 0)
                        {
                            if(num_cpu > 1)
                            {
                                if(turn > turn_player)
                                {
                                    display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn) + "'s turn - CPU " + tostring(turn) + " has to play " + color_to_text(color_request) + "!", text_align::center);
                                }
                                else
                                {
                                    display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn + 1) + "'s turn - CPU " + tostring(turn + 1) + " has to play " + color_to_text(color_request) + "!", text_align::center);
                                }
                            }
                            else
                            {
                                display_text(center_x, center_y - gout.cascent(), "CPU's turn - CPU has to play " + color_to_text(color_request) + "!", text_align::center);
                            }
                        }
                    }
                    else
                    {
                        if(num_cpu > 1)
                        {
                            if(turn > turn_player)
                            {
                                display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn) + "'s turn!", text_align::center);
                            }
                            else
                            {
                                display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn + 1) + "'s turn!", text_align::center);
                            }
                        }
                        else
                        {
                            display_text(center_x, center_y - gout.cascent(), "CPU's turn!", text_align::center);
                        }
                    }
                    if(ev.type == ev_timer)
                    {
                        if(cpu_wait > 0)
                        {
                            cpu_wait--;
                        }
                        else
                        {
                            if(to_draw > 0 && color_request > 0)
                            {
                                if(to_draw > pile_draw.size())
                                {
                                    reshuffle(pile_draw, pile_discard);
                                }
                                while(to_draw > 0)
                                {
                                    players[turn]->draw_card(pile_draw);
                                    to_draw--;
                                }
                                vector<size_t> cpu_hand;
                                size_t index = 0;
                                for(auto *i:players[turn]->get_hand())
                                {
                                    if(i->get_color() == color_request)
                                    {
                                        cpu_hand.push_back(index);
                                    }
                                    index++;
                                }
                                if(cpu_hand.size() > 0)
                                {
                                    players[turn]->play_card(pile_discard, cpu_hand[rand() % cpu_hand.size()]);
                                    set_modifiers(pile_discard, dir_rev, is_skip, color_request, to_draw);
                                    color_request = 0;
                                }
                                else
                                {
                                    players[turn]->draw_card(pile_draw);
                                }
                                turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                cpu_wait = wait_reset();
                            }
                            else if(to_draw > 0 || color_request > 0)
                            {
                                if(to_draw > 0)
                                {
                                    if(to_draw > pile_draw.size())
                                    {
                                        reshuffle(pile_draw, pile_discard);
                                    }
                                    while(to_draw > 0)
                                    {
                                        players[turn]->draw_card(pile_draw);
                                        to_draw--;
                                    }
                                    turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                    cpu_wait = wait_reset();
                                }
                                else if(color_request > 0)
                                {
                                    vector<size_t> cpu_hand;
                                    size_t index = 0;
                                    for(auto *i:players[turn]->get_hand())
                                    {
                                        if(i->get_color() == color_request)
                                        {
                                            cpu_hand.push_back(index);
                                        }
                                        index++;
                                    }
                                    if(cpu_hand.size() > 0)
                                    {
                                        players[turn]->play_card(pile_discard, cpu_hand[rand() % cpu_hand.size()]);
                                        set_modifiers(pile_discard, dir_rev, is_skip, color_request, to_draw);
                                        color_request = 0;
                                    }
                                    else
                                    {
                                        players[turn]->draw_card(pile_draw);
                                    }
                                    turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                    cpu_wait = wait_reset();
                                }
                            }
                            else
                            {
                                vector<size_t> cpu_hand;
                                size_t index = 0;
                                for(auto *i:players[turn]->get_hand())
                                {
                                    if(i->match(pile_discard.back()))
                                    {
                                        cpu_hand.push_back(index);
                                    }
                                    index++;
                                }
                                if(cpu_hand.size() > 0)
                                {
                                    players[turn]->play_card(pile_discard, cpu_hand[rand() % cpu_hand.size()]);
                                    set_modifiers(pile_discard, dir_rev, is_skip, color_request, to_draw);
                                }
                                else
                                {
                                    players[turn]->draw_card(pile_draw);
                                }
                                turn_next(turn, players.size(), dir_rev, is_skip, turn_total);
                                cpu_wait = wait_reset();
                            }
                        }
                    }
                }
                update();
            }
            int win_index = is_win(players);
            if(win_index != -1)
            {
                set_font(font_type, 48);
                if(win_index == (int)turn_player)
                {
                    trim(COLOR_GOLD);
                    display_text(center_x, 150, "CONGRATULATIONS - YOU WIN!", text_align::center);
                }
                else
                {
                    trim(COLOR_TAN);
                    if(num_cpu > 1)
                    {
                        if(win_index > (int)turn_player)
                        {
                            display_text(center_x, 150, "WINNER: CPU " + tostring(win_index) + "!", text_align::center);
                        }
                        else
                        {
                            display_text(center_x, 150, "WINNER: CPU " + tostring(win_index + 1) + "!", text_align::center);
                        }
                    }
                    else
                    {
                        display_text(center_x, 150, "WINNER: CPU!", text_align::center);
                    }
                }
                pile_discard.back()->place(500, 300);
                pile_discard.back()->show();
                set_font(font_type, 32);
                display_text(center_x, 200, "Total turns played: " + tostring(turn_total), text_align::center);
                set_font(font_type, 16);
                display_text(center_x, 700, "Press ESC to return to menu.", text_align::center);
                gout << refresh;
                while(gin >> ev && ev.keycode != key_escape) {};
            }
            if(ev.keycode == key_escape)
            {
                button_start->set_state(2);
                button_settings->set_state(2);
                button_help->set_state(2);
                master_index = game_index::menu;
            }
        }
        update();
    }
    return 0;
}
