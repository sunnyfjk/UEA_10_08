#include "getewaybase.h"

GeteWayBase::GeteWayBase(QWidget *parent) : QWidget(parent)
{

}

void GeteWayBase::switch_ui(GeteWayBase::GeteWayBaseUi_t id)
{
    switch (id) {
    case BedRoom_ui:
        break;
    case CookRoom_ui:
        break;
    case Parlour_ui:
        break;
    case StudyRoom_ui:
        break;
    case SubalternRoom_ui:
        break;
    case Toilet_ui:
        break;
    default:
        break;
    };
}

void GeteWayBase::switch_ui_init()
{

}

void GeteWayBase::setSerialPort(STM32F407 *port)
{
    stm=port;
}
