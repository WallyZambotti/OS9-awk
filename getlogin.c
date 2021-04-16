static char spid[6];

char *getlogin()
{
    sprintf(spid, "%02x", getuid());
    return spid;
}