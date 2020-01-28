#include "progressevent.h"

// EventId已经定义了
ProgessEvent::ProgessEvent(int progress) : QEvent(static_cast<Type>(EventId)),
    m_progress(progress)
{

}

int ProgessEvent::progress() const
{
    return m_progress;
}
