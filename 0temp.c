
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

test "$#" -gt 0 || usage_exit

#endif


