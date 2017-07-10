#ifndef UIPROXY_H_INCLUDED
#define UIPROXY_H_INCLUDED

#include <map>

#include <QObject>
#include <QString>
#include <QWidget>

#include "Proxy.h"
#include "stubs.h"
#include "widgets/all.h"

class UIProxy : public QObject
{
    Q_OBJECT

public:
    virtual ~UIProxy();

    static UIProxy * getInstance(QObject *parent = 0);
    static void destroyInstance();

private:
    UIProxy(QObject *parent = 0);

    static UIProxy *instance;

public:
    static QWidget *vrepMainWindow;
    static simFloat wheelZoomFactor;

public slots:
    void onDestroy(Proxy *proxy);
    void onCreate(Proxy *proxy);
    void onButtonClick();
    void onLinkActivated(const QString &link);
    void onValueChangeInt(int value);
    void onValueChangeDouble(double value);
    void onValueChangeString(QString value);
    void onEditingFinished();
    void onPlottableClick(QCPAbstractPlottable *plottable, int index, QMouseEvent *event);
    void onLegendClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);
    void onCellActivate(int row, int col);
    void onTableSelectionChange();
    void onTreeSelectionChange();
    void onMouseEvent(Image *image, int type, bool shift, bool control, int x, int y);
    void onNodeAdded(QDataflowModelNode *node);
    void onNodeRemoved(QDataflowModelNode *node);
    void onNodeValidChanged(QDataflowModelNode *node, bool valid);
    void onNodePosChanged(QDataflowModelNode *node, QPoint pos);
    void onNodeTextChanged(QDataflowModelNode *node, QString text);
    void onNodeInletCountChanged(QDataflowModelNode *node, int count);
    void onNodeOutletCountChanged(QDataflowModelNode *node, int count);
    void onConnectionAdded(QDataflowModelConnection *conn);
    void onConnectionRemoved(QDataflowModelConnection *conn);
    void onTextChanged();
    void onAnchorClicked(const QUrl &link);
    void onViewCenterChanged(const QVector3D &viewCenter);
    void onPositionChanged(const QVector3D &position);
    // ---
    void onSetStyleSheet(Widget *widget, std::string styleSheet);
    void onSetButtonText(Button *button, std::string text);
    void onSetButtonPressed(Button *button, bool pressed);
    void onShowWindow(Window *window);
    void onHideWindow(Window *window);
    void onSetPosition(Window *window, int x, int y);
    void onSetSize(Window *window, int w, int h);
    void onSetTitle(Window *window, std::string title);
    void onSetImage(Image *image, const char *data, int w, int h);
    void onSceneChange(Window *window, int oldSceneID, int newSceneID);
    void onSetEnabled(Widget *widget, bool enabled);
    void onSetEditValue(Edit *edit, std::string value, bool suppressSignals);
    void onSetSpinboxValue(Spinbox *spinbox, double value, bool suppressSignals);
    void onSetLabelText(Label *label, std::string text, bool suppressSignals);
    void onSetSliderValue(Slider *slider, int value, bool suppressSignals);
    void onSetCheckboxValue(Checkbox *checkbox, Qt::CheckState value, bool suppressSignals);
    void onSetRadiobuttonValue(Radiobutton *radiobutton, bool value, bool suppressSignals);
    void onInsertComboboxItem(Combobox *combobox, int index, std::string text, bool suppressSignals);
    void onRemoveComboboxItem(Combobox *combobox, int index, bool suppressSignals);
    void onSetComboboxItems(Combobox *combobox, std::vector<std::string> items, int index, bool suppressSignals);
    void onSetComboboxSelectedIndex(Combobox *combobox, int index, bool suppressSignals);
    void onSetCurrentTab(Tabs *tabs, int index, bool suppressSignals);
    void onSetWidgetVisibility(Widget *widget, bool visible);
    void onReplot(Plot *plot);
    void onAddCurve(Plot *plot, int type, std::string name, std::vector<int> color, int style, curve_options *opts);
    void onAddCurveTimePoints(Plot *plot, std::string name, std::vector<double> x, std::vector<double> y);
    void onAddCurveXYPoints(Plot *plot, std::string name, std::vector<double> t, std::vector<double> x, std::vector<double> y);
    void onClearCurve(Plot *plot, std::string name);
    void onRemoveCurve(Plot *plot, std::string name);
    void onSetPlotRanges(Plot *plot, double xmin, double xmax, double ymin, double ymax);
    void onSetPlotXRange(Plot *plot, double xmin, double xmax);
    void onSetPlotYRange(Plot *plot, double ymin, double ymax);
    void onGrowPlotRanges(Plot *plot, double xmin, double xmax, double ymin, double ymax);
    void onGrowPlotXRange(Plot *plot, double xmin, double xmax);
    void onGrowPlotYRange(Plot *plot, double ymin, double ymax);
    void onSetPlotLabels(Plot *plot, std::string x, std::string y);
    void onSetPlotXLabel(Plot *plot, std::string label);
    void onSetPlotYLabel(Plot *plot, std::string label);
    void onRescaleAxes(Plot *plot, std::string name, bool onlyEnlargeX, bool onlyEnlargeY);
    void onRescaleAxesAll(Plot *plot, bool onlyEnlargeX, bool onlyEnlargeY);
    void onSetMouseOptions(Plot *plot, bool panX, bool panY, bool zoomX, bool zoomY);
    void onSetLegendVisibility(Plot *plot, bool visible);
    void onClearTable(Table *table, bool suppressSignals);
    void onSetRowCount(Table *table, int count, bool suppressSignals);
    void onSetColumnCountTable(Table *table, int count, bool suppressSignals);
    void onSetItem(Table *table, int row, int column, std::string text, bool suppressSignals);
    void onSetRowHeaderText(Table *table, int row, std::string text);
    void onSetColumnHeaderTextTable(Table *table, int column, std::string text);
    void onSetItemEditable(Table *table, int row, int column, bool editable);
    void onRestoreStateTable(Table *table, std::string state);
    void onSetRowHeight(Table *table, int row, int min_size, int max_size);
    void onSetColumnWidthTable(Table *table, int column, int min_size, int max_size);
    void onSetTableSelection(Table *table, int row, int column, bool suppressSignals);
    void onSetProgress(Progressbar *progressbar, int value);
    void onSetColumnCountTree(Tree *tree, int count, bool suppressSignals);
    void onSetColumnHeaderTextTree(Tree *tree, int column, std::string text);
    void onRestoreStateTree(Tree *tree, std::string state);
    void onSetColumnWidthTree(Tree *tree, int column, int min_size, int max_size);
    void onClearTree(Tree *tree, bool suppressSignals);
    void onAddTreeItem(Tree *tree, int item_id, int parent_id, std::vector<std::string> text, bool expanded, bool suppressSignals);
    void onUpdateTreeItemText(Tree *tree, int item_id, std::vector<std::string> text);
    void onUpdateTreeItemParent(Tree *tree, int item_id, int parent_id, bool suppressSignals);
    void onRemoveTreeItem(Tree *tree, int item_id, bool suppressSignals);
    void onSetTreeSelection(Tree *tree, int item_id, bool suppressSignals);
    void onExpandAll(Tree *tree, bool suppressSignals);
    void onCollapseAll(Tree *tree, bool suppressSignals);
    void onExpandToDepth(Tree *tree, int depth, bool suppressSignals);
    void onAddNode(Dataflow *dataflow, int id, QPoint pos, QString text, int inlets, int outlets);
    void onRemoveNode(Dataflow *dataflow, int id);
    void onSetNodeValid(Dataflow *dataflow, int id, bool valid);
    void onSetNodePos(Dataflow *dataflow, int id, QPoint pos);
    void onSetNodeText(Dataflow *dataflow, int id, QString text);
    void onSetNodeInletCount(Dataflow *dataflow, int id, int count);
    void onSetNodeOutletCount(Dataflow *dataflow, int id, int count);
    void onAddConnection(Dataflow *dataflow, int srcId, int srcOutlet, int dstId, int dstInlet);
    void onRemoveConnection(Dataflow *dataflow, int srcId, int srcOutlet, int dstId, int dstInlet);
    void onSetText(TextBrowser *textbrowser, std::string text, bool suppressSignals);
    void onSetUrl(TextBrowser *textbrowser, std::string url);
    void onAddScene3DNode(Scene3D *scene3d, int id, int parentId, int type);
    void onRemoveScene3DNode(Scene3D *scene3d, int id);
    void onSetScene3DNodeEnabled(Scene3D *scene3d, int id, bool enabled);
    void onSetScene3DIntParam(Scene3D *scene3d, int id, std::string param, int value);
    void onSetScene3DFloatParam(Scene3D *scene3d, int id, std::string param, float value);
    void onSetScene3DStringParam(Scene3D *scene3d, int id, std::string param, std::string value);
    void onSetScene3DVector2Param(Scene3D *scene3d, int id, std::string param, float x, float y);
    void onSetScene3DVector3Param(Scene3D *scene3d, int id, std::string param, float x, float y, float z);
    void onSetScene3DVector4Param(Scene3D *scene3d, int id, std::string param, float x, float y, float z, float w);

signals:
    void buttonClick(Widget *widget);
    void linkActivated(Widget *widget, QString link);
    void valueChangeInt(Widget *widget, int value);
    void valueChangeDouble(Widget *widget, double value);
    void valueChangeString(Widget *widget, QString value);
    void editingFinished(Edit *edit, QString value);
    void windowClose(Window *window);
    void loadImageFromFile(Image *image, const char *filename, int w, int h);
    void plottableClick(Plot *plot, std::string name, int index, double x, double y);
    void legendClick(Plot *plot, std::string name);
    void cellActivate(Table *table, int row, int col, std::string value);
    void tableSelectionChange(Table *table, int row, int col);
    void treeSelectionChange(Tree *tree, int id);
    void mouseEvent(Image *image, int type, bool shift, bool control, int x, int y);
    void nodeAdded(Dataflow *dataflow, int id, QPoint pos, QString text, int inlets, int outlets);
    void nodeRemoved(Dataflow *dataflow, int id);
    void nodeValidChanged(Dataflow *dataflow, int id, bool valid);
    void nodePosChanged(Dataflow *dataflow, int id, QPoint pos);
    void nodeTextChanged(Dataflow *dataflow, int id, QString text);
    void nodeInletCountChanged(Dataflow *dataflow, int id, int inlets);
    void nodeOutletCountChanged(Dataflow *dataflow, int id, int outlets);
    void connectionAdded(Dataflow *dataflow, int srcNodeId, int srcOutlet, int dstNodeId, int dstInlet);
    void connectionRemoved(Dataflow *dataflow, int srcNodeId, int srcOutlet, int dstNodeId, int dstInlet);
    void keyPressed(Widget *widget, int key, std::string text);
};

#endif // UIPROXY_H_INCLUDED

