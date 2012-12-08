//==============================================================================
// Collapsible widget
//==============================================================================

#include "collapsiblewidget.h"
#include "coreutils.h"

//==============================================================================

#include <QLabel>
#include <QLayout>
#include <QSettings>
#include <QToolButton>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CollapsibleHeaderWidget::CollapsibleHeaderWidget(const QColor &pSeparatorColor,
                                                 QWidget *pParent) :
    QWidget(pParent),
    mCollapsed(false),
    mLastHeader(false)
{
    // Create our main (vertical) layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setMargin(0);
    layout->setSpacing(0);

    setLayout(layout);

    // Create a sub-widget with a horizontal layout

    QWidget *subWidget = new QWidget(this);

    QHBoxLayout *subLayout = new QHBoxLayout(subWidget);

    subLayout->setMargin(0);
    subLayout->setSpacing(0);

    subWidget->setLayout(subLayout);

    // Create and customise our button and title

    mButton = new QToolButton(subWidget);
    mTitle  = new QLabel(QString(), subWidget);

    int iconSize = 0.4*mTitle->height();

    mButton->setIcon(QIcon(":/oxygen/actions/arrow-down.png"));
    mButton->setIconSize(QSize(iconSize, iconSize));
    mButton->setStyleSheet("QToolButton {"
                           "    border: 0px;"
                           "}"
                           ""
                           "QToolButton::pressed {"
                           "    margin: 0px;"
                           "}");

    mTitle->setAlignment(Qt::AlignCenter);

    // Add our button and title to our sub-layout

    subLayout->addWidget(mButton);
    subLayout->addWidget(mTitle);

    // Add a top separator, our sub-widget and a bottom separator to our main
    // layout
    // Note: we keep track of our top and bottom separators since we may need
    //       to hide them in some cases/circumstances...

    mTopSeparator    = Core::newLineWidget(pSeparatorColor, this);
    mBottomSeparator = Core::newLineWidget(pSeparatorColor, this);

    layout->addWidget(mTopSeparator);
    layout->addWidget(subWidget);
    layout->addWidget(mBottomSeparator);

    // Create a connection to keep track of the collapsed state of our header

    connect(mButton, SIGNAL(clicked()),
            this, SLOT(toggleCollapsedState()));
}

//==============================================================================

void CollapsibleHeaderWidget::setFirstHeader(const bool &pFirstHeader)
{
    // Show/hide our top separator depending on whether we are the first header

    mTopSeparator->setVisible(!pFirstHeader);
}

//==============================================================================

void CollapsibleHeaderWidget::setLastHeader(const bool &pLastHeader)
{
    // Keep track of our last header status and update our bottom separator
    // visible status, if needed

    if (pLastHeader != mLastHeader) {
        mLastHeader = pLastHeader;

        updateBottomSeparatorVisibleStatus();
    }
}

//==============================================================================

bool CollapsibleHeaderWidget::isCollapsed() const
{
    // Return wheter we are collapsed

    return mCollapsed;
}

//==============================================================================

void CollapsibleHeaderWidget::setCollapsed(const bool &pCollapsed)
{
    // Collapse or uncollapse ourselves, if needed

    if (pCollapsed != mCollapsed)
        toggleCollapsedState();
}

//==============================================================================

QString CollapsibleHeaderWidget::title() const
{
    // Return our title

    return mTitle->text();
}

//==============================================================================

void CollapsibleHeaderWidget::setTitle(const QString &pTitle)
{
    // Set our title

    mTitle->setText(pTitle);
}

//==============================================================================

void CollapsibleHeaderWidget::updateBottomSeparatorVisibleStatus()
{
    // Show/hide our bottom separator depending on whether we are collapsed or
    // whether we are the last header

    mBottomSeparator->setVisible(!mCollapsed || mLastHeader);
}

//==============================================================================

void CollapsibleHeaderWidget::toggleCollapsedState()
{
    // Toggle our collapsed state

    mCollapsed = !mCollapsed;

    // Update our button's icon to reflect our new collapsed state

    if (mCollapsed)
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-right.png"));
    else
        mButton->setIcon(QIcon(":/oxygen/actions/arrow-down.png"));

    // Update our bottom separator visible status

    updateBottomSeparatorVisibleStatus();

    // Let people know about our new new collapsed state

    emit widgetVisible(!mCollapsed);
}

//==============================================================================

void CollapsibleWidget::constructor(const QColor &pSeparatorColor)
{
    // Some initialisations

    mSeparatorColor = pSeparatorColor;

    mHeaders = QList<CollapsibleHeaderWidget *>();

    // Create a vertical layout which will contain our headers and widgets

    mLayout = new QVBoxLayout(this);

    mLayout->setMargin(0);
    mLayout->setSpacing(0);

    setLayout(mLayout);

    // Add a stretch to our layout so that its contents will take as little
    // space as possible

    mLayout->addStretch(1);
}

//==============================================================================

CollapsibleWidget::CollapsibleWidget(const QColor &pSeparatorColor,
                                     QWidget *pParent) :
    Widget(QSize(), pParent)
{
    // Construct our object

    constructor(pSeparatorColor);
}

//==============================================================================

CollapsibleWidget::CollapsibleWidget(QWidget *pParent) :
    Widget(QSize(), pParent)
{
    // Construct our object

    constructor();
}

//==============================================================================

static const QString SettingsCollapsed = "Collapsed%1";

//==============================================================================

void CollapsibleWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve our collapsable state

    for (int i = 0, iMax = mHeaders.count(); i < iMax; ++i)
        setCollapsed(i, pSettings->value(SettingsCollapsed.arg(i), false).toBool());
}

//==============================================================================

void CollapsibleWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our collapsable state

    for (int i = 0, iMax = mHeaders.count(); i < iMax; ++i)
        pSettings->setValue(SettingsCollapsed.arg(i), isCollapsed(i));
}

//==============================================================================

bool CollapsibleWidget::isCollapsed(const int &pIndex) const
{
    // Return wheter our requested header is collapsed

    return mHeaders.value(pIndex)?mHeaders.at(pIndex)->isCollapsed():false;
}

//==============================================================================

void CollapsibleWidget::setCollapsed(const int &pIndex, const bool &pCollapsed)
{
    // Collapse or uncollapse our requested header

    if (mHeaders.value(pIndex))
        mHeaders.at(pIndex)->setCollapsed(pCollapsed);
}

//==============================================================================

QString CollapsibleWidget::headerTitle(const int &pIndex) const
{
    // Return the title of our requested header

    return mHeaders.value(pIndex)?mHeaders.at(pIndex)->title():QString();
}

//==============================================================================

void CollapsibleWidget::setHeaderTitle(const int &pIndex, const QString &pTitle)
{
    // Set the title of our requested header

    if (mHeaders.value(pIndex))
        mHeaders.at(pIndex)->setTitle(pTitle);
}

//==============================================================================

void CollapsibleWidget::addWidget(QWidget *pWidget)
{
    // Make sure that there is a widget to add

    if (!pWidget)
        return;

    // We want to add a new widget, so we first need to add a new header to our
    // layout

    int beforeIndex = 2*mHeaders.count();

    CollapsibleHeaderWidget *header = new CollapsibleHeaderWidget(mSeparatorColor, this);

    // Let our header know whether it is the first header

    header->setFirstHeader(!beforeIndex);

    // Let our header know that, if anything, it is our new last header, meaning
    // that our previous header is not the last one anymore

    if (!mHeaders.isEmpty())
        mHeaders.last()->setLastHeader(false);

    header->setLastHeader(true);

    // Now, we can actually add the header to our layout, as well as keep track
    // of it

    mLayout->insertWidget(beforeIndex, header);

    mHeaders.append(header);

    // Add the new widget itself

    mLayout->insertWidget(++beforeIndex, pWidget);

    // Create a connection to show/hide our widget depending on the collapsed
    // state of our header

    connect(header, SIGNAL(widgetVisible(const bool &)),
            pWidget, SLOT(setVisible(bool)));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
