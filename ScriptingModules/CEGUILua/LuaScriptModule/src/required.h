#include "CEGUI.h"
typedef CEGUI::String string;
using namespace CEGUI;

namespace CEGUI
{

const MouseCursorEventArgs& toMouseCursorEventArgs(const EventArgs& e);
const WindowEventArgs& toWindowEventArgs(const EventArgs& e);
const ActivationEventArgs& toActivationEventArgs(const EventArgs& e);
const HeaderSequenceEventArgs& toHeaderSequenceEventArgs(const EventArgs& e);
const MouseEventArgs& toMouseEventArgs(const EventArgs& e);

}
