/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2019-2021 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include <inviwo/qt/editor/welcomewidget.h>

#include <inviwo/core/util/document.h>
#include <inviwo/core/io/serialization/deserializer.h>
#include <inviwo/core/util/raiiutils.h>
#include <inviwo/core/util/logfilter.h>
#include <inviwo/core/util/stringconversion.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/properties/propertyfactory.h>
#include <inviwo/core/util/exception.h>
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/network/workspacemanager.h>

#include <inviwo/qt/editor/filetreewidget.h>
#include <inviwo/qt/editor/workspaceannotationsqt.h>
#include <inviwo/qt/editor/lineediteventfilter.h>
#include <modules/qtwidgets/inviwoqtutils.h>

#include <warn/push>
#include <warn/ignore/all>

#include <QScreen>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QFrame>
#include <QSettings>
#include <QToolButton>
#include <QPixmap>
#include <QByteArray>
#include <QFileInfo>
#include <QDateTime>
#include <QShowEvent>
#include <QKeyEvent>
#include <QAction>
#include <QFile>
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
#include <QWindow>
#endif

#include <warn/pop>

#ifndef INVIWO_ALL_DYN_LINK
struct InitQtChangelogResources {
    // Needed for loading of resources when building statically
    // see https://wiki.qt.io/QtResources#Q_INIT_RESOURCE
    InitQtChangelogResources() { Q_INIT_RESOURCE(changelog); }
    ~InitQtChangelogResources() { Q_CLEANUP_RESOURCE(changelog); }
} initQtChangelogResources;
#endif

namespace inviwo {

WelcomeWidget::WelcomeWidget(InviwoApplication* app, QWidget* parent)
    : QSplitter(parent), app_(app) {
    setObjectName("WelcomeWidget");

    setOrientation(Qt::Horizontal);
    setHandleWidth(5);

    auto leftWidget = new QWidget(this);
    auto gridLayout = new QGridLayout();
    const auto space = utilqt::refSpacePx(this);
    gridLayout->setContentsMargins(static_cast<int>(space * 1.5), 0, 0,
                                   static_cast<int>(space * 1.5));
    gridLayout->setSpacing(space);

    auto captionLayout = new QHBoxLayout();  // caption: logo + "get started"

    {
        auto inviwoLogo = new QLabel(leftWidget);
        inviwoLogo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        inviwoLogo->setPixmap(QPixmap(":/inviwo/inviwo_light.png"));
        inviwoLogo->setScaledContents(false);
        captionLayout->addWidget(inviwoLogo);
    }
    {
        auto title = new QLabel("Get Started", leftWidget);
        title->setObjectName("WelcomeHeader");
        title->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
        captionLayout->addWidget(title);
    }

    gridLayout->addLayout(captionLayout, 0, 0, 1, 2);

    // left column: workspace filter, list of recently used workspaces, and examples
    {
        filetree_ = new FileTreeWidget(app_, leftWidget);
        filetree_->setObjectName("FileTreeWidget");
        filetree_->setMinimumWidth(200);
        filetree_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QObject::connect(filetree_, &FileTreeWidget::selectedFileChanged, this,
                         [this](const QString& filename, bool isExample) {
                             updateDetails(filename);

                             loadWorkspaceBtn_->disconnect();
                             QObject::connect(loadWorkspaceBtn_, &QToolButton::clicked, this,
                                              [this, filename, isExample]() {
                                                  emit loadWorkspace(filename, isExample);
                                              });
                         });
        QObject::connect(filetree_, &FileTreeWidget::loadFile, this, &WelcomeWidget::loadWorkspace);

        filterLineEdit_ = new QLineEdit(leftWidget);
        filterLineEdit_->setPlaceholderText("Search for Workspace...");
        filterLineEdit_->installEventFilter(new LineEditEventFilter(filetree_, filterLineEdit_));

        QIcon clearIcon;
        clearIcon.addFile(":/svgicons/lineedit-clear.svg", utilqt::emToPx(this, QSizeF(0.3, 0.3)),
                          QIcon::Normal);
        clearIcon.addFile(":/svgicons/lineedit-clear-active.svg",
                          utilqt::emToPx(this, QSizeF(0.3, 0.3)), QIcon::Active);
        clearIcon.addFile(":/svgicons/lineedit-clear-active.svg",
                          utilqt::emToPx(this, QSizeF(0.3, 0.3)), QIcon::Selected);
        auto clearAction = filterLineEdit_->addAction(clearIcon, QLineEdit::TrailingPosition);
        clearAction->setVisible(false);
        connect(clearAction, &QAction::triggered, filterLineEdit_, &QLineEdit::clear);
        connect(filterLineEdit_, &QLineEdit::textChanged, this,
                [this, clearAction](const QString& str) {
                    filetree_->setFilter(str);
                    clearAction->setVisible(!str.isEmpty());
                });
    }
    gridLayout->addWidget(filterLineEdit_, 1, 0, 1, 1);
    gridLayout->addWidget(filetree_, 2, 0, 2, 1);

    // center column: workspace details and buttons for loading workspaces
    {  // workspace details
        details_ = new QTextEdit(leftWidget);
        details_->setObjectName("NetworkDetails");
        details_->setReadOnly(true);
        details_->setFrameShape(QFrame::NoFrame);
        details_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
    gridLayout->addWidget(details_, 1, 1, 2, 1);

    {  // tool buttons
        auto buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(6);
        auto createButton = [leftWidget, this](const QString& str, auto iconpath) {
            auto button = new QToolButton(leftWidget);
            button->setText(str);
            button->setIcon(QIcon(iconpath));
            button->setIconSize(utilqt::emToPx(this, QSize(7, 7)));
            button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
            return button;
        };

        loadWorkspaceBtn_ = createButton("Load", ":/svgicons/open.svg");
        loadWorkspaceBtn_->setToolTip("Open selected workspace");
        loadWorkspaceBtn_->setObjectName("LoadWorkspaceToolButton");
        buttonLayout->addWidget(loadWorkspaceBtn_);

        auto horizontalSpacer =
            new QSpacerItem(utilqt::emToPx(this, 2.0), utilqt::emToPx(this, 2.0),
                            QSizePolicy::Expanding, QSizePolicy::Minimum);
        buttonLayout->addItem(horizontalSpacer);

        auto newButton = createButton("New", ":/svgicons/newfile.svg");
        newButton->setObjectName("NewWorkspaceToolButton");
        newButton->setToolTip("Create an empty workspace");
        QObject::connect(newButton, &QToolButton::clicked, this, [this]() { emit newWorkspace(); });
        buttonLayout->addWidget(newButton);

        auto openFileButton = createButton("Open...", ":/svgicons/open.svg");
        openFileButton->setObjectName("OpenWorkspaceToolButton");
        openFileButton->setToolTip("Open workspace from disk");
        QObject::connect(openFileButton, &QToolButton::clicked, this,
                         [this]() { emit openWorkspace(); });
        buttonLayout->addWidget(openFileButton);

        restoreButton_ = createButton("Restore", ":/svgicons/revert.svg");
        restoreButton_->setObjectName("OpenWorkspaceToolButton");
        restoreButton_->setToolTip("Restore last automatically saved workspace if available");
        QObject::connect(restoreButton_, &QToolButton::clicked, this,
                         [this]() { emit restoreWorkspace(); });
        buttonLayout->addWidget(restoreButton_);
        gridLayout->addLayout(buttonLayout, 3, 1, 1, 1);

        setTabOrder(loadWorkspaceBtn_, newButton);
        setTabOrder(newButton, openFileButton);
        setTabOrder(openFileButton, restoreButton_);
        setTabOrder(restoreButton_, details_);
    }

    // add some space between center and right column
    auto horizontalSpacer_2 =
        new QSpacerItem(space, space, QSizePolicy::Fixed, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_2, 1, 2, 1, 1);

    leftWidget->setLayout(gridLayout);
    gridLayout->setRowStretch(1, 0);
    gridLayout->setRowStretch(2, 2);
    gridLayout->setRowStretch(3, 1);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 2);

    {  // right column: changelog and options
        auto rightColumn = new QFrame(this);
        rightColumn->setObjectName("WelcomeRightColumn");
        auto rightColumnLayout = new QVBoxLayout(rightColumn);
        rightColumnLayout->setContentsMargins(space, 0, 0, 2 * space);

        {
            auto title = new QLabel("Latest Changes", rightColumn);
            title->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignTop);
            title->setObjectName("ChangeLog");
            rightColumnLayout->addWidget(title);
        }

        {
            changelog_ = new QTextEdit(rightColumn);
            changelog_->setObjectName("Changelog");
            QSizePolicy sizePolicy1(changelog_->sizePolicy());
            sizePolicy1.setVerticalStretch(100);
            changelog_->setSizePolicy(sizePolicy1);
            changelog_->setFrameShape(QFrame::NoFrame);
            changelog_->setTextInteractionFlags(Qt::TextBrowserInteraction);
            changelog_->document()->setIndentWidth(utilqt::emToPx(this, 1.5));
            rightColumnLayout->addWidget(changelog_);
        }

        rightColumnLayout->addItem(
            new QSpacerItem(space, space, QSizePolicy::Minimum, QSizePolicy::Fixed));

        {
            QSettings settings;
            settings.beginGroup("InviwoMainWindow");

            auto autoloadWorkspace =
                new QCheckBox("&Automatically load most recently used workspace", rightColumn);
            autoloadWorkspace->setChecked(settings.value("autoloadLastWorkspace", true).toBool());
            QObject::connect(autoloadWorkspace, &QCheckBox::toggled, this, [](bool checked) {
                QSettings settings;
                settings.beginGroup("InviwoMainWindow");
                settings.setValue("autoloadLastWorkspace", checked);
                settings.endGroup();
            });
            rightColumnLayout->addWidget(autoloadWorkspace);

            auto showOnStartup = new QCheckBox("&Show this page on startup", rightColumn);
            showOnStartup->setChecked(settings.value("showWelcomePage", true).toBool());
            QObject::connect(showOnStartup, &QCheckBox::toggled, this, [](bool checked) {
                QSettings settings;
                settings.beginGroup("InviwoMainWindow");
                settings.setValue("showWelcomePage", checked);
                settings.endGroup();
            });
            rightColumnLayout->addWidget(showOnStartup);

            settings.endGroup();

            setTabOrder(changelog_, autoloadWorkspace);
            setTabOrder(autoloadWorkspace, showOnStartup);
        }

        addWidget(rightColumn);
    }

    // ensure that the splitter handle responds to hover events
    // see https://bugreports.qt.io/browse/QTBUG-13768
    handle(1)->setAttribute(Qt::WA_Hover);

    // hide changelog on screens with less width than 1920
    auto getScreen = []() {
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
        return utilqt::getApplicationMainWindow()->window()->windowHandle()->screen();
#else
        return utilqt::getApplicationMainWindow()->screen();
#endif
    };

    if (getScreen()->size().width() < 1920) {
        this->setSizes(QList<int>({1, 0}));
    }

    setTabOrder(filterLineEdit_, filetree_);
    setTabOrder(filetree_, loadWorkspaceBtn_);
    setTabOrder(details_, changelog_);

    initChangelog();
}

void WelcomeWidget::updateRecentWorkspaces(const QStringList& list) {
    filetree_->updateRecentWorkspaces(list);
}

void WelcomeWidget::enableRestoreButton(bool hasRestoreWorkspace) {
    restoreButton_->setEnabled(hasRestoreWorkspace);
}

void WelcomeWidget::setFilterFocus() { filterLineEdit_->setFocus(Qt::OtherFocusReason); }

void WelcomeWidget::showEvent(QShowEvent* event) {
    if (!event->spontaneous()) {
        QModelIndex index = filetree_->selectionModel()->currentIndex();

        filetree_->updateExampleEntries();
        filetree_->updateRegressionTestEntries();

        filetree_->expandItems();

        if (index.isValid()) {
            filetree_->selectionModel()->setCurrentIndex(
                index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        } else {
            filetree_->selectRecentWorkspace(0);
        }
    }
    QWidget::showEvent(event);
}

void WelcomeWidget::keyPressEvent(QKeyEvent* event) {
    if ((event->key() >= Qt::Key_0) && (event->key() <= Qt::Key_9)) {
        int number = [key = event->key()]() {
            if (key == Qt::Key_0) return 9;
            return key - Qt::Key_1;
        }();
        if (filetree_->selectRecentWorkspace(number)) {
            if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
                loadWorkspaceBtn_->animateClick();
            }
            event->accept();
        }
    } else if ((event->key() == Qt::Key_Return) || (event->key() == Qt::Key_Enter)) {
        loadWorkspaceBtn_->animateClick();
        event->accept();
    }
    QWidget::keyPressEvent(event);
}

void WelcomeWidget::initChangelog() {
    QFile file(":/changelog.html");
    if (file.open(QFile::ReadOnly | QFile::Text) && file.size() > 0) {
        changelog_->setHtml(file.readAll());
    } else {
        changelog_->setHtml(
            R"(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<style type="text/css">
    body {color: #9d9995;}
    a {color: #268bd2;}
    a:visited { color: #1E6A9E; }
    h1 { font-size: xx-large; color: #268bd2; margin-bottom:1em; }
    h2 { font-size: larger; color: #268bd2; margin-top:1em; margin-bottom:0em; }
    p { margin-bottom: 0.2em; margin-top: 0.1em; }
</style>
</head>
<body >
<p>For latest changes see <a href='https://github.com/inviwo/inviwo/blob/master/CHANGELOG.md'>https://github.com/inviwo/inviwo/blob/master/CHANGELOG.md</a>.
</body>
</html>
    )");
    }
}

void WelcomeWidget::updateDetails(const QString& filename) {
    QFileInfo info(filename);
    if (filename.isEmpty() || !info.exists()) {
        details_->clear();
        loadWorkspaceBtn_->setEnabled(false);
        return;
    }

    loadWorkspaceBtn_->setEnabled(true);

    // extract annotations including network screenshot and canvas images from workspace
    WorkspaceAnnotationsQt annotations;
    bool fileBroken = false;
    try {
        auto istream = filesystem::ifstream(utilqt::fromQString(filename));
        if (istream.is_open()) {
            LogFilter logger{LogCentral::getPtr(), LogVerbosity::None};
            auto d = app_->getWorkspaceManager()->createWorkspaceDeserializer(
                istream, utilqt::fromQString(filename), &logger);
            d.setExceptionHandler([](const ExceptionContext) {});
            d.deserialize("WorkspaceAnnotations", annotations);
        } else {
            fileBroken = true;
        }
    } catch (Exception& e) {
        util::log(e.getContext(), e.getMessage(), LogLevel::Warn);
        fileBroken = "true";
    }

    const auto dateformat = "yyyy-MM-dd hh:mm:ss";
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    auto createdStr = utilqt::fromQString(info.created().toString(dateformat));
#else
    auto createdStr = utilqt::fromQString(info.birthTime().toString(dateformat));
#endif
    auto modifiedStr = utilqt::fromQString(info.lastModified().toString(dateformat));

    const bool hasTitle = !annotations.getTitle().empty();
    auto titleStr =
        hasTitle ? annotations.getTitle() : utilqt::fromQString(info.completeBaseName());
    auto description = htmlEncode(annotations.getDescription());
    replaceInString(description, "\n", "<br/>");

    if (titleStr.size() > 0) titleStr[0] = static_cast<char>(std::toupper(titleStr[0]));
    replaceInString(titleStr, "-", " ");
    replaceInString(titleStr, "_", " ");

    Document doc;
    auto body = doc.append("html").append("body");
    body.append("div", titleStr, {{"style", "font-size:45pt; color:#268bd2"}});

    auto table = body.append("table", "", {{"style", "margin-bottom:1em;"}});
    auto addrow = [&](std::string_view name, std::string_view value) {
        if (value.empty()) return;
        auto tr = table.append("tr");
        tr.append("td", name, {{"style", "text-align:right;"}});
        tr.append("td", htmlEncode(value), {{"style", "color:#ebe5df; font-weight:500;"}});
    };

    addrow("File", utilqt::fromQString(info.fileName()));
    addrow("Modified", modifiedStr);
    addrow("Created", createdStr);
    addrow("Author", annotations.getAuthor());
    addrow("Tags", annotations.getTags());
    addrow("Categories", annotations.getCategories());
    addrow("Description", description);

    if (fileBroken) {
        body.append("h3", "Workspace file could not be opened!",
                    {{"style", "color:firebrick;font-weight:600;"}});
    }
    auto content = body.append("span");
    auto addImage = [&content](auto item) {
        const int fixedImgHeight = 256;
        if (!item.isValid()) return;
        auto img = content.append("table", "", {{"style", "float: left;"}});
        img.append("tr").append("td").append("h3", item.name, {{"style", "font-weight:600;"}});
        img.append("tr").append("td").append(
            "img", "",
            {{"height", std::to_string(std::min(fixedImgHeight, item.size.y))},
             {"src", "data:image/jpeg;base64," + item.base64jpeg}});
    };

    for (auto& elem : annotations.getCanvasImages()) {
        addImage(elem);
    }
    addImage(annotations.getNetworkImage());

    std::string txt = doc;

    details_->setHtml(utilqt::toQString(doc));
}

}  // namespace inviwo
