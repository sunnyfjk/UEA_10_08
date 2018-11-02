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
    case HistoryData_ui:
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

void GeteWayBase::sendPm25Value(uint16_t id, uint16_t value)
{

}

void GeteWayBase::sendLedState(uint16_t id, uint8_t power)
{

}

void GeteWayBase::sendMotorValue(uint16_t id, uint16_t value)
{

}

void GeteWayBase::sendAirQuAlity(uint16_t id, uint16_t value)
{

}

void GeteWayBase::sendWaterHeater(uint16_t id, uint16_t value)
{

}
