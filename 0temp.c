
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

play_file()
{
}

parse_file()
{
    IFS="="
    while read -r key val; do
        value=${val//\"/}
        case "$key" in
            af)
                opt_af="$value"
            ;;
            volume)
                opt_volume="$value"
            ;;
            url)
                opt_url="$value"
            ;;
            *)
            ;;
        esac
    done < "$1"
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

while (($#)); do
    case "$1" in
        -add)
            add_file "$2" "$3"
            exit 0
        ;;
        -infos)
            test "$#" -eq 2 || usage_exit
            probe_file "$2"
            exit 0
        ;;
        -list)
            ls -la "$radios"
            exit 0
        ;;
        -remove)
            test "$#" -eq 2 || usage_exit
            rm "$radios/$2"
            exit 0
        ;;
        -show)
            test "$#" -eq 2 || usage_exit
            cat "$radios/$2"
            echo
            exit 0
        ;;
        -volume)
            test "$#" -gt 1 || usage_exit
            shift
            opt_volume="$1"
        ;;
        -stop)
            pkill ffplay
            exit 0
        ;;
        *)
            play_file "$1"
            exit 0
        ;;
    esac
    shift
done


#endif


