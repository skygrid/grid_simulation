//
// Created by ken on 29.07.16.
//

jobType charToEnum(char *sval)
{
    jobType result=USER;
    int i=0;
    for (i=0; typesStr[i]!=NULL; ++i, ++result)
        if (0==strcmp(sval, typesStr[i])) return result;
    return UNKNOWN;
}