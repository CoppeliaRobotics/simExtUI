#include "debug.h"

#include "XMLUtils.h"

#include "UIProxy.h"

#include <boost/format.hpp>

std::map<QWidget *, Widget *> Widget::widgetByQWidget;
std::set<Widget *> Widget::widgets;

Widget::Widget(std::string widgetClass_)
    : qwidget(NULL),
      proxy(NULL),
      widgetClass(widgetClass_)
{
    // don't do this here because id is set by user:
    // Widget::widgets[id] = this;

    DBG << "this=" << this << ", widgetClass=" << widgetClass << std::endl;
}

Widget::~Widget()
{
    Widget::widgets.erase(this);

    DBG << "this=" << this << ", id=" << id << ", widgetClass=" << widgetClass << std::endl;

    // this should be destroyed from the UI thread

    if(qwidget)
    {
        //DBG << this << "  delete 'qwidget' member (deleteLater())" << std::endl;

        //qwidget->deleteLater();

        Widget::widgetByQWidget.erase(qwidget);
    }

    if(proxy)
    {
        proxy->widgets.erase(id);
    }
}

void Widget::setQWidget(QWidget *qwidget_)
{
    if(qwidget)
        throw std::runtime_error("qwidget has been already set");

    qwidget = qwidget_;
    Widget::widgetByQWidget[qwidget] = this;
}

void Widget::setProxy(Proxy *proxy_)
{
    proxy = proxy_;
    proxy->widgets[id] = this;
}

Widget * Widget::byId(int handle, int id)
{
    Proxy *proxy = Proxy::byHandle(handle);
    if(!proxy) return NULL;
    return proxy->getWidgetById(id);
}

Widget * Widget::byQWidget(QWidget *w)
{
    std::map<QWidget*, Widget*>::const_iterator it = Widget::widgetByQWidget.find(w);
    Widget *ret = it == Widget::widgetByQWidget.end() ? NULL : it->second;
    return ret;
}

bool Widget::exists(Widget *w)
{
    return Widget::widgets.find(w) != Widget::widgets.end();
}

Widget * Widget::parseAny(Widget *parent, std::map<int, Widget*>& widgets, tinyxml2::XMLElement *e)
{
    std::string tag(e->Value());
    if(tag == "button") return parse1<Button>(parent, widgets, e);
    if(tag == "edit") return parse1<Edit>(parent, widgets, e);
    if(tag == "hslider") return parse1<HSlider>(parent, widgets, e);
    if(tag == "vslider") return parse1<VSlider>(parent, widgets, e);
    if(tag == "label") return parse1<Label>(parent, widgets, e);
    if(tag == "checkbox") return parse1<Checkbox>(parent, widgets, e);
    if(tag == "radiobutton") return parse1<Radiobutton>(parent, widgets, e);
    if(tag == "spinbox") return parse1<Spinbox>(parent, widgets, e);
    if(tag == "combobox") return parse1<Combobox>(parent, widgets, e);
    if(tag == "group") return parse1<Group>(parent, widgets, e);
    if(tag == "tabs") return parse1<Tabs>(parent, widgets, e);
    if(tag == "stretch") return parse1<Stretch>(parent, widgets, e);
    if(tag == "image") return parse1<Image>(parent, widgets, e);
    if(tag == "plot") return parse1<Plot>(parent, widgets, e);
    if(tag == "table") return parse1<Table>(parent, widgets, e);
    if(tag == "tree") return parse1<Tree>(parent, widgets, e);
    if(tag == "progressbar") return parse1<Progressbar>(parent, widgets, e);
    if(tag == "dataflow") return parse1<Dataflow>(parent, widgets, e);
    if(tag == "text-browser") return parse1<TextBrowser>(parent, widgets, e);

    throw std::range_error((boost::format("invalid element <%s>") % tag).str());
}

void Widget::parse(Widget *parent, std::map<int, Widget*>& widgets, tinyxml2::XMLElement *e)
{
    if(xmlutils::hasAttr(e, "id"))
    {
        id = xmlutils::getAttrInt(e, "id", 0);
        // user defined IDs must be positive (see below)
        if(id <= 0)
        {
            throw std::range_error("id must be a positive integer");
        }
    }
    else
    {
        // automatically assigned IDs are negative, so we can distinguish them
        // in diagnostics
        static int nextId = -1;
        id = nextId--;
    }

    geometry.isSet = false;
    if(xmlutils::hasAttr(e, "geometry"))
    {
        std::vector<int> geom = xmlutils::getAttrIntV(e, "geometry", "-1,-1,-1,-1", 4, 4, ",");
        geometry.isSet = true;
        geometry.x = geom[0];
        geometry.y = geom[1];
        geometry.width = geom[2];
        geometry.height = geom[3];
    }

    style = xmlutils::getAttrStr(e, "style", "");

    enabled = xmlutils::getAttrBool(e, "enabled", true);

    visible = xmlutils::getAttrBool(e, "visible", true);

    std::string tag(e->Value());
    if(tag != widgetClass)
    {
        throw std::range_error((boost::format("element must be <%s>") % widgetClass).str());
    }

    Widget::widgets.insert(this);
}

