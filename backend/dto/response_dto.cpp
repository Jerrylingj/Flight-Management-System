// response_dto.cpp
#include "response_dto.h"

ResponseDTOBase::ResponseDTOBase(QObject *parent)
    : QObject(parent)
    , m_code(200)
    , m_success(true)
{
}
