
#if 0

opt_volume=66

add_file()
{
    filepath="$radios/$1"
    printf "#volume=100\n" > "$filepath"
    printf "#af=acompressor=threshold=-12dB:ratio=2:knee=2.82:attack=6:release=60\n" >> "$filepath"
    printf "url=$2\n" >> "$filepath"
}

printf("%s -add \"name\" \"https://my/radio/stream\"\n", APPNAME);
printf("%s -show \"name\"\n", APPNAME);
printf("%s -remove \"name\"\n", APPNAME);

if (strcmp(argv[n], "-add") == 0)
{
    if (++n >= argc)
        usage_exit();

    //add_file "$2" "$3"
    //exit 0

    return EXIT_SUCCESS;
}

else if (strcmp(argv[n], "-volume") == 0)
{
    if (++n >= argc)
        usage_exit();

    //test "$#" -gt 1 || usage_exit
    //shift
    //opt_volume="$1"

    return EXIT_SUCCESS;
}

else if (strcmp(argv[n], "-remove") == 0)
{
    if (++n >= argc)
        usage_exit();

    //test "$#" -eq 2 || usage_exit
    //rm "$radios/$2"
    //exit 0

    return EXIT_SUCCESS;
}

#endif

