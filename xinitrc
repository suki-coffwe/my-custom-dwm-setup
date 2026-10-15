export XDG_RUNTIME_DIR=/run/user/$(id -u)
[ -d "$XDG_RUNTIME_DIR" ] || { mkdir -p /tmp/run-$(id -u) && export XDG_RUNTIME_DIR=/tmp/run-$(id -u) && chmod 700 "$XDG_RUNTIME_DIR"; }

pipewire &
wireplumber &

~/taskbar &

#~/functioning_programs/3_brightness_slider &
#~/functioning_programs/4_volume_slider &

exec dwm
