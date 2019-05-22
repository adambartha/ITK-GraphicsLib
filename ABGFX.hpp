#ifndef ABGFX_HPP
#define ABGFX_HPP

#include "graphics.hpp"
#include "ABUTL.hpp"
#include "GFXCLR.hpp"
#include <cstdlib>
#include <ctime>
#include <functional>
#include <fstream>

/** ABGFX v10.0 **/

using namespace genv;

static size_t res_x, res_y, center_x, center_y, evx, evy, font_size;
static bool shift_pressed = false, btn_left_pressed = false;
static string font_type;

class control;
class shape;

void set_font(string set_font_type = "", size_t set_font_size = 16)
{
    if(font_type == set_font_type && font_size == set_font_size)
    {
        return;
    }
    if(set_font_type != "")
    {
        font_type = set_font_type;
        font_size = set_font_size;
        set_font_type += ".ttf";
        if(!gout.load_font(set_font_type, set_font_size))
        {
            cerr << "Error loading font [" << set_font_type << "]" << endl;
        }
    }
}

inline void init(size_t set_res_x, size_t set_res_y, string set_font_type = "", size_t set_font_size = 16)
{
    res_x = set_res_x;
    res_y = set_res_y;
    center_x = (res_x / 2);
    center_y = (res_y / 2);
    set_font(set_font_type, set_font_size);
    gout.open(res_x, res_y);
}

class color3
{
    unsigned char r, g, b;
public:
    color3(unsigned char set_r = (rand() % 256), unsigned char set_g = (rand() % 256), unsigned char set_b = (rand() % 256))
    {
        r = set_r;
        g = set_g;
        b = set_b;
    }
    color to_gout()
    {
        return color(r, g, b);
    }
    color3 operator / (const double& d)
    {
        return color3(r / d, g / d, b / d);
    }
    color3 blend(const color3 with)
    {
        return color3((r + with.r) / 2, (g + with.g) / 2, (b + with.b) / 2);
    }
    color3 desaturate()
    {
        unsigned char gs = sqrt((pow(r, 2) + pow(g, 2) + pow(b, 2)) / 3);
        return color3(gs, gs, gs);
    }
};

void trim(color3 set_color = COLOR_BLACK)
{
    gout << move_to(0, 0) << set_color.to_gout() << box(res_x, res_y);
}

vector<control*> controls;

class control
{
protected:
    string type;
    size_t x, y, sx, sy, wstroke, state;
    control()
    {
        state = 2;
        controls.push_back(this);
    }
public:
    bool in_range()
    {
        return (evx >= x && evx < (x + sx) && evy >= y && evy < (y + sy));
    }
    point2 get_pos()
    {
        return point2(x, y);
    }
    point2 get_size()
    {
        return point2(sx, sy);
    }
    virtual void draw() {};
    void translate(int by_x, int by_y)
    {
        x += by_x;
        y += by_y;
    }
    void move(size_t to_x, size_t to_y)
    {
        x = to_x;
        y = to_y;
    }
    bool get_type(const string check_type)
    {
        return (type == check_type);
    }
    void set_state(const size_t mod_state)
    {
        state = mod_state;
    }
    size_t get_state()
    {
        return state;
    }
};

class button: public control
{
    string label;
    color3 cfill, ctext, cstroke, cfill_active, ctext_active, cstroke_active;
    function<void()> task;
    bool task_enabled;
public:
    button(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1, const string set_label = "Button")
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        wstroke = set_wstroke;
        label = set_label;
        task = nullptr;
        task_enabled = true;
        type = "Button";
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_ctext = COLOR_BLACK, color3 set_cstroke = COLOR_NAVY, color3 set_cfill_active = COLOR_DEEPBLUE, color3 set_ctext_active = COLOR_WHITE, color3 set_cstroke_active = COLOR_YELLOW)
    {
        cfill = set_cfill;
        ctext = set_ctext;
        cstroke = set_cstroke;
        cfill_active = set_cfill_active;
        ctext_active = set_ctext_active;
        cstroke_active = set_cstroke_active;
    }
    void draw()
    {
        int offset_x = (x + (((int)sx - gout.twidth(label)) / 2));
        int offset_y = (y + ((int)sy / 2) - ((2 * gout.cascent()) / 3));
        if(state == 1)
        {
            gout << move_to(x, y) << cstroke.blend(COLOR_GREY).to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + wstroke, y + wstroke) << (cfill.blend(COLOR_WHITE)).blend(COLOR_GREY).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
            << move_to(x + wstroke + 1, y + sy - wstroke - 1) << (cfill / 2).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + sx - wstroke - 1, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
            << move_to(offset_x + 1, offset_y + 1) << text(label)
            << move_to(offset_x, offset_y) << ctext.blend(COLOR_GREY).to_gout() << text(label);
        }
        else
        {
            if(in_range())
            {
                gout << move_to(x, y) << cstroke_active.to_gout() << box(sx, sy)
                << move_to(x + wstroke, y + wstroke) << cfill_active.to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
                << move_to(x + wstroke, y + wstroke) << cfill_active.blend(COLOR_WHITE).to_gout() << line(sx - (wstroke * 2) - 1, 0)
                << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
                << move_to(x + wstroke + 1, y + sy - wstroke - 1) << (cfill_active / 2).to_gout() << line(sx - (wstroke * 2) - 1, 0)
                << move_to(x + sx - wstroke - 1, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
                << move_to(offset_x + 1, offset_y + 1) << text(label)
                << move_to(offset_x, offset_y) << ctext_active.to_gout() << text(label);
            }
            else
            {
                gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
                << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
                << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_WHITE).to_gout() << line(sx - (wstroke * 2) - 1, 0)
                << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
                << move_to(x + wstroke + 1, y + sy - wstroke - 1) << (cfill / 2).to_gout() << line(sx - (wstroke * 2) - 1, 0)
                << move_to(x + sx - wstroke - 1, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
                << move_to(offset_x + 1, offset_y + 1) << text(label)
                << move_to(offset_x, offset_y) << ctext.to_gout() << text(label);
            }
        }
    }
    void set_task(function<void()> new_task)
    {
        task = new_task;
    }
    void run_task()
    {
        if(task_enabled)
        {
            task();
        }
    }
    void enable_task(bool set_enabled = true)
    {
        task_enabled = set_enabled;
    }
    bool has_task()
    {
        return (task != nullptr);
    }
};

class container: public control
{
protected:
    vector<string> items;
    size_t selected, scroll_index, items_max;
    color3 cfill, ctext, cstroke, ctext_active;
    void auto_limit()
    {
        selected = limit(selected, 0, count() - 1);
        scroll_index = limit(scroll_index, 0, count() - items_max);
    }
public:
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_ctext = COLOR_BLACK, color3 set_cstroke = COLOR_NAVY, color3 set_ctext_active = COLOR_DEEPBLUE)
    {
        cfill = set_cfill;
        ctext = set_ctext;
        cstroke = set_cstroke;
        ctext_active = set_ctext_active;
    }
    void select(const size_t index)
    {
        selected = index;
    }
    void add(const string item)
    {
        items.push_back(item);
    }
    void add(vector<string> add_items)
    {
        for(auto &i:add_items)
        {
            items.push_back(i);
        }
    }
    void remove(const string item)
    {
        for(size_t i = 0; i < count(); i++)
        {
            if(items[i] == item)
            {
                items.erase(items.begin() + i);
                auto_limit();
                break;
            }
        }
    }
    void remove(const size_t index)
    {
        if(index < count())
        {
            items.erase(items.begin() + index);
            auto_limit();
        }
    }
    void remove_selected()
    {
        if(count() > 0)
        {
            items.erase(items.begin() + selected);
            auto_limit();
        }
    }
    virtual void clear() {};
    void modify(const string modify_to)
    {
        if(modify_to.length() > 0)
        {
            items[selected] = modify_to;
        }
    }
    void scroll(int set_scroll, size_t jump_to = 0)
    {
        if(set_scroll == 0)
        {
            scroll_index = (jump_to * (count() - items_max));
        }
        else if(set_scroll < 0 && (scroll_index + set_scroll) < scroll_index)
        {
            scroll_index += set_scroll;
        }
        else if(set_scroll > 0)
        {
            scroll_index = limit(scroll_index + set_scroll, 0, count() - items_max);
        }
    }
    string get_item(const size_t index)
    {
        if(index < count())
        {
            return items[index];
        }
        return "";
    }
    string get_selected_item()
    {
        if(selected < count())
        {
            return items[selected];
        }
        return "";
    }
    size_t get_index()
    {
        return selected;
    }
    size_t count()
    {
        return items.size();
    }
    size_t get_items_max()
    {
        return items_max;
    }
    virtual bool in_range_select(size_t) = 0;
    bool is_overflow()
    {
        return (count() > items_max);
    }
};

class combobox: public container
{
    bool is_open;
public:
    combobox(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1, size_t set_items_max = 10)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        wstroke = set_wstroke;
        items_max = set_items_max;
        selected = 0;
        scroll_index = 0;
        is_open = false;
        type = "ComboBox";
        set_colors();
    }
    void draw()
    {
        size_t arrow = snap((sy - (wstroke * 2)) / 3, 2);
        if(state == 1)
        {
            gout << move_to(x, y) << cstroke.blend(COLOR_GREY).to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box(sx - sy - wstroke, sy - (wstroke * 2))
            << move_to(x + sx - sy + wstroke, y + wstroke) << box(sy - (wstroke * 2), sy - (wstroke * 2)) << ctext.blend(COLOR_GREY).to_gout();
            for(size_t i = 0; i < arrow; i++)
            {
                gout << move_to(x + sx - (sy / 2) - i, y + ((sy + arrow) / 2) - i) << line(i * 2, 0);
            }
            gout << move_to(x + wstroke + (sy / 5), y + ((sy - gout.cascent()) / 2));
            if(count() > 0)
            {
                gout << text(items[selected]);
            }
            else
            {
                gout << (cfill.blend(ctext)).blend(COLOR_GREY).to_gout() << text("<üres>");
            }
        }
        else
        {
            gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - sy - wstroke, sy - (wstroke * 2))
            << move_to(x + sx - sy + wstroke, y + wstroke) << box(sy - (wstroke * 2), sy - (wstroke * 2));
            if(in_range())
            {
                gout << ctext_active.to_gout();
            }
            else
            {
                gout << ctext.to_gout();
            }
            if(is_open)
            {
                for(size_t i = 0; i < arrow; i++)
                {
                    gout << move_to(x + sx - (sy / 2) - i, y + ((sy - arrow) / 2) + i) << line(i * 2, 0);
                }
                size_t offset = (gout.cascent() + 10);
                if(is_overflow())
                {
                    for(size_t i = 0; i < items_max; i++)
                    {
                        gout << move_to(x, y + sy + (i * offset));
                        if(in_range_select(i + scroll_index))
                        {
                            gout << ctext_active.to_gout() << box(sx - 7, offset) << cfill.to_gout();
                        }
                        else if((i + scroll_index) == selected)
                        {
                            gout << ctext_active.blend(cfill).to_gout() << box(sx - 7, offset) << cfill.to_gout();
                        }
                        else
                        {
                            gout << cfill.to_gout() << box(sx - 7, offset) << ctext_active.to_gout();
                        }
                        gout << move_to(x + (sy / 5), y + sy + (i * offset) + 3) << text(items[i + scroll_index]);
                    }
                    double hscroll = ((offset * pow(items_max, 2)) / (double)count());
                    gout << move_to(x + sx - 7, y + sy) << cfill.to_gout() << box(7, offset * items_max)
                    << move_to(x + sx - 5, y + sy + ((scroll_index * hscroll) / items_max)) << ctext_active.to_gout() << box(5, ceil(hscroll));
                }
                else
                {
                    for(size_t i = 0; i < count(); i++)
                    {
                        gout << move_to(x, y + sy + (i * offset));
                        if(in_range_select(i))
                        {
                            gout << ctext_active.to_gout() << box(sx, offset) << cfill.to_gout();
                        }
                        else if(i == selected)
                        {
                            gout << ctext_active.blend(cfill).to_gout() << box(sx, offset) << cfill.to_gout();
                        }
                        else
                        {
                            gout << cfill.to_gout() << box(sx, offset) << ctext_active.to_gout();
                        }
                        gout << move_to(x + (sy / 5), y + sy + (i * offset) + 3) << text(items[i]);
                    }
                }
                gout << ctext_active.to_gout();
            }
            else
            {
                for(size_t i = 0; i < arrow; i++)
                {
                    gout << move_to(x + sx - (sy / 2) - i, y + ((sy + arrow) / 2) - i) << line(i * 2, 0);
                }
            }
            gout << move_to(x + wstroke + (sy / 5), y + ((sy - gout.cascent()) / 2));
            if(count() > 0)
            {
                gout << text(items[selected]);
            }
            else
            {
                if(in_range())
                {
                    gout << cfill.blend(ctext_active).to_gout();
                }
                else
                {
                    gout << cfill.blend(ctext).to_gout();
                }
                gout << text("<üres>");
            }
        }
    }
    void drop()
    {
        if(count() > 0)
        {
            is_open = !is_open;
        }
    }
    void close()
    {
        if(count() > 0 && is_open)
        {
            is_open = false;
        }
    }
    void clear()
    {
        items.clear();
        is_open = false;
        selected = 0;
        scroll_index = 0;
    }
    bool in_range_select(const size_t index)
    {
        size_t offset0 = (y + sy + ((index - scroll_index) * (gout.cascent() + 10))), offset1 = (y + sy + ((index - scroll_index + 1) * (gout.cascent() + 10)));
        return (evx >= x && evx < (x + sx) && evy >= offset0 && evy < offset1);
    }
    bool get_open()
    {
        return is_open;
    }
};

class listbox: public container
{
public:
    listbox(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        wstroke = set_wstroke;
        sy = (snap(set_sy, (gout.cascent() + 10)) + (wstroke * 2));
        items_max = (sy / (gout.cascent() + 10));
        selected = 0;
        scroll_index = 0;
        type = "ListBox";
        set_colors();
    }
    void draw()
    {
        gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
        << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2));
        size_t offset = (gout.cascent() + 10);
        if(count() > 0)
        {
            if(is_overflow())
            {
                for(size_t i = 0; i < items_max; i++)
                {
                    if(in_range_select(i + scroll_index))
                    {
                        gout << move_to(x + wstroke, y + (i * offset) + wstroke) << ctext_active.to_gout()
                        << box(sx - (wstroke * 2) - 7, offset) << cfill.to_gout();
                    }
                    else if(selected == (i + scroll_index))
                    {
                        gout << move_to(x + wstroke, y + (i * offset) + wstroke) << (ctext_active.blend(cfill)).to_gout()
                        << box(sx - (wstroke * 2) - 7, offset) << cfill.to_gout();
                    }
                    else
                    {
                        gout << ctext.to_gout();
                    }
                    gout << move_to(x + wstroke + 5, y + (i * offset) + 5) << text(items[i + scroll_index]);
                }
                double hscroll = ((offset * pow(items_max, 2)) / (double)count());
                gout << move_to(x + sx - wstroke - 7, y + wstroke) << cfill.to_gout() << box(7, offset * items_max)
                << move_to(x + sx - wstroke - 5, y + wstroke + ((scroll_index * hscroll) / items_max)) << ctext_active.to_gout() << box(5, ceil(hscroll));
            }
            else
            {
                for(size_t i = 0; i < count(); i++)
                {
                    if(in_range_select(i))
                    {
                        gout << move_to(x + wstroke, y + (i * offset) + wstroke) << ctext_active.to_gout()
                        << box(sx - (wstroke * 2), offset) << cfill.to_gout();
                    }
                    else if(selected == i)
                    {
                        gout << move_to(x + wstroke, y + (i * offset) + wstroke) << (ctext_active.blend(cfill)).to_gout()
                        << box(sx - (wstroke * 2), offset) << cfill.to_gout();
                    }
                    else
                    {
                        gout << ctext.to_gout();
                    }
                    gout << move_to(x + wstroke + 5, y + (i * offset) + 5) << text(items[i]);
                }
            }
        }
        else
        {
            gout << move_to(x + ((sx - gout.twidth("<üres>")) / 2), y + ((sy - gout.cascent()) / 2));
            if(in_range())
            {
                gout << cfill.blend(ctext_active).to_gout();
            }
            else
            {
                gout << cfill.blend(ctext).to_gout();
            }
            gout << text("<üres>");
        }
    }
    void clear()
    {
        items.clear();
        selected = 0;
        scroll_index = 0;
    }
    bool in_range_select(const size_t index)
    {
        size_t offset0 = (y + wstroke + ((index - scroll_index) * (gout.cascent() + 10))), offset1 = (y + wstroke + ((index - scroll_index + 1) * (gout.cascent() + 10)));
        return (evx >= x && evx < (x + sx) && evy >= offset0 && evy < offset1);
    }
};

class checkbox: public control
{
    color3 cfill, ctext, cstroke, ccheck;
    string label;
    bool is_checked;
public:
    checkbox(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1, string set_label = "CheckBox", bool set_checked = false)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        wstroke = set_wstroke;
        label = set_label;
        is_checked = set_checked;
        type = "CheckBox";
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_ctext = COLOR_WHITE, color3 set_cstroke = COLOR_CAMO, color3 set_ccheck = COLOR_DARKGREEN)
    {
        cfill = set_cfill;
        ctext = set_ctext;
        cstroke = set_cstroke;
        ccheck = set_ccheck;
    }
    void draw()
    {
        int offset = (y + ((int)sy / 2) - ((2 * gout.cascent()) / 3));
        if(state == 1)
        {
            gout << move_to(x, y) << cstroke.blend(COLOR_GREY).to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + sx + 11, offset + 1) << COLOR_BLACK.to_gout() << text(label)
            << move_to(x + sx + 10, offset) << ctext.blend(COLOR_GREY).to_gout() << text(label);
            if(is_checked)
            {
                gout << move_to(x + wstroke + 2, y + wstroke + 2) << ccheck.blend(COLOR_GREY).to_gout();
            }
        }
        else
        {
            gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + sx + 11, offset + 1) << COLOR_BLACK.to_gout() << text(label)
            << move_to(x + sx + 10, offset) << ctext.to_gout() << text(label);
            if(is_checked)
            {
                gout << move_to(x + wstroke + 2, y + wstroke + 2) << ccheck.to_gout();
            }
            else if(in_range())
            {
                gout << move_to(x + wstroke + 2, y + wstroke + 2) << cfill.blend(ccheck).to_gout();
            }
        }
        gout << move_to(x + wstroke + 2, y + wstroke + 2)
        << box(sx - (wstroke * 2) - 4, sy - (wstroke * 2) - 4);
    }
    void check()
    {
        is_checked = !is_checked;
    }
    int get_checked()
    {
        if(is_checked)
        {
            return 1;
        }
        return -1;
    }
};

class radiobutton: public control
{
    color3 cfill, ctext, cstroke, ccheck;
    string label;
    size_t group;
    bool is_selected;
public:
    radiobutton(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1, string set_label = "RadioButton", size_t set_group = 0, bool set_selected = false)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        wstroke = set_wstroke;
        label = set_label;
        group = set_group;
        is_selected = set_selected;
        type = "RadioButton";
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_ctext = COLOR_WHITE, color3 set_cstroke = COLOR_RED, color3 set_ccheck = COLOR_CRIMSON)
    {
        cfill = set_cfill;
        ctext = set_ctext;
        cstroke = set_cstroke;
        ccheck = set_ccheck;
    }
    void draw()
    {
        int offset = (y + ((int)sy / 2) - ((2 * gout.cascent()) / 3));
        if(state == 1)
        {
            gout << move_to(x, y) << cstroke.blend(COLOR_GREY).to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + sx + 11, offset + 1) << COLOR_BLACK.to_gout() << text(label)
            << move_to(x + sx + 10, offset) << ctext.blend(COLOR_GREY).to_gout() << text(label);
            if(is_selected)
            {
                gout << move_to(x + wstroke + 2, y + wstroke + 2) << ccheck.blend(COLOR_GREY).to_gout();
            }
        }
        else
        {
            gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + sx + 11, offset + 1) << COLOR_BLACK.to_gout() << text(label)
            << move_to(x + sx + 10, offset) << ctext.to_gout() << text(label);
            if(is_selected)
            {
                gout << move_to(x + wstroke + 2, y + wstroke + 2) << ccheck.to_gout();
            }
            else if(in_range())
            {
                gout << move_to(x + wstroke + 2, y + wstroke + 2) << cfill.blend(ccheck).to_gout();
            }
        }
        gout << move_to(x + wstroke + 2, y + wstroke + 2)
        << box(sx - (wstroke * 2) - 4, sy - (wstroke * 2) - 4);
    }
    size_t get_group()
    {
        return group;
    }
    bool get_selected()
    {
        return is_selected;
    }
    void select(const bool set_selected = true)
    {
        is_selected = set_selected;
    }
};

class textbox: public control
{
    color3 cfill, ctext, cstroke, ctext_active;
    string content, content_edit;
    bool is_focused, is_secret;
public:
    textbox(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1, string set_content = "TextBox", bool set_secret = false)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        wstroke = set_wstroke;
        content = set_content;
        content_edit = "";
        is_focused = false;
        is_secret = set_secret;
        type = "TextBox";
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_ctext = COLOR_BLACK, color3 set_cstroke = COLOR_NAVY, color3 set_ctext_active = COLOR_DEEPBLUE)
    {
        cfill = set_cfill;
        ctext = set_ctext;
        cstroke = set_cstroke;
        ctext_active = set_ctext_active;
    }
    void draw()
    {
        int offset = (y + ((int)sy / 2) - ((2 * gout.cascent()) / 3));
        if(state == 1)
        {
            gout << move_to(x, y) << cstroke.blend(COLOR_GREY).to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + wstroke, y + wstroke) << (cfill / 2).blend(COLOR_GREY).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
            << move_to(x + wstroke + 1, y + sy - wstroke - 1) << (cfill.blend(COLOR_WHITE)).blend(COLOR_GREY).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + sx - wstroke - 1, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
            << move_to(x + wstroke + 5, offset) << ctext.blend(COLOR_GREY).to_gout();
            if(is_secret)
            {
                gout << text(string(content.length(), '*'));
            }
            else
            {
                gout << text(content);
            }
        }
        else
        {
            gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + wstroke, y + wstroke) << (cfill / 2).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
            << move_to(x + wstroke + 1, y + sy - wstroke - 1) << cfill.blend(COLOR_WHITE).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + sx - wstroke - 1, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
            << move_to(x + wstroke + 5, offset);
            if(is_secret)
            {
                if(is_focused)
                {
                    gout << ctext_active.to_gout() << text(string(content_edit.length(), '*') + "|");
                }
                else
                {
                    if(in_range())
                    {
                        gout << ctext.blend(ctext_active).to_gout() << text(string(content.length(), '*'));
                    }
                    else
                    {
                        gout << ctext.to_gout() << text(string(content.length(), '*'));
                    }
                }
            }
            else
            {
                if(is_focused)
                {
                    gout << ctext_active.to_gout() << text(content_edit + "|");
                }
                else
                {
                    if(in_range())
                    {
                        gout << ctext.blend(ctext_active).to_gout() << text(content);
                    }
                    else
                    {
                        gout << ctext.to_gout() << text(content);
                    }
                }
            }
        }
    }
    void focus(const bool set_focus)
    {
        is_focused = set_focus;
        if(!set_focus)
        {
            content_edit = "";
        }
    }
    void edit(int input)
    {
        if(input == key_enter)
        {
            content = content_edit;
            content_edit = "";
            is_focused = false;
        }
        else
        {
            if(input == 8 && content_edit.length() > 0)
            {
                content_edit = content_edit.substr(0, content_edit.length() - 1);
            }
            else if(input != 8 && (size_t)gout.twidth(content_edit) < (sx - (wstroke * 2) - 10))
            {
                content_edit += (char)input;
            }
        }
    }
    bool get_focus()
    {
        return is_focused;
    }
    string get_content()
    {
        return content;
    }
};

class multitextbox: public control
{
    size_t hspace, scroll_index, lines_max;
    color3 cfill, ctext, cstroke;
    vector<string> content;
public:
    multitextbox(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1, size_t set_hspace = 5)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = snap(set_sy, gout.cascent());
        wstroke = set_wstroke;
        hspace = set_hspace;
        scroll_index = 0;
        lines_max = (sy / (gout.cascent() + hspace));
        type = "MultiTextBox";
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_ctext = COLOR_BLACK, color3 set_cstroke = COLOR_NAVY, color3 set_ctext_active = COLOR_DEEPBLUE)
    {
        cfill = set_cfill;
        ctext = set_ctext;
        cstroke = set_cstroke;
    }
    void draw()
    {
        size_t offset = (gout.cascent() + hspace);
        if(state == 1)
        {
            gout << move_to(x, y) << cstroke.blend(COLOR_GREY).to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + wstroke, y + wstroke) << (cfill / 2).blend(COLOR_GREY).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
            << move_to(x + wstroke + 1, y + sy - wstroke - 1) << (cfill.blend(COLOR_WHITE)).blend(COLOR_GREY).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + sx - wstroke - 1, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
            << move_to(x + wstroke + 5, offset) << ctext.blend(COLOR_GREY).to_gout();
            if(is_overflow())
            {
                for(size_t i = 0; i < lines_max; i++)
                {
                    gout << move_to(x + wstroke + 5, y + wstroke + (i * offset)) << text(content[i + scroll_index]);
                }
                double hscroll = ((offset * pow(lines_max, 2)) / (double)count());
                gout << move_to(x + sx - wstroke - 5, y + wstroke + ((scroll_index * hscroll) / lines_max)) << ctext.blend(COLOR_GREY).to_gout() << box(5, ceil(hscroll));
            }
            else
            {
                for(size_t i = 0; i < count(); i++)
                {
                    gout << move_to(x + wstroke + 5, y + wstroke + (i * offset)) << text(content[i]);
                }
            }
        }
        else
        {
            gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - (wstroke * 2), sy - (wstroke * 2))
            << move_to(x + wstroke, y + wstroke) << (cfill / 2).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
            << move_to(x + wstroke + 1, y + sy - wstroke - 1) << cfill.blend(COLOR_WHITE).to_gout() << line(sx - (wstroke * 2) - 1, 0)
            << move_to(x + sx - wstroke - 1, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
            << move_to(x + wstroke + 5, offset) << ctext.to_gout();
            if(is_overflow())
            {
                for(size_t i = 0; i < lines_max; i++)
                {
                    gout << move_to(x + wstroke + 5, y + wstroke + (i * offset)) << text(content[i + scroll_index]);
                }
                double hscroll = ((offset * pow(lines_max, 2)) / (double)count());
                gout << move_to(x + sx - wstroke - 5, y + wstroke + ((scroll_index * hscroll) / lines_max)) << ctext.to_gout() << box(5, ceil(hscroll));
            }
            else
            {
                for(size_t i = 0; i < count(); i++)
                {
                    gout << move_to(x + wstroke + 5, y + wstroke + (i * offset)) << text(content[i]);
                }
            }
        }
    }
    void load(string file_name)
    {
        ifstream file_load(file_name);
        if(!file_load.good())
        {
            cout << "Error loading file [" << file_name << "]" << endl;
            return;
        }
        content.clear();
        scroll_index = 0;
        vector<string> words;
        string word_get;
        size_t break_get, tab_get;
        while(!file_load.eof())
        {
            getline(file_load, word_get, ' ');
            while((break_get = word_get.find('\n')) < word_get.length())
            {
                while((tab_get = word_get.find('\t')) < word_get.length())
                {
                    word_get = (word_get.substr(0, tab_get) + string(4, ' ') + word_get.substr(tab_get + 1));
                }
                words.push_back(word_get.substr(0, break_get));
                word_get = word_get.substr(break_get + 1);
                words.push_back("\n");
            }
            while((tab_get = word_get.find('\t')) < word_get.length())
            {
                word_get = (string(4, ' ') + word_get.substr(tab_get + 1));
            }
            if(word_get.length() > 0)
            {
                words.push_back(word_get);
            }
        }
        string line;
        size_t boundary = (sx - (wstroke * 2) - 10);
        for(auto w:words)
        {
            if(w == "\n")
            {
                if(line.length() > 0)
                {
                    content.push_back(line);
                }
                line = "";
            }
            else if(line.length() > 0)
            {
                if((size_t)gout.twidth(line + ' ' + w) > boundary)
                {
                    content.push_back(line);
                    line = w;
                }
                else
                {
                    line += ' ' + w;
                }
            }
            else
            {
                line = w;
            }
        }
        if(line.length() > 0)
        {
            content.push_back(line);
        }
    }
    void clear()
    {
        content.clear();
        scroll_index = 0;
    }
    void scroll(int set_scroll, size_t jump_to = 0)
    {
        if(set_scroll == 0)
        {
            scroll_index = (jump_to * (count() - lines_max));
        }
        else if(set_scroll < 0 && (scroll_index + set_scroll) < scroll_index)
        {
            scroll_index += set_scroll;
        }
        else if(set_scroll > 0)
        {
            scroll_index = limit(scroll_index + set_scroll, 0, count() - lines_max);
        }
    }
    size_t get_lines_max()
    {
        return lines_max;
    }
    size_t count()
    {
        return content.size();
    }
    bool is_overflow()
    {
        return (count() * (gout.cascent() + hspace) > sy);
    }
};

class numeric: public control
{
    int value;
    size_t increment;
    interval bound;
    color3 cfill, ctext, cstroke, ctext_active;
public:
    numeric(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1, int set_value = 0, interval set_bound = interval(), size_t set_increment = 1)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        wstroke = set_wstroke;
        value = set_value;
        bound = set_bound;
        increment = set_increment;
        type = "Numeric";
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_ctext = COLOR_BLACK, color3 set_cstroke = COLOR_NAVY, color3 set_ctext_active = COLOR_DEEPBLUE)
    {
        cfill = set_cfill;
        ctext = set_ctext;
        cstroke = set_cstroke;
        ctext_active = set_ctext_active;
    }
    void draw()
    {
        int offset = (y + ((int)sy / 2) - ((2 * gout.cascent()) / 3));
        if(state == 1)
        {
            gout << move_to(x, y) << cstroke.blend(COLOR_GREY).to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box(sx - (sy / 2) - (wstroke * 2) + 1, sy - (wstroke * 2))
            << move_to(x + wstroke, y + wstroke) << (cfill / 2).blend(COLOR_GREY).to_gout() << line(sx - (sy / 2) - (wstroke * 2), 0)
            << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
            << move_to(x + wstroke + 1, y + sy - wstroke - 1) << (cfill.blend(COLOR_WHITE)).blend(COLOR_GREY).to_gout() << line(sx - (sy / 2) - (wstroke * 2) - 1, 0)
            << move_to(x + sx - (wstroke * 2) - (sy / 2) + 2, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1)
            << move_to(x + sx - (sy / 2) + 1, y + wstroke) << cfill.blend(COLOR_GREY).to_gout() << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
            << move_to(x + sx - (sy / 2) + 1, y + (sy / 2) + wstroke - 1) << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
            << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << ctext.blend(COLOR_GREY).to_gout() << box((sy / 2) - (wstroke * 4) - 1, 2)
            << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
            << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << box((sy / 2) - (wstroke * 4) - 1, 2)
            << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << box((sy / 2) - (wstroke * 4) - 1, 2)
            << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
            << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << box((sy / 2) - (wstroke * 4) - 1, 2);
        }
        else
        {
            gout << move_to(x, y) << cstroke.to_gout() << box(sx, sy)
            << move_to(x + wstroke, y + wstroke) << cfill.to_gout() << box(sx - (sy / 2) - (wstroke * 2) + 1, sy - (wstroke * 2))
            << move_to(x + wstroke, y + wstroke) << (cfill / 2).to_gout() << line(sx - (sy / 2) - (wstroke * 2), 0)
            << move_to(x + wstroke, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 2)
            << move_to(x + wstroke + 1, y + sy - wstroke - 1) << cfill.blend(COLOR_WHITE).to_gout() << line(sx - (sy / 2) - (wstroke * 2) - 1, 0)
            << move_to(x + sx - (wstroke * 2) - (sy / 2) + 2, y + wstroke + 1) << line(0, sy - (wstroke * 2) - 1);
            if(in_range())
            {
                if(in_range_change() == 1)
                {
                    gout << move_to(x + sx - (sy / 2) + 1, y + wstroke) << ctext_active.to_gout() << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                    << move_to(x + sx - (sy / 2) + 1, y + (sy / 2) + wstroke - 1) << cfill.to_gout() << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                    << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << box((sy / 2) - (wstroke * 4) - 1, 2)
                    << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
                    << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << ctext_active.to_gout() << box((sy / 2) - (wstroke * 4) - 1, 2);
                    if(!bound.is_null())
                    {
                        gout << move_to(evx + 16, evy - 2) << COLOR_BLACK.to_gout() << text("Max: " + tostring(bound.imax))
                        << move_to(evx + 15, evy - 3) << ctext_active.blend(COLOR_WHITE).to_gout() << text("Max: " + tostring(bound.imax)) << ctext_active.to_gout();
                    }
                }
                else if(in_range_change() == -1)
                {
                    gout << move_to(x + sx - (sy / 2) + 1, y + wstroke) << cfill.to_gout() << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                    << move_to(x + sx - (sy / 2) + 1, y + (sy / 2) + wstroke - 1) << ctext_active.to_gout() << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                    << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << box((sy / 2) - (wstroke * 4) - 1, 2)
                    << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
                    << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << cfill.to_gout() << box((sy / 2) - (wstroke * 4) - 1, 2) << ctext_active.to_gout();
                    if(!bound.is_null())
                    {
                        gout << move_to(evx + 16, evy - 2) << COLOR_BLACK.to_gout() << text("Min: " + tostring(bound.imin))
                        << move_to(evx + 15, evy - 3) << ctext_active.blend(COLOR_WHITE).to_gout() << text("Min: " + tostring(bound.imin)) << ctext_active.to_gout();
                    }
                }
                else
                {
                    gout << move_to(x + sx - (sy / 2) + 1, y + wstroke) << cfill.to_gout() << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                    << move_to(x + sx - (sy / 2) + 1, y + (sy / 2) + wstroke - 1) << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                    << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << ctext_active.to_gout() << box((sy / 2) - (wstroke * 4) - 1, 2)
                    << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
                    << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << box((sy / 2) - (wstroke * 4) - 1, 2)
                    << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << box((sy / 2) - (wstroke * 4) - 1, 2)
                    << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
                    << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << box((sy / 2) - (wstroke * 4) - 1, 2);
                }
            }
            else
            {
                gout << move_to(x + sx - (sy / 2) + 1, y + wstroke) << cfill.to_gout() << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                << move_to(x + sx - (sy / 2) + 1, y + (sy / 2) + wstroke - 1) << box((sy / 2) - (wstroke * 2) + 1, (sy / 2) - (wstroke * 2) + 1)
                << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << ctext.to_gout() << box((sy / 2) - (wstroke * 4) - 1, 2)
                << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
                << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << box((sy / 2) - (wstroke * 4) - 1, 2)
                << move_to(x + sx - (sy / 2) + 4, y + (sy / 4)) << box((sy / 2) - (wstroke * 4) - 1, 2)
                << move_to(x + sx - (sy / 4) - 2, y + wstroke + 3) << box(2, (sy / 2) - (wstroke * 4) - 1)
                << move_to(x + sx - (sy / 2) + 4, y + ((sy * 3) / 4) - 1) << box((sy / 2) - (wstroke * 4) - 1, 2);
            }
        }
        gout << move_to(x + wstroke + 5, offset) << text(tostring(value));
    }
    void update_value(const int delta)
    {
        if(bound.is_null())
        {
            value += (delta * (int)increment);
        }
        else
        {
            value = limit(value + (delta * (int)increment), bound.imin, bound.imax);
        }
    }
    void update_increment(const size_t set_increment)
    {
        increment = set_increment;
    }
    int get_value()
    {
        return value;
    }
    interval get_bound()
    {
        return bound;
    }
    int in_range_change()
    {
        if(evx >= (x + sx - (sy / 2) + 1) && evx < (x + sx - (wstroke * 2) + 2))
        {
            if(evy >= (y + wstroke) && evy < (y + (sy / 2) - wstroke + 1))
            {
                return 1;
            }
            else if(evy >= (y + (sy / 2) + wstroke - 1) && evy < (y + sy - wstroke))
            {
                return -1;
            }
        }
        return 0;
    }
};

class slider: public control
{
protected:
    double value;
    color3 cline, chandle, chandle_active;
public:
    slider(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, double set_value = 0)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        value = limit(set_value, 0, 1);
        type = "Slider";
        set_colors();
    }
    void set_colors(color3 set_cline = COLOR_WHITE, color3 set_chandle = COLOR_DEEPBLUE, color3 set_chandle_active = COLOR_ORANGE)
    {
        cline = set_cline;
        chandle = set_chandle;
        chandle_active = set_chandle_active;
    }
    virtual void draw() {};
    virtual void update_value(double = -1) {};
    double get_value()
    {
        return value;
    }
};

class hslider: public slider
{
public:
    hslider(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, double set_value = 0):slider(set_x, set_y, set_sx, set_sy, set_value) {};
    void draw()
    {
        if(state == 1)
        {
            gout << move_to(x, y + ((sy * 2) / 5)) << cline.blend(COLOR_GREY).to_gout() << box(sx, (sy / 5))
            << move_to(x, y + (sy / 4)) << box(2, (sy / 2))
            << move_to(x + sx - 2, y + (sy / 4)) << box(2, (sy / 2))
            << move_to(x + (value * sx) - (sy / 4), y)
            << (chandle.blend(COLOR_BLACK)).blend(COLOR_GREY).to_gout() << box((sy / 2), sy)
            << move_to(x + (value * sx) - (sy / 4) + 2, y + 2) << chandle.blend(COLOR_GREY).to_gout() << box((sy / 2) - 4, sy - 4)
            << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) - 7) << (chandle.blend(COLOR_BLACK)).blend(COLOR_GREY).to_gout() << box((sy / 2) - 8, 2)
            << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) - 1) << box((sy / 2) - 8, 2)
            << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) + 5) << box((sy / 2) - 8, 2);
        }
        else
        {
            gout << move_to(x, y + ((sy * 2) / 5)) << cline.to_gout() << box(sx, (sy / 5))
            << move_to(x, y + (sy / 4)) << box(2, (sy / 2))
            << move_to(x + sx - 2, y + (sy / 4)) << box(2, (sy / 2))
            << move_to(x + (value * sx) - (sy / 4), y);
            if(in_range())
            {
                gout << chandle_active.blend(COLOR_BLACK).to_gout() << box((sy / 2), sy)
                << move_to(x + (value * sx) - (sy / 4) + 2, y + 2) << chandle_active.to_gout() << box((sy / 2) - 4, sy - 4)
                << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) - 7) << chandle_active.blend(COLOR_BLACK).to_gout() << box((sy / 2) - 8, 2)
                << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) - 1) << box((sy / 2) - 8, 2)
                << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) + 5) << box((sy / 2) - 8, 2)
                << move_to(evx + 16, evy - 2) << COLOR_BLACK.to_gout() << text(tostring(value * 100) + "%")
                << move_to(evx + 15, evy - 3) << chandle_active.blend(COLOR_WHITE).to_gout() << text(tostring(value * 100) + "%");
            }
            else
            {
                gout << chandle.blend(COLOR_BLACK).to_gout() << box((sy / 2), sy)
                << move_to(x + (value * sx) - (sy / 4) + 2, y + 2) << chandle.to_gout() << box((sy / 2) - 4, sy - 4)
                << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) - 7) << chandle.blend(COLOR_BLACK).to_gout() << box((sy / 2) - 8, 2)
                << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) - 1) << box((sy / 2) - 8, 2)
                << move_to(x + (value * sx) - (sy / 4) + 4, y + (sy / 2) + 5) << box((sy / 2) - 8, 2);
            }
        }
    }
    void update_value(const double direct = -1)
    {
        if(direct == -1)
        {
            value = limit((evx - x) / (double)sx, 0, 1);
        }
        else
        {
            value = limit(direct, 0, 1);
        }
    }
};

class vslider: public slider
{
public:
    vslider(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, double set_value = 0):slider(set_x, set_y, set_sx, set_sy, set_value) {};
    void draw()
    {
        if(state == 1)
        {
            gout << move_to(x + ((sx * 2) / 5), y) << cline.blend(COLOR_GREY).to_gout() << box((sx / 5), sy)
            << move_to(x + (sx / 4), y) << box((sx / 2), 2)
            << move_to(x + (sx / 4), y + sy - 2) << box((sx / 2), 2)
            << move_to(x, y + ((1 - value) * sy) - (sx / 4))
            << (chandle.blend(COLOR_BLACK)).blend(COLOR_GREY).to_gout() << box(sx, (sx / 2))
            << move_to(x + 2, y + ((1 - value) * sy) - (sx / 4) + 2) << chandle.blend(COLOR_GREY).to_gout() << box(sx - 4, (sx / 2) - 4)
            << move_to(x + (sx / 2) - 7, y + ((1 - value) * sy) - (sx / 4) + 4) << (chandle.blend(COLOR_BLACK)).blend(COLOR_GREY).to_gout() << box(2, (sx / 2) - 8)
            << move_to(x + (sx / 2) - 1, y + ((1 - value) * sy) - (sx / 4) + 4) << box(2, (sx / 2) - 8)
            << move_to(x + (sx / 2) + 5, y + ((1 - value) * sy) - (sx / 4) + 4) << box(2, (sx / 2) - 8);
        }
        else
        {
            gout << move_to(x + ((sx * 2) / 5), y) << cline.to_gout() << box((sx / 5), sy)
            << move_to(x + (sx / 4), y) << box((sx / 2), 2)
            << move_to(x + (sx / 4), y + sy - 2) << box((sx / 2), 2)
            << move_to(x, y + ((1 - value) * sy) - (sx / 4));
            if(in_range())
            {
                gout << chandle_active.blend(COLOR_BLACK).to_gout() << box(sx, (sx / 2))
                << move_to(x + 2, y + ((1 - value) * sy) - (sx / 4) + 2) << chandle_active.to_gout() << box(sx - 4, (sx / 2) - 4)
                << move_to(x + (sx / 2) - 7, y + ((1 - value) * sy) - (sx / 4) + 4) << chandle_active.blend(COLOR_BLACK).to_gout() << box(2, (sx / 2) - 8)
                << move_to(x + (sx / 2) - 1, y + ((1 - value) * sy) - (sx / 4) + 4) << box(2, (sx / 2) - 8)
                << move_to(x + (sx / 2) + 5, y + ((1 - value) * sy) - (sx / 4) + 4) << box(2, (sx / 2) - 8)
                << move_to(evx - 4, evy + 16) << COLOR_BLACK.to_gout() << text(tostring(value * 100) + "%")
                << move_to(evx - 5, evy + 15) << chandle_active.blend(COLOR_WHITE).to_gout() << text(tostring(value * 100) + "%");
            }
            else
            {
                gout << chandle.blend(COLOR_BLACK).to_gout() << box(sx, (sx / 2))
                << move_to(x + 2, y + ((1 - value) * sy) - (sx / 4) + 2) << chandle.to_gout() << box(sx - 4, (sx / 2) - 4)
                << move_to(x + (sx / 2) - 7, y + ((1 - value) * sy) - (sx / 4) + 4) << chandle.blend(COLOR_BLACK).to_gout() << box(2, (sx / 2) - 8)
                << move_to(x + (sx / 2) - 1, y + ((1 - value) * sy) - (sx / 4) + 4) << box(2, (sx / 2) - 8)
                << move_to(x + (sx / 2) + 5, y + ((1 - value) * sy) - (sx / 4) + 4) << box(2, (sx / 2) - 8);
            }
        }
    }
    void update_value(const double direct = -1)
    {
        if(direct == -1)
        {
            value = limit(1 - ((evy - y) / (double)sy), 0, 1);
        }
        else
        {
            value = limit(direct, 0, 1);
        }
    }
};

class graph: public control
{
    string label_x, label_y;
    color3 cfill, cscreen, ctext;
    vector<int> values_x, values_y;
public:
    graph(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, string set_label_x = "X-tengely", string set_label_y = "Y-tengely")
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        label_x = set_label_x;
        label_y = set_label_y;
        type = "Graph";
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_LIGHTGREY, color3 set_cscreen = COLOR_SLATE, color3 set_ctext = COLOR_BLACK)
    {
        cfill = set_cfill;
        cscreen = set_cscreen;
        ctext = set_ctext;
    }
    void draw()
    {
        gout << move_to(x, y) << cfill.to_gout() << box(sx, sy)
        << move_to(x + 10, y + 10) << cscreen.to_gout() << box(sx - 20, sy - 20)
        << move_to(x + 19, y + 24 + gout.cascent()) << ctext.to_gout() << line(0, sy - 44 - gout.cascent())
        << move_to(x + 19, y + sy - 20) << line(sx - gout.twidth(label_x) - 42, 0)
        << move_to(x + sx - gout.twidth(label_x) - 15, y + sy - gout.cascent() - 20) << text(label_x)
        << move_to(x + 15, y + 15) << text(label_y);
    }
};

vector<shape*> shapes;

class shape
{
protected:
    size_t x, y, wstroke;
    shape()
    {
        shapes.push_back(this);
    }
public:
    virtual void draw() {};
    virtual void resize() {};
    virtual void set_colors() {};
    void translate(const int by_x, const int by_y)
    {
        x += by_x;
        y += by_y;
    }
    void move(const size_t to_x, const size_t to_y)
    {
        x = to_x;
        y = to_y;
    }
};

class circle: public shape
{
    size_t radius;
    color3 cfill, cstroke;
public:
    circle(size_t set_x, size_t set_y, size_t set_radius, size_t set_wstroke = 1)
    {
        x = set_x;
        y = set_y;
        radius = set_radius;
        wstroke = set_wstroke;
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_WHITE, color3 set_cstroke = COLOR_DEEPBLUE)
    {
        cfill = set_cfill;
        cstroke = set_cstroke;
    }
    void draw()
    {
        int r = (int)radius;
        for(int i = -r; i <= r; i++)
        {
            for(int j = -r; j <= r; j++)
            {
                if((pow(i, 2) + pow(j, 2)) < pow(r - wstroke, 2))
                {
                    gout << move_to(x + i, y + j) << cfill.to_gout() << dot;
                }
                else if((pow(i, 2) + pow(j, 2)) <= pow(r, 2))
                {
                    gout << move_to(x + i, y + j) << cstroke.to_gout() << dot;
                }
            }
        }
    }
    void resize(size_t set_radius)
    {
        radius = limit(set_radius, max((size_t)5, wstroke), min(res_x, res_y) / 4);
    }
    int get_radius()
    {
        return radius;
    }
};

class ring: public shape
{
    size_t radius;
    color3 cstroke;
public:
    ring(size_t set_x, size_t set_y, size_t set_radius, size_t set_wstroke)
    {
        x = set_x;
        y = set_y;
        radius = set_radius;
        wstroke = set_wstroke;
        set_colors();
    }
    void set_colors(color3 set_cstroke = COLOR_DEEPBLUE)
    {
        cstroke = set_cstroke;
    }
    void draw()
    {
        gout << cstroke.to_gout();
        int r = (int)radius;
        double d;
        for(int i = -r; i <= r; i++)
        {
            for(int j = -r; j <= r; j++)
            {
                d = (pow(i, 2) + pow(j, 2));
                if(d <= pow(r, 2) && d > pow(r - wstroke, 2))
                {
                    gout << move_to(x + i, y + j) << dot;
                }
            }
        }
    }
    void resize(size_t set_radius)
    {
        radius = limit(set_radius, max(5, (int)wstroke), min(res_x, res_y) / 4);
    }
    int get_radius()
    {
        return radius;
    }
};

class triangle: public shape
{
    size_t z, u, v, w;
    color3 cstroke;
public:
    triangle(size_t set_x, size_t set_y, size_t set_z, size_t set_u, size_t set_v, size_t set_w)
    {
        x = set_x;
        y = set_y;
        z = set_z;
        u = set_u;
        v = set_v;
        w = set_w;
        set_colors();
    }
    void set_colors(color3 set_cstroke = COLOR_DEEPBLUE)
    {
        cstroke = set_cstroke;
    }
    void draw()
    {
        gout << move_to(x, y) << cstroke.to_gout() << line_to(z, u) << line_to(v, w) << line_to(x, y);
    }
    vector<size_t> get_bounds()
    {
        vector<size_t> value = {(size_t)min3(x, z, v), (size_t)min3(y, u, w), (size_t)max3(x, z, v), (size_t)max3(y, u, w)};
        return value;
    }
};

class hbox: public shape
{
    size_t sx, sy;
    color3 cstroke;
public:
    hbox(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, size_t set_wstroke = 1)
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        wstroke = set_wstroke;
        set_colors();
    }
    void set_colors(color3 set_cstroke = COLOR_DEEPBLUE)
    {
        cstroke = set_cstroke;
    }
    void draw()
    {
        gout << cstroke.to_gout()
        << move_to(x, y) << box(sx - wstroke, wstroke)
        << move_to(x + sx - wstroke, y) << box(wstroke, sy - wstroke)
        << move_to(x + wstroke, y + sy - wstroke) << box(sx - wstroke, wstroke)
        << move_to(x, y + wstroke) << box(wstroke, sy - wstroke);
    }
    void resize(const size_t set_sx, const size_t set_sy)
    {
        sx = set_sx;
        sy = set_sy;
    }
};

class rbox: public shape
{
    size_t sx, sy;
    double angle;
    point2 pivot;
    color3 cfill;
public:
    rbox(size_t set_x, size_t set_y, size_t set_sx, size_t set_sy, double set_angle = 0, point2 set_pivot = point2())
    {
        x = set_x;
        y = set_y;
        sx = set_sx;
        sy = set_sy;
        angle = angular_limit(deg_to_rad(set_angle));
        pivot = set_pivot;
        pivot.x = limit(pivot.x, 0, 1);
        pivot.y = limit(pivot.y, 0, 1);
        set_colors();
    }
    void set_colors(color3 set_cfill = COLOR_DEEPBLUE)
    {
        cfill = set_cfill;
    }
    void draw()
    {
        double rx, ry;
        gout << cfill.to_gout();
        for(size_t i = 0; i < sx; i++)
        {
            for(size_t j = 0; j < sy; j++)
            {
                rx = (x + (i * cos(angle)) - (j * sin(angle)));
                ry = (y + (i * sin(angle)) + (j * cos(angle)));
                gout << move_to(rx, ry) << dot;
            }
        }
        // PIVOT TEST
        gout << move_to(x - 1, y - 1) << color(255, 255, 0) << box(3, 3)
        << move_to(x + (pivot.x * sx) - 1, y + (pivot.y * sy) - 1) << color(0, 255, 255) << box(3, 3);
    }
    void resize(const size_t set_sx, const size_t set_sy)
    {
        sx = set_sx;
        sy = set_sy;
    }
    void rotate(double set_angle)
    {
        angle = angular_limit(angle + deg_to_rad(set_angle));
    }
    void rotate_to(double set_angle)
    {
        angle = angular_limit(deg_to_rad(set_angle));
    }
    double get_angle()
    {
        return angle;
    }
};

void controls_log()
{
    ofstream logfile("log.txt");
    logfile << "#ABGFX\n:log_begin\n";
    size_t logged = 0;
    for(auto *c:controls)
    {
        if(c->get_state() > 0)
        {
            point2 cpos = c->get_pos();
            if(c->get_type("ComboBox"))
            {
                combobox* cb = static_cast<combobox*>(c);
                logfile << "\t:combobox\n\t\t:pos(" << cpos.x << ";" << cpos.y << ")\n\t\t:value(" << cb->get_selected_item() << ")\n\t\t:value_type(string)\n";
                logged++;
            }
            else if(c->get_type("CheckBox"))
            {
                checkbox* cb = static_cast<checkbox*>(c);
                logfile << "\t:checkbox\n\t\t:pos(" << cpos.x << ";" << cpos.y << ")\n\t\t:value(";
                if(cb->get_checked() == 1)
                {
                    logfile << "true";
                }
                else if(cb->get_checked() == -1)
                {
                    logfile << "false";
                }
                logfile << "\n\t\t:value_type(bool)\n";
                logged++;
            }
            else if(c->get_type("RadioButton"))
            {
                radiobutton* rb = static_cast<radiobutton*>(c);
                logfile << "\t:radiobutton\n\t\t:pos(" << cpos.x << ";" << cpos.y << ")\n\t\t:value(";
                if(rb->get_selected())
                {
                    logfile << "true";
                }
                else
                {
                    logfile << "false";
                }
                logfile << ")\n\t\t:value_type(bool)\n\t\t:rb_group(" << rb->get_group() << ")\n";
                logged++;
            }
            else if(c->get_type("TextBox"))
            {
                textbox* tb = static_cast<textbox*>(c);
                logfile << "\t:textbox\n\t\t:pos(" << cpos.x << ";" << cpos.y << ")\n\t\t:value(" << tb->get_content() << ")\n\t\t:value_type(string)\n";
                logged++;
            }
            else if(c->get_type("MultiTextBox"))
            {
                multitextbox* mtb = static_cast<multitextbox*>(c);
                logfile << "\t:multitextbox\n\t\t:pos(" << cpos.x << ";" << cpos.y << ")\n\t\t:line_count(" << mtb->count() << ")\n";
                logged++;
            }
            else if(c->get_type("Numeric"))
            {
                numeric* n = static_cast<numeric*>(c);
                interval b = n->get_bound();
                logfile << "\t:numeric\n\t\t:pos(" << cpos.x << ";" << cpos.y << ")\n\t\t:value(" << n->get_value() << ")\n\t\t:value_type(int)\n\t\t:interval(";
                if(b.is_null())
                {
                    logfile << "null";
                }
                else
                {
                    logfile << b.imin << ";" << b.imax << ";";
                    if(b.is_closed)
                    {
                        logfile << "closed";
                    }
                    else
                    {
                        logfile << "open";
                    }
                }
                logfile << ")\n";
                logged++;
            }
            else if(c->get_type("Slider"))
            {
                slider* s = static_cast<slider*>(c);
                logfile << "\t:slider\n\t\t:pos(" << cpos.x << ";" << cpos.y << ")\n\t\t:value(" << s->get_value() << ")\n\t\t:value_type(double)\n";
                logged++;
            }
        }
    }
    logfile << ":log_end\n#" << logged;
    cout << "Success: " << logged << " controls logged!" << endl;
}

void update()
{
    for(auto *s:shapes)
    {
        s->draw();
    }
    for(auto *c:controls)
    {
        if(c->get_state() > 0)
        {
            c->draw();
        }
    }
    gout << refresh;
}

void run_events(const event &ev)
{
    if(ev.type != ev_timer)
    {
        if(ev.type == ev_mouse)
        {
            evx = ev.pos_x;
            evy = ev.pos_y;
            if(ev.button == btn_left)
            {
                btn_left_pressed = true;
                for(auto *c:controls)
                {
                    if(c->get_state() == 2)
                    {
                        if(c->get_type("Button"))
                        {
                            button* b = static_cast<button*>(c);
                            if(b->has_task() && b->in_range())
                            {
                                b->run_task();
                            }
                        }
                        else if(c->get_type("ComboBox"))
                        {
                            combobox* cb = static_cast<combobox*>(c);
                            if(cb->in_range())
                            {
                                cb->drop();
                            }
                            else if(!cb->in_range() && cb->get_open())
                            {
                                size_t index = 0;
                                while(!cb->in_range_select(index) && index < cb->count())
                                {
                                    index++;
                                }
                                if(index < cb->count())
                                {
                                    cb->select(index);
                                }
                                cb->close();
                            }
                        }
                        else if(c->get_type("ListBox"))
                        {
                            listbox* lb = static_cast<listbox*>(c);
                            if(lb->in_range())
                            {
                                size_t index = 0;
                                while(!lb->in_range_select(index) && index < lb->count())
                                {
                                    index++;
                                }
                                if(index < lb->count())
                                {
                                    lb->select(index);
                                }
                            }
                        }
                        else if(c->get_type("TextBox"))
                        {
                            textbox* tb = static_cast<textbox*>(c);
                            if(tb->in_range() && !tb->get_focus())
                            {
                                tb->focus(true);
                                break;
                            }
                            else if(!tb->in_range() && tb->get_focus())
                            {
                                tb->focus(false);
                                break;
                            }
                        }
                        else if(c->get_type("CheckBox"))
                        {
                            checkbox* cb = static_cast<checkbox*>(c);
                            if(cb->in_range())
                            {
                                cb->check();
                                break;
                            }
                        }
                        else if(c->get_type("RadioButton"))
                        {
                            radiobutton* rb = static_cast<radiobutton*>(c);
                            if(!rb->get_selected() && rb->in_range())
                            {
                                size_t rb_group = rb->get_group();
                                for(auto *c:controls)
                                {
                                    if(c->get_type("RadioButton") && c != rb)
                                    {
                                        radiobutton* rbd = static_cast<radiobutton*>(c);
                                        if(rbd->get_group() == rb_group)
                                        {
                                            rbd->select(false);
                                        }
                                    }
                                }
                                rb->select();
                                break;
                            }
                        }
                        else if(c->get_type("Numeric"))
                        {
                            numeric* n = static_cast<numeric*>(c);
                            if(n->in_range_change() == 1)
                            {
                                if(shift_pressed)
                                {
                                    n->update_value(10);
                                }
                                else
                                {
                                    n->update_value(1);
                                }
                                break;
                            }
                            else if(n->in_range_change() == -1)
                            {
                                if(shift_pressed)
                                {
                                    n->update_value(-10);
                                }
                                else
                                {
                                    n->update_value(-1);
                                }
                                break;
                            }
                        }
                    }
                }
            }
            else if(ev.button == -btn_left)
            {
                btn_left_pressed = false;
            }
            else if(ev.button >= btn_wheelup)
            {
                for(auto *c:controls)
                {
                    if(c->get_state() == 2)
                    {
                        if(c->get_type("ComboBox"))
                        {
                            combobox* cb = static_cast<combobox*>(c);
                            if(cb->get_open() && cb->is_overflow())
                            {
                                if(ev.button == btn_wheelup)
                                {
                                    if(shift_pressed)
                                    {
                                        cb->scroll(-cb->get_items_max());
                                    }
                                    else
                                    {
                                        cb->scroll(-1);
                                    }
                                }
                                else if(ev.button == btn_wheeldown)
                                {
                                    if(shift_pressed)
                                    {
                                        cb->scroll(cb->get_items_max());
                                    }
                                    else
                                    {
                                        cb->scroll(1);
                                    }
                                }
                                break;
                            }
                        }
                        else if(c->get_type("ListBox"))
                        {
                            listbox* lb = static_cast<listbox*>(c);
                            if(lb->in_range() && lb->is_overflow())
                            {
                                if(ev.button == btn_wheelup)
                                {
                                    if(shift_pressed)
                                    {
                                        lb->scroll(-lb->get_items_max());
                                    }
                                    else
                                    {
                                        lb->scroll(-1);
                                    }
                                }
                                else if(ev.button == btn_wheeldown)
                                {
                                    if(shift_pressed)
                                    {
                                        lb->scroll(lb->get_items_max());
                                    }
                                    else
                                    {
                                        lb->scroll(1);
                                    }
                                }
                                break;
                            }
                        }
                        else if(c->get_type("MultiTextBox"))
                        {
                            multitextbox* mtb = static_cast<multitextbox*>(c);
                            if(mtb->is_overflow())
                            {
                                if(ev.button == btn_wheelup)
                                {
                                    if(shift_pressed)
                                    {
                                        mtb->scroll(-mtb->get_lines_max());
                                    }
                                    else
                                    {
                                        mtb->scroll(-1);
                                    }
                                }
                                else if(ev.button == btn_wheeldown)
                                {
                                    if(shift_pressed)
                                    {
                                        mtb->scroll(mtb->get_lines_max());
                                    }
                                    else
                                    {
                                        mtb->scroll(1);
                                    }
                                }
                                break;
                            }
                        }
                        else if(c->get_type("Slider"))
                        {
                            slider* s = static_cast<slider*>(c);
                            if(s->in_range())
                            {
                                if(ev.button == btn_wheelup)
                                {
                                    s->update_value(s->get_value() + 0.01);
                                }
                                else if(ev.button == btn_wheeldown)
                                {
                                    s->update_value(s->get_value() - 0.01);
                                }
                                break;
                            }
                        }
                    }
                }
            }
            if(btn_left_pressed)
            {
                for(auto *c:controls)
                {
                    if(c->get_state() == 2 && c->get_type("Slider"))
                    {
                        slider* s = static_cast<slider*>(c);
                        if(s->in_range())
                        {
                            s->update_value();
                            break;
                        }
                    }
                }
            }
        }
        if(ev.type == ev_key)
        {
            if((ev.keycode == key_lshift || ev.keycode == key_rshift) && !shift_pressed)
            {
                shift_pressed = true;
            }
            else if((ev.keycode == -key_lshift || ev.keycode == -key_rshift) && shift_pressed)
            {
                shift_pressed = false;
            }
            else if(ev.keycode == key_tab)
            {
                point2 cpos, csize;
                for(size_t i = 0; i < controls.size(); i++)
                {
                    if(controls[i]->in_range())
                    {
                        if(shift_pressed)
                        {
                            if(i > 0)
                            {
                                cpos = controls[i - 1]->get_pos();
                                csize = controls[i - 1]->get_size();
                            }
                            else
                            {
                                cpos = controls[controls.size() - 1]->get_pos();
                                csize = controls[controls.size() - 1]->get_size();
                            }
                        }
                        else
                        {
                            if(i < (controls.size() - 1))
                            {
                                cpos = controls[i + 1]->get_pos();
                                csize = controls[i + 1]->get_size();
                            }
                            else
                            {
                                cpos = controls[0]->get_pos();
                                csize = controls[0]->get_size();
                            }
                        }
                        break;
                    }
                    else if(!controls[i]->in_range() && cpos != point2())
                    {
                        cpos = controls[0]->get_pos();
                        csize = controls[0]->get_size();
                        break;
                    }
                }
                if(cpos == point2())
                {
                    cpos = controls[0]->get_pos();
                    csize = controls[0]->get_size();
                }
                gout.movemouse(cpos.x + (csize.x / 2), cpos.y + (csize.y / 2));
            }
            else if(ev.keycode == key_home)
            {
                controls_log();
            }
            else if(ev.keycode > 0)
            {
                for(auto *c:controls)
                {
                    if(c->get_state() == 2)
                    {
                        if(c->get_type("ComboBox"))
                        {
                            combobox* cb = static_cast<combobox*>(c);
                            if(cb->get_open() && cb->is_overflow())
                            {
                                if(ev.keycode == key_pgup)
                                {
                                    cb->scroll(0, 0);
                                }
                                else if(ev.keycode == key_pgdn)
                                {
                                    cb->scroll(0, 1);
                                }
                                break;
                            }
                        }
                        if(c->get_type("ListBox"))
                        {
                            listbox* lb = static_cast<listbox*>(c);
                            if(lb->in_range() && lb->is_overflow())
                            {
                                if(ev.keycode == key_pgup)
                                {
                                    lb->scroll(0, 0);
                                }
                                else if(ev.keycode == key_pgdn)
                                {
                                    lb->scroll(0, 1);
                                }
                                break;
                            }
                        }
                        if(c->get_type("TextBox"))
                        {
                            textbox* tb = static_cast<textbox*>(c);
                            if(tb->get_focus())
                            {
                                tb->edit(ev.keycode);
                                break;
                            }
                        }
                        else if(c->get_type("MultiTextBox"))
                        {
                            multitextbox* mtb = static_cast<multitextbox*>(c);
                            if(mtb->is_overflow())
                            {
                                if(ev.keycode == key_pgup)
                                {
                                    mtb->scroll(0, 0);
                                }
                                else if(ev.keycode == key_pgdn)
                                {
                                    mtb->scroll(0, 1);
                                }
                                break;
                            }
                        }
                        else if(c->get_type("Numeric"))
                        {
                            numeric* n = static_cast<numeric*>(c);
                            if(n->in_range())
                            {
                                if(ev.keycode == key_up)
                                {
                                    n->update_value(1);
                                }
                                else if(ev.keycode == key_down)
                                {
                                    n->update_value(-1);
                                }
                                else if(ev.keycode == key_pgup)
                                {
                                    n->update_value(10);
                                }
                                else if(ev.keycode == key_pgdn)
                                {
                                    n->update_value(-10);
                                }
                                break;
                            }
                        }
                        else if(c->get_type("Slider"))
                        {
                            slider* s = static_cast<slider*>(c);
                            if(s->in_range())
                            {
                                if(ev.keycode == key_right || ev.keycode == key_up)
                                {
                                    s->update_value(s->get_value() + 0.05);
                                }
                                else if(ev.keycode == key_left || ev.keycode == key_down)
                                {
                                    s->update_value(s->get_value() - 0.05);
                                }
                                else if(ev.keycode == key_pgup)
                                {
                                    s->update_value(1);
                                }
                                else if(ev.keycode == key_pgdn)
                                {
                                    s->update_value(0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

#endif // ABGFX_HPP
