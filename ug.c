
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <ctype.h>

#define LEVEL_1 1
#define LEVEL_2 2
 // useless, just testing the map
static const struct {
    const char *name;
    int level;
} user_map[] = {
    { "root", LEVEL_1 },
    { "user", LEVEL_2 }
};


char *username_from_userid(uid_t uid)
{
    struct passwd *pwd;

    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

uid_t userid_from_name(const char *name)
{
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || name == '\0')
        return -1;
    
    // if string
    u = strtol(name, &endptr, 10);
    
    if (*endptr == '\0')
        return u;

    pwd = getpwnam(name);
    if (pwd == NULL)
        return -1;
    
    return pwd->pw_uid;
}
