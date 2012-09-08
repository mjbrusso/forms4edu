// Form based input dialog

#ifndef ioform_H
#define ioform_H
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Output.H>
//#include <Fl/Fl_Spinner.H>

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

namespace forms4edu
{
template<typename T>
T to_number(const string& s) {
    istringstream  oss(s);
    T n;
    oss >> n;
    return n;
}

template<typename T>
string to_string(T n, int digits=-1) {
    ostringstream  oss;
    if(digits<0) oss << n;
    else oss <<  std::fixed << std::setprecision(digits) << n;
    return oss.str();
}

enum form_field_flags {
    FIELD_IS_INPUT = 1,
    FIELD_IS_OUPUT = (1<<1),
    FIELD_NEED_MEASURING = (1<<2),
    FIELD_AUTO_POSITION = (1<<3),
    FIELD_IS_OPTIONAL = (1<<4)
};

class ioform
{
private:
    const static int input_border_size = 6, button_char_w=7;
    Fl_Window *m_wnd;
    Fl_Group* m_container, *m_footer;
    istringstream m_in;
    ostringstream m_out;
    int m_padding, m_spacing, m_fontsize, curr_y;
    bool m_canceled;
    vector<int> field_flags;

    Fl_Group* container() const {
        return m_container;
    }

    Fl_Group* footer() const{
        return m_footer;
    }

    istringstream& in(){
        return m_in;
    }

    ostringstream& out(){
        return m_out;
    }

    void adjust_layout(const char* b0=0, const char* b1=0);
    void internal_show();
    void show(string b=close_message);
    int add_field(Fl_Widget* o, const string& prompt, int flags);
//    int add_spinner(const string& prompt, double minimum, double maximum, double step, int size, uchar type);
    string file_chooser(int type, int options=0);

public:
    static string ok_message, cancel_message, close_message;
    static bool show_flag;

    ioform(bool redirect_io=false, int pad=20, int spac=6, int fontsz=14);
    virtual ~ioform();

    void set_title(const char * s) {
        window()->copy_label(s);
    }
    void set_title(string s) {
        set_title(s.c_str());
    }

    void set_padding(int p) {
        curr_y = m_padding = p;
    }
    int padding() const{
        return m_padding;
    }

    void set_spacing(int s) {
        m_spacing = s;
    }
    int spacing() const{
        return m_spacing;
    }

    void set_font_size(int s) {
        m_fontsize = s;
        fl_message_font(FL_HELVETICA, s);
    }
    int font_size() const{
        return m_fontsize;
    }

    Fl_Window* window(){
        return m_wnd;
    }

    Fl_Widget* widget(int i) const{
        return container()->child(i);
    }

    void set_canceled(bool c){
        m_canceled = c;
    }
    bool canceled() const{
        return m_canceled;
    }

    string field_string_value(int i);
    int field_int_value(int i);
    double field_float_value(int i);
    bool field_bool_value(int i);

    string read(bool stay_open=false, const string& b0=ok_message, const string& b1=cancel_message);
    void clear();
    bool validate();

    int add_string_input(const string& prompt, int size=15) {
        Fl_Input* o = new Fl_Input(window()->w()/2, curr_y, (font_size()*size)+input_border_size, font_size()+input_border_size);
        o->textsize(font_size());
        return add_field(o, prompt, FIELD_IS_INPUT|FIELD_NEED_MEASURING|FIELD_AUTO_POSITION);
    }

    int add_secret_input(const string& prompt, int size=10) {
        Fl_Secret_Input* o = new Fl_Secret_Input(window()->w()/2, curr_y, (font_size()*size)+input_border_size, font_size()+input_border_size);
        o->textsize(font_size());
        return add_field(o, prompt, FIELD_IS_INPUT|FIELD_NEED_MEASURING|FIELD_AUTO_POSITION);
    }

    int add_int_input(const string& prompt, int size=10) {
        Fl_Int_Input*o = new Fl_Int_Input(window()->w()/2, curr_y, (font_size()*size*0.6)+input_border_size, font_size()+input_border_size);
        o->textsize(font_size());
        return add_field(o, prompt, FIELD_IS_INPUT|FIELD_NEED_MEASURING|FIELD_AUTO_POSITION);
    }

    int add_float_input(const string& prompt, int size=10) {
        Fl_Float_Input*o =new Fl_Float_Input(window()->w()/2, curr_y, (font_size()*size*0.6)+input_border_size, font_size()+input_border_size);
        o->textsize(font_size());
        return add_field(o, prompt, FIELD_IS_INPUT|FIELD_NEED_MEASURING|FIELD_AUTO_POSITION);
    }

//    int add_float_spinner(const string& prompt, double minimum=0.0, double maximum=100.0, double step=0.1, int size=10) {
//        return add_spinner(prompt, minimum, maximum, step, size, FL_FLOAT_INPUT);
//    }
//
//    int add_int_spinner(const string& prompt, int minimum=0, int maximum=1000, int step=1, int size=10) {
//        return add_spinner(prompt, minimum, maximum, step, size, FL_INT_INPUT);
//    }

    int add_bool_input(const string& prompt) {
        Fl_Check_Button*o =new Fl_Check_Button(window()->w()/2, curr_y, 200, font_size());
        return add_field(o, prompt, FIELD_IS_INPUT|FIELD_AUTO_POSITION);
    }

    int add_choice_input(const string& prompt, const string& options, int size=15) {
        Fl_Choice*o =new Fl_Choice(window()->w()/2, curr_y, (font_size()*size*0.6)+input_border_size, font_size()+input_border_size);
        o->add(options.c_str());
        o->textsize(font_size());
        o->value(0);
        return add_field(o, prompt, FIELD_IS_INPUT|FIELD_NEED_MEASURING|FIELD_AUTO_POSITION);
    }

    string choice_selected_text(int field) const{
        Fl_Choice* pc = dynamic_cast<Fl_Choice*>(widget(field));
        return (pc) ? string(pc->text(pc->value())) : "";
    }

    int add_output(const string& prompt, int size=10) {
        Fl_Output* o = new Fl_Output(window()->w()/2, curr_y, (font_size()*size)+input_border_size, font_size()+input_border_size);
        o->textsize(font_size());
        o->clear_visible_focus();
        o->color(FL_GRAY);
        return add_field(o, prompt, FIELD_IS_OUPUT|FIELD_NEED_MEASURING|FIELD_AUTO_POSITION);
    }

    int add_text(const string& prompt);

    void set_field_value(int i, const string& value) const{
        Fl_Input* pi = dynamic_cast<Fl_Input*>(widget(i));
        if(pi) {
            pi->value(value.c_str());
            return;
        }
    }

    template<typename T>
    void set_field_value(int i, T value) const{
        Fl_Button* pb = dynamic_cast<Fl_Button*>(widget(i));
        if(pb) {
            pb->value((int)value);
            return;
        }
        Fl_Choice* pc = dynamic_cast<Fl_Choice*>(widget(i));
        if(pc) {
            pc->value((int)value);
            return;
        }
//        Fl_Spinner* ps = dynamic_cast<Fl_Spinner*>(widget(i));
//        if(ps) {
//            ps->value((double)value);
//            return;
//        }

        string s = to_string(value);
        set_field_value(i, s);
    }

    void show_cout();

// Immediate input dialogs
    int read_int(const string& prompt);
    double read_float(const string& prompt);
    string read_string(const string& prompt);
    string read_password(const string& prompt);
    int read_choice(const string& prompt, const string& options);
    void message(const string& prompt);
    bool confirm(const string& prompt);

    string select_input_file();
    string select_output_file(bool confirm_replace=true);
    string select_folder();
};
}
#endif // ioform_H
