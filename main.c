#include <cinifile.h>
#include <cfile.h>
#include <libapp.h>
#include <stdio.h>

#define APPNAME "radio"
#define DEFAULT_VOLUME "66"

static void error_exit(const char *msg)
{
    if (!msg || *msg == '\0')
    {
        msg = "an error occurred";
    }

    printf("*** %s\nabort...\n", msg);

    exit(EXIT_FAILURE);
}

static void usage_exit()
{
    printf("*** usage :\n");

    printf("%s \"name\"\n", APPNAME);
    printf("%s \"https://my/radio/stream\"\n", APPNAME);
    printf("%s \"path/to/file.mp3\"\n", APPNAME);

    printf("%s -infos \"name\"\n", APPNAME);
    printf("%s -list\n", APPNAME);
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
    RadioEntry *radio = (RadioEntry*) malloc(sizeof(RadioEntry));

    radio->url = cstr_new_size(128);
    radio->volume = cstr_new_size(4);
    radio->af = cstr_new_size(128);

    return radio;
}

void radio_free(RadioEntry *radio)
{
    if (!radio)
        return;

    cstr_free(radio->url);
    cstr_free(radio->volume);
    cstr_free(radio->af);

    free(radio);
}

void radio_clear(RadioEntry *radio)
{
    cstr_clear(radio->url);
    cstr_clear(radio->volume);
    cstr_clear(radio->af);
}

void config_get_path(CString *result)
{
    get_homedir(result);
    cstr_append(result, "/.config/radio.list");
}

bool config_find(CIniFile *inifile, RadioEntry *radio, const char *name)
{
    if (!radio || !inifile || !name)
        return false;

    CIniSection *section = cinifile_section(inifile, name);
    if (!section)
        return false;

    if (!cinisection_key_value(section, radio->url, "url", ""))
        return false;

    cinisection_key_value(section, radio->volume, "volume", "");
    cinisection_key_value(section, radio->af, "af", "");

    return true;
}

bool command_infos(CIniFile *inifile, const char *name)
{
    RadioEntry *radio = radio_new();

    if (!config_find(inifile, radio, name))
    {
        radio_free(radio);
        return false;
    }

    printf("infos: %s\n", name);

    CStringAuto *cmd = cstr_new_size(128);
    cstr_copy(cmd, "ffprobe -hide_banner \"");
    cstr_append(cmd, c_str(radio->url));
    cstr_append(cmd, "\"");

    system(c_str(cmd));

    radio_free(radio);

    return true;
}

bool command_list(CIniFile *inifile)
{
    if (!inifile)
        return false;

    int size = cinifile_size(inifile);

    for (int i = 0; i < size; ++i)
    {
        CIniSection *iniSection = cinifile_section_at(inifile, i);

        printf("%s\n", c_str(cinisection_name(iniSection)));
    }

    return true;
}

bool command_show(CIniFile *inifile, const char *name)
{
    if (!inifile || !name)
        return false;

    RadioEntry *radio = radio_new();

    if (!config_find(inifile, radio, name)
        || cstr_isempty(radio->url))
    {
        radio_free(radio);
        return false;
    }

    printf("name:   %s\n", name);
    printf("url:    %s\n", c_str(radio->url));

    if (!cstr_isempty(radio->af))
        printf("af:     %s\n", c_str(radio->af));

    if (!cstr_isempty(radio->volume))
        printf("volume: %s\n", c_str(radio->volume));

    radio_free(radio);

    return true;
}

bool command_play(CIniFile *inifile, const char *name)
{
    if (!inifile || !name)
        return false;

    RadioEntry *radio = radio_new();

    CStringAuto *cmd = cstr_new_size(128);
    cstr_copy(cmd, "ffplay -nodisp");

    if (strncmp(name, "http://", 7) == 0
        || strncmp(name, "https://", 8) == 0)
    {
        cstr_copy(radio->url, name);
    }
    else if (file_exists("name"))
    {
        cstr_copy(radio->url, name);
    }
    else
    {
        if (!config_find(inifile, radio, name))
        {
            radio_free(radio);
            return false;
        }
    }

    if (cstr_isempty(radio->volume))
        cstr_copy(radio->volume, DEFAULT_VOLUME);

    if (cstr_isempty(radio->af)
        && !cstr_isempty(radio->volume))
    {
        cstr_copy(radio->af, "volume=volume=");
        cstr_append(radio->af, c_str(radio->volume));
        cstr_append(radio->af, "/100");
    }

    if (!cstr_isempty(radio->af))
    {
        cstr_append(cmd, " -af \"");
        cstr_append(cmd, c_str(radio->af));
        cstr_append(cmd, "\"");
    }

    cstr_append(cmd, " \"");
    cstr_append(cmd, c_str(radio->url));
    cstr_append(cmd, "\"");

    cstr_append(cmd, " > /dev/null 2>&1 &");

    system("pkill ffplay");

    printf("%s\n", c_str(cmd));
    system(c_str(cmd));

    radio_free(radio);

    return true;
}

int main(int argc, const char **argv)
{
    CStringAuto *inipath = cstr_new_size(64);
    config_get_path(inipath);

    CIniFileAuto *inifile = cinifile_new();

    if (!cinifile_read(inifile, c_str(inipath)))
        return EXIT_FAILURE;

    if (argc < 2)
        usage_exit();

    int n = 1;

    while (n < argc)
    {
        if (strcmp(argv[n], "-infos") == 0)
        {
            if (++n >= argc)
                usage_exit();

            command_infos(inifile, argv[n]);

            break;
        }
        else if (strcmp(argv[n], "-list") == 0)
        {
            command_list(inifile);

            break;
        }
        else if (strcmp(argv[n], "-show") == 0)
        {
            if (++n >= argc)
                usage_exit();

            command_show(inifile, argv[n]);

            break;
        }
        else if (strcmp(argv[n], "-stop") == 0)
        {
            system("pkill ffplay");

            break;
        }
        else
        {
            if (n + 1 != argc)
                usage_exit();

            if (argv[n][0] == '-')
                error_exit("invalid option");

            if (!command_play(inifile, argv[n]))
                error_exit("radio not found\n");

            break;
        }

        ++n;
    }

    return EXIT_SUCCESS;
}


