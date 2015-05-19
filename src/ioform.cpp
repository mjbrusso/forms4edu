#include "ioform.h"
#include <algorithm>
#include <iostream>
#include <FL/Fl_Input.H>
#include <FL/Fl_Return_Button.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <typeinfo>

namespace forms4edu
{

void b0_cb(Fl_Widget* b, void* v) // OK Button
{
    ioform* f = static_cast<ioform *>(v);
    if(!f->validate()) return;

    f->set_canceled(false);

    if(f->show_flag())
        f->set_show_flag(false);
    else
        b->window()->hide();
}

void b1_cb(Fl_Widget* b, void* v) // Cancel Button
{
    ioform* f = static_cast<ioform *>(v);
    b->window()->hide();
}

string ioform::ok_message="Ok";
string ioform::cancel_message="Cancelar";
string ioform::close_message="Fechar";

ioform::ioform(bool redirect_io, int pad, int spac, int fontsz)
{
    Fl::get_system_colors();
    set_padding(pad);
    set_spacing(spac);
    set_font_size(fontsz);
    fl_close = close_message.c_str();
    curr_y = padding();

    m_wnd= new Fl_Window(Fl::w(), Fl::h(), 0);

    // Creating fields group
    m_container = new Fl_Group(0, 0, window()->w(), window()->h());
    window()->add(m_container);
    container()->end();

    if(redirect_io) {
        //m_in = new istringstream;
        std::cin.rdbuf(m_in.rdbuf());
        //m_out =  new ostringstream;
        std::cout.rdbuf(m_out.rdbuf());
    }
    //else
    window()->set_modal();
}

ioform::~ioform()
{
//    if(redirect_io) {
//        delete m_in;
//        delete m_out;
//    }
    delete m_wnd;
}

void ioform::adjust_layout(const char* b0, const char* b1)
{
    int lw, lh, maxw=0, wsize=0;

    // Find the largest label
    for(int i=0; i<container()->children(); ++i) {
        Fl_Widget* o = widget(i);
        if(field_flags[i]&FIELD_NEED_MEASURING) {
            lw=lh=0;
            o->measure_label(lw, lh);
            maxw = std::max(maxw, lw);
        }
    }

    maxw += padding()+font_size();
    for(int i=0; i<container()->children(); ++i) {
        Fl_Widget*o = widget(i);
        if(field_flags[i] & FIELD_AUTO_POSITION) // is input field
            o->position(maxw, o->y());

        wsize = std::max(wsize, o->x() + o->w());
    }

    wsize=std::max(wsize, font_size()*button_char_w*2+spacing()*2)+padding();

    // Creating buttons group (footer)
    m_footer = new Fl_Group(0, curr_y, wsize, font_size()*3);
    window()->add(m_footer);
    footer()->color(FL_LIGHT2);

    // Add the 1st button to the footer group
    int b0y=(b1)?wsize/2-font_size()*button_char_w-spacing():(wsize/2)-((font_size()*button_char_w)/2);
    Fl_Return_Button *btn0 = new Fl_Return_Button(b0y, footer()->y()+font_size()/2, font_size()*button_char_w,font_size()*2);
    btn0->copy_label(b0);
    btn0->labelsize(font_size());
    btn0->callback(b0_cb, this);

    if(b1) {         // Add 2nd button to the footer group
        Fl_Button *btn1 = new Fl_Button(wsize/2+spacing(), footer()->y()+font_size()/2, font_size()*button_char_w,font_size()*2);
        btn1->copy_label(b1);
        btn1->labelsize(font_size());
        btn1->callback(b1_cb, this);
    }
    footer()->end();

    // Resize window and align fields
    // window()->size(wsize, footer()->y()+footer()->h());
    window()->resize(100, 100, wsize, footer()->y()+footer()->h());
}

int ioform::add_field(Fl_Widget* o, const string& prompt, int flags)
{
    o->labelsize(font_size());
    o->copy_label(prompt.c_str());
    container()->add(o);
    curr_y+=(o->h()+spacing());
    int n=field_flags.size();
    field_flags.push_back(flags);
    return n;
}

//int ioform::add_spinner(const string& prompt, double minimum, double maximum, double step, int size, uchar type)
//{
//    Fl_Spinner*o =new Fl_Spinner(window()->w()/2, curr_y, (font_size()*(size+1)*0.6)+input_border_size, font_size()+input_border_size);
//    o->type(type);
//    o->textsize(font_size());
//    o->range(minimum, maximum);
//    o->step(step);
//    o->value(minimum);
//    return add_field(o, prompt, FIELD_IS_INPUT|FIELD_NEED_MEASURING|FIELD_AUTO_POSITION);
//}

int ioform::field_int_value(int i)
{
    Fl_Int_Input* pi = dynamic_cast<Fl_Int_Input*>(widget(i));
    if(pi) return to_number<int>(pi->value());

    Fl_Button* pb = dynamic_cast<Fl_Button*>(widget(i));
    if(pb) return pb->value();

    Fl_Choice* pc = dynamic_cast<Fl_Choice*>(widget(i));
    if(pc) return pc->value();

//    Fl_Spinner* ps = dynamic_cast<Fl_Spinner*>(widget(i));
//    if(ps) return ps->value();

    return 0;
}

string ioform::field_string_value(int i)
{
    Fl_Input* pi = dynamic_cast<Fl_Input*>(widget(i));
    if(pi) return string(pi->value());

//    Fl_Spinner* ps = dynamic_cast<Fl_Spinner*>(widget(i));
//    if(ps) return to_string<double>(ps->value());

    return to_string<int>(field_int_value(i));
}

double ioform::field_float_value(int i)
{
    Fl_Float_Input* pf = dynamic_cast<Fl_Float_Input*>(widget(i));
    if(pf) return to_number<double>(pf->value());

//    Fl_Spinner* ps = dynamic_cast<Fl_Spinner*>(widget(i));
//    if(ps) return ps->value();

    return double(field_int_value(i));
}

bool ioform::field_bool_value(int i)
{
    return bool(field_int_value(i));
}


int ioform::add_text(const string& prompt)
{
    Fl_Box* o= new Fl_Box(spacing(), curr_y, 340, 20);
    o->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    int n=add_field(o, prompt, FIELD_IS_OUTPUT);

    // Resize the box
    int w=0, h=0;
    o->measure_label(w,h);
    if(w<350) w=350;
    o->size(w, h);
    if(h>font_size()) curr_y+=h;
    return n;
}

string ioform::read(bool stay_open, const string& b0, const string& b1)
{
    string s="";
    if(!window()->visible())
        adjust_layout(b0.c_str(), b1.c_str());

    set_show_flag(stay_open);
    set_canceled(true);
    internal_show();

    if(!canceled()) {
        int first_input = -1;
        for(int i=0; i<container()->children(); ++i) {
            if(field_flags[i] & FIELD_IS_INPUT) {
                if(first_input==-1) first_input = i;
                s += field_string_value(i)+"\n";
            }
        }
        in().str(s);

        s = (first_input==-1)?"":field_string_value(first_input);
    }
    return s;
}

void ioform::show(string b)
{
    adjust_layout(b.c_str());
    internal_show();
    set_canceled(false);
}

void ioform::internal_show()
{
    if(!window()->visible()) {
        window()->show();

#ifdef WIN32
        HWND hw1 = fl_xid(window());
        SetClassLong(hw1,GCL_STYLE,GetClassLong(hw1,GCL_STYLE));
        SetWindowLong(hw1,GWL_STYLE,GetWindowLong(hw1,GWL_STYLE)&~WS_MINIMIZEBOX);
        SendMessage(hw1,WM_NCPAINT,0,0);
#endif
    }
    bool sf=show_flag();

    while(window()->visible() && sf==show_flag())
        Fl::wait();
}

bool ioform::validate()
{
    bool valid=true;
    for(int i=0; i<container()->children(); ++i) {
        if(!(field_flags[i]&FIELD_IS_INPUT) || field_flags[i]&FIELD_IS_OPTIONAL) continue;
        Fl_Input* pi = dynamic_cast<Fl_Input*>(widget(i));

        if(pi) {
            const char *s=pi->value();
            if(!s || !*s) {
                pi->labelcolor(FL_RED);
                pi->redraw_label();
                if(valid) pi->take_focus();
                valid = false;
            } else {
                pi->labelcolor(FL_FOREGROUND_COLOR);
                pi->redraw_label();
            }
        }
    }
    return valid;
}

void ioform::clear()
{
    container()->clear();
    Fl::delete_widget(footer());
    curr_y = padding();
    window()->label(0);
    window()->size(Fl::w(), Fl::h());
    field_flags.clear();
}

// Immediate input dialogs
int ioform::read_int(const string& prompt)
{
    add_int_input(prompt);
    string s=read();
    clear();
    return canceled()? 0 : to_number<int>(s);
}

double ioform::read_float(const string& prompt)
{
    add_float_input(prompt);
    string s=read();
    clear();
    return canceled() ? 0.0 : to_number<double>(s);
}

string ioform::read_string(const string& prompt)
{
    add_string_input(prompt, 20);
    string s=read();
    clear();
    return s;
}

string ioform::read_password(const string& prompt)
{
    add_secret_input(prompt);
    string s=read();
    clear();
    return s;
}

int ioform::read_choice(const string& prompt, const string& options)
{
    add_choice_input(prompt, options);
    string s=read();
    clear();
    return canceled()? 0 : to_number<int>(s);
}

void ioform::message(const string& prompt)
{
    ioform f(false, padding(), spacing(), font_size());
    f.window()->set_modal();
    f.add_text(prompt);
    f.show();
}

bool ioform::confirm(const string& prompt)
{
    ioform f(false, padding(), spacing(), font_size());
    f.add_text(prompt);
    f.read();
    set_canceled(f.canceled());
    return !f.canceled();
}

void ioform::show_cout()
{
    ioform f(false, padding(), spacing(), font_size());
    f.add_text(out().str());
    f.show();
    out().str("");
}

string ioform::file_chooser(int type, int options)
{
    Fl_Native_File_Chooser fc(type);
    fc.options(options);

    if (fc.show()==0) {
        set_canceled(false);
        string s = fc.filename();
        in().str(s);
        return s;
    }

    set_canceled(true);
    return "";
}


string ioform::select_input_file()
{
    return file_chooser(Fl_Native_File_Chooser::BROWSE_FILE);
}

string ioform::select_output_file(bool confirm_replace)
{
    return file_chooser(Fl_Native_File_Chooser::BROWSE_SAVE_FILE, confirm_replace?Fl_Native_File_Chooser::SAVEAS_CONFIRM:0);
}

string ioform::select_folder()
{
    return file_chooser(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
}

}
