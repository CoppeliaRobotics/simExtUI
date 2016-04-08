#include "UIModel.h"
#include "UIProxy.h"

#include <cstring>
#include <iostream>
#include <sstream>

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QDialog>
#include <QLayout>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>

using tinyxml2::XML_NO_ERROR;

int Widget::nextId = 1000000;
std::map<int, Widget *> Widget::widgets;
std::map<QWidget *, Widget *> Widget::widgetByQWidget;

Widget::Widget()
    : qwidget(NULL)
{
    // don't do this here because id is set by user:
    // Widget::widgets[id] = this;
}

Widget::~Widget()
{
    // this should be destroyed from the UI thread

    if(qwidget)
    {
        qwidget->deleteLater();

        Widget::widgetByQWidget.erase(qwidget);
    }

    Widget::widgets.erase(id);
}

Widget * Widget::byId(int id)
{
    try
    {
        return Widget::widgets[id];
    }
    catch(std::exception& ex)
    {
        return NULL;
    }
}

Widget * Widget::byQWidget(QWidget *w)
{
    try
    {
        return Widget::widgetByQWidget[w];
    }
    catch(std::exception& ex)
    {
        return NULL;
    }
}

template<typename T>
Widget * Widget::tryParse(XMLElement *e)
{
    std::vector<std::string> errors;
    T *obj = new T;
    if(obj->parse(e, errors))
    {
        Widget::widgets[obj->id] = obj;
        return obj;
    }
    else
    {
        delete obj;
        return NULL;
    }
}

Widget * Widget::parseAny(XMLElement *e, std::vector<std::string>& errors)
{
    Widget *w = NULL;
    if((w = tryParse<Button>(e))) return w;
    if((w = tryParse<Edit>(e))) return w;
    if((w = tryParse<HSlider>(e))) return w;
    if((w = tryParse<VSlider>(e))) return w;
    if((w = tryParse<Label>(e))) return w;
    if((w = tryParse<Checkbox>(e))) return w;
    if((w = tryParse<Radiobutton>(e))) return w;
    if((w = tryParse<Group>(e))) return w;

    std::string tag(e->Value());
    std::stringstream ss;
    ss << "invalid element: <" << tag << ">";
    errors.push_back(ss.str());
    return w;
}

bool layoutFromString(const char *s, Layout *l)
{
    std::string n(s);
    if(n == "vbox")
    {
        *l = VBOX;
        return true;
    }
    if(n == "hbox")
    {
        *l = HBOX;
        return true;
    }
    if(n == "grid")
    {
        *l = GRID;
        return true;
    }
    if(n == "form")
    {
        *l = FORM;
        return true;
    }
    return false;
}

bool Widget::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!e->Attribute("id") || e->QueryIntAttribute("id", &id) != XML_NO_ERROR)
    {
        id = nextId++;
    }

    return true;
}

Button::Button()
    : Widget()
{
}

Button::~Button()
{
}

bool Button::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "button")
    {
        errors.push_back("element must be <button>");
        return false;
    }

    if(e->Attribute("text")) text = e->Attribute("text");
    else text = "???";

    if(e->Attribute("onclick")) onclick = e->Attribute("onclick");
    else onclick = "";

    return true;
}

QWidget * Button::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QPushButton *button = new QPushButton(QString::fromStdString(text), parent);
    QObject::connect(button, SIGNAL(released()), uiproxy, SLOT(onButtonClick()));
    qwidget = button;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return button;
}

Edit::Edit()
    : Widget()
{
}

Edit::~Edit()
{
}

bool Edit::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "edit")
    {
        errors.push_back("element must be <edit>");
        return false;
    }

    if(e->Attribute("onchange")) onchange = e->Attribute("onchange");
    else onchange = "";

    return true;
}

QWidget * Edit::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QLineEdit *edit = new QLineEdit(parent);
    QObject::connect(edit, SIGNAL(textChanged(QString)), uiproxy, SLOT(onValueChange(QString)));
    qwidget = edit;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return edit;
}

HSlider::HSlider()
    : Widget()
{
}

HSlider::~HSlider()
{
}

bool HSlider::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "hslider")
    {
        errors.push_back("element must be <hslider>");
        return false;
    }

    if(!e->Attribute("minimum") || e->QueryIntAttribute("minimum", &minimum) != XML_NO_ERROR)
        minimum = 0;

    if(!e->Attribute("maximum") || e->QueryIntAttribute("maximum", &maximum) != XML_NO_ERROR)
        maximum = 100;

    if(e->Attribute("onchange")) onchange = e->Attribute("onchange");
    else onchange = "";

    return true;
}

QWidget * HSlider::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QSlider *slider = new QSlider(Qt::Horizontal, parent);
    slider->setMinimum(minimum);
    slider->setMaximum(maximum);
    QObject::connect(slider, SIGNAL(valueChanged(int)), uiproxy, SLOT(onValueChange(int)));
    qwidget = slider;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return slider;
}

VSlider::VSlider()
    : Widget()
{
}

VSlider::~VSlider()
{
}

bool VSlider::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "vslider")
    {
        errors.push_back("element must be <vslider>");
        return false;
    }

    if(!e->Attribute("minimum") || e->QueryIntAttribute("minimum", &minimum) != XML_NO_ERROR)
        minimum = 0;

    if(!e->Attribute("maximum") || e->QueryIntAttribute("maximum", &maximum) != XML_NO_ERROR)
        maximum = 100;

    if(e->Attribute("onchange")) onchange = e->Attribute("onchange");
    else onchange = "";

    return true;
}

QWidget * VSlider::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QSlider *slider = new QSlider(Qt::Vertical, parent);
    slider->setMinimum(minimum);
    slider->setMaximum(maximum);
    QObject::connect(slider, SIGNAL(valueChanged(int)), uiproxy, SLOT(onValueChange(int)));
    qwidget = slider;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return slider;
}

Label::Label()
    : Widget()
{
}

Label::~Label()
{
}

bool Label::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "label")
    {
        errors.push_back("element must be <label>");
        return false;
    }

    if(e->Attribute("text")) text = e->Attribute("text");
    else text = "";

    return true;
}

QWidget * Label::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QLabel *label = new QLabel(QString::fromStdString(text), parent);
    qwidget = label;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return label;
}

Checkbox::Checkbox()
    : Widget()
{
}

Checkbox::~Checkbox()
{
}

bool Checkbox::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "checkbox")
    {
        errors.push_back("element must be <checkbox>");
        return false;
    }

    if(e->Attribute("text")) text = e->Attribute("text");
    else text = "???";

    if(e->Attribute("onchange")) onchange = e->Attribute("onchange");
    else onchange = "";

    return true;
}

QWidget * Checkbox::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QCheckBox *checkbox = new QCheckBox(QString::fromStdString(text), parent);
    QObject::connect(checkbox, SIGNAL(stateChanged(int)), uiproxy, SLOT(onValueChange(int)));
    qwidget = checkbox;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return checkbox;
}

Radiobutton::Radiobutton()
    : Widget()
{
}

Radiobutton::~Radiobutton()
{
}

bool Radiobutton::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "radiobutton")
    {
        errors.push_back("element must be <radiobutton>");
        return false;
    }

    if(e->Attribute("text")) text = e->Attribute("text");
    else text = "";

    if(e->Attribute("onclick")) onclick = e->Attribute("onclick");
    else onclick = "";

    return true;
}

QWidget * Radiobutton::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QRadioButton *button = new QRadioButton(QString::fromStdString(text), parent);
    QObject::connect(button, SIGNAL(released()), uiproxy, SLOT(onButtonClick()));
    qwidget = button;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return button;
}

Group::Group()
    : Widget()
{
}

Group::~Group()
{
    for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
    {
        for(std::vector<Widget*>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
        {
            delete *it2;
        }
    }
}

bool Group::parse(XMLElement *e, std::vector<std::string>& errors)
{
    if(!Widget::parse(e, errors)) return false;

    std::string tag(e->Value());
    if(tag != "group")
    {
        errors.push_back("element must be <group>");
        return false;
    }

    if(e->Attribute("text")) text = e->Attribute("text");
    else text = "";

    if(e->Attribute("layout"))
    {
        if(!layoutFromString(e->Attribute("layout"), &layout))
        {
            errors.push_back("invalid value for attribute 'layout'");
            return false;
        }
    }
    else layout = VBOX;

    std::vector<Widget*> row;
    for(XMLElement *e1 = e->FirstChildElement(); e1; e1 = e1->NextSiblingElement())
    {
        std::string tag1(e1->Value());

        if(tag1 == "br" && layout == GRID)
        {
            children.push_back(row);
            row.clear();
            continue;
        }

        Widget *w = Widget::parseAny(e1, errors);
        if(!w) return false;
        row.push_back(w);

        if((layout == FORM && row.size() == 2) ||
                layout == VBOX ||
                layout == HBOX)
        {
            children.push_back(row);
            row.clear();
        }
    }
    if(row.size() > 0 && layout == GRID)
    {
        children.push_back(row);
        row.clear();
    }
    if(row.size() > 0)
    {
        errors.push_back("extra elements in layout");
        return false;
    }

    return true;
}

QWidget * Group::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QGroupBox *groupBox = new QGroupBox(QString::fromStdString(text), parent);
    switch(layout)
    {
    case VBOX:
    case HBOX:
        {
            QBoxLayout *qlayout;
            if(layout == VBOX) qlayout = new QVBoxLayout(groupBox);
            if(layout == HBOX) qlayout = new QHBoxLayout(groupBox);
            for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
            {
                QWidget *w = (*it)[0]->createQtWidget(proxy, uiproxy, groupBox);
                qlayout->addWidget(w);
            }
            groupBox->setLayout(qlayout);
        }
        break;
    case GRID:
        {
            QGridLayout *qlayout = new QGridLayout(groupBox);
            int row = 0, col = 0;
            for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
            {
                col = 0;
                for(std::vector<Widget*>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
                {
                    QWidget *w = (*it2)->createQtWidget(proxy, uiproxy, groupBox);
                    qlayout->addWidget(w, row, col);
                    col++;
                }
                row++;
            }
            groupBox->setLayout(qlayout);
        }
        break;
    case FORM:
        {
            QFormLayout *qlayout = new QFormLayout(groupBox);
            for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
            {
                QWidget *w1 = (*it)[0]->createQtWidget(proxy, uiproxy, groupBox);
                QWidget *w2 = (*it)[1]->createQtWidget(proxy, uiproxy, groupBox);
                qlayout->addRow(w1, w2);
            }
            groupBox->setLayout(qlayout);
        }
        break;
    }
    qwidget = groupBox;
    Widget::widgetByQWidget[qwidget] = this;
    this->proxy = proxy;
    return groupBox;
}

Window::Window()
{
}

Window::~Window()
{
    for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
    {
        for(std::vector<Widget*>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
        {
            delete *it2;
        }
    }
}

bool Window::parse(XMLElement *e, std::vector<std::string>& errors)
{
    std::string tag(e->Value());
    if(tag != "ui")
    {
        errors.push_back("root element must be <ui>");
        return false;
    }

    if(e->Attribute("title")) title = e->Attribute("title");
    else title = "Custom UI";

    if(e->Attribute("layout"))
    {
        if(!layoutFromString(e->Attribute("layout"), &layout))
        {
            errors.push_back("invalid value for attribute 'layout'");
            return false;
        }
    }
    else layout = VBOX;

    std::vector<Widget*> row;
    for(XMLElement *e1 = e->FirstChildElement(); e1; e1 = e1->NextSiblingElement())
    {
        std::string tag1(e1->Value());

        if(tag1 == "br" && layout == GRID)
        {
            children.push_back(row);
            row.clear();
            continue;
        }

        Widget *w = Widget::parseAny(e1, errors);
        if(!w) return false;
        row.push_back(w);

        if((layout == FORM && row.size() == 2) ||
                layout == VBOX ||
                layout == HBOX)
        {
            children.push_back(row);
            row.clear();
        }
    }
    if(row.size() > 0 && layout == GRID)
    {
        children.push_back(row);
        row.clear();
    }
    if(row.size() > 0)
    {
        errors.push_back("extra elements in layout");
        return false;
    }

    return true;
}

QWidget * Window::createQtWidget(Proxy *proxy, UIProxy *uiproxy, QWidget *parent)
{
    QDialog *window = new QDialog(parent, Qt::Tool);
    switch(layout)
    {
    case VBOX:
    case HBOX:
        {
            QBoxLayout *qlayout;
            if(layout == VBOX) qlayout = new QVBoxLayout(window);
            if(layout == HBOX) qlayout = new QHBoxLayout(window);
            for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
            {
                QWidget *w = (*it)[0]->createQtWidget(proxy, uiproxy, window);
                qlayout->addWidget(w);
            }
            window->setLayout(qlayout);
        }
        break;
    case GRID:
        {
            QGridLayout *qlayout = new QGridLayout(window);
            int row = 0, col = 0;
            for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
            {
                col = 0;
                for(std::vector<Widget*>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
                {
                    QWidget *w = (*it2)->createQtWidget(proxy, uiproxy, window);
                    qlayout->addWidget(w, row, col);
                    col++;
                }
                row++;
            }
            window->setLayout(qlayout);
        }
        break;
    case FORM:
        {
            QFormLayout *qlayout = new QFormLayout(window);
            for(std::vector< std::vector<Widget*> >::iterator it = children.begin(); it != children.end(); ++it)
            {
                QWidget *w1 = (*it)[0]->createQtWidget(proxy, uiproxy, window);
                QWidget *w2 = (*it)[1]->createQtWidget(proxy, uiproxy, window);
                qlayout->addRow(w1, w2);
            }
            window->setLayout(qlayout);
        }
        break;
    }
    window->setWindowTitle(QString::fromStdString(title));
    window->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
    qwidget = window;
    this->proxy = proxy;
    return window;
}
