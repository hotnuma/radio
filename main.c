#include <cinifile.h>
#include <libapp.h>
#include <stdio.h>

#define APPNAME "radio"

//static void error_exit(const char *msg)
//{
//    if (!msg || *msg == '\0')
//    {
//        msg = "an error occurred";
//    }

//    printf("*** %s\nabort...\n", msg);

//    exit(EXIT_FAILURE);
//}

static void usage_exit()
{
    printf("*** usage :\n");
    printf("%s -list\n", APPNAME);
    printf("%s -add \"name\" \"https://my/radio/stream\"\n", APPNAME);
    printf("%s -show \"name\"\n", APPNAME);
    printf("%s -remove \"name\"\n", APPNAME);
    printf("%s \"name\"\n", APPNAME);
    printf("%s \"https://my/radio/stream\"\n", APPNAME);
    printf("%s \"path/to/file.mp3\"\n", APPNAME);
    printf("%s -stop\n", APPNAME);
    printf("abort...\n");

    exit(EXIT_FAILURE);
}

typedef struct _RadioEntry
{
    CString *url;
    CString *volume;
    CString *af;

} RadioEntry;

RadioEntry* radio_new()
{
    RadioEntry *entry = (RadioEntry*) malloc(sizeof(RadioEntry));

    entry->url = cstr_new_size(128);
    entry->volume = cstr_new_size(4);
    entry->af = cstr_new_size(128);

    return entry;
}

void radio_free(RadioEntry *entry)
{
    if (!entry)
        return;

    cstr_free(entry->url);
    cstr_free(entry->volume);
    cstr_free(entry->af);

    free(entry);
}

bool radio_find(RadioEntry *entry, const char *inipath, const char *name)
{
    if (!entry)
        return false;

    CIniFileAuto *inifile = cinifile_new();

    if (!cinifile_read(inifile, inipath))
        return false;

    CIniSection *section = cinifile_section(inifile, name);
    if (!section)
        return false;

    if (!cinisection_value(section, entry->url, "url", ""))
        return false;

    cinisection_value(section, entry->volume, "volume", "");
    cinisection_value(section, entry->af, "af", "");

    return true;
}

void radio_get_config(CString *result)
{
    get_homedir(result);
    cstr_append(result, "/.config/radio.list");
}

bool radio_play(CString *inipath, RadioEntry *radio, const char *name)
{
    //printf("%s\n", name);

    if (!radio_find(radio, c_str(inipath), name))
        return false;

    //printf("url = %s\n", c_str(radio->url));

    //opt_url=""

    //if [[ -f "$1" ]]; then
    //    opt_url="$1"
    //elif [[ "$1" == "http://"* ]] || [[ "$1" == "https://"* ]]; then
    //    opt_url="$1"
    //else
    //    filepath="$radios/$1"
    //    test -f "$filepath" || error_exit "invalid file"
    //    parse_file "$filepath"
    //fi

    CStringAuto *cmd = cstr_new_size(128);
    cstr_copy(cmd, "ffplay -nodisp");

    if (!cstr_isempty(radio->af))
    {
        cstr_append(cmd, " -af \"");
        cstr_append(cmd, c_str(radio->af));
        cstr_append(cmd, "\"");
    }
    else
    {
        cstr_append(cmd, " -af \"volume=volume=");
        cstr_append(cmd, c_str(radio->volume));
        cstr_append(cmd, "/100\"");
    }

    cstr_append(cmd, " ");
    cstr_append(cmd, c_str(radio->url));

    cstr_append(cmd, " > /dev/null 2>&1 &");

    system("pkill ffplay");

    printf("%s\n", c_str(cmd));
    system(c_str(cmd));

    return true;
}

int main(int argc, const char **argv)
{
    CStringAuto *inipath = cstr_new_size(64);
    radio_get_config(inipath);

    RadioEntry *radio = radio_new();

    int n = 1;

    while (n < argc)
    {
        if (strcmp(argv[n], "-opt") == 0)
        {
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[n], "-arg") == 0)
        {
            if (++n >= argc)
                usage_exit();

            return EXIT_SUCCESS;
        }
        else
        {
            if (!radio_play(inipath, radio, argv[n]))
                printf("radio not found\n");
        }

        ++n;
    }

    radio_free(radio);

    return EXIT_SUCCESS;
}


