#include "msgutil.h"

void getMsgFieldName(const char** p, CStr& fieldName, int& arrayIndex)
{
   fieldName.getFieldName(p, arrayIndex);
}
