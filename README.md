# Salt Mine
Automatically download my salty dotfiles


## Build Dependencies
* ncurses
* curl
* libgit2

## Runtime Dependencies
* tar
* patch
* git (for git checkout)


## TODO
- [ ] Implement checkout function for cloning dotfiles
- [X] Implement cloning Suckless Utilites
- [ ] Implement building Suckless Utilites
- [ ] Use su -c instead of sudo for building
- [X] Fix fonts
- [X] Fix herbe
- [X] Add submenus (Sukless utilites and fonts)
- [X] Rewrite draw functions (make windows/clear them/refresh them automatically) (wmove, wresize, wrefresh)
