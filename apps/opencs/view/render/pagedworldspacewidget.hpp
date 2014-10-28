#ifndef OPENCS_VIEW_PAGEDWORLDSPACEWIDGET_H
#define OPENCS_VIEW_PAGEDWORLDSPACEWIDGET_H

#include <map>

#include "../../model/world/cellselection.hpp"

#include "worldspacewidget.hpp"
#include "cell.hpp"

class QElapsedTimer;

namespace CSVRender
{

    class TextOverlay;
    class OverlayMask;

    class PagedWorldspaceWidget : public WorldspaceWidget
    {
            Q_OBJECT

            CSMDoc::Document& mDocument;
            CSMWorld::CellSelection mSelection;
            std::map<CSMWorld::CellCoordinates, Cell *> mCells;
            std::string mWorldspace;
            CSVWidget::SceneToolToggle *mControlElements;
            bool mDisplayCellCoord;
            std::map<CSMWorld::CellCoordinates, TextOverlay *> mTextOverlays;
            OverlayMask *mOverlayMask;

            enum MouseState {
                Mouse_Grab,
                Mouse_Drag,
                Mouse_Edit,
                Mouse_Default
            };
            MouseState mMouseState;
            QPoint mOldPos;
            std::string mCurrentObj;
            QElapsedTimer *mMouseEventTimer;
            Ogre::Plane *mPlane;
            Ogre::SceneNode *mObjSceneNode;
            Ogre::Vector3 mOrigObjPos;
            Ogre::Vector3 mOrigMousePos;

        private:

            std::pair<int, int> getCoordinatesFromId(const std::string& record) const;

            /// Bring mCells into sync with mSelection again.
            ///
            /// \return Any cells added or removed?
            bool adjustCells();

            virtual void referenceableDataChanged (const QModelIndex& topLeft,
                const QModelIndex& bottomRight);

            virtual void referenceableAboutToBeRemoved (const QModelIndex& parent, int start, int end);

            virtual void referenceableAdded (const QModelIndex& index, int start, int end);

            virtual void referenceDataChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);

            virtual void referenceAboutToBeRemoved (const QModelIndex& parent, int start, int end);

            virtual void referenceAdded (const QModelIndex& index, int start, int end);

            virtual std::string getStartupInstruction();

            std::pair<std::string, Ogre::Vector3> isObjectUnderCursor(float mouseX, float mouseY);
            std::pair<bool, Ogre::Vector3> mousePositionOnPlane(QMouseEvent *event, Ogre::Plane &plane);
            void debugMousePicking(float mouseX, float mouseY);
            void updateSelectionHighlight(std::string sceneNode, const Ogre::Vector3 &position);

        public:

            PagedWorldspaceWidget (QWidget *parent, CSMDoc::Document& document);
            ///< \note Sets the cell area selection to an invalid value to indicate that currently
            /// no cells are displayed. The cells to be displayed will be specified later through
            /// hint system.

            virtual ~PagedWorldspaceWidget();

            void useViewHint (const std::string& hint);

            void setCellSelection(const CSMWorld::CellSelection& selection);

            /// \return Drop handled?
            virtual bool handleDrop (const std::vector<CSMWorld::UniversalId>& data,
                DropType type);

            virtual dropRequirments getDropRequirements(DropType type) const;

            /// \attention The created tool is not added to the toolbar (via addTool). Doing
            /// that is the responsibility of the calling function.
            virtual CSVWidget::SceneToolToggle *makeControlVisibilitySelector (
                CSVWidget::SceneToolbar *parent);

            virtual unsigned int getElementMask() const;

        protected:

            virtual void updateOverlay();

            virtual void mouseMoveEvent (QMouseEvent *event);
            virtual void mousePressEvent (QMouseEvent *event);
            virtual void mouseReleaseEvent (QMouseEvent *event);
            virtual void mouseDoubleClickEvent (QMouseEvent *event);
            virtual void wheelEvent (QWheelEvent *event);

        signals:

            void cellSelectionChanged (const CSMWorld::CellSelection& selection);

        private slots:

            virtual void cellDataChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);

            virtual void cellRemoved (const QModelIndex& parent, int start, int end);

            virtual void cellAdded (const QModelIndex& index, int start, int end);

    };
}

#endif
