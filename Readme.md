
A simple launcher to play web radios with ffplay.

* Requirements
    
    ```
    apt install build-essential git meson ninja-build
    ```

* Install
    
    ```
    git clone https://github.com/hotnuma/libtinyc.git
    pushd libtinyc
    ./install.sh
    popd
    
    git clone https://github.com/hotnuma/radio.git
    pushd radio
    ./install.sh
    popd
    ```

* Configuration File

    Example configuration file to create in `$HOME/.config/radio.list` :

    ```
    [elettrica]
    url=https://nr8.newradio.it/proxy/apselett?mp=/stream
    volume=50

    [mizrahit]
    url=https://mzr.mediacast.co.il/mzradio

    [sabrosita]
    url=https://playerservices.streamtheworld.com/api/livestream-redirect/XEPHAMAAC.aac
    af=acompressor=mode=upward:threshold=-8dB:ratio=2:knee=2.82:attack=6:release=60,volume=60/100
    ```

