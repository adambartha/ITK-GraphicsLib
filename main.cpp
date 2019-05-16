#include "ABUNO.hpp"

unsigned short wait_reset()
{
    return (100 + rand() % 400);
}

int main()
{
    init(1200, 800, "Segoe UI Bold Italic", 32);
    event ev;
    gout.set_title("UNO!");

    srand(time(0));

    vector<card*> deck;
    vector<card*> deck_discard;
    vector<player*> players;

    unsigned short master_index = game_index::menu, num_cpu = 3, num_start = 5, turn = 0, turn_player;

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
        load_deck(deck);
        turn_player = (rand() % (num_cpu + 1));
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
                p->draw_card(deck, (50 + (i * 225)), 450);
            }
        }
        discard(deck, deck_discard);
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
            display_text(center_x, 400, "<insert_help_here>", text_align::center);
        }
        else if(master_index == game_index::game)
        {
            set_font(font_type, 64);
            display_text(center_x, center_y - gout.cascent(), "Your turn: " + tostring(turn_player + 1) + "/" + tostring(players.size()), text_align::center);
            set_font(font_type, 32);
            display_text(center_x, center_y + gout.cascent(), "Press ANY KEY to begin!", text_align::center);
            gout << refresh;
            while(gin >> ev && ev.keycode == 0) {};
            unsigned short cpu_wait = wait_reset();
            while(gin >> ev && ev.keycode != key_escape)
            {
                trim(COLOR_TAN);
                run_events(ev);
                gout << move_to(30, 430) << COLOR_STEEL.to_gout() << box(1140, 340);
                players[turn]->show_hand();//players[turn_player]->show_hand();
                deck_discard.back()->show();
                set_font(font_type, 16);
                gout << move_to(25, 25) << COLOR_TAN.blend(COLOR_GREY).to_gout() << box(100, (players.size() * (gout.cascent() + 5)))
                << move_to(25, 25 + (turn * (gout.cascent() + 5))) << COLOR_RED.to_gout() << box(100, gout.cascent() + 5);
                unsigned short n = 1, to_y;
                for(size_t i = 0; i < players.size(); i++)
                {
                    to_y = (24 + (i * (gout.cascent() + 5)));
                    if(i == turn_player)
                    {
                        display_text(30, to_y, tostring(i + 1) + ". You");
                    }
                    else
                    {
                        display_text(30, to_y, tostring(i + 1) + ". CPU " + tostring(n));
                        n++;
                    }
                }
                set_font(font_type, 32);
                if(turn == turn_player)
                {
                    gin.timer(0);
                    display_text(center_x, center_y - gout.cascent(), "Your turn!", text_align::center);
                    if(ev.button == btn_right)
                    {
                        turn_next(turn, players.size());
                        cpu_wait = wait_reset();
                    }
                }
                else
                {
                    gin.timer(1);
                    if(turn > turn_player)
                    {
                        display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn) + "'s turn!", text_align::center);
                    }
                    else
                    {
                        display_text(center_x, center_y - gout.cascent(), "CPU " + tostring(turn + 1) + "'s turn!", text_align::center);
                    }
                    if(ev.type == ev_timer)
                    {
                        if(cpu_wait > 0)
                        {
                            cpu_wait--;
                        }
                        else
                        {
                            bool can_place = false;
                            for(auto *i:players[turn]->get_hand())
                            {
                                if(i->match(deck_discard.back()))
                                {
                                    can_place = true;
                                    break;
                                }
                            }
                            if(can_place)
                            {
                                cout << "OK" << endl;
                            }
                            else
                            {
                                cout << "NOPE" << endl;
                            }
                            turn_next(turn, players.size());
                            cpu_wait = wait_reset();
                        }
                    }
                }
                update();
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
