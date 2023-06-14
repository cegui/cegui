#include "CEGUI/InputEvent.h"
#include "CEGUI/Window.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
CursorInputEventArgs::CursorInputEventArgs(Window* wnd, const glm::vec2& globalPos, MouseButtons buttons, ModifierKeys modifiers)
    : WindowEventArgs(wnd), d_globalPos(globalPos), d_buttons(buttons), d_modifiers(modifiers)
{
    d_surfacePos = wnd ? wnd->getUnprojectedPosition(globalPos) : globalPos;
    d_localPos = wnd ? CoordConverter::screenToWindow(*wnd, d_surfacePos) : d_surfacePos;
}

//! \brief Predefined semantics that can be associated with specific input operations.
namespace SemanticValue
{
    // TODO StringAtom!
    const String GoToPreviousCharacter("GoToPreviousCharacter");
    const String GoToNextCharacter("GoToNextCharacter");
    const String GoToPreviousWord("GoToPreviousWord");
    const String GoToNextWord("GoToNextWord");
    const String GoToStartOfLine("GoToStartOfLine");
    const String GoToEndOfLine("GoToEndOfLine");
    const String GoToStartOfDocument("GoToStartOfDocument");
    const String GoToEndOfDocument("GoToEndOfDocument");
    const String GoToNextPage("GoToNextPage");
    const String GoToPreviousPage("GoToPreviousPage");
    const String GoDown("GoDown");
    const String GoUp("GoUp");

    const String SelectRange("SelectRange");
    const String SelectCumulative("SelectCumulative");
    const String SelectWord("SelectWord");
    const String SelectAll("SelectAll");
    const String SelectPreviousCharacter("SelectPreviousCharacter");
    const String SelectNextCharacter("SelectNextCharacter");
    const String SelectPreviousWord("SelectPreviousWord");
    const String SelectNextWord("SelectNextWord");
    const String SelectToStartOfLine("SelectToStartOfLine");
    const String SelectToEndOfLine("SelectToEndOfLine");
    const String SelectToStartOfDocument("SelectToStartOfDocument");
    const String SelectToEndOfDocument("SelectToEndOfDocument");
    const String SelectToNextPage("SelectToNextPage");
    const String SelectToPreviousPage("SelectToPreviousPage");
    const String SelectNextPage("SelectNextPage");
    const String SelectPreviousPage("SelectPreviousPage");
    const String SelectUp("SelectUp");
    const String SelectDown("SelectDown");

    const String DeleteNextCharacter("DeleteNextCharacter");
    const String DeletePreviousCharacter("DeletePreviousCharacter");
    const String Confirm("Confirm");
    const String Back("Back");
    const String Undo("Undo");
    const String Redo("Redo");
    const String Cut("Cut");
    const String Copy("Copy");
    const String Paste("Paste");
    const String NavigateToNext("NavigateToNext");
    const String NavigateToPrevious("NavigateToPrevious");
}

}
