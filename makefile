all:
	gcc ./tty-mazes.c -o ./tty-mazes -Wall
install:
	sudo cp ./tty-mazes /usr/bin/
remove:
	sudo rm /ust/bin/tty-mazes
