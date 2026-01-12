
#if 0

opt_volume=66
opt_af=

add_file()
{
    filepath="$radios/$1"
    printf "#volume=100\n" > "$filepath"
    printf "#af=acompressor=threshold=-12dB:ratio=2:knee=2.82:attack=6:release=60\n" >> "$filepath"
    printf "url=$2\n" >> "$filepath"
}

probe_file()
{
    echo "infos: $1"
    filepath="$radios/$1"
    test -f "$filepath" || error_exit "invalid file"
    opt_url=""
    parse_file "$filepath"
    ffprobe -hide_banner "$opt_url"
}

test "$#" -gt 0 || usage_exit

#endif


