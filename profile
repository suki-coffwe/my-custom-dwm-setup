export ENV="$HOME/.ashrc"

alias sx='startx ~/xinitrc'

alias sudo='doas'

alias reb='sudo reboot'
alias off='sudo poweroff'

alias n='nano --softwrap --linenumbers --tabstospaces --tabsize=4'
alias q='n ~/.profile'
alias w='source ~/.profile'
alias p='sudo apk add'
alias ls='ls --color=never -F'
alias l='ls'
alias la='ls -A'

ac() {
    nasm -f elf64 "$1" -o "./executables/{$1}"
}
