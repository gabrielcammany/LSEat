# Set up the default search paths:
PATH=$PATH:$HOME:/bin:/usr/bin:/usr/contrib/bin:/usr/local/bin:.

#
stty erase "^?" kill "^U" intr "^C" eof "^D" susp "^Z" hupcl ixon ixoff tostop tabs

# Set up shell environment:
HISTSIZE=7000
HISTFILESIZE=7000
LC_CTYPE=iso_8859_1

absolute_path="/users/home/alumnes/ls30652/SO/Practica"
picard_exe="$absolute_path/picard/picard.exe $absolute_path/picard/config.dat" 
enterprise_exe="$absolute_path/enterprise/enterprise.exe $absolute_path/enterprise/config.dat $absolute_path/enterprise/menu.txt"
data_exe="$absolute_path/data/data.exe"

alias picard=$picard_exe
alias data=$data_exe
alias enterprise=$enterprise_exe

make(){
	if [ $# -eq 0 ]; then
		command make -C $absolute_path
	else
		command make $@.build -C $absolute_path
	fi
}

valgrind(){
	flags="--leak-check=full --show-leak-kinds=all --track-origins=yes"
	if [ $@ == "picard" ]; then
		command valgrind $flags $picard_exe
	elif [ $@ == "enterprise" ]; then
		command valgrind $flags $enterprise_exe 
	elif [ $@ == "data" ]; then
		command valgrind $flags $data_exe
	fi
}

### Setting aliases
alias ll="ls -al"
alias ls="ls --color"
alias f="finger"                                   # finger *******************
alias j="jobs"                                     # treballs pendents ********
alias l="ls -ligu"

TERM=vt220                                         	# tipus de terminal ********
mesg n
umask 077                                          	# Equival: rwx------
TMPDIR=$HOME

#*********************************** Mensajes *********************************
# La quota no funciona amb la cabina unity
#quota  $LOGNAME
echo " "
PS1="\[\033[01;32m\]\u\[\033[00m\]@\[\033[01;34m\]\h\[\033[00m\]:\w\[\033[01;32m\]>\[\033[00m\] "
