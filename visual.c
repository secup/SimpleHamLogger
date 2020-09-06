#include "visual.h"

void highlight_current_field(FORM *form, FIELD *fields[]) {

    FIELD *cfield = current_field(form);
    FIELD *currfield;
    int i = 0;
    while ((currfield = fields[i]) != NULL)
    {
        if (currfield == cfield)
            set_field_back(currfield, A_STANDOUT);
        else
            set_field_back(currfield, A_UNDERLINE);

        ++i;
    }
}
